/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 *
 * This program fetches HTTP URLs.
 */

#define MG_ENABLE_CALLBACK_USERDATA 1
#include "mongoose.h"

static int s_exit_flag = 0;
static int s_show_headers = 0;

static void ev_handler(struct mg_connection *c, int ev, void *ev_data,
                       void *userdata) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_CONNECT:
      if (*(int *) ev_data != 0) {
        fprintf(stderr, "connect() failed: %s\n", strerror(*(int *) ev_data));
        s_exit_flag = 1;
      } else {
        // Stage 2. Connection to the HTTP proxy is established.
        // Write CONNECT request, and turn this connection to HTTP.
        // NOTE: target URL is passed to us as userdata.
        unsigned port = 80;
        struct mg_str scheme, host;
        mg_parse_uri(mg_mk_str((char *) userdata), &scheme, NULL, &host, &port,
                     NULL, NULL, NULL);
        if (port == 0) port = (scheme.len == 5) ? 443 : 80;
        mg_printf(c, "CONNECT %.*s:%u HTTP/1.1\r\n\r\n", (int) host.len, host.p,
                  port);
        // Now set the flag and wait for the connection establishment
        c->flags |= MG_F_USER_1;
      }
      break;
    case MG_EV_RECV:
      // Stage 3. Check if proxy replied. Here, we don't parse the reply
      // for simplicity. Assume success, and write HTTP request.
      if (c->flags & MG_F_USER_1) {
        struct mg_str host, path;
        c->flags &= ~MG_F_USER_1;
        mg_parse_uri(mg_mk_str((char *) userdata), NULL, NULL, &host, NULL,
                     &path, NULL, NULL);
        if (path.len == 0) path = mg_mk_str("/");
        mg_printf(c, "GET %.*s HTTP/1.0\r\nHost: %.*s\r\n\r\n", (int) path.len,
                  path.p, (int) host.len, host.p);
        mg_set_protocol_http_websocket(c);
      }
      break;
    case MG_EV_HTTP_REPLY:
      c->flags |= MG_F_CLOSE_IMMEDIATELY;
      if (s_show_headers) {
        fwrite(hm->message.p, 1, hm->message.len, stdout);
      } else {
        fwrite(hm->body.p, 1, hm->body.len, stdout);
      }
      putchar('\n');
      s_exit_flag = 1;
      break;
    case MG_EV_CLOSE:
      if (s_exit_flag == 0) {
        printf("Server closed connection\n");
        s_exit_flag = 1;
      }
      break;
    default:
      break;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  int i;

  mg_mgr_init(&mgr, NULL);

  /* Process command line arguments */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--show-headers") == 0) {
      s_show_headers = 1;
    } else if (strcmp(argv[i], "--hexdump") == 0 && i + 1 < argc) {
      mgr.hexdump_file = argv[++i];
    } else {
      break;
    }
  }

  if (i + 2 != argc) {
    fprintf(stderr,
            "Usage: %s [--hexdump <file>] "
            "[--show-headers] PROXY_HOST:PROXY_PORT URL\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  // Stage 1. Connect to the HTTP proxy as to a plain TCP server.
  // Pass URL as a callback argument
  mg_connect(&mgr, argv[i], ev_handler, argv[i + 1]);

  while (s_exit_flag == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
