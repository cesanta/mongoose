// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "esp_spiffs.h"
#include "freertos/FreeRTOS.h"
#include "mongoose.h"

#define WIFI_SSID "WIFI_NETWORK"   // SET THIS!
#define WIFI_PASS "WIFI_PASSWORD"  // SET THIS!
#define FS_ROOT "/spiffs"

// SPIFFS is flat, so tell Mongoose that the FS root is a directory
// This cludge is not required for filesystems with directory support
static int my_stat(const char *path, size_t *size, time_t *mtime) {
  int flags = mg_fs_posix.st(path, size, mtime);
  if (strcmp(path, FS_ROOT) == 0) flags |= MG_FS_DIR;
  return flags;
}

// Event handler for an server (accepted) connection. Implemented endpoints:
//    /api/stats  - return JSON object with ESP32 stats (free RAM)
//    any other   - serve files from the filesystem
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = ev_data;
    if (mg_http_match_uri(hm, "/api/stats")) {
      mg_http_reply(c, 200, "", "{\"ram\": %lu}\n", xPortGetFreeHeapSize());
    } else {
      struct mg_fs fs = mg_fs_posix;
      fs.st = my_stat;
      struct mg_http_serve_opts opts = {.root_dir = FS_ROOT, .fs = &fs};
      mg_http_serve_dir(c, hm, &opts);
    }
  }
}

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
  mg_log_set("3");
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:80", cb, &mgr);  // Listening server
  MG_INFO(("Starting Mongoose web server v%s", MG_VERSION));
  for (;;) mg_mgr_poll(&mgr, 1000);  // Infinite event loop
}
