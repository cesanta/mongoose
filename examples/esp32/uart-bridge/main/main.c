// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "main.h"

const char *s_listening_url = "http://0.0.0.0:80";

#if 0
char *config_read(void) {
  return mg_file_read(&mg_fs_posix, FS_ROOT "/config.json", NULL);
}
#endif

void config_write(struct mg_str config) {
  mg_file_write(&mg_fs_posix, FS_ROOT "/config.json", config.ptr, config.len);
}

void app_main(void) {
  // Mount filesystem
  esp_vfs_spiffs_conf_t conf = {
      .base_path = FS_ROOT, .max_files = 20, .format_if_mount_failed = true};
  int res = esp_vfs_spiffs_register(&conf);
  MG_INFO(("FS at %s initialised, status: %d", conf.base_path, res));

  // Try to connect to wifi by using saved WiFi credentials
  struct mg_str json = mg_file_read(&mg_fs_posix, WIFI_FILE);
  if (json.ptr != NULL) {
    char *ssid = mg_json_get_str(json, "$.ssid");
    char *pass = mg_json_get_str(json, "$.pass");
    while (!wifi_init(ssid, pass)) (void) 0;
    free(ssid);
    free(pass);
    free((void *) json.ptr);
  } else {
    // If WiFi is not configured, run CLI until configured
    MG_INFO(("WiFi is not configured, running CLI. Press enter"));
    cli_init();
    for (;;) {
      uint8_t ch = getchar();
      cli(ch);
      usleep(10000);
    }
  }

  // Connected to WiFi, now start HTTP server
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set log level
  MG_INFO(("Mongoose v%s on %s", MG_VERSION, s_listening_url));
  mg_http_listen(&mgr, s_listening_url, uart_bridge_fn, NULL);
  for (;;) mg_mgr_poll(&mgr, 10);  // Infinite event loop
}
