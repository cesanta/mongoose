#pragma once

#include "config.h"
#include "log.h"
#include "url.h"
#include "util.h"

#if MG_ENABLE_MBEDTLS
#include <mbedtls/debug.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ssl_ticket.h>

struct mg_tls_ctx {
  mbedtls_x509_crt ca;      // Parsed CA certificate
  mbedtls_x509_crt cert;    // Parsed certificate
  mbedtls_pk_context pk;    // Private key context
  mbedtls_ssl_ticket_context ticket_ctx; // Session tickets context
  uint8_t have_ca:1;        // CA certificate is set
  uint8_t have_cert:1;      // Certificate is set
};

struct mg_tls {
  mbedtls_ssl_config conf;  // SSL-TLS config
  struct mbedtls_ssl_context ssl; // SSL-TLS context
};

#endif
