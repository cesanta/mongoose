// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"

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

bool mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
}


static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    (void) hm;
    mg_http_reply(c, 200, "", "hi\n");
  }
}

int main(void) {
  gpio_output(LED);               // Setup blue LED
  uart_init(UART_DEBUG, 115200);  // Initialise debug printf
  ethernet_init();

  struct mg_mgr mgr;        // Initialise
  mg_mgr_init(&mgr);        // Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level


  MG_INFO(("Init TCP/IP stack ..."));
  struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(),
                            .driver = &mg_tcpip_driver_stm32f,
                            .l2type = MG_TCPIP_L2_PPPoE,
                            .recv_queue.size = 4096};
  mg_tcpip_init(&mgr, &mif);
  mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_cb, &mgr);
  mg_http_listen(&mgr, "tcp://0.0.0.0:80", fn, &mgr);

  MG_INFO(("Init done, starting main loop ..."));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
