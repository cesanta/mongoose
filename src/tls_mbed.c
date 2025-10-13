#include "log.h"
#include "printf.h"
#include "profile.h"
#include "tls.h"
#include "util.h"

#if MG_TLS == MG_TLS_MBED

#if defined(MBEDTLS_VERSION_NUMBER) && MBEDTLS_VERSION_NUMBER >= 0x03000000
#define MG_MBEDTLS_RNG_GET , mg_mbed_rng, NULL
#else
#define MG_MBEDTLS_RNG_GET
#endif

static int mg_tls_err(struct mg_connection *c, int rc) {
  char s[80];
  mbedtls_strerror(rc, s, sizeof(s));
  MG_ERROR(("%lu %s", ((struct mg_connection *) c)->id, s));
  return rc;
}

static int mg_mbed_rng(void *ctx, unsigned char *buf, size_t len) {
  mg_random(buf, len);
  (void) ctx;
  return 0;
}

static bool mg_load_cert(struct mg_str str, mbedtls_x509_crt *p) {
  int rc;
  if (str.buf == NULL || str.buf[0] == '\0' || str.buf[0] == '*') return true;
  if (str.buf[0] == '-') str.len++;  // PEM, include trailing NUL
  if ((rc = mbedtls_x509_crt_parse(p, (uint8_t *) str.buf, str.len)) != 0) {
    MG_ERROR(("cert err %#x", -rc));
    return false;
  }
  return true;
}

static bool mg_load_key(struct mg_str str, mbedtls_pk_context *p) {
  int rc;
  if (str.buf == NULL || str.buf[0] == '\0' || str.buf[0] == '*') return true;
  if (str.buf[0] == '-') str.len++;  // PEM, include trailing NUL
  if ((rc = mbedtls_pk_parse_key(p, (uint8_t *) str.buf, str.len, NULL,
                                 0 MG_MBEDTLS_RNG_GET)) != 0) {
    MG_ERROR(("key err %#x", -rc));
    return false;
  }
  return true;
}

void mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls != NULL) {
    mbedtls_ssl_free(&tls->ssl);
    mbedtls_pk_free(&tls->pk);
    mbedtls_x509_crt_free(&tls->ca);
    mbedtls_x509_crt_free(&tls->cert);
    mbedtls_ssl_config_free(&tls->conf);
#ifdef MBEDTLS_SSL_SESSION_TICKETS
    mbedtls_ssl_ticket_free(&tls->ticket);
#endif
#if defined(MBEDTLS_VERSION_NUMBER) && MBEDTLS_VERSION_NUMBER >= 0x03000000 && \
    defined(MBEDTLS_PSA_CRYPTO_C)
    mbedtls_psa_crypto_free(); // https://github.com/Mbed-TLS/mbedtls/issues/9223#issuecomment-2144898336
#endif
    mg_free(tls);
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
    mg_error(c, "TLS handshake: -%#x", -mg_tls_err(c, rc));  // Error
  }
}

static void debug_cb(void *c, int lev, const char *s, int n, const char *s2) {
  n = (int) strlen(s2) - 1;
  MG_INFO(("%lu %d %.*s", ((struct mg_connection *) c)->id, lev, n, s2));
  (void) s;
}

void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) mg_calloc(1, sizeof(*tls));
  int rc = 0;
  c->tls = tls;
  if (c->tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }
  if (c->is_listening) goto fail;
  MG_DEBUG(("%lu Setting TLS", c->id));
  MG_PROF_ADD(c, "mbedtls_init_start");
#if defined(MBEDTLS_VERSION_NUMBER) && MBEDTLS_VERSION_NUMBER >= 0x03000000 && \
    defined(MBEDTLS_PSA_CRYPTO_C)
  psa_crypto_init();  // https://github.com/Mbed-TLS/mbedtls/issues/9072#issuecomment-2084845711
#endif
  mbedtls_ssl_init(&tls->ssl);
  mbedtls_ssl_config_init(&tls->conf);
  mbedtls_x509_crt_init(&tls->ca);
  mbedtls_x509_crt_init(&tls->cert);
  mbedtls_pk_init(&tls->pk);
  mbedtls_ssl_conf_dbg(&tls->conf, debug_cb, c);
#if defined(MG_MBEDTLS_DEBUG_LEVEL)
  mbedtls_debug_set_threshold(MG_MBEDTLS_DEBUG_LEVEL);
#endif
  if ((rc = mbedtls_ssl_config_defaults(
           &tls->conf,
           c->is_client ? MBEDTLS_SSL_IS_CLIENT : MBEDTLS_SSL_IS_SERVER,
           MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
    mg_error(c, "tls defaults %#x", -mg_tls_err(c, rc));
    goto fail;
  }
  mbedtls_ssl_conf_rng(&tls->conf, mg_mbed_rng, c);

  if (opts->ca.len == 0 || mg_strcmp(opts->ca, mg_str("*")) == 0) {
    // NOTE: MBEDTLS_SSL_VERIFY_NONE is not supported for TLS1.3 on client side
    // See https://github.com/Mbed-TLS/mbedtls/issues/7075
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_NONE);
  } else {
    if (mg_load_cert(opts->ca, &tls->ca) == false) goto fail;
    mbedtls_ssl_conf_ca_chain(&tls->conf, &tls->ca, NULL);
    if (c->is_client && opts->name.buf != NULL && opts->name.buf[0] != '\0') {
      char *host = mg_mprintf("%.*s", opts->name.len, opts->name.buf);
      mbedtls_ssl_set_hostname(&tls->ssl, host);
      MG_DEBUG(("%lu hostname verification: %s", c->id, host));
      mg_free(host);
    }
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
  }
  if (!mg_load_cert(opts->cert, &tls->cert)) goto fail;
  if (!mg_load_key(opts->key, &tls->pk)) goto fail;
  if (tls->cert.version &&
      (rc = mbedtls_ssl_conf_own_cert(&tls->conf, &tls->cert, &tls->pk)) != 0) {
    mg_error(c, "own cert %#x", -mg_tls_err(c, rc));
    goto fail;
  }

#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_conf_session_tickets_cb(
      &tls->conf, mbedtls_ssl_ticket_write, mbedtls_ssl_ticket_parse,
      &((struct mg_tls_ctx *) c->mgr->tls_ctx)->tickets);
#endif

  if ((rc = mbedtls_ssl_setup(&tls->ssl, &tls->conf)) != 0) {
    mg_error(c, "setup err %#x", -mg_tls_err(c, rc));
    goto fail;
  }
  c->is_tls = 1;
  c->is_tls_hs = 1;
  mbedtls_ssl_set_bio(&tls->ssl, c, mg_net_send, mg_net_recv, 0);
  MG_PROF_ADD(c, "mbedtls_init_end");
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
#if defined(MBEDTLS_ERR_SSL_RECEIVED_NEW_SESSION_TICKET)
  if (n == MBEDTLS_ERR_SSL_RECEIVED_NEW_SESSION_TICKET) {
    return MG_IO_WAIT;
  }
#endif
  if (n <= 0) return MG_IO_ERR;
  return n;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  long n;
  bool was_throttled = c->is_tls_throttled;  // see #3074
  n = was_throttled ? mbedtls_ssl_write(&tls->ssl, tls->throttled_buf,
                                        tls->throttled_len) /* flush old data */
                    : mbedtls_ssl_write(&tls->ssl, (unsigned char *) buf,
                                        len);  // encrypt current data
  c->is_tls_throttled =
      (n == MBEDTLS_ERR_SSL_WANT_READ || n == MBEDTLS_ERR_SSL_WANT_WRITE);
  if (was_throttled) return MG_IO_WAIT;  // flushed throttled data instead
  if (c->is_tls_throttled) {
    tls->throttled_buf = (unsigned char *)buf; // MbedTLS code actually ignores
    tls->throttled_len = len; //  these, but let's play API rules
    return (long) len;  // already encripted that when throttled
  } // if last chunk fails to be sent, it needs to be flushed
  if (n <= 0) return MG_IO_ERR;
  return n;
}

void mg_tls_flush(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (c->is_tls_throttled) {
    long n = mbedtls_ssl_write(&tls->ssl, tls->throttled_buf, tls->throttled_len);
    c->is_tls_throttled = (n == MBEDTLS_ERR_SSL_WANT_READ || n == MBEDTLS_ERR_SSL_WANT_WRITE);
  }
}

void mg_tls_ctx_init(struct mg_mgr *mgr) {
  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) mg_calloc(1, sizeof(*ctx));
  if (ctx == NULL) {
    MG_ERROR(("TLS context init OOM"));
  } else {
#ifdef MBEDTLS_SSL_SESSION_TICKETS
    int rc;
    mbedtls_ssl_ticket_init(&ctx->tickets);
    if ((rc = mbedtls_ssl_ticket_setup(&ctx->tickets, mg_mbed_rng, NULL,
                                       MBEDTLS_CIPHER_AES_128_GCM, 86400)) !=
        0) {
      MG_ERROR((" mbedtls_ssl_ticket_setup %#x", -rc));
    }
#endif
    mgr->tls_ctx = ctx;
  }
}

void mg_tls_ctx_free(struct mg_mgr *mgr) {
  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) mgr->tls_ctx;
  if (ctx != NULL) {
#ifdef MBEDTLS_SSL_SESSION_TICKETS
    mbedtls_ssl_ticket_free(&ctx->tickets);
#endif
    mg_free(ctx);
    mgr->tls_ctx = NULL;
  }
}
#endif
