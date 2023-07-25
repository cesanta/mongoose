#pragma once

#if MG_TLS == MG_TLS_OPENSSL

#include <openssl/err.h>
#include <openssl/ssl.h>

struct mg_tls_ctx {
  X509 *server_cert;
  EVP_PKEY *server_key;
  STACK_OF(X509_INFO) *server_ca;
  X509 *client_cert;
  EVP_PKEY *client_key;
  STACK_OF(X509_INFO) *client_ca;
};

struct mg_tls {
  SSL_CTX *ctx;
  SSL *ssl;
};
#endif
