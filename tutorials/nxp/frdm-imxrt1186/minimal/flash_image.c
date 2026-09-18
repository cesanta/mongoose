#include <stdint.h>

#define LUT_OPERAND0(x) ((uint32_t) (x) << 0)
#define LUT_NUM_PADS0(x) ((uint32_t) (x) << 8)
#define LUT_OPCODE0(x) ((uint32_t) (x) << 10)
#define LUT_OPERAND1(x) ((uint32_t) (x) << 16)
#define LUT_NUM_PADS1(x) ((uint32_t) (x) << 24)
#define LUT_OPCODE1(x) ((uint32_t) (x) << 26)
#define LUT_SEQ(cmd0, pads0, operand0, cmd1, pads1, operand1)              \
  (LUT_OPERAND0(operand0) | LUT_NUM_PADS0(pads0) | LUT_OPCODE0(cmd0) |    \
   LUT_OPERAND1(operand1) | LUT_NUM_PADS1(pads1) | LUT_OPCODE1(cmd1))

enum {
  LUT_STOP = 0x00,
  LUT_CMD_SDR = 0x01,
  LUT_RADDR_SDR = 0x02,
  LUT_WRITE_SDR = 0x08,
  LUT_READ_SDR = 0x09,
  LUT_DUMMY_SDR = 0x0c,
  LUT_1_PAD = 0,
  LUT_4_PADS = 2,
};

// FlexSPI2 NOR configuration block for the FRDM-IMXRT1186 QSPI flash
// The ROM expects this 512-byte structure at flash offset 0x400
__attribute__((section(".boot_hdr.conf"), used))
const uint32_t qspi_flash_nor_config[128] = {
    [0] = 0x42464346,   // Tag: "FCFB"
    [1] = 0x56010400,   // Version 1.4.0
    [3] = 0x00030301,   // DQS loopback, CS hold/setup = 3
    [16] = 0x00000010,  // Safe configuration
    [17] = 0x00050401,  // Serial NOR, 4 pads, 100 MHz
    [20] = 16U * 1024U * 1024U,

    // Read
    [32] =
        LUT_SEQ(LUT_CMD_SDR, LUT_1_PAD, 0xeb, LUT_RADDR_SDR, LUT_4_PADS, 0x18),
    [33] =
        LUT_SEQ(LUT_DUMMY_SDR, LUT_4_PADS, 0x06, LUT_READ_SDR, LUT_4_PADS, 0x04),

    // Read status
    [36] =
        LUT_SEQ(LUT_CMD_SDR, LUT_1_PAD, 0x05, LUT_READ_SDR, LUT_1_PAD, 0x04),

    // Write enable
    [44] =
        LUT_SEQ(LUT_CMD_SDR, LUT_1_PAD, 0x06, LUT_STOP, LUT_1_PAD, 0x00),

    // Erase 4-KiB sector
    [52] =
        LUT_SEQ(LUT_CMD_SDR, LUT_1_PAD, 0x20, LUT_RADDR_SDR, LUT_1_PAD, 0x18),

    // Erase 64-KiB block
    [64] =
        LUT_SEQ(LUT_CMD_SDR, LUT_1_PAD, 0xd8, LUT_RADDR_SDR, LUT_1_PAD, 0x18),

    // Program page
    [68] =
        LUT_SEQ(LUT_CMD_SDR, LUT_1_PAD, 0x02, LUT_RADDR_SDR, LUT_1_PAD, 0x18),
    [69] =
        LUT_SEQ(LUT_WRITE_SDR, LUT_1_PAD, 0x04, LUT_STOP, LUT_1_PAD, 0x00),

    // Erase chip
    [76] =
        LUT_SEQ(LUT_CMD_SDR, LUT_1_PAD, 0x60, LUT_STOP, LUT_1_PAD, 0x00),

    [112] = 256,           // Page size
    [113] = 4U * 1024U,   // Sector size
    [114] = 1,             // IP-command serial clock
    [116] = 64U * 1024U,  // Block size
};

typedef struct __attribute__((packed)) {
  uint8_t version;
  uint16_t length;
  uint8_t tag;
  uint32_t flags;
  uint16_t software_version;
  uint8_t fuse_version;
  uint8_t image_count;
  uint16_t signature_block_offset;
  uint16_t reserved;
} container_header_t;

typedef struct __attribute__((packed)) {
  uint32_t offset;
  uint32_t size;
  uint32_t load_address;
  uint32_t reserved1;
  uint32_t entry_address;
  uint32_t reserved2;
  uint32_t flags;
  uint32_t metadata;
  uint8_t hash[64];
  uint8_t iv[32];
} container_image_t;

typedef struct __attribute__((packed)) {
  uint8_t version;
  uint16_t length;
  uint8_t tag;
  uint16_t certificate_offset;
  uint16_t srk_table_offset;
  uint16_t signature_offset;
  uint16_t blob_offset;
  uint32_t reserved;
} signature_block_t;

typedef struct __attribute__((packed)) {
  container_header_t header;
  container_image_t image[1];
  signature_block_t signature;
} boot_container_t;

extern const uint32_t _image_offset[];
extern const uint32_t _image_size[];
extern const uint32_t _image_loadaddr[];
extern const uint32_t Reset_Handler[];

// RT1180 Program Image Container. The ROM expects it at flash offset 0x1000
__attribute__((section(".boot_hdr.container"), used))
const boot_container_t container_data = {
  .header = {
    .version = 0,
    .length = sizeof(boot_container_t),
    .tag = 0x87,
    .flags = 0,
    .software_version = 0,
    .fuse_version = 0,
    .image_count = 1,
    .signature_block_offset =
      sizeof(container_header_t) + sizeof(container_image_t),
    .reserved = 0,
  },
  .image =  {{
    .offset = (uint32_t) _image_offset,
    .size = (uint32_t) _image_size,
    .load_address = (uint32_t) _image_loadaddr,
    .reserved1 = 0,
    .entry_address = (uint32_t) Reset_Handler,
    .reserved2 = 0,
    .flags = 0x00000213,  // Plain SHA-512 CM33 executable image
    .metadata = 0,
    .hash = {0},
    .iv = {0},
  }},
  .signature = {
    .version = 0,
    .length = sizeof(signature_block_t),
    .tag = 0x90,
    .certificate_offset = 0,
    .srk_table_offset = 0,
    .signature_offset = 0,
    .blob_offset = 0,
    .reserved = 0,
  },
};
