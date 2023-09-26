// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved
//
// Example streaming HTTP client.
// The default HTTP handler waits until the whole HTTP message is buffered in
// c->recv IO buffer. It can be large. This example shows how to receive
// a potentially large response in chunks by handling the MG_EV_READ events.
//
// You can change `s_url` from the command line by executing: ./example YOUR_URL
// To enable SSL/TLS, see https://mongoose.ws/tutorials/tls/#how-to-build

#include "mongoose.h"

// The very first web page in history. You can replace it from command line
static const char *s_url = "http://info.cern.ch";

// Print HTTP response and signal that we're done
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_CONNECT) {
    // Connected to server. Extract host name from URL
    struct mg_str host = mg_url_host(s_url);
    if (mg_url_is_ssl(s_url)) {
      struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
                                 .name = host};
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
  } else if (ev == MG_EV_READ) {
    // Parse the incoming data ourselves. If we can parse the request,
    // store two size_t variables in the c->data: expected len and recv len.
    size_t *data = (size_t *) c->data;
    if (data[0]) {  // Already parsed, simply print received data
      data[1] += c->recv.len;
      fwrite(c->recv.buf, 1, c->recv.len, stdout);
      c->recv.len = 0;  // And cleanup the receive buffer. Streming!
      if (data[1] >= data[0]) * (bool *) fn_data = true;
    } else {
      struct mg_http_message hm;
      int n = mg_http_parse((char *) c->recv.buf, c->recv.len, &hm);
      if (n < 0) mg_error(c, "Bad response");
      if (n > 0) {
        fwrite(c->recv.buf + n, 1, c->recv.len - n, stdout);  // Print body
        data[0] = n + hm.body.len;
        data[1] = c->recv.len;
        c->recv.len = 0;  // Cleanup receive buffer
        if (data[1] >= data[0]) * (bool *) fn_data = true;
      }
    }
  } else if (ev == MG_EV_CLOSE) {
    *(bool *) fn_data = true;  // tell event loop to stop
  } else if (ev == MG_EV_ERROR) {
    *(bool *) fn_data = true;  // Error, tell event loop to stop
  }
  (void) ev_data;
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;                    // Event manager
  bool done = false;                    // Event handler flips it to true
  const char *log_level = getenv("V");  // Log level

  mg_mgr_init(&mgr);                       // Initialise event manager
  if (log_level == NULL) log_level = "2";  // If not set, set to DEBUG
  mg_log_set(atoi(log_level));             // Set to 0 to disable debug log
  if (argc > 1) s_url = argv[1];           // Use URL from command line

  mg_http_connect(&mgr, s_url, fn, &done);  // Create client connection
  while (!done) mg_mgr_poll(&mgr, 1000);    // Infinite event loop
  mg_mgr_free(&mgr);                        // Free resources

  return 0;
}
