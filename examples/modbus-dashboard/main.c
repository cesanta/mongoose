// Copyright (c) 2020-2023 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "net.h"

#define CONFIG_FILE "settings.bin"

static int s_sig_num;
static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_sig_num = sig_num;
}

bool web_load_settings(void *buf, size_t len) {
  bool ok = false;
  struct mg_str data = mg_file_read(&mg_fs_posix, CONFIG_FILE);
  if (data.ptr == NULL) {
    MG_ERROR(("Error reading %s", CONFIG_FILE));
  } else if (data.len != len) {
    MG_ERROR(("%s size != %lu", CONFIG_FILE, len));
  } else {
    memcpy(buf, data.ptr, len);
  }
  free((void *) data.ptr);
  return ok;
}

bool web_save_settings(void *buf, size_t len) {
  MG_INFO(("Saving to %s", CONFIG_FILE));
  return mg_file_write(&mg_fs_posix, CONFIG_FILE, buf, len);
}

int main(void) {
  struct mg_mgr mgr;

  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_mgr_init(&mgr);

  web_init(&mgr);
  while (s_sig_num == 0) {
    mg_mgr_poll(&mgr, 50);
  }

  mg_mgr_free(&mgr);
  MG_INFO(("Exiting on signal %d", s_sig_num));

  return 0;
}
