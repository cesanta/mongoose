// Copyright (c) 2020-2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

const char *s_listening_url = "http://0.0.0.0:8000";
const char *s_listening_surl = "https://0.0.0.0:8443";

void device_dashboard_fn(struct mg_connection *, int, void *, void *);

int main(void) {
  struct mg_mgr mgr;
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, s_listening_url, device_dashboard_fn, NULL);
  MG_INFO(("Listening on %s", s_listening_url));
#if MG_ENABLE_MBEDTLS || MG_ENABLE_OPENSSL
  mg_http_listen(&mgr, s_listening_surl, device_dashboard_fn, "");
  MG_INFO(("Listening on %s", s_listening_surl));
#endif
  while (mgr.conns != NULL) mg_mgr_poll(&mgr, 500);
  mg_mgr_free(&mgr);
  return 0;
}
