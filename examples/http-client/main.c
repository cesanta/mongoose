// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example HTTP client. Connect to `s_url`, send request, wait for a response,
// print the response and exit.
//
// To enable SSL/TLS for this client, build it like this:
//    make MBEDTLS_DIR=/path/to/your/mbedtls/installation

#include "mongoose.h"

// The very first web page in history
static const char *s_url = "http://info.cern.ch";

// Print HTTP response and signal that we're done
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_CONNECT) {
    // Connected to server
    struct mg_str host = mg_url_host(s_url);

    if (mg_url_is_ssl(s_url)) {
      // If s_url is https://, tell client connection to use TLS
      struct mg_tls_opts opts = {.ca = "ca.pem"};
      mg_tls_init(c, &opts);
    }
    // Send request
    mg_printf(c, "GET %s HTTP/1.0\r\nHost: %.*s\r\n\r\n", mg_url_uri(s_url),
              (int) host.len, host.ptr);
  } else if (ev == MG_EV_HTTP_MSG) {
    // Response is received. Print it
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    printf("%.*s", (int) hm->message.len, hm->message.ptr);
    c->is_closing = 1;         // Tell mongoose to close this connection
    *(bool *) fn_data = true;  // Tell event loop to stop
  }
}

int main(void) {
  struct mg_mgr mgr;                        // Event manager
  bool done = false;                        // Event handler flips it to true
  mg_log_set("3");                          // Set to 0 to disable debug
  mg_mgr_init(&mgr);                        // Initialise event manager
  mg_http_connect(&mgr, s_url, fn, &done);  // Create client connection
  while (!done) mg_mgr_poll(&mgr, 1000);    // Infinite event loop
  mg_mgr_free(&mgr);                        // Free resources
  return 0;
}
