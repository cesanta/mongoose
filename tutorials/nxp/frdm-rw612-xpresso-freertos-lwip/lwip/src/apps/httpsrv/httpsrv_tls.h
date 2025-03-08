/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018,2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*   Header for HTTPSRV TLS.*/

#ifndef _HTTPSRV_TLS_H_
#define _HTTPSRV_TLS_H_

#include "httpsrv_config.h"

#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE
#include "httpsrv.h"

#if HTTPSRV_CFG_WOLFSSL_ENABLE
#include "wolfssl/ssl.h"

typedef WOLFSSL *httpsrv_tls_sock_t;
typedef WOLFSSL_CTX *httpsrv_tls_ctx_t;
#endif

#if HTTPSRV_CFG_MBEDTLS_ENABLE
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/debug.h"

#ifndef MBEDTLS_THREADING_C
/*
 * MBEDTLS_THREADING_C must be defined even if there is only one session,
 * as it can coincidence with the main server thread
 * which uses mbedTLS when establishing newly accepted connection.
 */
#error "MBEDTLS_THREADING_C must be defined to protect mbedTLS operations from concurrent access."
#endif

typedef mbedtls_ssl_context *httpsrv_tls_sock_t;

typedef struct
{
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt srvcert;
    mbedtls_pk_context pkey;
#if defined(MBEDTLS_SSL_CACHE_C)
    mbedtls_ssl_cache_context cache;
#endif
} *httpsrv_tls_ctx_t;

#endif

#ifdef __cplusplus
extern "C" {
#endif

httpsrv_tls_ctx_t httpsrv_tls_init(const HTTPSRV_TLS_PARAM_STRUCT *params);
void httpsrv_tls_release(httpsrv_tls_ctx_t ctx);
httpsrv_tls_sock_t httpsrv_tls_socket(httpsrv_tls_ctx_t ctx, int sock);
void httpsrv_tls_shutdown(httpsrv_tls_sock_t tls_sock);
int httpsrv_tls_recv(httpsrv_tls_sock_t tls_sock, void *buf, size_t len, int flags);
int32_t httpsrv_tls_send(httpsrv_tls_sock_t tls_sock, const void *buf, size_t len, int flags);

#ifdef __cplusplus
}
#endif

#endif /* HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE */

#endif /* _HTTPSRV_TLS_H_ */
