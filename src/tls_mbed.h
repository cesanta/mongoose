#pragma once

#include "config.h"
#include "log.h"
#include "url.h"
#include "util.h"

#if MG_TLS == MG_TLS_MBED
#include <mbedtls/debug.h>
#include <mbedtls/error.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ssl_ticket.h>

struct mg_tls_ctx {
  int dummy;
#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_ticket_context tickets;
#endif
};

struct mg_tls {
  mbedtls_x509_crt ca;      // Parsed CA certificate
  mbedtls_x509_crt cert;    // Parsed certificate
  mbedtls_pk_context pk;    // Private key context
  mbedtls_ssl_context ssl;  // SSL/TLS context
  mbedtls_ssl_config conf;  // SSL-TLS config
#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_ticket_context ticket;  // Session tickets context
#endif
  // https://github.com/Mbed-TLS/mbedtls/blob/3b3c652d/include/mbedtls/ssl.h#L5071C18-L5076C29
  unsigned char *throttled_buf;  // see #3074
  size_t throttled_len;
};
#endif
