// Load NXP BLE/802.15.4 combo firmware to CPU2 and start it.
// Port of NXP mcux-component conn_fwloader: fsl_loader_utils.c (SB3 loader,
// load_service_monolithic) and nboot_hal.c (manifest parameters).
// Copyright 2024 NXP
// SPDX-License-Identifier: BSD-3-Clause

#include "hal.h"
#include "fsl_ocotp.h"
#include "fsl_power.h"
#include "fsl_romapi_nboot.h"
#include "fsl_romapi_otp.h"
#include "fsl_kb_api.h"
#include "fsl_sbloader_v3.h"

// The firmware image, linked into our flash. Signed SB3 container, see
// https://github.com/NXP/wifi_nb_fw, rw61x/rw61x_sb_ble_15d4_combo_a2.bin
__asm__(
    ".section .rodata.radio_fw,\"a\"\n"
    ".balign 16\n"
    "radio_fw:\n"
    ".incbin \"" RADIO_FW "\"\n"
    ".balign 4\n"
    "radio_fw_end:\n"
    ".previous\n");
extern const uint8_t radio_fw[], radio_fw_end[];

#define CIU2_RST_SW3 (*(volatile uint32_t *) 0x4424011cU)
#define CIU2_RST_SW3_CPU2_RST BIT(4)
#define CPU2_MAGIC (*(volatile uint32_t *) 0x443c0000U)  // Set by firmware
#define CPU2_MAGIC_VALUE 0xabcdef89U
#define OTP_LCS_FUSE_IDX 45U     // Life cycle state, and its redundant copy
#define OTP_PCK_BLOB_FUSE_IDX 116U  // Part common key blob, 48 bytes

// SB3 loader state. See fsl_ldr_Context_v3_t
struct sb3 {
  nboot_context_t nboot;     // ROM nboot context
  uint8_t buf[MAX(NBOOT_SB3_MANIFEST_MAX_SIZE_IN_BYTES,
                  NBOOT_SB3_BLOCK_MAX_SIZE_IN_BYTES)];  // Block being read
  uint32_t buf_size;         // Bytes needed for the current block
  uint32_t buf_pos;          // Bytes in buf so far
  bool header_done;          // Manifest (block 0) processed
  uint32_t data_offset;      // Data offset within a block
  uint32_t blocks;           // Data blocks processed
  uint8_t *data;             // Data of the current block
  uint32_t pos;              // Position in data
  bool in_section;           // Inside a data section
  uint32_t section_done;     // Bytes of the section handled
  sb3_section_header_t section;
  bool in_range;             // Inside a data range
  uint32_t range_gap;        // Padding after the range, 16 byte alignment
  sb3_data_range_header_t range;
  bool has_expansion;        // Range expansion read
  bool executed;             // Execute command seen: firmware loaded
};
static struct sb3 s_sb3;

static uint32_t antipole(uint32_t v) {
  return (v & 0xffffU) | (~(v & 0xffffU) << 16);
}

// Manifest parameters: root of trust for NXP signed images, rev A2 chips.
// See nboot_hal_get_sb3_manifest_params(), nboot_hal.c
static bool manifest_params(nboot_sb3_load_manifest_parms_t *p) {
  static const uint32_t rkh_a2[8] = {0xE7C7E9BB, 0x12C8C535, 0x37E61148,
                                     0x2BE7F18C, 0x8F0E3094, 0xB2BA7F32,
                                     0xEC9B4ECB, 0xAD9FC941};
  uint32_t i, lcs = OCOTP->OTP_SHADOW[OTP_LCS_FUSE_IDX];
  memset(p, 0, sizeof(*p));
  if ((lcs & 0xff) != ((lcs >> 8) & 0xff)) return false;  // Corrupt LCS
  for (i = 0; i < NBOOT_ROOT_CERT_COUNT; i++) {
    p->soc_RoTNVM.soc_rootKeyRevocation[i] = kNBOOT_RootKey_Enabled;
  }
  memcpy(p->soc_RoTNVM.soc_rkh, rkh_a2, sizeof(rkh_a2));
  p->soc_RoTNVM.soc_numberOfRootKeys = 1;
  for (i = 1; i < NBOOT_ROOT_CERT_COUNT; i++) {
    p->soc_RoTNVM.soc_rootKeyUsage[i] = kNBOOT_RootKeyUsage_Unused;
  }
  p->soc_RoTNVM.soc_rootKeyTypeAndLength = kNBOOT_RootKey_Ecdsa_P256;
  p->soc_RoTNVM.soc_lifecycle = (nboot_soc_lifecycle_t) antipole(lcs & 0xff);
  for (i = 0; i < sizeof(p->pckBlob) / 4; i++) {
    if (OCOTP_OtpFuseRead(OTP_PCK_BLOB_FUSE_IDX + i,
                          (uint32_t *) &p->pckBlob[4 * i]) != kStatus_Success) {
      return false;
    }
  }
  return true;
}

// Manifest (block 0): verify, learn the block layout. See ldr_DoHeader_v3()
static bool sb3_header(struct sb3 *s) {
  nboot_sb3_header_t *h = (nboot_sb3_header_t *) s->buf;
  nboot_sb3_load_manifest_parms_t parms;
  if (s->buf_size == sizeof(nboot_sb3_header_t)) {  // Header only so far
    s->buf_size = h->imageTotalLength;
    s->buf_pos = sizeof(nboot_sb3_header_t);
    return s->buf_size <= sizeof(s->buf);
  }
  if (!manifest_params(&parms)) return false;
  // Protected status: the status code is in the low 32 bits
  if ((nboot_status_t) nboot_sb3_load_manifest(&s->nboot, (uint32_t *) h,
                                               &parms) != kStatus_NBOOT_Success) {
    return false;
  }
  s->data_offset = 4 + h->certificateBlockOffset - sizeof(nboot_sb3_header_t);
  s->buf_size = s->data_offset + NBOOT_SB3_CHUNK_SIZE_IN_BYTES;
  s->header_done = true;
  return true;
}

// Copy (part of) the current load range to its destination
static void sb3_load(struct sb3 *s) {
  uint32_t avail = NBOOT_SB3_CHUNK_SIZE_IN_BYTES - s->pos;
  uint32_t n = s->range.length < avail ? s->range.length : avail;
  memcpy((void *) s->range.startAddress, &s->data[s->pos], n);
  s->pos += n, s->section_done += n;
  s->range.startAddress += n, s->range.length -= n;
  if (s->range.length == 0) {  // Range done, skip alignment padding
    s->in_range = false;
    s->pos += s->range_gap, s->section_done += s->range_gap;
  }
}

// Process one decrypted data block. See ldr_DoBlock()
static bool sb3_block(struct sb3 *s) {
  while (s->pos < NBOOT_SB3_CHUNK_SIZE_IN_BYTES) {
    if (!s->in_section) {  // New section
      memcpy(&s->section, &s->data[s->pos], sizeof(s->section));
      if (s->section.sectionType == kSectionNone) break;  // Rest is unused
      if (s->section.sectionType != kSectionDataRange) return false;
      s->in_section = true, s->section_done = 0, s->in_range = false;
      s->pos += sizeof(sb3_section_header_t);
    }
    if (!s->in_range) {  // New range
      memcpy(&s->range, &s->data[s->pos], sizeof(s->range));
      if (s->range.tag != SB3_DATA_RANGE_HEADER_TAG) return false;
      s->in_range = true, s->has_expansion = false, s->range_gap = 0;
      s->pos += sizeof(sb3_data_range_header_t);
      s->section_done += sizeof(sb3_data_range_header_t);
      if (s->range.cmd == kSB3_CmdLoad) {
        s->range_gap = (SB3_DATA_ALIGNMENT_SIZE_IN_BYTE -
                        (s->range.length & 15U)) & 15U;
      }
    }
    if (s->range.cmd == kSB3_CmdLoad) {
      if (!s->has_expansion) {
        if (s->pos == NBOOT_SB3_CHUNK_SIZE_IN_BYTES) break;  // Next block
        s->has_expansion = true;
        s->pos += sizeof(sb3_data_range_expansion_t);
        s->section_done += sizeof(sb3_data_range_expansion_t);
      }
      if (s->pos < NBOOT_SB3_CHUNK_SIZE_IN_BYTES) sb3_load(s);
    } else if (s->range.cmd == kSB3_CmdExecute) {
      s->executed = true;
      return true;
    } else {
      s->in_range = false;  // Other commands: nothing to do
    }
    if (s->section_done == s->section.length) s->in_section = false;
    if (s->section_done > s->section.length) return false;
  }
  s->pos = 0;
  return true;
}

// Feed image bytes, process complete blocks. See fsl_sbloader_pump()
static bool sb3_pump(struct sb3 *s, const uint8_t *p, uint32_t len) {
  while (len > 0 && !s->executed) {
    uint32_t n = s->buf_size - s->buf_pos;
    if (n > len) n = len;
    memcpy(&s->buf[s->buf_pos], p, n);
    s->buf_pos += n, p += n, len -= n;
    if (s->buf_pos < s->buf_size) break;
    s->buf_pos = 0;
    if (!s->header_done) {
      if (!sb3_header(s)) return false;
    } else {
      if (s->blocks >= s->nboot.totalBlocks) return false;
      if ((nboot_status_t) nboot_sb3_load_block(
              &s->nboot, (uint32_t *) s->buf) != kStatus_NBOOT_Success) {
        return false;
      }
      s->blocks++;
      s->data = &s->buf[s->data_offset];
      if (!sb3_block(s)) return false;
    }
  }
  return true;
}

// Authenticate the image with the ROM, copy it to CPU2 memory.
// See loader_process_sb_file()
static bool sb3_process(const uint8_t *img, size_t len) {
  struct sb3 *s = &s_sb3;
  bool ok = false;
  memset(s, 0, sizeof(*s));
  s->buf_size = sizeof(nboot_sb3_header_t);
  // Start ELS only if it is off: the ROM keeps keys in it after boot
  if ((CLKCTL0->PSCCTL0 & CLKCTL0_PSCCTL0_ELS_MASK) == 0 ||
      (CLKCTL0->PSCCTL1 & CLKCTL0_PSCCTL1_ELS_APB_MASK) == 0 ||
      (RSTCTL0->PRSTCTL0 & RSTCTL0_PRSTCTL0_ELS_MASK) != 0) {
    CLOCK_EnableClock(kCLOCK_Els);
    CLOCK_EnableClock(kCLOCK_ElsApb);
    RESET_PeripheralReset(kELS_RST_SHIFT_RSTn);
  }
  if (nboot_context_init(&s->nboot) == kStatus_NBOOT_Success) {
    ok = sb3_pump(s, img, (uint32_t) len) && s->executed;
    (void) nboot_context_deinit(&s->nboot);
  }
  return ok;
}

// Load the radio firmware to CPU2 and start it. See sb3_fw_reset() and
// load_service_monolithic(), fsl_loader_utils.c. Call before changing the
// boot clock: the ROM trims the ELS glitch detector for it, and after the
// clock change ELS reports glitch events. Interrupts may be off.
// Return 0 on success, or the failed step: 1 chip revision, 2 OTP, 3 image
// load, 4 firmware start
int hal_radio_init(void) {
  uint32_t n = 100000000;  // Firmware start timeout, loop iterations
  bool ok;
  if ((SOCCTRL->CHIP_INFO & 15U) != 2U) return 1;  // Image is for rev A2
  POWER_PowerOffBle();
  spin(100000);
  if (otp_init(0) != kStatus_Success) return 2;
  POWER_PowerOnBle();
  spin(100000);
  CPU2_MAGIC = 0;
  ok = sb3_process(radio_fw, (size_t) (radio_fw_end - radio_fw));
  (void) otp_deinit();
  if (!ok) return 3;
  CIU2_RST_SW3 |= CIU2_RST_SW3_CPU2_RST;
  while (CPU2_MAGIC != CPU2_MAGIC_VALUE) {
    if (--n == 0) return 4;  // Firmware did not start
  }
  return 0;
}
