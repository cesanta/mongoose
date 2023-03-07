#pragma once

#if MG_ENABLE_WOLFSSL

#include <wolfssl/options.h>
#include <wolfssl/ssl.h> /* wolfSSL secure read/write methods */

struct mg_tls {
  WOLFSSL_CTX *ctx;
  WOLFSSL *ssl;
};
#endif
