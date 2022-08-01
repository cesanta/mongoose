// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
const char *s_listening_url = "http://0.0.0.0:8000";

void uart_bridge_fn(struct mg_connection *, int, void *, void *);

int main(void) {
  struct mg_mgr mgr;
  mg_log_set(MG_LL_DEBUG);
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, s_listening_url, uart_bridge_fn, &mgr);
  MG_INFO(("Listening on %s", s_listening_url));
  while (mgr.conns != NULL) mg_mgr_poll(&mgr, 500);
  mg_mgr_free(&mgr);
  return 0;
}
