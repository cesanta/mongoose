// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

#define WIFI_SSID "WIFI_NETWORK"   // SET THIS!
#define WIFI_PASS "WIFI_PASSWORD"  // SET THIS!

#define SERVER_URL "http://0.0.0.0:80"
#define CLIENT_URL "http://info.cern.ch"

// Event handler for an server (accepted) connection
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "Hello from ESP!\n");
  }
}

// Event handler for a client connection - fetch the first web page in history
// To enable TLS for HTTP,
//   1. Copy "ca.pem" file to the ESP32 flash FS
//   2. Add TLS init snippet for the connection, see examples/http-client
static void cb2(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_CONNECT) {
    struct mg_str s = mg_url_host(CLIENT_URL);
    mg_printf(c, "GET / HTTP/1.0\r\nHost: %.*s\r\n\r\n", (int) s.len, s.ptr);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = ev_data;  // Print HTTP response
    LOG(LL_INFO, ("Fetched:\n%.*s", (int) hm->message.len, hm->message.ptr));
    c->is_closing = 1;
  }
}

// Called after we're connected to WiFi network
static void run_mongoose(void) {
  struct mg_mgr mgr;
  mg_log_set("3");
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, SERVER_URL, cb, &mgr);    // Listening server
  mg_http_connect(&mgr, CLIENT_URL, cb2, &mgr);  // Example client
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
  run_mongoose();
}
