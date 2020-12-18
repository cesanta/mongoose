// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example HTTP client. Usage:
//    make
//    ./example http://www.ladyada.net/
//
// To enable SSL/TLS for this client, build it like this:
//    make MBEDTLS_DIR=/path/to/your/mbedtls/installation

#include "mongoose.h"

// Print HTTP response and signal that we're done
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    printf("%.*s", (int) hm->message.len, hm->message.ptr);
    c->is_closing = 1;
    *(bool *) fn_data = true;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Initialise event manager

  if (argc != 2) {
    fprintf(stderr, "Usage: %s URL\n", argv[0]);  // Print error
  } else {
    bool done = false;  // Event handler flips it to true
    struct mg_connection *c = mg_http_connect(&mgr, argv[1], fn, &done);
    if (c != NULL) {
      mg_printf(c, "GET %s HTTP/1.0\r\n\r\n", mg_url_uri(argv[1]));
      // If target URL is https://, tell client connection to use TLS
      if (mg_url_is_ssl(argv[1])) {
        struct mg_tls_opts opts = {.ca = "ca.pem"};
        mg_tls_init(c, &opts);
      }
    }
    while (done == false) mg_mgr_poll(&mgr, 1000);  // Event loop
  }

  mg_mgr_free(&mgr);

  return 0;
}
