// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "esp_spiffs.h"
#include "mongoose.h"
#include "net.h"

#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"  // SET THIS!
#define WIFI_PASS "YOUR_WIFI_PASSWORD"      // SET THIS!
#define FS_ROOT "/spiffs"

void app_main(void) {
  // Mount filesystem
  esp_vfs_spiffs_conf_t conf = {
      .base_path = FS_ROOT, .max_files = 20, .format_if_mount_failed = true};
  int res = esp_vfs_spiffs_register(&conf);
  MG_INFO(("FS %s, %d", conf.base_path, res));
  mg_file_printf(&mg_fs_posix, FS_ROOT "/hello.txt", "%s", "hello from ESP");

  // Setup wifi. This function is implemented in wifi.c
  // It blocks until connected to the configured WiFi network
  void wifi_init(const char *ssid, const char *pass);
  wifi_init(WIFI_SSID, WIFI_PASS);

  // Connected to WiFi, now start HTTP server
  struct mg_mgr mgr;
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_mgr_init(&mgr);
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", HTTP_URL));
#if MG_ENABLE_MBEDTLS
  MG_INFO(("Listening on     : %s", HTTPS_URL));
#endif

  web_init(&mgr);
  for (;;) mg_mgr_poll(&mgr, 1000);  // Infinite event loop
}
