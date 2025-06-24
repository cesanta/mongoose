// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example Websocket server. See https://mongoose.ws/tutorials/websocket-server/

#include "mongoose.h"

static const char *s_listen_on = "ws://localhost:8000";
static const char *s_web_root = ".";
static const char *s_ca_path = "ca.pem";
static const char *s_cert_path = "cert.pem";
static const char *s_key_path = "key.pem";
struct mg_str s_ca, s_cert, s_key;

// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   /rest - respond with JSON string {"result": 123}
//   any other URI serves static files from s_web_root
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if(c->is_tls && ev == MG_EV_ACCEPT) {
    s_ca = mg_file_read(&mg_fs_posix, s_ca_path);
    s_cert = mg_file_read(&mg_fs_posix, s_cert_path);
    s_key = mg_file_read(&mg_fs_posix, s_key_path);
    struct mg_tls_opts opts = {.ca = s_ca, .cert = s_cert, .key = s_key};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/websocket"), NULL)) {
      // Upgrade to websocket. From now on, connection is full-duplex
      // Websocket connection, which will receive MG_EV_WS_MSG events.
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_match(hm->uri, mg_str("/rest"), NULL)) {
      // Serve REST response
      mg_http_reply(c, 200, "", "{\"result\": %d}\n", 123);
    } else {
      // Serve static files
      struct mg_http_serve_opts opts = {.root_dir = s_web_root};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  } else if (ev == MG_EV_WS_MSG) {
    // Got websocket frame. Received data is wm->data. Echo it back!
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    mg_ws_send(c, wm->data.buf, wm->data.len, WEBSOCKET_OP_TEXT);
  } else if (ev == MG_EV_CLOSE && c->is_tls) {
    mg_free(s_ca.buf);
    mg_free(s_cert.buf);
    mg_free(s_key.buf);
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;  // Event manager
  int i;

  // Parse command-line flags
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-url") == 0 && argv[i + 1] != NULL) {
      s_listen_on = argv[++i];
    } else if (strcmp(argv[i], "-ca") == 0 && argv[i + 1] != NULL) {
      s_ca_path = argv[++i];
    } else if (strcmp(argv[i], "-cert") == 0 && argv[i + 1] != NULL) {
      s_cert_path = argv[++i];
    } else if (strcmp(argv[i], "-key") == 0 && argv[i + 1] != NULL) {
      s_key_path = argv[++i];
    } else {
      printf(
          "Usage: %s OPTIONS\n"
          "  -ca PATH  - Path to the CA file, default: '%s'\n"
          "  -cert PATH  - Path to the CERT file, default: '%s'\n"
          "  -key PATH  - Path to the KEY file, default: '%s'\n"
          "  -url URL  - Listen on URL, default: '%s'\n",
          argv[0], s_ca_path, s_cert_path, s_key_path, s_listen_on);
      return 1;
    }
  }

  mg_mgr_init(&mgr);  // Initialise event manager
  printf("Starting WS listener on %s/websocket\n", s_listen_on);
  mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);
  return 0;
}
