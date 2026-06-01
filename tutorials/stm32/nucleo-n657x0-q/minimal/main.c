// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"

#define DEBUG_UART USART1
#define UART_TX PIN('E', 5)
#define UART_RX PIN('E', 6)

#define LED_1 PIN('G', 0)   // Green LED
#define LED_2 PIN('G', 8)   // Blue LED
#define LED_3 PIN('G', 10)  // Red LED

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

// Blink green LED every 500ms
static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (hal_timer_expired(&blink_timer, 500, hal_get_tick())) {
    hal_gpio_toggle(LED_1);
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
  hal_uart_init(DEBUG_UART, UART_TX, UART_RX, 115200);
  mg_log_set_fn(log_fn, DEBUG_UART);
  hal_rng_init();

  // Initialise pins and turn them off: they are active high
  hal_gpio_output(LED_1), hal_gpio_write(LED_1, true);
  hal_gpio_output(LED_2), hal_gpio_write(LED_2, true);
  hal_gpio_output(LED_3), hal_gpio_write(LED_3, true);

  printf("Initialised. CPU clock: %lu MHz\r\n", SystemCoreClock / 1000000);
  printf("APB1: %u, APB2: %u\r\n", HAL_APB1_FREQUENCY, HAL_APB2_FREQUENCY);

  hal_ethernet_init();

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
