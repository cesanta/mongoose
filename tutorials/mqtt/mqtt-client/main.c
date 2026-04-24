// Copyright (c) 2023-2025 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_mqtt_init(&mgr);

  for (;;) {
    mg_mgr_poll(&mgr, 100);
    mg_mqtt_poll(&mgr);
  }

  mg_mgr_free(&mgr);

  return 0;
}
