// Copyright (c) 2020-2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

extern void run_web_server(struct mg_mgr *);

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  run_web_server(&mgr);
  return 0;
}
