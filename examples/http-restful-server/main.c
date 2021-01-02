// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// HTTP server example. This server serves both static and dynamic content.
// It implements the following endpoints:
//    /api/f1 - respond with JSON string {"result": 123}
//    /api/f2/:id - wildcard example, respond with JSON string {"result": "URI"}
//    any other URI serves static files from s_web_directory
//
// To enable SSL/TLS (using self-signed certificates in PEM files),
//    1. Change s_listen_on from http:// to https://
//    2. make MBEDTLS_DIR=/path/to/your/mbedtls/installation
//    3. curl -k https://127.0.0.1:8000

#include "mongoose.h"

static const char *s_listen_on = "https://localhost:8000";
static const char *s_web_directory = ".";

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_ACCEPT && mg_url_is_ssl(s_listen_on)) {
    // If s_listen_on URL is https://, tell listening connection to use TLS
    struct mg_tls_opts opts = {
        //.ca = "ca.pem",         // Uncomment to enable two-way SSL
        .cert = "server.pem",     // Certificate PEM file
        .certkey = "server.pem",  // This pem conains both cert and key
    };
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/f1")) {
      mg_http_reply(c, 200, "", "{\"result\": %d}\n", 123);  // Serve REST
    } else if (mg_http_match_uri(hm, "/api/f2/*")) {
      mg_http_reply(c, 200, "", "{\"result\": \"%.*s\"}\n", (int) hm->uri.len,
                    hm->uri.ptr);
    } else {
      struct mg_http_serve_opts opts = {.root_dir = s_web_directory};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;                            // Event manager
  mg_log_set("2");                              // Set to 3 to enable debug
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);
  return 0;
}
