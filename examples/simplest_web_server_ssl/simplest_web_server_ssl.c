/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_SSL
/*
 * This example starts an SSL web server on https://localhost:8443/
 *
 * Please note that the certificate used is a self-signed one and will not be
 * recognised as valid. You should expect an SSL error and will need to
 * explicitly allow the browser to proceed.
 */

#include "mongoose.h"

static const char *s_http_port = "8443";
static const char *s_ssl_cert = "server.pem";
static const char *s_ssl_key = "server.key";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_bind_opts bind_opts;
  const char *err;

  mg_mgr_init(&mgr, NULL);
  memset(&bind_opts, 0, sizeof(bind_opts));
  bind_opts.ssl_cert = s_ssl_cert;
  bind_opts.ssl_key = s_ssl_key;
  bind_opts.error_string = &err;

  printf("Starting SSL server on port %s, cert from %s, key from %s\n",
         s_http_port, bind_opts.ssl_cert, bind_opts.ssl_key);
  nc = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);
  if (nc == NULL) {
    printf("Failed to create listener: %s\n", err);
    return 1;
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
#else
int main(void) {
  return 0;
}
#endif /* MG_ENABLE_SSL */
