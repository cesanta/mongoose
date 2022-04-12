#pragma once

#include "net.h"
#include "tls_mbed.h"
#include "tls_openssl.h"

struct mg_tls_opts {
  const char *ca;         // CA certificate file. For both listeners and clients
  const char *crl;        // Certificate Revocation List. For clients
  const char *cert;       // Certificate
  const char *certkey;    // Certificate key
  const char *ciphers;    // Cipher list
  struct mg_str srvname;  // If not empty, enables server name verification
  struct mg_fs *fs;       // FS API for reading certificate files
};

void mg_tls_init(struct mg_connection *, const struct mg_tls_opts *);
void mg_tls_free(struct mg_connection *);
long mg_tls_send(struct mg_connection *, const void *buf, size_t len);
long mg_tls_recv(struct mg_connection *, void *buf, size_t len);
size_t mg_tls_pending(struct mg_connection *);
void mg_tls_handshake(struct mg_connection *);
