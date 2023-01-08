// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "tusb.h"
#include "pico/stdlib.h"

static struct mip_if *s_ifp;

const uint8_t tud_network_mac_address[6] = {2, 2, 0x84, 0x6A, 0x96, 0};

static void blink_cb(void *arg) {  // Blink periodically
  gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get_out_level(PICO_DEFAULT_LED_PIN));
  (void) arg;
}

bool tud_network_recv_cb(const uint8_t *buf, uint16_t len) {
  mip_qwrite((void *) buf, len, s_ifp);
  // MG_INFO(("RECV %hu", len));
  // mg_hexdump(buf, len);
  tud_network_recv_renew();
  return true;
}

void tud_network_init_cb(void) {
}

uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg) {
  // MG_INFO(("SEND %hu", arg));
  memcpy(dst, ref, arg);
  return arg;
}

static size_t usb_tx(const void *buf, size_t len, struct mip_if *ifp) {
  if (!tud_ready()) return 0;
  while (!tud_network_can_xmit(len)) tud_task();
  tud_network_xmit((void *) buf, len);
  (void) ifp;
  return len;
}

static bool usb_up(struct mip_if *ifp) {
  (void) ifp;
  return tud_inited() && tud_ready() && tud_connected();
}

void device_dashboard_fn(struct mg_connection *, int, void *, void *);

int main(void) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    stdio_init_all();

  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the MIP interface
  mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_cb, &mgr);

  struct mip_driver driver = {.tx = usb_tx, .rx = mip_driver_rx, .up = usb_up};
  struct mip_if mif = {.mac = {2, 0, 1, 2, 3, 0x77},
                       .ip = mg_htonl(MG_U32(192, 168, 3, 1)),
                       .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                       .enable_dhcp_server = true,
                       .driver = &driver,
                       .queue.len = 4096};
  s_ifp = &mif;
  mip_init(&mgr, &mif);
  mg_http_listen(&mgr, "tcp://0.0.0.0:80", device_dashboard_fn, &mgr);

  tusb_init();

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    tud_task();
  }

  return 0;
}
