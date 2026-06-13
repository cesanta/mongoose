// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"

#ifndef UART_DEBUG
#define UART_DEBUG USART3
#define UART_DEBUG_TX_PIN PIN('D', 8)
#define UART_DEBUG_RX_PIN PIN('D', 9)
#else
#define UART_DEBUG_TX_PIN PIN('A', 9)
#define UART_DEBUG_RX_PIN PIN('A', 10)
#endif

#define LED1 PIN('B', 0)
#define LED2 PIN('E', 1)
#define LED3 PIN('B', 14)

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
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
  if (ev == MG_EV_HTTP_HDRS) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/ota/update"), NULL)) {
      mg_http_start_ota(c, hm, NULL);
    }
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), hal_get_tick());
    } else if (mg_match(hm->uri, mg_str("/api/ota/commit"), NULL)) {
      c->data[0] = 1;
      mg_http_reply(c, 200, "", "ok\n");
    } else if (mg_match(hm->uri, mg_str("/api/ota/rollback"), NULL)) {
      c->data[0] = 2;
      mg_http_reply(c, 200, "", "ok\n");
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n", hal_get_tick());
    }
  } else if (ev == MG_EV_CLOSE && c->data[0] == 1) {
    MG_OTA_STATE_SET(MG_OTA_CONFIRMED);
    NVIC_SystemReset();
  } else if (ev == MG_EV_CLOSE && c->data[0] == 2) {
    MG_OTA_STATE_SET(MG_OTA_CONFIRMED);
    MG_OTA_ROLLBACK();
  }
}

int main(void) {
  hal_clock_init();
  hal_uart_init(UART_DEBUG, UART_DEBUG_TX_PIN, UART_DEBUG_RX_PIN, 115200);
  mg_log_set_fn(log_fn, UART_DEBUG);
  hal_rng_init();
  hal_ethernet_init();
  hal_gpio_output(LED1);
  hal_gpio_output(LED2);
  hal_gpio_output(LED3);

  MG_INFO(("Initialised. CPU clock: %lu MHz", SystemCoreClock / 1000000));

  MG_OTA_BOOT_CHECK();  // Must be called after clock init

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    blink_task();
  }

  return 0;
}
