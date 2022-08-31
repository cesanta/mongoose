#include "mip.h"

#if MG_ENABLE_MIP
static void mip_driver_enc28j60_init(uint8_t *mac, void *data) {
  (void) mac, (void) data;
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

struct mip_driver mip_driver_enc28j60 = {.init = mip_driver_enc28j60_init,
                                         .tx = mip_driver_enc28j60_tx,
                                         .rx = mip_driver_enc28j60_rx,
                                         .up = mip_driver_enc28j60_up};
#endif
