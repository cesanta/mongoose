#include "fs.h"
#include "printf.h"
#include "tls.h"

#if MG_TLS == MG_TLS_MBED

#if defined(MBEDTLS_VERSION_NUMBER) && MBEDTLS_VERSION_NUMBER >= 0x03000000
#define MGRNG , rng_get, NULL
#else
#define MGRNG
#endif

void mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls != NULL) {
    mbedtls_ssl_free(&tls->ssl);
    mbedtls_ssl_config_free(&tls->conf);
    free(tls);
    c->tls = NULL;
  }
}

static int mg_net_send(void *ctx, const unsigned char *buf, size_t len) {
  long n = mg_io_send((struct mg_connection *) ctx, buf, len);
  MG_VERBOSE(("%lu n=%ld e=%d", ((struct mg_connection *) ctx)->id, n, errno));
  if (n == MG_IO_WAIT) return MBEDTLS_ERR_SSL_WANT_WRITE;
  if (n == MG_IO_RESET) return MBEDTLS_ERR_NET_CONN_RESET;
  if (n == MG_IO_ERR) return MBEDTLS_ERR_NET_SEND_FAILED;
  return (int) n;
}

static int mg_net_recv(void *ctx, unsigned char *buf, size_t len) {
  long n = mg_io_recv((struct mg_connection *) ctx, buf, len);
  MG_VERBOSE(("%lu n=%ld", ((struct mg_connection *) ctx)->id, n));
  if (n == MG_IO_WAIT) return MBEDTLS_ERR_SSL_WANT_WRITE;
  if (n == MG_IO_RESET) return MBEDTLS_ERR_NET_CONN_RESET;
  if (n == MG_IO_ERR) return MBEDTLS_ERR_NET_RECV_FAILED;
  return (int) n;
}

void mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc = mbedtls_ssl_handshake(&tls->ssl);
  if (rc == 0) {  // Success
    MG_DEBUG(("%lu success", c->id));
    c->is_tls_hs = 0;
    mg_call(c, MG_EV_TLS_HS, NULL);
  } else if (rc == MBEDTLS_ERR_SSL_WANT_READ ||
             rc == MBEDTLS_ERR_SSL_WANT_WRITE) {  // Still pending
    MG_VERBOSE(("%lu pending, %d%d %d (-%#x)", c->id, c->is_connecting,
                c->is_tls_hs, rc, -rc));
  } else {
    mg_error(c, "TLS handshake: -%#x", -rc);  // Error
  }
}

static int mbed_rng(void *ctx, unsigned char *buf, size_t len) {
  mg_random(buf, len);
  (void) ctx;
  return 0;
}

static void debug_cb(void *c, int lev, const char *s, int n, const char *s2) {
  n = (int) strlen(s2) - 1;
  MG_INFO(("%lu %d %.*s", ((struct mg_connection *) c)->id, lev, n, s2));
  (void) s;
}

#ifdef MBEDTLS_SSL_SESSION_TICKETS
static int rng_get(void *p_rng, unsigned char *buf, size_t len) {
  (void) p_rng;
  mg_random(buf, len);
  return 0;
}
#endif

void mg_tls_init(struct mg_connection *c, struct mg_str hostname) {
  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) c->mgr->tls_ctx;
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  int rc = 0;

  c->tls = tls;
  if (c->tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }

  MG_DEBUG(("%lu Setting TLS", c->id));
  mbedtls_ssl_init(&tls->ssl);
  mbedtls_ssl_config_init(&tls->conf);
  mbedtls_ssl_conf_dbg(&tls->conf, debug_cb, c);
#if defined(MG_MBEDTLS_DEBUG_LEVEL)
  mbedtls_debug_set_threshold(MG_MBEDTLS_DEBUG_LEVEL);
#endif
  if ((rc = mbedtls_ssl_config_defaults(
           &tls->conf,
           c->is_client ? MBEDTLS_SSL_IS_CLIENT : MBEDTLS_SSL_IS_SERVER,
           MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
    mg_error(c, "tls defaults %#x", -rc);
    goto fail;
  }
  mbedtls_ssl_conf_rng(&tls->conf, mbed_rng, c);

  if (c->is_client && ctx->client_ca.version) {
    mbedtls_ssl_conf_ca_chain(&tls->conf, &ctx->client_ca, NULL);
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
    if (hostname.ptr != NULL && hostname.ptr[0] != '\0') {
      struct mg_addr addr;
      if (!mg_aton(hostname, &addr)) {  // if srvname is not an IP address
        char *host = mg_mprintf("%.*s", (int) hostname.len, hostname.ptr);
        mbedtls_ssl_set_hostname(&tls->ssl, host);
        free(host);
      }
    }
  } else if (!c->is_client && ctx->server_ca.version) {
    mbedtls_ssl_conf_ca_chain(&tls->conf, &ctx->server_ca, NULL);
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
  } else {
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_NONE);
  }
  if (c->is_client && ctx->client_cert.version &&
      (rc = mbedtls_ssl_conf_own_cert(&tls->conf, &ctx->client_cert,
                                      &ctx->client_key)) != 0) {
    mg_error(c, "own cert %#x", -rc);
    goto fail;
  }
  if (!c->is_client && ctx->server_cert.version &&
      (rc = mbedtls_ssl_conf_own_cert(&tls->conf, &ctx->server_cert,
                                      &ctx->server_key)) != 0) {
    mg_error(c, "own cert %#x", -rc);
    goto fail;
  }
#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_conf_session_tickets_cb(&tls->conf, mbedtls_ssl_ticket_write,
                                      mbedtls_ssl_ticket_parse,
                                      &ctx->ticket_ctx);
#endif

  if ((rc = mbedtls_ssl_setup(&tls->ssl, &tls->conf)) != 0) {
    mg_error(c, "setup err %#x", -rc);
    goto fail;
  }

  c->tls = tls;
  c->is_tls = 1;
  c->is_tls_hs = 1;
  mbedtls_ssl_set_bio(&tls->ssl, c, mg_net_send, mg_net_recv, 0);
  if (c->is_client && c->is_resolving == 0 && c->is_connecting == 0) {
    mg_tls_handshake(c);
  }
  return;
fail:
  mg_tls_free(c);
}

size_t mg_tls_pending(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  return tls == NULL ? 0 : mbedtls_ssl_get_bytes_avail(&tls->ssl);
}

long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  long n = mbedtls_ssl_read(&tls->ssl, (unsigned char *) buf, len);
  if (n == MBEDTLS_ERR_SSL_WANT_READ || n == MBEDTLS_ERR_SSL_WANT_WRITE)
    return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  long n = mbedtls_ssl_write(&tls->ssl, (unsigned char *) buf, len);
  if (n == MBEDTLS_ERR_SSL_WANT_READ || n == MBEDTLS_ERR_SSL_WANT_WRITE)
    return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

static bool load_cert(struct mg_str str, mbedtls_x509_crt *p) {
  int rc;
  if (str.ptr == NULL || str.ptr[0] == '\0' || str.ptr[0] == '*') return true;
  if (str.ptr[0] == '-') str.len++;  // PEM, include trailing NUL
  if ((rc = mbedtls_x509_crt_parse(p, (uint8_t *) str.ptr, str.len)) != 0) {
    MG_ERROR(("cert err %#x", -rc));
    return false;
  }
  return true;
}

static bool load_key(struct mg_str str, mbedtls_pk_context *p) {
  int rc;
  if (str.ptr == NULL || str.ptr[0] == '\0' || str.ptr[0] == '*') return true;
  if (str.ptr[0] == '-') str.len++;  // PEM, include trailing NUL
  if ((rc = mbedtls_pk_parse_key(p, (uint8_t *) str.ptr, str.len, NULL,
                                 0 MGRNG)) != 0) {
    MG_ERROR(("key err %#x", -rc));
    return false;
  }
  return true;
}

void mg_tls_ctx_init(struct mg_mgr *mgr, const struct mg_tls_opts *opts) {
  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) calloc(1, sizeof(*ctx));
  if (ctx == NULL) goto fail;
  MG_DEBUG(("Setting up TLS context"));

#if defined(MG_MBEDTLS_DEBUG_LEVEL)
  mbedtls_debug_set_threshold(MG_MBEDTLS_DEBUG_LEVEL);
#endif

  if (!load_cert(opts->client_ca, &ctx->client_ca)) goto fail;
  if (!load_cert(opts->server_ca, &ctx->server_ca)) goto fail;
  if (!load_cert(opts->client_cert, &ctx->client_cert)) goto fail;
  if (!load_cert(opts->server_cert, &ctx->server_cert)) goto fail;
  if (!load_key(opts->server_key, &ctx->server_key)) goto fail;
  if (!load_key(opts->client_key, &ctx->client_key)) goto fail;

#ifdef MBEDTLS_SSL_SESSION_TICKETS
  {
    int rc;
    mbedtls_ssl_ticket_init(&ctx->ticket_ctx);
    if ((rc = mbedtls_ssl_ticket_setup(&ctx->ticket_ctx, rng_get, NULL,
                                       MBEDTLS_CIPHER_AES_128_GCM, 86400)) !=
        0) {
      MG_ERROR(("setup session tickets err %#x", -rc));
      goto fail;
    }
  }
#endif
  mgr->tls_ctx = ctx;
  return;
fail:
  mg_tls_ctx_free(mgr);
}

void mg_tls_ctx_free(struct mg_mgr *mgr) {
  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) mgr->tls_ctx;
  if (ctx != NULL) {
    mbedtls_x509_crt_free(&ctx->server_cert);
    mbedtls_pk_free(&ctx->server_key);
    mbedtls_x509_crt_free(&ctx->client_cert);
    mbedtls_pk_free(&ctx->client_key);
    mbedtls_x509_crt_free(&ctx->client_ca);
    mbedtls_x509_crt_free(&ctx->server_ca);
#ifdef MBEDTLS_SSL_SESSION_TICKETS
    mbedtls_ssl_ticket_free(&ctx->ticket_ctx);
#endif
    free(ctx);
    mgr->tls_ctx = NULL;
  }
}
#endif
