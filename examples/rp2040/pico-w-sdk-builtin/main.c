// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include "pico/stdlib.h"

#include "mongoose.h"
#include "net.h"

#include "driver_pico-w.h"

#define WIFI_SSID "yourWiFiSSID"
#define WIFI_PASS "yourWiFiPassword"


int main(void) {
  // initialize stdio
  stdio_init_all();  

  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  // Initialise WiFi creds
  struct mg_tcpip_driver_pico_w_data driver_data = {
    .ssid = WIFI_SSID,
    .pass = WIFI_PASS
  };
  // Initialise Mongoose network stack
  // Either set use_dhcp or enter a static config.
  // For static configuration, specify IP/mask/GW in network byte order
  struct mg_tcpip_if mif = {
      .ip = 0,
      .driver = &mg_tcpip_driver_pico_w,
      .driver_data = &driver_data,
      .recv_queue.size = 8192
  };
  
  mg_tcpip_init(&mgr, &mif);
  MG_INFO(("Init done, starting main loop"));

  MG_INFO(("Initialising application..."));
  web_init(&mgr);

  MG_INFO(("Starting event loop"));
  for (;;) {
    driver_pico_w_poll();
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}

