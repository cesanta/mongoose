// Copyright (c) 2020-2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

void device_dashboard_fn(struct mg_connection *, int, void *, void *);

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:8000", device_dashboard_fn, &mgr);
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
