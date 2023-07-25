#pragma once

#include "config.h"
#include "log.h"
#include "url.h"
#include "util.h"

#if MG_TLS == MG_TLS_MBED
#include <mbedtls/debug.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ssl_ticket.h>

struct mg_tls_ctx {
  mbedtls_x509_crt server_ca;     // Parsed CA certificate
  mbedtls_x509_crt client_ca;     // Parsed CA certificate
  mbedtls_x509_crt server_cert;   // Parsed server certificate
  mbedtls_pk_context server_key;  // Parsed server private key context
  mbedtls_x509_crt client_cert;   // Parsed client certificate
  mbedtls_pk_context client_key;  // Parsed client private key context
#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_ticket_context ticket_ctx;  // Session tickets context
#endif
};

struct mg_tls {
  mbedtls_ssl_context ssl;  // SSL/TLS context
  mbedtls_ssl_config conf;  // SSL-TLS config
};
#endif
