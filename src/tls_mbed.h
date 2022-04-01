#pragma once

#include "config.h"
#include "log.h"
#include "url.h"
#include "util.h"

#if MG_ENABLE_MBEDTLS
#include <mbedtls/debug.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>

struct mg_tls {
  char *cafile;             // CA certificate path
  mbedtls_x509_crt ca;      // Parsed CA certificate
  mbedtls_x509_crt cert;    // Parsed certificate
  mbedtls_ssl_context ssl;  // SSL/TLS context
  mbedtls_ssl_config conf;  // SSL-TLS config
  mbedtls_pk_context pk;    // Private key context
};
#endif
