#pragma once

#if MG_ENABLE_OPENSSL

#include <openssl/err.h>
#include <openssl/ssl.h>

struct mg_tls {
  SSL_CTX *ctx;
  SSL *ssl;
};
#endif
