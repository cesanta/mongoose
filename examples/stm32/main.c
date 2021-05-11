// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved

#include "device.h"
#include "mongoose.h"

// static const char *s_debug_level = "2";
// static const char *s_listening_address = "http://0.0.0.0:80";

// Event handler for the listening connection.
// static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
// {
//   if (ev == MG_EV_HTTP_MSG) {
//     mg_http_reply(c, 200, "", "hello, %s!\n", "world");
//   }
//   (void) fn_data;
//   (void) ev_data;
// }

static void fn(void *args) {
  // struct mg_mgr mgr;
  // struct mg_connection *c;

  // mg_log_set(s_debug_level);
  // mg_mgr_init(&mgr);
  // if ((c = mg_http_listen(&mgr, s_listening_address, cb, &mgr)) == NULL) {
  //   LOG(LL_ERROR, ("Cannot listen on %s. Use http://ADDR:PORT or :PORT",
  //                  s_listening_address));
  // }
  // LOG(LL_INFO, ("Starting Mongoose v%s", MG_VERSION));
  // for (;;) mg_mgr_poll(&mgr, 1000);
  // mg_mgr_free(&mgr);
  // int delay_ms = *(int *) args;
  for (;;) {
    led_toggle();
    spin(500000);
    // vTaskDelay(pdMS_TO_TICKS(0));
  };
  (void) args;
}

int main(void) {
  init_ram();
  init_hardware();
  fn(NULL);
  // xTaskCreate(fn, "server", 512, NULL, configMAX_PRIORITIES - 1, NULL);
  // vTaskStartScheduler();
  return 0;
}
