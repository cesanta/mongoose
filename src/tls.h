#pragma once

#include "net.h"
#include "tls_mbed.h"
#include "tls_openssl.h"

struct mg_tls_opts {
  const char *server_ca;         // CA certificate file for server
  const char *client_ca;         // CA certificate file for client
  const char *server_cert;
  const char *server_key;
  const char *client_cert;
  const char *client_key;
  struct mg_fs *fs;
};

struct mg_tls_session_opts {
  struct mg_str srvname;  // If not empty, enables server name verification
};

void* mg_tls_ctx_init(const struct mg_tls_opts *);
void mg_tls_ctx_free(void *ctx);
void mg_tls_init(struct mg_connection *, struct mg_tls_session_opts *opts);
void mg_tls_free(struct mg_connection *);
long mg_tls_send(struct mg_connection *, const void *buf, size_t len);
long mg_tls_recv(struct mg_connection *, void *buf, size_t len);
size_t mg_tls_pending(struct mg_connection *);
void mg_tls_handshake(struct mg_connection *);
