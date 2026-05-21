// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static void blink_task(void) {
  static uint64_t timer;
  if (mg_timer_expired(&timer, 500, mg_now())) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,
                        !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
  }
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
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);

  for (;;) {
    mg_mgr_poll(&mgr, 1);
    blink_task();
  }
}

int main(void) {
  stdio_init_all();
  // do not access the CYW43 LED before Mongoose initializes !
  MG_INFO(("Hardware initialised, starting firmware..."));
  run_mongoose();
  return 0;
}
