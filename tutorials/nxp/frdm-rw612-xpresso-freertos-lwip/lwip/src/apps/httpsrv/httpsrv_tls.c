/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*   This file contains the HTTPSRV TLS implementation.*/
#include "httpsrv_tls.h"
#include "httpsrv_port.h"
#include "httpsrv_supp.h"

#if HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE

/* Initialize TLS */
httpsrv_tls_ctx_t httpsrv_tls_init(const HTTPSRV_TLS_PARAM_STRUCT *params)
{
    httpsrv_tls_ctx_t ctx = NULL;

    if (params)
    {
#if HTTPSRV_CFG_WOLFSSL_ENABLE
        WOLFSSL_METHOD *method;

        /* Initialize wolfSSL library */
        wolfSSL_Init();

        /* Get encryption method */
        method = wolfTLSv1_2_server_method();

        /* Create wolfSSL_CTX */
        ctx = wolfSSL_CTX_new(method);
        if (ctx && params->certificate_buffer && params->certificate_buffer_size && params->private_key_buffer &&
            params->private_key_buffer_size)
        {
            /* Add cert to ctx */
            if (wolfSSL_CTX_use_certificate_buffer(ctx, params->certificate_buffer, params->certificate_buffer_size,
                                                   SSL_FILETYPE_PEM) != SSL_SUCCESS)
            {
                goto ERROR;
            }

            /* Load server key into ctx */
            if (wolfSSL_CTX_use_PrivateKey_buffer(ctx, params->private_key_buffer, params->private_key_buffer_size,
                                                  SSL_FILETYPE_PEM) != SSL_SUCCESS)
            {
                goto ERROR;
            }
        }
#endif /* HTTPSRV_CFG_WOLFSSL_ENABLE */

#if HTTPSRV_CFG_MBEDTLS_ENABLE

        const char *pers = "ssl_server";

        ctx = httpsrv_mem_alloc_zero(sizeof(*ctx));
        if (ctx)
        {
            mbedtls_ssl_config_init(&ctx->conf);
#if defined(MBEDTLS_SSL_CACHE_C)
            mbedtls_ssl_cache_init(&ctx->cache);
#endif
            mbedtls_x509_crt_init(&ctx->srvcert);
            mbedtls_pk_init(&ctx->pkey);
            mbedtls_entropy_init(&ctx->entropy);
            mbedtls_ctr_drbg_init(&ctx->ctr_drbg);

#if defined(MBEDTLS_DEBUG_C)
            mbedtls_debug_set_threshold(0); /* Set this to >= 2 to see debug info */
#endif

            if (params->certificate_buffer && params->certificate_buffer_size && params->private_key_buffer &&
                params->private_key_buffer_size)
            {
                if (mbedtls_x509_crt_parse(&ctx->srvcert, params->certificate_buffer,
                                           params->certificate_buffer_size) != 0)
                {
                    goto ERROR;
                }

                if (mbedtls_pk_parse_key(&ctx->pkey, params->private_key_buffer, params->private_key_buffer_size, NULL,
                                         0) != 0)
                {
                    goto ERROR;
                }

                /* Seed the RNG  */
                if (mbedtls_ctr_drbg_seed(&ctx->ctr_drbg, mbedtls_entropy_func, &ctx->entropy,
                                          (const unsigned char *)pers, strlen(pers)) != 0)
                {
                    goto ERROR;
                }

                /* Setup stuff */
                if (mbedtls_ssl_config_defaults(&ctx->conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM,
                                                MBEDTLS_SSL_PRESET_DEFAULT) != 0)
                {
                    goto ERROR;
                }

                mbedtls_ssl_conf_rng(&ctx->conf, mbedtls_ctr_drbg_random, &ctx->ctr_drbg);
#if 0 /* Debug info.*/
               mbedtls_ssl_conf_dbg(&ctx->conf, my_debug, NULL);
#endif
#if defined(MBEDTLS_SSL_CACHE_C)
                mbedtls_ssl_conf_session_cache(&ctx->conf, &ctx->cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
#endif
                mbedtls_ssl_conf_ca_chain(&ctx->conf, ctx->srvcert.next, NULL);
                if (mbedtls_ssl_conf_own_cert(&ctx->conf, &ctx->srvcert, &ctx->pkey) != 0)
                {
                    goto ERROR;
                }
            }
        }
#endif /* HTTPSRV_CFG_MBEDTLS_ENABLE */
    }
    return ctx;

ERROR:
    if (ctx)
    {
        httpsrv_tls_release(ctx);
    }

    return NULL;
}

/* Release TLS */
void httpsrv_tls_release(httpsrv_tls_ctx_t ctx)
{
    if (ctx)
    {
#if HTTPSRV_CFG_WOLFSSL_ENABLE
        wolfSSL_CTX_free(ctx);
        wolfSSL_Cleanup();
#endif /* HTTPSRV_CFG_WOLFSSL_ENABLE */

#if HTTPSRV_CFG_MBEDTLS_ENABLE
        mbedtls_x509_crt_free(&ctx->srvcert);
        mbedtls_pk_free(&ctx->pkey);
        mbedtls_ssl_config_free(&ctx->conf);
#if defined(MBEDTLS_SSL_CACHE_C)
        mbedtls_ssl_cache_free(&ctx->cache);
#endif
        mbedtls_ctr_drbg_free(&ctx->ctr_drbg);
        mbedtls_entropy_free(&ctx->entropy);

        httpsrv_mem_free(ctx);
#endif
    }
}

#if HTTPSRV_CFG_MBEDTLS_ENABLE

static int httpsrv_mbedtls_send(void *ctx, unsigned char const *buf, size_t len)
{
    int result;

    result = lwip_send((int)ctx, buf, len, 0);

    return result;
}

static int httpsrv_mbedtls_recv(void *ctx, unsigned char *buf, size_t len)
{
    int result;

    result = lwip_recv((int)ctx, buf, len, 0);

    return result;
}

#endif /* HTTPSRV_CFG_MBEDTLS_ENABLE */

/* Create TLS connection using sock, return TLS connection handle. */
httpsrv_tls_sock_t httpsrv_tls_socket(httpsrv_tls_ctx_t ctx, int sock)
{
    httpsrv_tls_sock_t tls_sock;

#if HTTPSRV_CFG_WOLFSSL_ENABLE
    /* Create wolfSSL object */
    tls_sock = wolfSSL_new(ctx);
    if (tls_sock)
    {
        /* Associate the socket (file descriptor) with the session.*/
        wolfSSL_set_fd(tls_sock, sock);
    }
#endif

#if HTTPSRV_CFG_MBEDTLS_ENABLE
    tls_sock = httpsrv_mem_alloc_zero(sizeof(*tls_sock));
    if (tls_sock)
    {
        mbedtls_ssl_init(tls_sock);

        if (mbedtls_ssl_setup(tls_sock, &ctx->conf) != 0)
        {
            httpsrv_mem_free(tls_sock);
            tls_sock = 0;
        }
        else
        {
            int ret;

            /* set IO functions */
            mbedtls_ssl_set_bio(tls_sock, (void *)sock, httpsrv_mbedtls_send, httpsrv_mbedtls_recv, NULL);

            /* perform handshake */
            ret = mbedtls_ssl_handshake(tls_sock);
            if (ret < 0)
            {
#if 0
                LWIP_PLATFORM_DIAG((" failed\r\n  ! mbedtls_ssl_handshake error %d", ret));
#endif
                httpsrv_tls_shutdown(tls_sock);
                tls_sock = 0;
            }
        }
    }
#endif
    return tls_sock;
}

/* Shutdown SSL connection. */
void httpsrv_tls_shutdown(httpsrv_tls_sock_t tls_sock)
{
    if (tls_sock)
    {
#if HTTPSRV_CFG_WOLFSSL_ENABLE
        /* Shuts down an active SSL/TLS connection */
        wolfSSL_shutdown(tls_sock);
        /* Free TLS socket.*/
        wolfSSL_free(tls_sock);
#endif

#if HTTPSRV_CFG_MBEDTLS_ENABLE
        /* Shuts down an active SSL/TLS connection */
        mbedtls_ssl_close_notify(tls_sock);
        /* Free TLS socket.*/
        mbedtls_ssl_free(tls_sock);

        httpsrv_mem_free(tls_sock);
#endif
    }
}

/* Receive data from TLS layer. */
int httpsrv_tls_recv(httpsrv_tls_sock_t tls_sock, void *buf, size_t len, int flags)
{
    int result;

#if HTTPSRV_CFG_WOLFSSL_ENABLE
    result = wolfSSL_recv(tls_sock, buf, len, flags);
#endif

#if HTTPSRV_CFG_MBEDTLS_ENABLE
    result = mbedtls_ssl_read(tls_sock, buf, len);
#endif

    return result;
}

/* Send data through TLS layer. */
int32_t httpsrv_tls_send(httpsrv_tls_sock_t tls_sock, const void *buf, size_t len, int flags)
{
    int result;

#if HTTPSRV_CFG_WOLFSSL_ENABLE
    result = wolfSSL_send(tls_sock, buf, len, flags);
#endif

#if HTTPSRV_CFG_MBEDTLS_ENABLE
    result = mbedtls_ssl_write(tls_sock, buf, len);
#endif

    return result;
}

#endif /* HTTPSRV_CFG_WOLFSSL_ENABLE || HTTPSRV_CFG_MBEDTLS_ENABLE */
