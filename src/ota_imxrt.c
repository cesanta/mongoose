#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA >= MG_OTA_RT1020 && MG_OTA <= MG_OTA_RT1170

static bool mg_imxrt_write(void *, const void *, size_t);
static bool mg_imxrt_swap(void);

#if MG_OTA <= MG_OTA_RT1060
#define MG_IMXRT_FLASH_START 0x60000000
#define FLEXSPI_NOR_INSTANCE 0
#elif MG_OTA == MG_OTA_RT1064
#define MG_IMXRT_FLASH_START 0x70000000
#define FLEXSPI_NOR_INSTANCE 1
#else // RT1170
#define MG_IMXRT_FLASH_START 0x30000000
#define FLEXSPI_NOR_INSTANCE 1
#endif

// TODO(): fill at init, support more devices in a dynamic way
// TODO(): then, check alignment is <= 256, see Wizard's #251
static struct mg_flash s_mg_flash_imxrt = {
    (void *) MG_IMXRT_FLASH_START,  // Start,
    4 * 1024 * 1024,                // Size, 4mb
    4 * 1024,                       // Sector size, 4k
    256,                            // Align,
    mg_imxrt_write,
    mg_imxrt_swap,
};

struct mg_flexspi_lut_seq {
  uint8_t seqNum;
  uint8_t seqId;
  uint16_t reserved;
};

struct mg_flexspi_mem_config {
  uint32_t tag;
  uint32_t version;
  uint32_t reserved0;
  uint8_t readSampleClkSrc;
  uint8_t csHoldTime;
  uint8_t csSetupTime;
  uint8_t columnAddressWidth;
  uint8_t deviceModeCfgEnable;
  uint8_t deviceModeType;
  uint16_t waitTimeCfgCommands;
  struct mg_flexspi_lut_seq deviceModeSeq;
  uint32_t deviceModeArg;
  uint8_t configCmdEnable;
  uint8_t configModeType[3];
  struct mg_flexspi_lut_seq configCmdSeqs[3];
  uint32_t reserved1;
  uint32_t configCmdArgs[3];
  uint32_t reserved2;
  uint32_t controllerMiscOption;
  uint8_t deviceType;
  uint8_t sflashPadType;
  uint8_t serialClkFreq;
  uint8_t lutCustomSeqEnable;
  uint32_t reserved3[2];
  uint32_t sflashA1Size;
  uint32_t sflashA2Size;
  uint32_t sflashB1Size;
  uint32_t sflashB2Size;
  uint32_t csPadSettingOverride;
  uint32_t sclkPadSettingOverride;
  uint32_t dataPadSettingOverride;
  uint32_t dqsPadSettingOverride;
  uint32_t timeoutInMs;
  uint32_t commandInterval;
  uint16_t dataValidTime[2];
  uint16_t busyOffset;
  uint16_t busyBitPolarity;
  uint32_t lookupTable[64];
  struct mg_flexspi_lut_seq lutCustomSeq[12];
  uint32_t reserved4[4];
};

struct mg_flexspi_nor_config {
  struct mg_flexspi_mem_config memConfig;
  uint32_t pageSize;
  uint32_t sectorSize;
  uint8_t ipcmdSerialClkFreq;
  uint8_t isUniformBlockSize;
  uint8_t reserved0[2];
  uint8_t serialNorType;
  uint8_t needExitNoCmdMode;
  uint8_t halfClkForNonReadCmd;
  uint8_t needRestoreNoCmdMode;
  uint32_t blockSize;
  uint32_t reserve2[11];
};

/* FLEXSPI memory config block related defintions */
#define MG_FLEXSPI_CFG_BLK_TAG (0x42464346UL)      // ascii "FCFB" Big Endian
#define MG_FLEXSPI_CFG_BLK_VERSION (0x56010400UL)  // V1.4.0

#define MG_FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)       \
  (MG_FLEXSPI_LUT_OPERAND0(op0) | MG_FLEXSPI_LUT_NUM_PADS0(pad0) | \
   MG_FLEXSPI_LUT_OPCODE0(cmd0) | MG_FLEXSPI_LUT_OPERAND1(op1) |   \
   MG_FLEXSPI_LUT_NUM_PADS1(pad1) | MG_FLEXSPI_LUT_OPCODE1(cmd1))

#define MG_CMD_SDR 0x01
#define MG_CMD_DDR 0x21
#define MG_DUMMY_SDR 0x0C
#define MG_DUMMY_DDR 0x2C
#define MG_RADDR_SDR 0x02
#define MG_RADDR_DDR 0x22
#define MG_READ_SDR 0x09
#define MG_READ_DDR 0x29
#define MG_WRITE_SDR 0x08
#define MG_WRITE_DDR 0x28
#define MG_STOP 0

#define MG_FLEXSPI_1PAD 0
#define MG_FLEXSPI_2PAD 1
#define MG_FLEXSPI_4PAD 2
#define MG_FLEXSPI_8PAD 3

#define MG_FLEXSPI_QSPI_LUT                                                    \
  {                                                                            \
    [0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0xEB, MG_RADDR_SDR,  \
                             MG_FLEXSPI_4PAD, 0x18),                           \
    [1] = MG_FLEXSPI_LUT_SEQ(MG_DUMMY_SDR, MG_FLEXSPI_4PAD, 0x06, MG_READ_SDR, \
                             MG_FLEXSPI_4PAD, 0x04),                           \
    [4 * 1 + 0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x05,        \
                                     MG_READ_SDR, MG_FLEXSPI_1PAD, 0x04),      \
    [4 * 3 + 0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x06,        \
                                     MG_STOP, MG_FLEXSPI_1PAD, 0x0),           \
    [4 * 5 + 0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x20,        \
                                     MG_RADDR_SDR, MG_FLEXSPI_1PAD, 0x18),     \
    [4 * 8 + 0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0xD8,        \
                                     MG_RADDR_SDR, MG_FLEXSPI_1PAD, 0x18),     \
    [4 * 9 + 0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x02,        \
                                     MG_RADDR_SDR, MG_FLEXSPI_1PAD, 0x18),     \
    [4 * 9 + 1] = MG_FLEXSPI_LUT_SEQ(MG_WRITE_SDR, MG_FLEXSPI_1PAD, 0x04,      \
                                     MG_STOP, MG_FLEXSPI_1PAD, 0x0),           \
    [4 * 11 + 0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x60,       \
                                      MG_STOP, MG_FLEXSPI_1PAD, 0x0),          \
  }

#define MG_FLEXSPI_LUT_OPERAND0(x) (((uint32_t) (((uint32_t) (x)))) & 0xFFU)
#define MG_FLEXSPI_LUT_NUM_PADS0(x) \
  (((uint32_t) (((uint32_t) (x)) << 8U)) & 0x300U)
#define MG_FLEXSPI_LUT_OPCODE0(x) \
  (((uint32_t) (((uint32_t) (x)) << 10U)) & 0xFC00U)
#define MG_FLEXSPI_LUT_OPERAND1(x) \
  (((uint32_t) (((uint32_t) (x)) << 16U)) & 0xFF0000U)
#define MG_FLEXSPI_LUT_NUM_PADS1(x) \
  (((uint32_t) (((uint32_t) (x)) << 24U)) & 0x3000000U)
#define MG_FLEXSPI_LUT_OPCODE1(x) \
  (((uint32_t) (((uint32_t) (x)) << 26U)) & 0xFC000000U)

#if MG_OTA == MG_OTA_RT1020
// RT102X boards support ROM API version 1.4
struct mg_flexspi_nor_driver_interface {
  uint32_t version;
  int (*init)(uint32_t instance, struct mg_flexspi_nor_config *config);
  int (*program)(uint32_t instance, struct mg_flexspi_nor_config *config,
                 uint32_t dst_addr, const uint32_t *src);
  uint32_t reserved;
  int (*erase)(uint32_t instance, struct mg_flexspi_nor_config *config,
               uint32_t start, uint32_t lengthInBytes);
  uint32_t reserved2;
  int (*update_lut)(uint32_t instance, uint32_t seqIndex,
                    const uint32_t *lutBase, uint32_t seqNumber);
  int (*xfer)(uint32_t instance, char *xfer);
  void (*clear_cache)(uint32_t instance);
};
#elif MG_OTA <= MG_OTA_RT1064
// RT104x and RT106x support ROM API version 1.5
struct mg_flexspi_nor_driver_interface {
  uint32_t version;
  int (*init)(uint32_t instance, struct mg_flexspi_nor_config *config);
  int (*program)(uint32_t instance, struct mg_flexspi_nor_config *config,
                 uint32_t dst_addr, const uint32_t *src);
  int (*erase_all)(uint32_t instance, struct mg_flexspi_nor_config *config);
  int (*erase)(uint32_t instance, struct mg_flexspi_nor_config *config,
               uint32_t start, uint32_t lengthInBytes);
  int (*read)(uint32_t instance, struct mg_flexspi_nor_config *config,
              uint32_t *dst, uint32_t addr, uint32_t lengthInBytes);
  void (*clear_cache)(uint32_t instance);
  int (*xfer)(uint32_t instance, char *xfer);
  int (*update_lut)(uint32_t instance, uint32_t seqIndex,
                    const uint32_t *lutBase, uint32_t seqNumber);
  int (*get_config)(uint32_t instance, struct mg_flexspi_nor_config *config,
                    uint32_t *option);
};
#else
// RT117x support ROM API version 1.7
struct mg_flexspi_nor_driver_interface {
  uint32_t version;
  int (*init)(uint32_t instance, struct mg_flexspi_nor_config *config);
  int (*program)(uint32_t instance, struct mg_flexspi_nor_config *config,
                 uint32_t dst_addr, const uint32_t *src);
  int (*erase_all)(uint32_t instance, struct mg_flexspi_nor_config *config);
  int (*erase)(uint32_t instance, struct mg_flexspi_nor_config *config,
               uint32_t start, uint32_t lengthInBytes);
  int (*read)(uint32_t instance, struct mg_flexspi_nor_config *config,
              uint32_t *dst, uint32_t addr, uint32_t lengthInBytes);
  uint32_t reserved;
  int (*xfer)(uint32_t instance, char *xfer);
  int (*update_lut)(uint32_t instance, uint32_t seqIndex,
                    const uint32_t *lutBase, uint32_t seqNumber);
  int (*get_config)(uint32_t instance, struct mg_flexspi_nor_config *config,
                    uint32_t *option);
  int (*erase_sector)(uint32_t instance, struct mg_flexspi_nor_config *config,
                      uint32_t address);
  int (*erase_block)(uint32_t instance, struct mg_flexspi_nor_config *config,
                     uint32_t address);
  void (*hw_reset)(uint32_t instance, uint32_t resetLogic);
  int (*wait_busy)(uint32_t instance, struct mg_flexspi_nor_config *config,
                  bool isParallelMode, uint32_t address);
  int (*set_clock_source)(uint32_t instance, uint32_t clockSrc);
  void (*config_clock)(uint32_t instance, uint32_t freqOption,
                  uint32_t sampleClkMode);
};
#endif

#if MG_OTA <= MG_OTA_RT1064
#define MG_FLEXSPI_BASE 0x402A8000
#define flexspi_nor                                                          \
  (*((struct mg_flexspi_nor_driver_interface **) (*(uint32_t *) 0x0020001c + \
                                                  16)))
#else
#define MG_FLEXSPI_BASE 0x400CC000
#define flexspi_nor                                                          \
  (*((struct mg_flexspi_nor_driver_interface **) (*(uint32_t *) 0x0021001c + \
                                                  12)))
#endif

static bool s_flash_irq_disabled;

MG_IRAM static bool flash_page_start(volatile uint32_t *dst) {
  char *base = (char *) s_mg_flash_imxrt.start, *end = base + s_mg_flash_imxrt.size;
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % s_mg_flash_imxrt.secsz) == 0;
}

// Note: the get_config function below works both for RT1020 and 1060
// must reside in RAM, as flash will be erased
static struct mg_flexspi_nor_config default_config = {
  .memConfig = {.tag = MG_FLEXSPI_CFG_BLK_TAG,
                .version = MG_FLEXSPI_CFG_BLK_VERSION,
                .readSampleClkSrc = 1,  // ReadSampleClk_LoopbackFromDqsPad
                .csHoldTime = 3,
                .csSetupTime = 3,
                .controllerMiscOption = MG_BIT(4),
                .deviceType = 1,  // serial NOR
                .sflashPadType = 4,
                .serialClkFreq = 7,  // 133MHz
                .sflashA1Size = 8 * 1024 * 1024,
                .lookupTable = MG_FLEXSPI_QSPI_LUT},
  .pageSize = 256,
  .sectorSize = 4 * 1024,
  .ipcmdSerialClkFreq = 1,
  .blockSize = 64 * 1024,
  .isUniformBlockSize = false
};
MG_IRAM static int flexspi_nor_get_config(
  struct mg_flexspi_nor_config **config) {
  *config = &default_config;
  return 0;
}

#if 0
// ROM API get_config call (ROM version >= 1.5)
MG_IRAM static int flexspi_nor_get_config(
    struct mg_flexspi_nor_config **config) {
  uint32_t options[] = {0xc0000000, 0x00};

  MG_ARM_DISABLE_IRQ();
  uint32_t status =
      flexspi_nor->get_config(FLEXSPI_NOR_INSTANCE, *config, options);
  if (!s_flash_irq_disabled) {
    MG_ARM_ENABLE_IRQ();
  }
  if (status) {
    MG_ERROR(("Failed to extract flash configuration: status %u", status));
  }
  return status;
}
#endif

MG_IRAM static void mg_spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

MG_IRAM static void flash_wait(void) {
  while ((*((volatile uint32_t *) (MG_FLEXSPI_BASE + 0xE0)) & MG_BIT(1)) == 0)
    mg_spin(1);
}

MG_IRAM static bool flash_erase(struct mg_flexspi_nor_config *config,
                                void *addr) {
  if (flash_page_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
    return false;
  }

  void *dst = (void *) ((char *) addr - (char *) s_mg_flash_imxrt.start);

  bool ok = (flexspi_nor->erase(FLEXSPI_NOR_INSTANCE, config, (uint32_t) dst,
                                s_mg_flash_imxrt.secsz) == 0);
  MG_DEBUG(("Sector starting at %p erasure: %s", addr, ok ? "ok" : "fail"));
  return ok;
}

#if 0
// standalone erase call
MG_IRAM static bool mg_imxrt_erase(void *addr) {
  struct mg_flexspi_nor_config config, *config_ptr = &config;
  bool ret;
  // Interrupts must be disabled before calls to ROM API in RT1020 and 1060
  MG_ARM_DISABLE_IRQ();
  ret = (flexspi_nor_get_config(&config_ptr) == 0);
  if (ret) ret = flash_erase(config_ptr, addr);
  MG_ARM_ENABLE_IRQ();
  return ret;
}
#endif

MG_IRAM bool mg_imxrt_swap(void) {
  return true;
}

MG_IRAM static bool mg_imxrt_write(void *addr, const void *buf, size_t len) {
  struct mg_flexspi_nor_config config, *config_ptr = &config;
  bool ok = false;
  // Interrupts must be disabled before calls to ROM API in RT1020 and 1060
  MG_ARM_DISABLE_IRQ();
  if (flexspi_nor_get_config(&config_ptr) != 0) goto fwxit;
  if ((len % s_mg_flash_imxrt.align) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, s_mg_flash_imxrt.align));
    goto fwxit;
  }
  if ((char *) addr < (char *) s_mg_flash_imxrt.start) {
    MG_ERROR(("Invalid flash write address: %p", addr));
    goto fwxit;
  }

  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  ok = true;

  while (ok && src < end) {
    if (flash_page_start(dst) && flash_erase(config_ptr, dst) == false) {
      ok = false;
      break;
    }
    uint32_t status;
    uint32_t dst_ofs = (uint32_t) dst - (uint32_t) s_mg_flash_imxrt.start;
    if ((char *) buf >= (char *) s_mg_flash_imxrt.start) {
      // If we copy from FLASH to FLASH, then we first need to copy the source
      // to RAM
      size_t tmp_buf_size = s_mg_flash_imxrt.align / sizeof(uint32_t);
      uint32_t tmp[tmp_buf_size];

      for (size_t i = 0; i < tmp_buf_size; i++) {
        flash_wait();
        tmp[i] = src[i];
      }
      status = flexspi_nor->program(FLEXSPI_NOR_INSTANCE, config_ptr,
                                    (uint32_t) dst_ofs, tmp);
    } else {
      status = flexspi_nor->program(FLEXSPI_NOR_INSTANCE, config_ptr,
                                    (uint32_t) dst_ofs, src);
    }
    src = (uint32_t *) ((char *) src + s_mg_flash_imxrt.align);
    dst = (uint32_t *) ((char *) dst + s_mg_flash_imxrt.align);
    if (status != 0) {
      ok = false;
    }
  }
  MG_DEBUG(("Flash write %lu bytes @ %p: %s.", len, dst, ok ? "ok" : "fail"));
fwxit:
  if (!s_flash_irq_disabled) MG_ARM_ENABLE_IRQ();
  return ok;
}

// just overwrite instead of swap
MG_IRAM static void single_bank_swap(char *p1, char *p2, size_t s, size_t ss) {
  // no stdlib calls here
  for (size_t ofs = 0; ofs < s; ofs += ss) {
    mg_imxrt_write(p1 + ofs, p2 + ofs, ss);
  }
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}

bool mg_ota_begin(size_t new_firmware_size) {
  return mg_ota_flash_begin(new_firmware_size, &s_mg_flash_imxrt);
}

bool mg_ota_write(const void *buf, size_t len) {
  return mg_ota_flash_write(buf, len, &s_mg_flash_imxrt);
}

bool mg_ota_end(void) {
  if (mg_ota_flash_end(&s_mg_flash_imxrt)) {
    if (0) {  // is_dualbank()
      // TODO(): no devices so far
      *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
    } else {
      // Swap partitions. Pray power does not go away
      MG_INFO(("Swapping partitions, size %u (%u sectors)",
               s_mg_flash_imxrt.size,
               s_mg_flash_imxrt.size / s_mg_flash_imxrt.secsz));
      MG_INFO(("Do NOT power off..."));
      mg_log_level = MG_LL_NONE;
      s_flash_irq_disabled = true;
      // Runs in RAM, will reset when finished
      single_bank_swap(
          (char *) s_mg_flash_imxrt.start,
          (char *) s_mg_flash_imxrt.start + s_mg_flash_imxrt.size / 2,
          s_mg_flash_imxrt.size / 2, s_mg_flash_imxrt.secsz);
    }
  }
  return false;
}

#endif
