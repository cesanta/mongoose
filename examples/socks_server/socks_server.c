/*
 * Copyright (c) 2017 Cesanta Software Limited
 * All rights reserved
 *
 * Use curl to test, e.g.
 * curl -i --socks5 127.0.0.1:1080 www.met.ie
 */

#include "mongoose.h"

static const char *s_listening_addr = "1080";

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;

  mg_mgr_init(&mgr, NULL);

  if ((c = mg_bind(&mgr, s_listening_addr, NULL)) == NULL) {
    fprintf(stderr, "mg_bind(%s) failed\n", s_listening_addr);
    exit(EXIT_FAILURE);
  }
  mg_set_protocol_socks(c);

  printf("Starting socks5 proxy server on %s\n", s_listening_addr);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
