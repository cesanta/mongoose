// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "pico/stdlib.h"
#include "tusb.h"

static struct mg_tcpip_if *s_ifp;

uint8_t tud_network_mac_address[6] = {2, 2, 0x84, 0x6A, 0x96, 0};

static void blink_task(void *arg) {  // Blink periodically
  gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get_out_level(PICO_DEFAULT_LED_PIN));
  (void) arg;
}

bool tud_network_recv_cb(const uint8_t *buf, uint16_t len) {
  mg_tcpip_qwrite((void *) buf, len, s_ifp);
  // MG_INFO(("RECV %hu", len));
  // mg_hexdump(buf, len);
  tud_network_recv_renew();
  return true;
}

void tud_network_init_cb(void) {}

uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg) {
  // MG_INFO(("SEND %hu", arg));
  memcpy(dst, ref, arg);
  return arg;
}

static size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  if (!tud_ready()) return 0;
  while (!tud_network_can_xmit(len)) tud_task();
  tud_network_xmit((void *) buf, len);
  (void) ifp;
  return len;
}

static bool usb_poll(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp;
  tud_task();
  return s1 ? tud_inited() && tud_ready() && tud_connected() : false;
}

static void driver_init(struct mg_mgr *mgr) {
  struct mg_tcpip_driver driver = {.tx = usb_tx, .poll = usb_poll};
  struct mg_tcpip_if mif = {.mac = {2, 0, 1, 2, 3, 0x77},
                       .ip = mg_htonl(MG_U32(192, 168, 3, 1)),
                       .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                       .enable_dhcp_server = true,
                       .driver = &driver,
                       .recv_queue.size = 4096};
  s_ifp = &mif;
  mg_tcpip_init(mgr, &mif);
  tusb_init();
}

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    uint64_t tick = to_ms_since_boot(get_absolute_time());
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), tick);
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n", tick);
    }
  }
}

// In an RTOS environment, give this task 8 KB of stack space.
static void run_mongoose(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  driver_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set log level
  MG_INFO(("Hardware initialised, starting firmware..."));
  mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_task, &mgr);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);
  for (;;) {
    mg_mgr_poll(&mgr, 1);
  }
}

int main(void) {
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  stdio_init_all();
  run_mongoose();
  return 0;
}
