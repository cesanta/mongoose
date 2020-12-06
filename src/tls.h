#pragma once
#include "net.h"

struct mg_tls_opts {
  const char *ca;        // CA certificate file. For both listeners and clients
  const char *cert;      // Certificate
  const char *certkey;   // Certificate key
  const char *ciphers;   // Cipher list
  const char *srvname;   // If not NULL, enables server name verification
};

int mg_tls_init(struct mg_connection *, struct mg_tls_opts *);
int mg_tls_free(struct mg_connection *);
int mg_tls_send(struct mg_connection *, const void *buf, size_t len, int *fail);
int mg_tls_recv(struct mg_connection *, void *buf, size_t len, int *fail);
int mg_tls_handshake(struct mg_connection *);
