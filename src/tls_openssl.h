#pragma once

#if MG_TLS == MG_TLS_OPENSSL || MG_TLS == MG_TLS_WOLFSSL

#include <openssl/err.h>
#include <openssl/ssl.h>

struct mg_tls {
  BIO_METHOD *bm;
  SSL_CTX *ctx;
  SSL *ssl;
};
#endif
