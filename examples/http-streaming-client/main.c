// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved
//
// Example HTTP client.
// Instead of buffering the whole HTTP response in memory and wait for the
// MG_EV_HTTP_MSG event, this client catches MG_EV_HTTP_CHUNK events and
// prints them. This is useful to download large content without spooling
// everything to memory, or catch chunked content.
//
// You can change `s_url` from the command line by executing: ./example YOUR_URL
//
// To enable SSL/TLS for this client, build it like this:
//    make MBEDTLS=/path/to/your/mbedtls/installation
//    make OPENSSL=/path/to/your/openssl/installation

#include "mongoose.h"

// The very first web page in history. You can replace it from command line
static const char *s_url = "http://info.cern.ch";

// Print HTTP response and signal that we're done
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_CONNECT) {
    // Connected to server. Extract host name from URL
    struct mg_str host = mg_url_host(s_url);

    // If s_url is https://, tell client connection to use TLS
    if (mg_url_is_ssl(s_url)) {
      struct mg_tls_opts opts = {.ca = "ca.pem", .srvname = host};
      mg_tls_init(c, &opts);
    }

    // Send request
    mg_printf(c,
              "GET %s HTTP/1.1\r\n"
              "Connection: keep-alive\r\n"
              "Keep-Alive: timeout=60\r\n"
              "Host: %.*s\r\n"
              "\r\n",
              mg_url_uri(s_url), (int) host.len, host.ptr);
  } else if (ev == MG_EV_HTTP_CHUNK) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    fwrite(hm->chunk.ptr, 1, hm->chunk.len, stdout);
    // fprintf(stderr, "c %u\n", (unsigned) hm->chunk.len);
    mg_http_delete_chunk(c, hm);
    if (hm->chunk.len == 0) *(bool *) fn_data = true;  // Last chunk
  } else if (ev == MG_EV_HTTP_MSG) {
    // Response is received. Print it
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    fwrite(hm->body.ptr, 1, hm->body.len, stdout);
    c->is_closing = 1;         // Tell mongoose to close this connection
    *(bool *) fn_data = true;  // Tell event loop to stop
  } else if (ev == MG_EV_ERROR) {
    *(bool *) fn_data = true;  // Error, tell event loop to stop
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;                        // Event manager
  bool done = false;                        // Event handler flips it to true
  const char *log_level = getenv("V");      // Log level
  if (log_level == NULL) log_level = "3";   // If not set, set to DEBUG
  mg_log_set(log_level);                    // Set to 0 to disable debug log
  if (argc > 1) s_url = argv[1];            // Use URL from command line
  mg_mgr_init(&mgr);                        // Initialise event manager
  mg_http_connect(&mgr, s_url, fn, &done);  // Create client connection
  while (!done) mg_mgr_poll(&mgr, 1000);    // Infinite event loop
  mg_mgr_free(&mgr);                        // Free resources
  return 0;
}
