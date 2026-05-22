// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "hal.h"

#define DEBUG_UART USART1
#define LED1 PIN('C', 7) // On-board LED pin (green)
#define LED2 PIN('B', 7) // On-board LED pin (blue)
#define LED3 PIN('G', 2) // On-board LED pin (red)

// Redirect stdout debug output to UART
int _write(int fd, char *ptr, int len) {
  if (fd == 1 || fd == 2) hal_uart_write_buf(DEBUG_UART, ptr, (size_t) len);
  return len;
}

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (hal_timer_expired(&blink_timer, 500, hal_get_tick())) {
    hal_gpio_toggle(LED1);
  }
}

uint64_t mg_millis(void) {
  return hal_get_tick();
}

bool mg_random(void *buf, size_t len) {
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = hal_rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
}

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), hal_get_tick());
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n",
                    hal_get_tick());
    }
  }
}

int main(void) {
  hal_clock_init();
  hal_rng_init();
  hal_gpio_output(LED1);
  hal_gpio_output(LED2);
  hal_gpio_output(LED3);
  hal_uart_init(DEBUG_UART, 115200);

  extern void hwspecific_spi_init();
  hwspecific_spi_init();

  // Start a minimal web server
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    blink_task();
  }

  return 0;
}
