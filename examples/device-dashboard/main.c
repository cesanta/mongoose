// Copyright (c) 2020-2023 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "net.h"

static int s_sig_num;
static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_sig_num = sig_num;
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
