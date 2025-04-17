#include "sdio.h"

#if MG_ENABLE_TCPIP && \
    (defined(MG_ENABLE_DRIVER_CYW_SDIO) && MG_ENABLE_DRIVER_CYW_SDIO)

// SDIO 6.9 Table 6-1 CCCR (Common Card Control Registers)
#define MG_SDIO_CCCR_SDIOREV 0x000
#define MG_SDIO_CCCR_SDREV 0x001
#define MG_SDIO_CCCR_IOEN 0x002
#define MG_SDIO_CCCR_IORDY 0x003
#define MG_SDIO_CCCR_INTEN 0x004
#define MG_SDIO_CCCR_BIC 0x007
#define MG_SDIO_CCCR_CCAP 0x008
#define MG_SDIO_CCCR_CCIS 0x009     // 3 registers
#define MG_SDIO_CCCR_F0BLKSZ 0x010  // 2 registers
#define MG_SDIO_CCCR_HISPD 0x013
// SDIO 6.10 Table 6-3 FBR (Function Basic Registers)
#define MG_SDIO_FBR_FnBLKSZ(n) (((n) &7) * 0x100 + 0x10)  // 2 registers

// SDIO 5.1 IO_RW_DIRECT Command (CMD52)
#define MG_SDIO_DATA(x) ((x) &0xFF)            // bits 0-7
#define MG_SDIO_ADDR(x) (((x) &0x1FFFF) << 9)  // bits 9-25
#define MG_SDIO_FUNC(x) (((x) &3) << 28)       // bits 28-30 (30 unused here)
#define MG_SDIO_WR MG_BIT(31)

// SDIO 5.3 IO_RW_EXTENDED Command (CMD53)
#define MG_SDIO_LEN(x) ((x) &0x1FF)  // bits 0-8
#define MG_SDIO_OPINC MG_BIT(26)
#define MG_SDIO_BLKMODE MG_BIT(27)

// - Drivers set blocksize, drivers request transfers. Requesting a read
// transfer > blocksize means block transfer will be used.
// - To simplify the use of DMA transfers and avoid intermediate buffers,
// drivers must have room to accomodate a whole block transfer, e.g.: blocksize
// = 64, read 65 => 2 blocks = 128 bytes
// - Transfers of more than 1 byte assume (uint32_t *) data. 1-byte transfers
// use (uint8_t *) data
// - 'len' is the number of _bytes_ to transfer
bool mg_sdio_transfer(struct mg_tcpip_sdio *sdio, bool write, unsigned int f,
                      uint32_t addr, void *data, uint32_t len) {
  uint32_t arg, val = 0;
  unsigned int blksz = 64;  // TODO(): mg_sdio_set_blksz() stores in an array,
                            // index on f, skip if 0
  if (len == 1) {
    arg = (write ? MG_SDIO_WR : 0) | MG_SDIO_FUNC(f) | MG_SDIO_ADDR(addr) |
          (write ? MG_SDIO_DATA(*(uint8_t *) data) : 0);
    bool res = sdio->txn(sdio, 52, arg, &val);  // IO_RW_DIRECT
    if (!write) *(uint8_t *) data = (uint8_t) val;
    return res;
  }
  // IO_RW_EXTENDED
  arg = (write ? MG_SDIO_WR : 0) | MG_SDIO_OPINC | MG_SDIO_FUNC(f) |
        MG_SDIO_ADDR(addr);
  if (len > 512 || (blksz != 0 && len > blksz)) {  // SDIO 5.3 512 -> len=0
    unsigned int blkcnt;
    if (blksz == 0) return false;  // > 512 requires block size set
    blkcnt = (len + blksz - 1) / blksz;
    if (blkcnt > 511) return false;  // we don't support "infinite" blocks
    arg |= MG_SDIO_BLKMODE | MG_SDIO_LEN(blkcnt);  // block transfer
    len = blksz * blkcnt;
  } else {
    arg |= MG_SDIO_LEN(len);  // multi-byte transfer
  }
  return sdio->xfr(sdio, write, arg,
                   (arg & MG_SDIO_BLKMODE) ? (uint16_t) blksz : 0,
                   (uint32_t *) data, len, &val);
}

bool mg_sdio_set_blksz(struct mg_tcpip_sdio *sdio, unsigned int f,
                       uint16_t blksz) {
  uint32_t val = blksz & 0xff;
  if (!mg_sdio_transfer(sdio, true, 0, MG_SDIO_FBR_FnBLKSZ(f), &val, 1))
    return false;
  val = (blksz >> 8) & 0x0f;  // SDIO 6.10 Table 6-4, max 2048
  if (!mg_sdio_transfer(sdio, true, 0, MG_SDIO_FBR_FnBLKSZ(f) + 1, &val, 1))
    return false;
  // TODO(): store in an array, index on f. Static 8-element array
  MG_VERBOSE(("F%c block size set", (f & 7) + '0'));
  return true;
}

// Enable Fx
bool mg_sdio_enable_f(struct mg_tcpip_sdio *sdio, unsigned int f) {
  uint8_t bit = 1U << (f & 7), bits;
  uint32_t val = 0;
  if (!mg_sdio_transfer(sdio, false, 0, MG_SDIO_CCCR_IOEN, &val, 1))
    return false;
  bits = (uint8_t) val | bit;
  unsigned int times = 501;
  while (times--) {
    val = bits; /* IOEf */
    ;
    if (!mg_sdio_transfer(sdio, true, 0, MG_SDIO_CCCR_IOEN, &val, 1))
      return false;
    mg_delayms(1);
    val = 0;
    if (!mg_sdio_transfer(sdio, false, 0, MG_SDIO_CCCR_IOEN, &val, 1))
      return false;
    if (val & bit) break;
  }
  if (times == (unsigned int) ~0) return false;
  MG_VERBOSE(("F%c enabled", (f & 7) + '0'));
  return true;
}

// Wait for Fx to be ready
bool mg_sdio_waitready_f(struct mg_tcpip_sdio *sdio, unsigned int f) {
  uint8_t bit = 1U << (f & 7);
  unsigned int times = 501;
  while (times--) {
    uint32_t val;
    if (!mg_sdio_transfer(sdio, false, 0, MG_SDIO_CCCR_IORDY, &val, 1))
      return false;
    if (val & bit) break;  // IORf
    mg_delayms(1);
  }
  if (times == (unsigned int) ~0) return false;
  MG_VERBOSE(("F%c ready", (f & 7) + '0'));
  return true;
}

// SDIO 6.14 Bus State Diagram
bool mg_sdio_init(struct mg_tcpip_sdio *sdio) {
  uint32_t val = 0;
  if (!sdio->txn(sdio, 0, 0, NULL)) return false;  // GO_IDLE_STATE
  sdio->txn(sdio, 5, 0, &val);                     // IO_SEND_OP_COND, no CRC
  MG_VERBOSE(("IO Functions: %u, Memory: %c", 1 + ((val >> 28) & 7),
              (val & MG_BIT(27)) ? 'Y' : 'N'));
  if (!sdio->txn(sdio, 3, 0, &val)) return false;  // SEND_RELATIVE_ADDR
  val = ((uint32_t) val) >> 16;                    // RCA
  if (!sdio->txn(sdio, 7, val << 16, &val))
    return false;  // SELECT/DESELECT_CARD
  mg_sdio_transfer(sdio, false, 0, MG_SDIO_CCCR_SDIOREV, &val, 1);
  MG_DEBUG(("CCCR: %u.%u, SDIO: %u.%u", 1 + ((val >> 2) & 3), (val >> 0) & 3,
            1 + ((val >> 6) & 3), (val >> 4) & 3));
  mg_sdio_transfer(sdio, false, 0, MG_SDIO_CCCR_SDREV, &val, 1);
  MG_VERBOSE(("SD: %u.%u", 1 + ((val >> 2) & 3), (val >> 0) & 3));
  mg_sdio_transfer(sdio, false, 0, MG_SDIO_CCCR_BIC, &val, 1);
  MG_SET_BITS(val, 3,
              MG_BIT(7) | MG_BIT(1));  // SDIO 6.9 Tables 6-1 6-2, 4-bit bus
  mg_sdio_transfer(sdio, true, 0, MG_SDIO_CCCR_BIC, &val, 1);
  // All Full-Speed SDIO cards support a 4-bit bus. Skip for Low-Speed SDIO
  // cards, we don't provide separate low-level functions for width and speed
  sdio->cfg(sdio, 0);  // set DS;
  if (!mg_sdio_transfer(sdio, false, 0, MG_SDIO_CCCR_HISPD, &val, 1))
    return false;
  if (val & MG_BIT(0) /* SHS */) {
    val = MG_BIT(1); /* EHS */
    if (!mg_sdio_transfer(sdio, true, 0, MG_SDIO_CCCR_HISPD, &val, 1))
      return false;
    sdio->cfg(sdio, 1);  // set HS;
    MG_VERBOSE(("Bus set to 4-bit @50MHz"));
  } else {
    MG_VERBOSE(("Bus set to 4-bit @25MHz"));
  }
  return true;
}

// - 6.11 Card Information Structure (CIS): 0x0001000-0x017FF; for card common
// and all functions
// - 16.5 SDIO Card Metaformat
// - 16.7.2 CISTPL_FUNCE (0x22): Function Extension Tuple, provides standard
// information about the card (common) and each individual function. One
// CISTPL_FUNCE in each function’s CIS, immediately following the CISTPL_FUNCID
// tuple
// - 16.7.3 CISTPL_FUNCE Tuple for Function 0 (common)
// - 16.7.4 CISTPL_FUNCE Tuple for Function 1-7

#endif
