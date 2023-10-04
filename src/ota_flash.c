#include "arch.h"
#include "log.h"
#include "ota.h"
#include "device.h"

// This OTA implementation uses the internal flash API outlined in device.h
// It splits flash into 2 equal partitions, and stores OTA status in the
// last sector of the partition.

#if MG_OTA == MG_OTA_FLASH

#define MG_OTADATA_KEY 0xb07afed0

static char *s_addr;      // Current address to write to
static size_t s_size;     // Firmware size to flash. In-progress indicator
static uint32_t s_crc32;  // Firmware checksum

struct mg_otadata {
  uint32_t crc32, size, timestamp, status;
};

static uint32_t mg_ota_key_current(void) {
  return MG_OTADATA_KEY + mg_flash_bank();
}

static uint32_t mg_ota_key_previous(void) {
  uint32_t key = MG_OTADATA_KEY + 1;
  return mg_ota_key_current() == key ? key + 1 : key;
}

bool mg_ota_begin(size_t new_firmware_size) {
  bool ok = false;
  if (s_size) {
    MG_ERROR(("OTA already in progress. Call mg_ota_end()"));
  } else {
    size_t half = mg_flash_size() / 2, max = half - mg_flash_sector_size();
    s_crc32 = 0;
#if MG_DEVICE_DUAL_BANK
    s_addr = (char *) mg_flash_start() + half;
#else
    s_addr = mg_flash_bank() == 1 ? (char *) mg_flash_start() + half :
                                    (char *) mg_flash_start();
#endif
    MG_INFO(("Starting writing firmware from %p", s_addr));
    MG_DEBUG(("Firmware %lu bytes, max %lu", s_size, max));
    if (new_firmware_size < max) {
      ok = true;
      s_size = new_firmware_size;
      MG_INFO(("Starting OTA, firmware size %lu", s_size));
    } else {
      MG_ERROR(("Firmware %lu is too big to fit %lu", new_firmware_size, max));
    }
  }
  return ok;
}

bool mg_ota_write(const void *buf, size_t len) {
  bool ok = false;
  if (s_size == 0) {
    MG_ERROR(("OTA is not started, call mg_ota_begin()"));
  } else {
    size_t align = mg_flash_write_align();
    size_t len_aligned_down = MG_ROUND_DOWN(len, align);
    if (len_aligned_down) ok = mg_flash_write(s_addr, buf, len_aligned_down);
    if (len_aligned_down < len) {
      size_t left = len - len_aligned_down;
      char tmp[align];
      memset(tmp, 0xff, sizeof(tmp));
      memcpy(tmp, (char *) buf + len_aligned_down, left);
      ok = mg_flash_write(s_addr + len_aligned_down, tmp, sizeof(tmp));
    }
    s_crc32 = mg_crc32(s_crc32, (char *) buf, len);  // Update CRC
    MG_DEBUG(("%#x %p %lu -> %d", s_addr - len, buf, len, ok));
    s_addr += len;
  }
  return ok;
}

bool mg_ota_end(void) {
#if MG_DEVICE_DUAL_BANK
  char *base = (char *) mg_flash_start() + mg_flash_size() / 2;
#else
  char *base = mg_flash_bank() == 1 ? (char *) mg_flash_start() +
                                              mg_flash_size() / 2 :
                                      (char *) mg_flash_start();

#endif
  bool ok = false;
  if (s_size) {
    size_t size = s_addr - base;
    uint32_t crc32 = mg_crc32(0, base, s_size);
    if (size == s_size && crc32 == s_crc32) {
      uint32_t now = (uint32_t) (mg_now() / 1000);
      struct mg_otadata od = {crc32, size, now, MG_OTA_FIRST_BOOT};
      uint32_t key = mg_ota_key_previous();
      ok = mg_flash_save(NULL, key, &od, sizeof(od));
    }
    MG_DEBUG(("CRC: %x/%x, size: %lu/%lu, status: %s", s_crc32, crc32, s_size,
              size, ok ? "ok" : "fail"));
    s_size = 0;
    if (ok) ok = mg_flash_swap_bank();
  }
  MG_INFO(("Finishing OTA: %s", ok ? "ok" : "fail"));
  return ok;
}

static struct mg_otadata mg_otadata(int fw) {
  struct mg_otadata od = {};
  int bank = mg_flash_bank();
  uint32_t key = MG_OTADATA_KEY + 1;
  if ((fw == MG_FIRMWARE_CURRENT && bank == 2)) key++;
  if ((fw == MG_FIRMWARE_PREVIOUS && bank == 1)) key++;
  mg_flash_load(NULL, key, &od, sizeof(od));
  // MG_DEBUG(("Loaded OTA data. fw %d, bank %d, key %p", fw, bank, key));
  // mg_hexdump(&od, sizeof(od));
  return od;
}

int mg_ota_status(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.status;
}
uint32_t mg_ota_crc32(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.crc32;
}
uint32_t mg_ota_timestamp(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.timestamp;
}
size_t mg_ota_size(int fw) {
  struct mg_otadata od = mg_otadata(fw);
  return od.size;
}

bool mg_ota_commit(void) {
  struct mg_otadata od = mg_otadata(MG_FIRMWARE_CURRENT);
  od.status |= MG_OTA_COMMITTED;
  uint32_t key = mg_ota_key_current();
  return mg_flash_save(NULL, key, &od, sizeof(od));
}

bool mg_ota_rollback(void) {
  MG_DEBUG(("Rolling firmware back"));
#if MG_DEVICE_DUAL_BANK
  return mg_flash_swap_bank();
#else
  struct mg_otadata prv_od = mg_otadata(MG_FIRMWARE_PREVIOUS);
  prv_od.status |= MG_OTA_ROLLBACK;
  if (mg_flash_bank() == 2) {
    struct mg_otadata crt_od = mg_otadata(MG_FIRMWARE_CURRENT);
    crt_od.status &= ~MG_OTA_ROLLBACK;
    mg_flash_save(NULL, mg_ota_key_current(), &crt_od, sizeof(crt_od));
  }
  return mg_flash_save(NULL, mg_ota_key_previous(), &prv_od, sizeof(prv_od));
#endif
}

void MG_IRAM mg_ota_bootloader(void) {
  MG_DEBUG(("calling bootloader from address: %p", mg_ota_bootloader));
  struct mg_otadata crt_ot = mg_otadata(MG_FIRMWARE_CURRENT);
  uint32_t crt_key = mg_ota_key_current();
  struct mg_otadata prv_ot = mg_otadata(MG_FIRMWARE_PREVIOUS);
  uint32_t prv_key = mg_ota_key_previous();

  // On each reset, the firmware from the 1st bank will start, even if
  // we reset from the fw on bank 2. Thus, the bootloader can only jump
  // to the 2nd firmware, which is always marked with the ROLLBACK flag
  // in order to instruct the bootloader to jump to it.
  if (mg_flash_bank() == 1) {
    if (crt_ot.status & (MG_OTA_FIRST_BOOT | MG_OTA_ROLLBACK)) {
      bool boot_first_fw = crt_ot.status & MG_OTA_FIRST_BOOT ? true : false;
      crt_ot.status &= ~(MG_OTA_FIRST_BOOT | MG_OTA_ROLLBACK);
      if (crt_ot.status == 0)
        crt_ot.status |= MG_BIT(31); // value of 0 is reserved for MG_OTA_UNAVAILABLE
      mg_flash_save(NULL, crt_key, &crt_ot, sizeof(crt_ot));

      if (boot_first_fw) {
        // If there is a new firmware in the first bank, the bootloader
        // must launch this instead of the one in the 2nd bank
        prv_ot.status &= ~MG_OTA_ROLLBACK;
        mg_flash_save(NULL, prv_key, &prv_ot, sizeof(prv_ot));
        return;
      }
    }

    if (prv_ot.status & (MG_OTA_FIRST_BOOT | MG_OTA_ROLLBACK)) {
      if (prv_ot.status & MG_OTA_FIRST_BOOT) {
        // The 2nd firmware must stay marked with the Rollback flag in case
        // of a reset, to prevent the bootloader starting the 1st firmware
        // after that. Normally, after a reset, the firmware at the beginning
        // of the flash will start.
        prv_ot.status &= ~MG_OTA_FIRST_BOOT;
        prv_ot.status |= MG_OTA_ROLLBACK;
        mg_flash_save(NULL, prv_key, &prv_ot, sizeof(prv_ot));
      }

      SCB->VTOR = (uint32_t) mg_flash_start() + mg_flash_size() / 2;
      void (*fw_reset_handler)(void) = (void (*)(void))
                                      *((uint32_t*)(SCB->VTOR + 4));
      MG_DEBUG(("Jumping to reset handler: 0x%x", fw_reset_handler));
      fw_reset_handler();
    }
  }
  return;
}
#endif
