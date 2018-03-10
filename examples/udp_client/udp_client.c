/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * This program sends provided data to the provided UDP server and port, and
 * then prints all incoming data, until user stops the program.
 */

#include "mongoose.h"

#define ADDR_BUF_SIZE 256

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    case MG_EV_RECV:
      fprintf(stderr, "%.*s", (int) nc->recv_mbuf.len, nc->recv_mbuf.buf);
      nc->recv_mbuf.len = 0;
      break;
    default:
      break;
  }

  (void) ev_data;
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  char addr[ADDR_BUF_SIZE];

  mg_mgr_init(&mgr, NULL);

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <server:port> <data>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  snprintf(addr, sizeof(addr), "udp://%s", argv[1]);

  fprintf(stderr, "Connecting to '%s'\n", addr);

  struct mg_connection *nc = mg_connect(&mgr, addr, ev_handler);

  if (nc == NULL) {
    fprintf(stderr, "Failed to connect");
    exit(1);
  }

  fprintf(stderr, "Sending '%s'\n", argv[2]);

  mg_send(nc, argv[2], strlen(argv[2]));

  fprintf(stderr, "Waiting for data\n");

  for (;;) {
    mg_mgr_poll(&mgr, 10);
  }
  mg_mgr_free(&mgr);

  return 0;
}
