#pragma once

#if MG_ENABLE_MBEDTLS
#include "log.h"
#include "url.h"
#include "util.h"

#include <mbedtls/debug.h>
#include <mbedtls/ssl.h>

#if defined(MBEDTLS_VERSION_NUMBER) && MBEDTLS_VERSION_NUMBER >= 0x03000000
#define RNG , rng_get, NULL
#else
#define RNG
#endif

// Different versions have those in different files, so declare here
EXTERN_C int mbedtls_net_recv(void *, unsigned char *, size_t);
EXTERN_C int mbedtls_net_send(void *, const unsigned char *, size_t);

struct mg_tls {
  char *cafile;             // CA certificate path
  mbedtls_x509_crt ca;      // Parsed CA certificate
  mbedtls_x509_crl crl;     // Parsed Certificate Revocation List
  mbedtls_x509_crt cert;    // Parsed certificate
  mbedtls_ssl_context ssl;  // SSL/TLS context
  mbedtls_ssl_config conf;  // SSL-TLS config
  mbedtls_pk_context pk;    // Private key context
};
#endif
