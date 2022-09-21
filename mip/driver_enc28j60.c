#include "mip.h"

#if MG_ENABLE_MIP

// Instruction set
enum { OP_RCR, OP_RBM, OP_WCR, OP_WBM, OP_BFS, OP_BFC, OP_SRC };

static uint8_t rd(struct mip_spi *spi, uint8_t op, uint8_t addr) {
  spi->begin(spi->spi);
  spi->txn(spi->spi, (uint8_t) ((op << 5) | (addr & 0x1f)));
  uint8_t value = spi->txn(spi->spi, 255);
  if (addr & 0x80) value = spi->txn(spi->spi, 255);
  spi->end(spi->spi);
  return value;
}

static bool mip_driver_enc28j60_init(uint8_t *mac, void *data) {
  (void) mac, (void) data;
  rd((struct mip_spi *) data, OP_SRC, 0x1f);
  return false;
}

static size_t mip_driver_enc28j60_tx(const void *buf, size_t len, void *data) {
  (void) buf, (void) len, (void) data;
  return 0;
}

static size_t mip_driver_enc28j60_rx(void *buf, size_t len, void *data) {
  (void) buf, (void) len, (void) data;
  return 0;
}

static bool mip_driver_enc28j60_up(void *data) {
  (void) data;
  return false;
}

struct mip_driver mip_driver_enc28j60 = {
    mip_driver_enc28j60_init, mip_driver_enc28j60_tx, mip_driver_enc28j60_rx,
    mip_driver_enc28j60_up, NULL};
#endif
