// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example HTTP reverse proxy
//    1. Run `make`. This builds and starts a proxy on port 8000
//    2. Start your browser, go to https://localhost:8000
//
// To enable SSL/TLS, build it like this:
//    make MBEDTLS_DIR=/path/to/your/mbedtls/installation

static const char *s_backend_url = "https://cesanta.com";
static const char *s_listen_url = "http://localhost:8000";

#include "mongoose.h"

// Forward client request to the backend connection, rewriting the Host header
static void forward_request(struct mg_http_message *hm,
                            struct mg_connection *c) {
  size_t i, max = sizeof(hm->headers) / sizeof(hm->headers[0]);
  struct mg_str host = mg_url_host(s_backend_url);
  mg_printf(c, "%.*s\r\n",
            (int) (hm->proto.ptr + hm->proto.len - hm->message.ptr),
            hm->message.ptr);
  for (i = 0; i < max && hm->headers[i].name.len > 0; i++) {
    struct mg_str *k = &hm->headers[i].name, *v = &hm->headers[i].value;
    if (mg_strcmp(*k, mg_str("Host")) == 0) v = &host;
    mg_printf(c, "%.*s: %.*s\r\n", (int) k->len, k->ptr, (int) v->len, v->ptr);
  }
  mg_send(c, "\r\n", 2);
  mg_send(c, hm->body.ptr, hm->body.len);
  LOG(LL_DEBUG, ("FORWARDING: %.*s %.*s", (int) hm->method.len, hm->method.ptr,
                 (int) hm->uri.len, hm->uri.ptr));
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_connection *c2 = fn_data;
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (c->label[0] != 'B' && c2 == NULL) {
      // Client request, create backend connection Note that we're passing
      // client connection `c` as fn_data for the created backend connection.
      // This way, we tie together these two connections via `fn_data` pointer:
      // client's c->fn_data points to the backend connection, and backend's
      // c->fn_data points to the client connection
      c2 = mg_connect(c->mgr, s_backend_url, fn, c);
      c->fn_data = c2;
      if (c2 != NULL) {
        c2->is_hexdumping = 1;
        c2->label[0] = 'B';  // Mark this connection as backend
      } else {
        c->is_closing = 1;
      }
    }
    if (c2 != NULL && c2->label[0] == 'B') forward_request(hm, c2);
  } else if (ev == MG_EV_READ) {
    if (c->label[0] == 'B' && c2 != NULL) {
      // All incoming data from the backend, forward to the client
      mg_send(c2, c->recv.buf, c->recv.len);
      mg_iobuf_delete(&c->recv, c->recv.len);
    }
  } else if (ev == MG_EV_CONNECT) {
    if (mg_url_is_ssl(s_backend_url)) {
      struct mg_tls_opts opts = {.ca = "ca.pem"};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_CLOSE) {
    if (c2 != NULL) c2->is_closing = 1;
    c->fn_data = NULL;
  }
}

int main(void) {
  struct mg_mgr mgr;

  mg_log_set("3");                               // Set log level
  mg_mgr_init(&mgr);                             // Initialise event manager
  mg_http_listen(&mgr, s_listen_url, fn, NULL);  // Start proxy
  for (;;) mg_mgr_poll(&mgr, 1000);              // Event loop
  mg_mgr_free(&mgr);

  return 0;
}
