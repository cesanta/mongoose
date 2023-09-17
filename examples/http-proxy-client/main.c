// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example HTTP client that uses a proxy server. Usage:
//    make
//    ./example PROXY:PORT http://www.ladyada.net
//
// To enable SSL/TLS, see https://mongoose.ws/tutorials/tls/#how-to-build
//
#include "mongoose.h"

// Print HTTP response and signal that we're done
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  const char *url = fn_data;
  static bool connected;
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    printf("%.*s", (int) hm->message.len, hm->message.ptr);
    exit(EXIT_SUCCESS);
  } else if (ev == MG_EV_CONNECT) {
    // Proxy TCP connection established. Send CONNECT request
    struct mg_str host = mg_url_host(url);

    if (mg_url_is_ssl(url)) {
      struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
                                 .name = host};
      mg_tls_init(c, &opts);
    }

    // c->is_hexdumping = 1;
    mg_printf(c, "CONNECT %.*s:%hu HTTP/1.1\r\nHost: %.*s:%hu\r\n\r\n",
              (int) host.len, host.ptr, mg_url_port(url), (int) host.len,
              host.ptr, mg_url_port(url));
  } else if (!connected && ev == MG_EV_READ) {
    struct mg_http_message hm;
    int n = mg_http_parse((char *) c->recv.buf, c->recv.len, &hm);
    if (n > 0) {
      struct mg_str host = mg_url_host(url);
      // CONNECT response - tunnel is established
      connected = true;
      MG_DEBUG(
          ("Connected to proxy, status: %.*s", (int) hm.uri.len, hm.uri.ptr));
      mg_iobuf_del(&c->recv, 0, n);
      // Send request to the target server
      mg_printf(c,
                "GET %s HTTP/1.0\r\n"
                "Host: %.*s\r\n"
                "\r\n",
                mg_url_uri(url), (int) host.len, host.ptr);
    }
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s PROXY_URL URL\n", argv[0]);
    return EXIT_FAILURE;
  }

  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_connect(&mgr, argv[1], fn, argv[2]);  // Connect to the proxy
  for (;;) mg_mgr_poll(&mgr, 1000);             // Event loop
  mg_mgr_free(&mgr);

  return 0;
}
