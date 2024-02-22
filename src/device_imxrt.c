#include "device.h"
#include "log.h"

#if MG_DEVICE == MG_DEVICE_RT1020 || MG_DEVICE == MG_DEVICE_RT1060

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

#define MG_FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                      \
  (MG_FLEXSPI_LUT_OPERAND0(op0) | MG_FLEXSPI_LUT_NUM_PADS0(pad0) | MG_FLEXSPI_LUT_OPCODE0(cmd0) | \
   MG_FLEXSPI_LUT_OPERAND1(op1) | MG_FLEXSPI_LUT_NUM_PADS1(pad1) | MG_FLEXSPI_LUT_OPCODE1(cmd1))

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

#define MG_FLEXSPI_QSPI_LUT                                                                        \
  {                                                                                                \
    [0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0xEB, MG_RADDR_SDR, MG_FLEXSPI_4PAD,     \
                             0x18),                                                                \
    [1] = MG_FLEXSPI_LUT_SEQ(MG_DUMMY_SDR, MG_FLEXSPI_4PAD, 0x06, MG_READ_SDR, MG_FLEXSPI_4PAD,    \
                             0x04),                                                                \
    [4 * 1 + 0] =                                                                                  \
        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x05, MG_READ_SDR, MG_FLEXSPI_1PAD, 0x04), \
    [4 * 3 + 0] =                                                                                  \
        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x06, MG_STOP, MG_FLEXSPI_1PAD, 0x0),      \
    [4 * 5 + 0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x20, MG_RADDR_SDR,              \
                                     MG_FLEXSPI_1PAD, 0x18),                                       \
    [4 * 8 + 0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0xD8, MG_RADDR_SDR,              \
                                     MG_FLEXSPI_1PAD, 0x18),                                       \
    [4 * 9 + 0] = MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x02, MG_RADDR_SDR,              \
                                     MG_FLEXSPI_1PAD, 0x18),                                       \
    [4 * 9 + 1] =                                                                                  \
        MG_FLEXSPI_LUT_SEQ(MG_WRITE_SDR, MG_FLEXSPI_1PAD, 0x04, MG_STOP, MG_FLEXSPI_1PAD, 0x0),    \
    [4 * 11 + 0] =                                                                                 \
        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x60, MG_STOP, MG_FLEXSPI_1PAD, 0x0),      \
  }

#define MG_FLEXSPI_LUT_OPERAND0(x) (((uint32_t) (((uint32_t) (x)))) & 0xFFU)
#define MG_FLEXSPI_LUT_NUM_PADS0(x) (((uint32_t) (((uint32_t) (x)) << 8U)) & 0x300U)
#define MG_FLEXSPI_LUT_OPCODE0(x) (((uint32_t) (((uint32_t) (x)) << 10U)) & 0xFC00U)
#define MG_FLEXSPI_LUT_OPERAND1(x) (((uint32_t) (((uint32_t) (x)) << 16U)) & 0xFF0000U)
#define MG_FLEXSPI_LUT_NUM_PADS1(x) (((uint32_t) (((uint32_t) (x)) << 24U)) & 0x3000000U)
#define MG_FLEXSPI_LUT_OPCODE1(x) (((uint32_t) (((uint32_t) (x)) << 26U)) & 0xFC000000U)

#define FLEXSPI_NOR_INSTANCE 0

#if MG_DEVICE == MG_DEVICE_RT1020
struct mg_flexspi_nor_driver_interface {
  uint32_t version;
  int (*init)(uint32_t instance, struct mg_flexspi_nor_config *config);
  int (*program)(uint32_t instance, struct mg_flexspi_nor_config *config, uint32_t dst_addr,
                 const uint32_t *src);
  uint32_t reserved;
  int (*erase)(uint32_t instance, struct mg_flexspi_nor_config *config, uint32_t start,
               uint32_t lengthInBytes);
  uint32_t reserved2;
  int (*update_lut)(uint32_t instance, uint32_t seqIndex, const uint32_t *lutBase,
                    uint32_t seqNumber);
  int (*xfer)(uint32_t instance, char *xfer);
  void (*clear_cache)(uint32_t instance);
};
#elif MG_DEVICE == MG_DEVICE_RT1060
struct mg_flexspi_nor_driver_interface {
  uint32_t version;
  int (*init)(uint32_t instance, struct mg_flexspi_nor_config *config);
  int (*program)(uint32_t instance, struct mg_flexspi_nor_config *config, uint32_t dst_addr,
                 const uint32_t *src);
  int (*erase_all)(uint32_t instance, struct mg_flexspi_nor_config *config);
  int (*erase)(uint32_t instance, struct mg_flexspi_nor_config *config, uint32_t start,
               uint32_t lengthInBytes);
  int (*read)(uint32_t instance, struct mg_flexspi_nor_config *config, uint32_t *dst, uint32_t addr,
              uint32_t lengthInBytes);
  void (*clear_cache)(uint32_t instance);
  int (*xfer)(uint32_t instance, char *xfer);
  int (*update_lut)(uint32_t instance, uint32_t seqIndex, const uint32_t *lutBase,
                    uint32_t seqNumber);
  int (*get_config)(uint32_t instance, struct mg_flexspi_nor_config *config, uint32_t *option);
};
#endif

#define flexspi_nor (*((struct mg_flexspi_nor_driver_interface**) \
                          (*(uint32_t*)0x0020001c + 16)))

static bool s_flash_irq_disabled;

MG_IRAM void *mg_flash_start(void) {
  return (void *) 0x60000000;
}
MG_IRAM size_t mg_flash_size(void) {
  return 8 * 1024 * 1024;
}
MG_IRAM size_t mg_flash_sector_size(void) {
  return 4 * 1024;  // 4k
}
MG_IRAM size_t mg_flash_write_align(void) {
  return 256;
}
MG_IRAM int mg_flash_bank(void) {
  return 0;
}

MG_IRAM static bool flash_page_start(volatile uint32_t *dst) {
  char *base = (char *) mg_flash_start(), *end = base + mg_flash_size();
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % mg_flash_sector_size()) == 0;
}

// Note: the get_config function below works both for RT1020 and 1060
#if MG_DEVICE == MG_DEVICE_RT1020
MG_IRAM static int flexspi_nor_get_config(struct mg_flexspi_nor_config *config) {
  struct mg_flexspi_nor_config default_config = {
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
      .isUniformBlockSize = false};

  *config = default_config;
  return 0;
}
#else
MG_IRAM static int flexspi_nor_get_config(struct mg_flexspi_nor_config *config) {
  uint32_t options[] = {0xc0000000, 0x00};

  MG_ARM_DISABLE_IRQ();
  uint32_t status =
      flexspi_nor->get_config(FLEXSPI_NOR_INSTANCE, config, options);
  if (!s_flash_irq_disabled) {
    MG_ARM_ENABLE_IRQ();
  }
  if (status) {
    MG_ERROR(("Failed to extract flash configuration: status %u", status));
  }
  return status;
}
#endif

MG_IRAM bool mg_flash_erase(void *addr) {
  struct mg_flexspi_nor_config config;
  if (flexspi_nor_get_config(&config) != 0) {
    return false;
  }
  if (flash_page_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
    return false;
  }

  void *dst = (void *)((char *) addr - (char *) mg_flash_start());

  // Note: Interrupts must be disabled before any call to the ROM API on RT1020
  // and 1060
  MG_ARM_DISABLE_IRQ();
  bool ok = (flexspi_nor->erase(FLEXSPI_NOR_INSTANCE, &config, (uint32_t) dst,
                                mg_flash_sector_size()) == 0);
  if (!s_flash_irq_disabled) {
    MG_ARM_ENABLE_IRQ();  // Reenable them after the call
  }
  MG_DEBUG(("Sector starting at %p erasure: %s", addr, ok ? "ok" : "fail"));
  return ok;
}

MG_IRAM bool mg_flash_swap_bank(void) {
  return true;
}

static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

static inline void flash_wait(void) {
  while ((*((volatile uint32_t *)(0x402A8000 + 0xE0)) & MG_BIT(1)) == 0)
    spin(1);
}

MG_IRAM static void *flash_code_location(void) {
  return (void *) ((char *) mg_flash_start() + 0x2000);
}

MG_IRAM bool mg_flash_write(void *addr, const void *buf, size_t len) {
  struct mg_flexspi_nor_config config;
  if (flexspi_nor_get_config(&config) != 0) {
    return false;
  }
  if ((len % mg_flash_write_align()) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, mg_flash_write_align()));
    return false;
  }

  if ((char *) addr < (char *) mg_flash_start()) {
    MG_ERROR(("Invalid flash write address: %p", addr));
    return false;
  }

  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  bool ok = true;

  // Note: If we overwrite the flash irq section of the image, we must also
  // make sure interrupts are disabled and are not reenabled until we write
  // this sector with another irq table.
  if ((char *) addr == (char *) flash_code_location()) {
    s_flash_irq_disabled = true;
    MG_ARM_DISABLE_IRQ();
  }

  while (ok && src < end) {
    if (flash_page_start(dst) && mg_flash_erase(dst) == false) {
      break;
    }
    uint32_t status;
    uint32_t dst_ofs = (uint32_t) dst - (uint32_t) mg_flash_start();
    if ((char *) buf >= (char *) mg_flash_start()) {
      // If we copy from FLASH to FLASH, then we first need to copy the source
      // to RAM
      size_t tmp_buf_size = mg_flash_write_align() / sizeof(uint32_t);
      uint32_t tmp[tmp_buf_size];

      for (size_t i = 0; i < tmp_buf_size; i++) {
        flash_wait();
        tmp[i] = src[i];
      }
      MG_ARM_DISABLE_IRQ();
      status = flexspi_nor->program(FLEXSPI_NOR_INSTANCE, &config,
                                    (uint32_t) dst_ofs, tmp);
    } else {
      MG_ARM_DISABLE_IRQ();
      status = flexspi_nor->program(FLEXSPI_NOR_INSTANCE, &config,
                                    (uint32_t) dst_ofs, src);
    }
    if (!s_flash_irq_disabled) {
      MG_ARM_ENABLE_IRQ();
    }
    src = (uint32_t *) ((char *) src + mg_flash_write_align());
    dst = (uint32_t *) ((char *) dst + mg_flash_write_align());
    if (status != 0) {
      ok = false;
    }
  }
  MG_DEBUG(("Flash write %lu bytes @ %p: %s.", len, dst, ok ? "ok" : "fail"));
  return ok;
}

MG_IRAM void mg_device_reset(void) {
  MG_DEBUG(("Resetting device..."));
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}

#endif
