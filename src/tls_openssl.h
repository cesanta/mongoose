#pragma once

#if MG_TLS == MG_TLS_OPENSSL || MG_TLS == MG_TLS_WOLFSSL

#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>

struct mg_tls {
  BIO_METHOD *bm;
  SSL_CTX *ctx;
  SSL *ssl;
  char *name;       // matching hostname
  bool check_name;  // set when hostname was set, but no CA certificate given
};
#endif
