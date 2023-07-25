#pragma once

#define MG_TLS_NONE 0     // No TLS support
#define MG_TLS_MBED 1     // mbedTLS
#define MG_TLS_OPENSSL 2  // OpenSSL
#define MG_TLS_BUILTIN 3  // Built-in
#define MG_TLS_CUSTOM 4   // Custom implementation

#ifndef MG_TLS
#define MG_TLS MG_TLS_NONE
#endif

#include "net.h"
#include "tls_mbed.h"
#include "tls_openssl.h"

struct mg_tls_opts {
  struct mg_str client_ca;
  struct mg_str server_ca;
  struct mg_str server_cert;
  struct mg_str server_key;
  struct mg_str client_cert;
  struct mg_str client_key;
};

void mg_tls_ctx_init(struct mg_mgr *, const struct mg_tls_opts *);
void mg_tls_ctx_free(struct mg_mgr *);
void mg_tls_init(struct mg_connection *, struct mg_str hostname);
void mg_tls_free(struct mg_connection *);
long mg_tls_send(struct mg_connection *, const void *buf, size_t len);
long mg_tls_recv(struct mg_connection *, void *buf, size_t len);
size_t mg_tls_pending(struct mg_connection *);
void mg_tls_handshake(struct mg_connection *);
