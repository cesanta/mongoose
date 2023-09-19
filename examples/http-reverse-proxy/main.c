// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example HTTP reverse proxy
//    1. Run `make`. This builds and starts a proxy on port 8000
//    2. Start your browser, go to https://localhost:8000
//
// To enable SSL/TLS, see https://mongoose.ws/tutorials/tls/#how-to-build

#include "mongoose.h"

static const char *s_backend_url =
#if MG_TLS
    "https://cesanta.com";
#else
    "http://info.cern.ch";
#endif
static const char *s_listen_url = "http://localhost:8000";

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
  MG_DEBUG(("FORWARDING: %.*s %.*s", (int) hm->method.len, hm->method.ptr,
            (int) hm->uri.len, hm->uri.ptr));
}

static void fn2(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_connection *c2 = (struct mg_connection *) fn_data;
  if (ev == MG_EV_READ) {
    // All incoming data from the backend, forward to the client
    if (c2 != NULL) mg_send(c2, c->recv.buf, c->recv.len);
    mg_iobuf_del(&c->recv, 0, c->recv.len);
  } else if (ev == MG_EV_CLOSE) {
    if (c2 != NULL) c2->fn_data = NULL;
  }
  (void) ev_data;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_connection *c2 = fn_data;
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    // Client request, create backend connection Note that we're passing
    // client connection `c` as fn_data for the created backend connection.
    c2 = mg_connect(c->mgr, s_backend_url, fn2, c);
    if (c2 == NULL) {
      mg_error(c, "Cannot create backend connection");
    } else {
      if (mg_url_is_ssl(s_backend_url)) {
        struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
                                   .name = mg_url_host(s_backend_url)};
        mg_tls_init(c2, &opts);
      }
      c->fn_data = c2;
      forward_request(hm, c2);
      c->is_resp = 0;  // process further msgs in keep-alive connection
      c2->is_hexdumping = 1;
    }
  } else if (ev == MG_EV_CLOSE) {
    if (c2 != NULL) c2->is_closing = 1;
    if (c2 != NULL) c2->fn_data = NULL;
  }
}

int main(void) {
  struct mg_mgr mgr;

  mg_log_set(MG_LL_DEBUG);                       // Set log level
  mg_mgr_init(&mgr);                             // Initialise event manager
  mg_http_listen(&mgr, s_listen_url, fn, NULL);  // Start proxy
  for (;;) mg_mgr_poll(&mgr, 1000);              // Event loop
  mg_mgr_free(&mgr);

  return 0;
}
