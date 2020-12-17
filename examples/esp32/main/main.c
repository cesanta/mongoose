// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

#define WIFI_SSID "MY_WIFI_NETWORK"
#define WIFI_PASS "MY_WIFI_PASSWORD"
#define LISTENING_ADDR "http://0.0.0.0:80"

// Event handler for an accepted connection
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "Hello from ESP32!");
  }
}

// Called after we're connected to WiFi network
static void start_server(void) {
  struct mg_mgr mgr;
  mg_log_set("3");
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, LISTENING_ADDR, cb, &mgr);
  LOG(LL_INFO, ("Starting Mongoose web server v%s", MG_VERSION));
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
}

void app_main(void) {
  // Setup wifi. This function is implemented in wifi.c
  // It blocks until connected to the configured WiFi network
  void wifi_init(const char *ssid, const char *pass);
  wifi_init(WIFI_SSID, WIFI_PASS);

  // Done connecting to WiFi, now start HTTP server
  start_server();
}
