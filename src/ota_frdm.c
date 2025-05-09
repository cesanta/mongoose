#include "flash.h"
#include "log.h"
#include "ota.h"

#if MG_OTA == MG_OTA_FRDM

MG_IRAM static bool mg_frdm_write(void *, const void *, size_t);
static bool mg_frdm_swap(void);

static struct mg_flash s_mg_flash_frdm = {(void *) 0x08000000,  // Start,
                                          0x200000,             // Size
                                          0x1000,               // Sector size
                                          0x100,                // Align
                                          mg_frdm_write,
                                          mg_frdm_swap};

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
  uint8_t isDataOrderSwapped;
  uint8_t reserved0[1];
  uint8_t serialNorType;
  uint8_t needExitNoCmdMode;
  uint8_t halfClkForNonReadCmd;
  uint8_t needRestoreNoCmdMode;
  uint32_t blockSize;
  uint32_t flashStateCtx;
  uint32_t reserve2[10];
};

struct mg_flexspi_nor_driver_interface {
  uint32_t version;
  uint32_t (*init)(uint32_t instance, struct mg_flexspi_nor_config *config);
  uint32_t (*wait_busy)(uint32_t instance, struct mg_flexspi_nor_config *config,
                        uint32_t address, bool keepState);
  uint32_t (*page_program)(uint32_t instance,
                           struct mg_flexspi_nor_config *config,
                           uint32_t dstAddr, const uint32_t *src,
                           bool keepState);
  uint32_t (*erase_all)(uint32_t instance,
                        struct mg_flexspi_nor_config *config);
  uint32_t (*erase)(uint32_t instance, struct mg_flexspi_nor_config *config,
                    uint32_t start, uint32_t length);
  uint32_t (*erase_sector)(uint32_t instance,
                           struct mg_flexspi_nor_config *config,
                           uint32_t address);
  uint32_t (*erase_block)(uint32_t instance,
                          struct mg_flexspi_nor_config *config,
                          uint32_t address);
  uint32_t (*read)(uint32_t instance, struct mg_flexspi_nor_config *config,
                   uint32_t *dst, uint32_t start, uint32_t bytes);
  void (*config_clock)(uint32_t instance, uint32_t freqOption,
                       uint32_t sampleClkMode);
  uint32_t (*set_clock_source)(uint32_t clockSrc);
  uint32_t (*get_config)(uint32_t instance,
                         struct mg_flexspi_nor_config *config,
                         uint32_t *option);
  void (*hw_reset)(uint32_t instance, uint32_t reset_logic);
  uint32_t (*xfer)(uint32_t instance, char *xfer);
  uint32_t (*update_lut)(uint32_t instance, uint32_t seqIndex,
                         const uint32_t *lutBase, uint32_t numberOfSeq);
  uint32_t (*partial_program)(uint32_t instance,
                              struct mg_flexspi_nor_config *config,
                              uint32_t dstAddr, const uint32_t *src,
                              uint32_t length, bool keepState);
};

#define MG_FLEXSPI_CFG_BLK_TAG (0x42464346UL)
#define MG_FLEXSPI_BASE 0x40134000UL

#define MG_CMD_SDR 0x01
#define MG_RADDR_SDR 0x02
#define MG_WRITE_SDR 0x08
#define MG_READ_SDR 0x09
#define MG_DUMMY_SDR 0x0C
#define MG_STOP_EXE 0

#define MG_FLEXSPI_1PAD 0
#define MG_FLEXSPI_4PAD 2

#define MG_FLEXSPI_LUT_OPERAND0(x) (((x) &0xFF) << 0)
#define MG_FLEXSPI_LUT_NUM_PADS0(x) (((x) &0x3) << 8)
#define MG_FLEXSPI_LUT_OPCODE0(x) (((x) &0x3F) << 10)
#define MG_FLEXSPI_LUT_OPERAND1(x) (((x) &0xFF) << 16)
#define MG_FLEXSPI_LUT_NUM_PADS1(x) (((x) &0x3) << 24)
#define MG_FLEXSPI_LUT_OPCODE1(x) (((x) &0x3F) << 26)

#define MG_FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)       \
  (MG_FLEXSPI_LUT_OPERAND0(op0) | MG_FLEXSPI_LUT_NUM_PADS0(pad0) | \
   MG_FLEXSPI_LUT_OPCODE0(cmd0) | MG_FLEXSPI_LUT_OPERAND1(op1) |   \
   MG_FLEXSPI_LUT_NUM_PADS1(pad1) | MG_FLEXSPI_LUT_OPCODE1(cmd1))

struct mg_flexspi_nor_config default_config = {
    .memConfig =
        {
            .tag = MG_FLEXSPI_CFG_BLK_TAG,
            .version = 0,
            .readSampleClkSrc = 1,
            .csHoldTime = 3,
            .csSetupTime = 3,
            .deviceModeCfgEnable = 1,
            .deviceModeSeq = {.seqNum = 1, .seqId = 2},
            .deviceModeArg = 0x0740,
            .configCmdEnable = 0,
            .deviceType = 0x1,
            .sflashPadType = 4,
            .serialClkFreq = 4,
            .sflashA1Size = 0x4000000U,
            .sflashA2Size = 0,
            .sflashB1Size = 0,
            .sflashB2Size = 0,
            .lookupTable =
                {
                    [0] =
                        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0xEB,
                                           MG_RADDR_SDR, MG_FLEXSPI_4PAD, 0x18),
                    [1] =
                        MG_FLEXSPI_LUT_SEQ(MG_DUMMY_SDR, MG_FLEXSPI_4PAD, 0x06,
                                           MG_READ_SDR, MG_FLEXSPI_4PAD, 0x04),
                    [4 * 1 + 0] =
                        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x05,
                                           MG_READ_SDR, MG_FLEXSPI_1PAD, 0x04),
                    [4 * 2 + 0] =
                        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x01,
                                           MG_WRITE_SDR, MG_FLEXSPI_1PAD, 0x02),
                    [4 * 3 + 0] =
                        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x06,
                                           MG_STOP_EXE, MG_FLEXSPI_1PAD, 0x00),
                    [4 * 5 + 0] =
                        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x20,
                                           MG_RADDR_SDR, MG_FLEXSPI_1PAD, 0x18),
                    [4 * 8 + 0] =
                        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x52,
                                           MG_RADDR_SDR, MG_FLEXSPI_1PAD, 0x18),
                    [4 * 9 + 0] =
                        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x02,
                                           MG_RADDR_SDR, MG_FLEXSPI_1PAD, 0x18),
                    [4 * 9 + 1] =
                        MG_FLEXSPI_LUT_SEQ(MG_WRITE_SDR, MG_FLEXSPI_1PAD, 0x00,
                                           MG_STOP_EXE, MG_FLEXSPI_1PAD, 0x00),
                    [4 * 11 + 0] =
                        MG_FLEXSPI_LUT_SEQ(MG_CMD_SDR, MG_FLEXSPI_1PAD, 0x60,
                                           MG_STOP_EXE, MG_FLEXSPI_1PAD, 0x00),
                },
        },
    .pageSize = 0x100,
    .sectorSize = 0x1000,
    .ipcmdSerialClkFreq = 0,
    .blockSize = 0x8000,
};

#define MG_FLEXSPI_NOR_INSTANCE 0
#define MG_ROMAPI_ADDRESS 0x13030000U
#define flexspi_nor                              \
  ((struct mg_flexspi_nor_driver_interface *) (( \
      (uint32_t *) MG_ROMAPI_ADDRESS)[5]))

MG_IRAM static bool flash_page_start(volatile uint32_t *dst) {
  char *base = (char *) s_mg_flash_frdm.start,
       *end = base + s_mg_flash_frdm.size;
  volatile char *p = (char *) dst;
  return p >= base && p < end && ((p - base) % s_mg_flash_frdm.secsz) == 0;
}

MG_IRAM static int flexspi_nor_get_config(
    struct mg_flexspi_nor_config *config) {
  uint32_t option = 0xc0000004;
  return flexspi_nor->get_config(MG_FLEXSPI_NOR_INSTANCE, config, &option);
}

MG_IRAM static int flash_init(void) {
  static bool initialized = false;
  if (!initialized) {
    struct mg_flexspi_nor_config config;
    memset(&config, 0, sizeof(config));
    flexspi_nor->set_clock_source(0);
    flexspi_nor->config_clock(MG_FLEXSPI_NOR_INSTANCE, 1, 0);
    if (flexspi_nor->init(MG_FLEXSPI_NOR_INSTANCE, &default_config)) {
      return 1;
    }
    flexspi_nor_get_config(&config);
    if (flexspi_nor->init(MG_FLEXSPI_NOR_INSTANCE, &config)) {
      return 1;
    }
    initialized = true;
  }
  return 0;
}

MG_IRAM static bool flash_erase(struct mg_flexspi_nor_config *config,
                                void *addr) {
  if (flash_page_start(addr) == false) {
    MG_ERROR(("%p is not on a sector boundary", addr));
    return false;
  }

  void *dst = (void *) ((char *) addr - (char *) s_mg_flash_frdm.start);
  bool ok = (flexspi_nor->erase_sector(MG_FLEXSPI_NOR_INSTANCE, config,
                                       (uint32_t) dst) == 0);
  MG_INFO(("Sector starting at %p erasure: %s", addr, ok ? "ok" : "fail"));
  return ok;
}

MG_IRAM bool mg_frdm_swap(void) {
  return true;
}

MG_IRAM static void flash_wait(void) {
  while ((*((volatile uint32_t *) (MG_FLEXSPI_BASE + 0xE0)) & MG_BIT(1)) == 0)
    (void) 0;
}

static bool s_flash_irq_disabled;

MG_IRAM static bool mg_frdm_write(void *addr, const void *buf, size_t len) {
  struct mg_flexspi_nor_config config;
  bool ok = false;
  MG_ARM_DISABLE_IRQ();
  if (flash_init() != 0) goto fwxit;
  if (flexspi_nor_get_config(&config) != 0) goto fwxit;
  if ((len % s_mg_flash_frdm.align) != 0) {
    MG_ERROR(("%lu is not aligned to %lu", len, s_mg_flash_frdm.align));
    goto fwxit;
  }
  if ((char *) addr < (char *) s_mg_flash_frdm.start) {
    MG_ERROR(("Invalid flash write address: %p", addr));
    goto fwxit;
  }

  uint32_t *dst = (uint32_t *) addr;
  uint32_t *src = (uint32_t *) buf;
  uint32_t *end = (uint32_t *) ((char *) buf + len);
  ok = true;

  while (ok && src < end) {
    if (flash_page_start(dst) && flash_erase(&config, dst) == false) {
      ok = false;
      break;
    }
    uint32_t status;
    uint32_t dst_ofs = (uint32_t) dst - (uint32_t) s_mg_flash_frdm.start;
    if ((char *) buf >= (char *) s_mg_flash_frdm.start &&
        (char *) buf <
            (char *) (s_mg_flash_frdm.start + s_mg_flash_frdm.size)) {
      // If we copy from FLASH to FLASH, then we first need to copy the source
      // to RAM
      size_t tmp_buf_size = s_mg_flash_frdm.align / sizeof(uint32_t);
      uint32_t tmp[tmp_buf_size];

      for (size_t i = 0; i < tmp_buf_size; i++) {
        flash_wait();
        tmp[i] = src[i];
      }
      status = flexspi_nor->page_program(MG_FLEXSPI_NOR_INSTANCE, &config,
                                         (uint32_t) dst_ofs, tmp, false);
    } else {
      status = flexspi_nor->page_program(MG_FLEXSPI_NOR_INSTANCE, &config,
                                         (uint32_t) dst_ofs, src, false);
    }
    src = (uint32_t *) ((char *) src + s_mg_flash_frdm.align);
    dst = (uint32_t *) ((char *) dst + s_mg_flash_frdm.align);
    if (status != 0) {
      ok = false;
    }
  }
  MG_INFO(("Flash write %lu bytes @ %p: %s.", len, dst, ok ? "ok" : "fail"));
fwxit:
  if (!s_flash_irq_disabled) MG_ARM_ENABLE_IRQ();
  return ok;
}

// just overwrite instead of swap
MG_IRAM static void single_bank_swap(char *p1, char *p2, size_t s, size_t ss) {
  // no stdlib calls here
  for (size_t ofs = 0; ofs < s; ofs += ss) {
    mg_frdm_write(p1 + ofs, p2 + ofs, ss);
  }
  *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
}

bool mg_ota_begin(size_t new_firmware_size) {
  return mg_ota_flash_begin(new_firmware_size, &s_mg_flash_frdm);
}

bool mg_ota_write(const void *buf, size_t len) {
  return mg_ota_flash_write(buf, len, &s_mg_flash_frdm);
}

bool mg_ota_end(void) {
  if (mg_ota_flash_end(&s_mg_flash_frdm)) {
    if (0) {  // is_dualbank()
      // TODO(): no devices so far
      *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;
    } else {
      // Swap partitions. Pray power does not go away
      MG_INFO(("Swapping partitions, size %u (%u sectors)",
               s_mg_flash_frdm.size,
               s_mg_flash_frdm.size / s_mg_flash_frdm.secsz));
      MG_INFO(("Do NOT power off..."));
      mg_log_level = MG_LL_NONE;
      s_flash_irq_disabled = true;
      // Runs in RAM, will reset when finished
      single_bank_swap(
          (char *) s_mg_flash_frdm.start,
          (char *) s_mg_flash_frdm.start + s_mg_flash_frdm.size / 2,
          s_mg_flash_frdm.size / 2, s_mg_flash_frdm.secsz);
    }
  }
  return false;
}

#endif
