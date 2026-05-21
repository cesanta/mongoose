// Copyright (c) 2025 Cesanta Software Limited
// All rights reserved

#include "pico/stdlib.h"
#include "mongoose.h"

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
  void driver_init(struct mg_mgr *mgr);
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  driver_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set log level
  MG_INFO(("Hardware initialised, starting firmware..."));
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);
  for (;;) {
    mg_mgr_poll(&mgr, 1);
  }
}

int main(void) {
  stdio_init_all();
  run_mongoose();
  return 0;
}
