#pragma once
#include "net.h"

struct mg_tls_opts {
  const char *ca;         // CA certificate file. For both listeners and clients
  const char *cert;       // Certificate
  const char *certkey;    // Certificate key
  const char *ciphers;    // Cipher list
  struct mg_str srvname;  // If not empty, enables server name verification
};

void mg_tls_init(struct mg_connection *, struct mg_tls_opts *);
void mg_tls_free(struct mg_connection *);
long mg_tls_send(struct mg_connection *, const void *buf, size_t len);
long mg_tls_recv(struct mg_connection *, void *buf, size_t len);
void mg_tls_handshake(struct mg_connection *);
