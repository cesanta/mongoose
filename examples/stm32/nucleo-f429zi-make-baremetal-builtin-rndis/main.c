// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "tusb.h"

static struct mg_tcpip_if *s_ifp;
const uint8_t tud_network_mac_address[6] = {2, 2, 0x84, 0x6A, 0x96, 0};

static void blink_cb(void *arg) {  // Blink periodically
  gpio_toggle(LED);
  (void) arg;
}

static volatile uint64_t s_ticks;  // Milliseconds since boot
void SysTick_Handler(void) {       // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

void mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}

bool tud_network_recv_cb(const uint8_t *buf, uint16_t len) {
  mg_tcpip_qwrite((void *) buf, len, s_ifp);
  // MG_INFO(("RECV %hu", len));
  // mg_hexdump(buf, len);
  tud_network_recv_renew();
  return true;
}

void tud_network_init_cb(void) {
}

void OTG_FS_IRQHandler(void) {  // USB interrupt handler
  tud_int_handler(0);           // Pass control to TinyUSB
}

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

static bool usb_up(struct mg_tcpip_if *ifp) {
  (void) ifp;
  return tud_inited() && tud_ready() && tud_connected();
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/debug")) {
      int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
      mg_log_set(level);
      mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
    } else {
      mg_http_reply(c, 200, "", "hi\n");
    }
  }
  (void) fn_data;
}

int main(void) {
  gpio_output(LED);               // Setup blue LED
  uart_init(UART_DEBUG, 115200);  // Initialise debug printf

  struct mg_mgr mgr;        // Initialise 
  mg_mgr_init(&mgr);        // Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level


  MG_INFO(("Init TCP/IP stack ..."));
  struct mg_tcpip_driver driver = {.tx = usb_tx, .up = usb_up};
  struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(),
                            .ip = mg_htonl(MG_U32(192, 168, 3, 1)),
                            .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                            .enable_dhcp_server = true,
                            .driver = &driver,
                            .recv_queue.size = 4096};
  s_ifp = &mif;
  mg_tcpip_init(&mgr, &mif);
  mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_cb, &mgr);
  mg_http_listen(&mgr, "tcp://0.0.0.0:80", fn, &mgr);

  MG_INFO(("Init USB ..."));
  usb_init();
  tusb_init();

  MG_INFO(("Init done, starting main loop ..."));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
    tud_task();
  }

  return 0;
}
