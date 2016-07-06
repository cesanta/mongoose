// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      mg_serve_http(c, (struct http_message *) ev_data, s_http_server_opts);
      break;
    case MG_EV_TIMER: {
      double now = *(double *) ev_data;
      double next = mg_set_timer(c, 0) + 0.5;
      printf("timer event, current time: %.2lf\n", now);
      mg_set_timer(c, next);  // Send us timer event again after 0.5 seconds
      break;
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;

  mg_mgr_init(&mgr, NULL);
  c = mg_bind(&mgr, s_http_port, ev_handler);
  if (c == NULL) {
    printf("Cannot start on port %s\n", s_http_port);
    return EXIT_FAILURE;
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(c);
  s_http_server_opts.document_root = ".";  // Serve current directory

  // Send us MG_EV_TIMER event after 2.5 seconds
  mg_set_timer(c, mg_time() + 2.5);

  printf("Starting on port %s, time: %.2lf\n", s_http_port, mg_time());
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return EXIT_SUCCESS;
}
