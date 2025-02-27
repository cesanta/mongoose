// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include <task.h>

#include "mongoose.h"
#include "net.h"

#define WIFI_SSID "yourWiFiSSID"
#define WIFI_PASS "yourWiFiPassword"

static void mongoose(void *args) {
  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  cyw43_arch_init();
  cyw43_arch_enable_sta_mode();
  cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK);

  MG_INFO(("Initialising application..."));
  web_init(&mgr);

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 10);
  }

  (void) args;
}


int main(void) {
  // initialize stdio
  stdio_init_all();  

  xTaskCreate(mongoose, "mongoose", 2048, 0, configMAX_PRIORITIES - 1, NULL);

  vTaskStartScheduler();  // This blocks

  return 0;
}

