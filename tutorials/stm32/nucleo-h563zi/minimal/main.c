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

#define LED_1 PIN('B', 0)  // On-board LED pin (green)
#define LED_2 PIN('F', 4)  // On-board LED pin (yellow)
#define LED_3 PIN('G', 4)  // On-board LED pin (red)

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

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
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), hal_get_tick());
    } else if (mg_match(hm->uri, mg_str("/files/#"), NULL)) {
      struct mg_http_serve_opts opts = {
          .root_dir = "/files/=/fs/",
          .allow_delete = true,
          .allow_upload = true,
      };
      mg_http_serve_dir(c, hm, &opts);
    } else {
      size_t u = hal_ram_used(), t = u + hal_ram_free();
      mg_http_reply(c, 200, "",
                    "Hi from Mongoose!\n"
                    "Tick %llu\n"
                    "RAM usage: %zu / %zu (%zu %%)\n",
                    hal_get_tick(), u, t, (size_t) (100 * u / t));
    }
  }
}

int main(void) {
  hal_clock_init();
  hal_uart_init(UART_DEBUG, UART_DEBUG_TX_PIN, UART_DEBUG_RX_PIN, 115200);
  mg_log_set_fn(log_fn, UART_DEBUG);

  hal_rng_init();
  hal_gpio_output(LED_1);
  hal_gpio_output(LED_2);
  hal_gpio_output(LED_3);
  hal_ethernet_init();
  MG_INFO(("Initialised. CPU clock: %lu MHz", SystemCoreClock / 1000000));

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);

  // Create directory for Web file manager. Will be mapped to /files/ URL
  mkdir("/fs", 0755);
  MG_INFO(("File manager operations:"));
  MG_INFO(("Upload: curl IP/files/NAME --data-binary @FILE"));
  MG_INFO(("Delete: curl IP/files/NAME -X DELETE"));
  MG_INFO(("Show:   curl IP/files/NAME"));
  MG_INFO(("List:   curl IP/files/"));

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    blink_task();
  }

  return 0;
}
