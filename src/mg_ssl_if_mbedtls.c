/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_SSL && MG_SSL_IF == MG_SSL_IF_MBEDTLS

#include <mbedtls/debug.h>
#include <mbedtls/ecp.h>
#include <mbedtls/net.h>
#include <mbedtls/platform.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ssl_internal.h>
#include <mbedtls/x509_crt.h>
#include <mbedtls/version.h>

static void mg_ssl_mbed_log(void *ctx, int level, const char *file, int line,
                            const char *str) {
  enum cs_log_level cs_level;
  switch (level) {
    case 1:
      cs_level = LL_ERROR;
      break;
    case 2:
      cs_level = LL_INFO;
      break;
    case 3:
      cs_level = LL_DEBUG;
      break;
    default:
      cs_level = LL_VERBOSE_DEBUG;
  }
  /* mbedTLS passes strings with \n at the end, strip it. */
  LOG(cs_level, ("%p %.*s", ctx, (int) (strlen(str) - 1), str));
  (void) ctx;
  (void) str;
  (void) file;
  (void) line;
  (void) cs_level;
}

struct mg_ssl_if_ctx {
  mbedtls_ssl_config *conf;
  mbedtls_ssl_context *ssl;
  mbedtls_x509_crt *cert;
  mbedtls_pk_context *key;
  mbedtls_x509_crt *ca_cert;
  struct mbuf cipher_suites;
  size_t saved_len;
};

/* Must be provided by the platform. ctx is struct mg_connection. */
extern int mg_ssl_if_mbed_random(void *ctx, unsigned char *buf, size_t len);

void mg_ssl_if_init() {
  LOG(LL_INFO, ("%s", MBEDTLS_VERSION_STRING_FULL));
}

enum mg_ssl_if_result mg_ssl_if_conn_accept(struct mg_connection *nc,
                                            struct mg_connection *lc) {
  struct mg_ssl_if_ctx *ctx =
      (struct mg_ssl_if_ctx *) MG_CALLOC(1, sizeof(*ctx));
  struct mg_ssl_if_ctx *lc_ctx = (struct mg_ssl_if_ctx *) lc->ssl_if_data;
  nc->ssl_if_data = ctx;
  if (ctx == NULL || lc_ctx == NULL) return MG_SSL_ERROR;
  ctx->ssl = (mbedtls_ssl_context *) MG_CALLOC(1, sizeof(*ctx->ssl));
  if (mbedtls_ssl_setup(ctx->ssl, lc_ctx->conf) != 0) {
    return MG_SSL_ERROR;
  }
  return MG_SSL_OK;
}

static enum mg_ssl_if_result mg_use_cert(struct mg_ssl_if_ctx *ctx,
                                         const char *cert, const char *key,
                                         const char **err_msg);
static enum mg_ssl_if_result mg_use_ca_cert(struct mg_ssl_if_ctx *ctx,
                                            const char *cert);
static enum mg_ssl_if_result mg_set_cipher_list(struct mg_ssl_if_ctx *ctx,
                                                const char *ciphers);
#ifdef MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED
static enum mg_ssl_if_result mg_ssl_if_mbed_set_psk(struct mg_ssl_if_ctx *ctx,
                                                    const char *identity,
                                                    const char *key);
#endif

enum mg_ssl_if_result mg_ssl_if_conn_init(
    struct mg_connection *nc, const struct mg_ssl_if_conn_params *params,
    const char **err_msg) {
  struct mg_ssl_if_ctx *ctx =
      (struct mg_ssl_if_ctx *) MG_CALLOC(1, sizeof(*ctx));
  DBG(("%p %s,%s,%s", nc, (params->cert ? params->cert : ""),
       (params->key ? params->key : ""),
       (params->ca_cert ? params->ca_cert : "")));

  if (ctx == NULL) {
    MG_SET_PTRPTR(err_msg, "Out of memory");
    return MG_SSL_ERROR;
  }
  nc->ssl_if_data = ctx;
  ctx->conf = (mbedtls_ssl_config *) MG_CALLOC(1, sizeof(*ctx->conf));
  mbuf_init(&ctx->cipher_suites, 0);
  mbedtls_ssl_config_init(ctx->conf);
  mbedtls_ssl_conf_dbg(ctx->conf, mg_ssl_mbed_log, nc);
  if (mbedtls_ssl_config_defaults(
          ctx->conf, (nc->flags & MG_F_LISTENING ? MBEDTLS_SSL_IS_SERVER
                                                 : MBEDTLS_SSL_IS_CLIENT),
          MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT) != 0) {
    MG_SET_PTRPTR(err_msg, "Failed to init SSL config");
    return MG_SSL_ERROR;
  }

  /* TLS 1.2 and up */
  mbedtls_ssl_conf_min_version(ctx->conf, MBEDTLS_SSL_MAJOR_VERSION_3,
                               MBEDTLS_SSL_MINOR_VERSION_3);
  mbedtls_ssl_conf_rng(ctx->conf, mg_ssl_if_mbed_random, nc);

  if (params->cert != NULL &&
      mg_use_cert(ctx, params->cert, params->key, err_msg) != MG_SSL_OK) {
    return MG_SSL_ERROR;
  }

  if (params->ca_cert != NULL &&
      mg_use_ca_cert(ctx, params->ca_cert) != MG_SSL_OK) {
    MG_SET_PTRPTR(err_msg, "Invalid SSL CA cert");
    return MG_SSL_ERROR;
  }

  if (mg_set_cipher_list(ctx, params->cipher_suites) != MG_SSL_OK) {
    MG_SET_PTRPTR(err_msg, "Invalid cipher suite list");
    return MG_SSL_ERROR;
  }

#ifdef MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED
  if (mg_ssl_if_mbed_set_psk(ctx, params->psk_identity, params->psk_key) !=
      MG_SSL_OK) {
    MG_SET_PTRPTR(err_msg, "Invalid PSK settings");
    return MG_SSL_ERROR;
  }
#endif

  if (!(nc->flags & MG_F_LISTENING)) {
    ctx->ssl = (mbedtls_ssl_context *) MG_CALLOC(1, sizeof(*ctx->ssl));
    mbedtls_ssl_init(ctx->ssl);
    if (mbedtls_ssl_setup(ctx->ssl, ctx->conf) != 0) {
      MG_SET_PTRPTR(err_msg, "Failed to create SSL session");
      return MG_SSL_ERROR;
    }
    if (params->server_name != NULL &&
        mbedtls_ssl_set_hostname(ctx->ssl, params->server_name) != 0) {
      return MG_SSL_ERROR;
    }
  }

#ifdef MG_SSL_IF_MBEDTLS_MAX_FRAG_LEN
  if (mbedtls_ssl_conf_max_frag_len(ctx->conf,
#if MG_SSL_IF_MBEDTLS_MAX_FRAG_LEN == 512
                                    MBEDTLS_SSL_MAX_FRAG_LEN_512
#elif MG_SSL_IF_MBEDTLS_MAX_FRAG_LEN == 1024
                                    MBEDTLS_SSL_MAX_FRAG_LEN_1024
#elif MG_SSL_IF_MBEDTLS_MAX_FRAG_LEN == 2048
                                    MBEDTLS_SSL_MAX_FRAG_LEN_2048
#elif MG_SSL_IF_MBEDTLS_MAX_FRAG_LEN == 4096
                                    MBEDTLS_SSL_MAX_FRAG_LEN_4096
#else
#error Invalid MG_SSL_IF_MBEDTLS_MAX_FRAG_LEN
#endif
                                    ) != 0) {
    return MG_SSL_ERROR;
  }
#endif

  nc->flags |= MG_F_SSL;

  return MG_SSL_OK;
}

static int mg_ssl_if_mbed_send(void *ctx, const unsigned char *buf,
                               size_t len) {
  struct mg_connection *nc = (struct mg_connection *) ctx;
  int n = nc->iface->vtable->tcp_send(nc, buf, len);
  if (n > 0) return n;
  if (n == 0) return MBEDTLS_ERR_SSL_WANT_WRITE;
  return MBEDTLS_ERR_NET_SEND_FAILED;
}

static int mg_ssl_if_mbed_recv(void *ctx, unsigned char *buf, size_t len) {
  struct mg_connection *nc = (struct mg_connection *) ctx;
  int n = nc->iface->vtable->tcp_recv(nc, buf, len);
  if (n > 0) return n;
  if (n == 0) return MBEDTLS_ERR_SSL_WANT_READ;
  return MBEDTLS_ERR_NET_RECV_FAILED;
}

static enum mg_ssl_if_result mg_ssl_if_mbed_err(struct mg_connection *nc,
                                                int ret) {
  enum mg_ssl_if_result res = MG_SSL_OK;
  if (ret == MBEDTLS_ERR_SSL_WANT_READ) {
    res = MG_SSL_WANT_READ;
  } else if (ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
    res = MG_SSL_WANT_WRITE;
  } else if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
    LOG(LL_DEBUG, ("%p TLS connection closed by peer", nc));
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
    res = MG_SSL_OK;
  } else {
    LOG(LL_ERROR, ("%p mbedTLS error: -0x%04x", nc, -ret));
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
    res = MG_SSL_ERROR;
  }
  nc->err = ret;
  return res;
}

static void mg_ssl_if_mbed_free_certs_and_keys(struct mg_ssl_if_ctx *ctx) {
  if (ctx->cert != NULL) {
    mbedtls_x509_crt_free(ctx->cert);
    MG_FREE(ctx->cert);
    ctx->cert = NULL;
    mbedtls_pk_free(ctx->key);
    MG_FREE(ctx->key);
    ctx->key = NULL;
  }
  if (ctx->ca_cert != NULL) {
    mbedtls_ssl_conf_ca_chain(ctx->conf, NULL, NULL);
#ifdef MBEDTLS_X509_CA_CHAIN_ON_DISK
    if (ctx->conf->ca_chain_file != NULL) {
      MG_FREE((void *) ctx->conf->ca_chain_file);
      ctx->conf->ca_chain_file = NULL;
    }
#endif
    mbedtls_x509_crt_free(ctx->ca_cert);
    MG_FREE(ctx->ca_cert);
    ctx->ca_cert = NULL;
  }
}

enum mg_ssl_if_result mg_ssl_if_handshake(struct mg_connection *nc) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  int err;
  /* If bio is not yet set, do it now. */
  if (ctx->ssl->p_bio == NULL) {
    mbedtls_ssl_set_bio(ctx->ssl, nc, mg_ssl_if_mbed_send, mg_ssl_if_mbed_recv,
                        NULL);
  }
  err = mbedtls_ssl_handshake(ctx->ssl);
  if (err != 0) return mg_ssl_if_mbed_err(nc, err);
#ifdef MG_SSL_IF_MBEDTLS_FREE_CERTS
  /*
   * Free the peer certificate, we don't need it after handshake.
   * Note that this effectively disables renegotiation.
   */
  mbedtls_x509_crt_free(ctx->ssl->session->peer_cert);
  mbedtls_free(ctx->ssl->session->peer_cert);
  ctx->ssl->session->peer_cert = NULL;
  /* On a client connection we can also free our own and CA certs. */
  if (nc->listener == NULL) {
    if (ctx->conf->key_cert != NULL) {
      /* Note that this assumes one key_cert entry, which matches our init. */
      MG_FREE(ctx->conf->key_cert);
      ctx->conf->key_cert = NULL;
    }
    mbedtls_ssl_conf_ca_chain(ctx->conf, NULL, NULL);
    mg_ssl_if_mbed_free_certs_and_keys(ctx);
  }
#endif
  return MG_SSL_OK;
}

int mg_ssl_if_read(struct mg_connection *nc, void *buf, size_t len) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  int n = mbedtls_ssl_read(ctx->ssl, (unsigned char *) buf, len);
  DBG(("%p %d -> %d", nc, (int) len, n));
  if (n < 0) return mg_ssl_if_mbed_err(nc, n);
  if (n == 0) nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  return n;
}

int mg_ssl_if_write(struct mg_connection *nc, const void *buf, size_t len) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  /* Per mbedTLS docs, if write returns WANT_READ or WANT_WRITE, the operation
   * should be retried with the same data and length.
   * Here we assume that the data being pushed will remain the same but the
   * amount may grow between calls so we save the length that was used and
   * retry. The assumption being that the data itself won't change and won't
   * be removed. */
  size_t l = len;
  if (ctx->saved_len > 0 && ctx->saved_len < l) l = ctx->saved_len;
  int n = mbedtls_ssl_write(ctx->ssl, (const unsigned char *) buf, l);
  DBG(("%p %d,%d,%d -> %d", nc, (int) len, (int) ctx->saved_len, (int) l, n));
  if (n < 0) {
    if (n == MBEDTLS_ERR_SSL_WANT_READ || n == MBEDTLS_ERR_SSL_WANT_WRITE) {
      ctx->saved_len = len;
    }
    return mg_ssl_if_mbed_err(nc, n);
  } else if (n > 0) {
    ctx->saved_len = 0;
  }
  return n;
}

void mg_ssl_if_conn_close_notify(struct mg_connection *nc) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  if (ctx == NULL) return;
  mbedtls_ssl_close_notify(ctx->ssl);
}

void mg_ssl_if_conn_free(struct mg_connection *nc) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  if (ctx == NULL) return;
  nc->ssl_if_data = NULL;
  if (ctx->ssl != NULL) {
    mbedtls_ssl_free(ctx->ssl);
    MG_FREE(ctx->ssl);
  }
  mg_ssl_if_mbed_free_certs_and_keys(ctx);
  if (ctx->conf != NULL) {
    mbedtls_ssl_config_free(ctx->conf);
    MG_FREE(ctx->conf);
  }
  mbuf_free(&ctx->cipher_suites);
  memset(ctx, 0, sizeof(*ctx));
  MG_FREE(ctx);
}

static enum mg_ssl_if_result mg_use_ca_cert(struct mg_ssl_if_ctx *ctx,
                                            const char *ca_cert) {
  if (ca_cert == NULL || strcmp(ca_cert, "*") == 0) {
    mbedtls_ssl_conf_authmode(ctx->conf, MBEDTLS_SSL_VERIFY_NONE);
    return MG_SSL_OK;
  }
  ctx->ca_cert = (mbedtls_x509_crt *) MG_CALLOC(1, sizeof(*ctx->ca_cert));
  mbedtls_x509_crt_init(ctx->ca_cert);
#ifdef MBEDTLS_X509_CA_CHAIN_ON_DISK
  ca_cert = strdup(ca_cert);
  mbedtls_ssl_conf_ca_chain_file(ctx->conf, ca_cert, NULL);
#else
  if (mbedtls_x509_crt_parse_file(ctx->ca_cert, ca_cert) != 0) {
    return MG_SSL_ERROR;
  }
  mbedtls_ssl_conf_ca_chain(ctx->conf, ctx->ca_cert, NULL);
#endif
  mbedtls_ssl_conf_authmode(ctx->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
  return MG_SSL_OK;
}

static enum mg_ssl_if_result mg_use_cert(struct mg_ssl_if_ctx *ctx,
                                         const char *cert, const char *key,
                                         const char **err_msg) {
  if (key == NULL) key = cert;
  if (cert == NULL || cert[0] == '\0' || key == NULL || key[0] == '\0') {
    return MG_SSL_OK;
  }
  ctx->cert = (mbedtls_x509_crt *) MG_CALLOC(1, sizeof(*ctx->cert));
  mbedtls_x509_crt_init(ctx->cert);
  ctx->key = (mbedtls_pk_context *) MG_CALLOC(1, sizeof(*ctx->key));
  mbedtls_pk_init(ctx->key);
  if (mbedtls_x509_crt_parse_file(ctx->cert, cert) != 0) {
    MG_SET_PTRPTR(err_msg, "Invalid SSL cert");
    return MG_SSL_ERROR;
  }
  if (mbedtls_pk_parse_keyfile(ctx->key, key, NULL) != 0) {
    MG_SET_PTRPTR(err_msg, "Invalid SSL key");
    return MG_SSL_ERROR;
  }
  if (mbedtls_ssl_conf_own_cert(ctx->conf, ctx->cert, ctx->key) != 0) {
    MG_SET_PTRPTR(err_msg, "Invalid SSL key or cert");
    return MG_SSL_ERROR;
  }
  return MG_SSL_OK;
}

static const int mg_s_cipher_list[] = {
#if CS_PLATFORM != CS_P_ESP8266
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_DHE_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_DHE_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA,
    MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA,
#else
    /*
     * ECDHE is way too slow on ESP8266 w/o cryptochip, this sometimes results
     * in WiFi STA deauths. Use weaker but faster cipher suites. Sad but true.
     * Disable DHE completely because it's just hopelessly slow.
     */
    MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA,
    MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA,
#endif /* CS_PLATFORM != CS_P_ESP8266 */
    0,
};

/*
 * Ciphers can be specified as a colon-separated list of cipher suite names.
 * These can be found in
 * https://github.com/ARMmbed/mbedtls/blob/development/library/ssl_ciphersuites.c#L267
 * E.g.: TLS-ECDHE-ECDSA-WITH-AES-128-GCM-SHA256:TLS-DHE-RSA-WITH-AES-256-CCM
 */
static enum mg_ssl_if_result mg_set_cipher_list(struct mg_ssl_if_ctx *ctx,
                                                const char *ciphers) {
  if (ciphers != NULL) {
    int l, id;
    const char *s = ciphers, *e;
    char tmp[50];
    while (s != NULL) {
      e = strchr(s, ':');
      l = (e != NULL ? (e - s) : (int) strlen(s));
      strncpy(tmp, s, l);
      tmp[l] = '\0';
      id = mbedtls_ssl_get_ciphersuite_id(tmp);
      DBG(("%s -> %04x", tmp, id));
      if (id != 0) {
        mbuf_append(&ctx->cipher_suites, &id, sizeof(id));
      }
      s = (e != NULL ? e + 1 : NULL);
    }
    if (ctx->cipher_suites.len == 0) return MG_SSL_ERROR;
    id = 0;
    mbuf_append(&ctx->cipher_suites, &id, sizeof(id));
    mbuf_trim(&ctx->cipher_suites);
    mbedtls_ssl_conf_ciphersuites(ctx->conf,
                                  (const int *) ctx->cipher_suites.buf);
  } else {
    mbedtls_ssl_conf_ciphersuites(ctx->conf, mg_s_cipher_list);
  }
  return MG_SSL_OK;
}

#ifdef MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED
static enum mg_ssl_if_result mg_ssl_if_mbed_set_psk(struct mg_ssl_if_ctx *ctx,
                                                    const char *identity,
                                                    const char *key_str) {
  unsigned char key[32];
  size_t key_len;
  if (identity == NULL && key_str == NULL) return MG_SSL_OK;
  if (identity == NULL || key_str == NULL) return MG_SSL_ERROR;
  key_len = strlen(key_str);
  if (key_len != 32 && key_len != 64) return MG_SSL_ERROR;
  size_t i = 0;
  memset(key, 0, sizeof(key));
  key_len = 0;
  for (i = 0; key_str[i] != '\0'; i++) {
    unsigned char c;
    char hc = tolower((int) key_str[i]);
    if (hc >= '0' && hc <= '9') {
      c = hc - '0';
    } else if (hc >= 'a' && hc <= 'f') {
      c = hc - 'a' + 0xa;
    } else {
      return MG_SSL_ERROR;
    }
    key_len = i / 2;
    key[key_len] <<= 4;
    key[key_len] |= c;
  }
  key_len++;
  DBG(("identity = '%s', key = (%u)", identity, (unsigned int) key_len));
  /* mbedTLS makes copies of psk and identity. */
  if (mbedtls_ssl_conf_psk(ctx->conf, (const unsigned char *) key, key_len,
                           (const unsigned char *) identity,
                           strlen(identity)) != 0) {
    return MG_SSL_ERROR;
  }
  return MG_SSL_OK;
}
#endif

const char *mg_set_ssl(struct mg_connection *nc, const char *cert,
                       const char *ca_cert) {
  const char *err_msg = NULL;
  struct mg_ssl_if_conn_params params;
  memset(&params, 0, sizeof(params));
  params.cert = cert;
  params.ca_cert = ca_cert;
  if (mg_ssl_if_conn_init(nc, &params, &err_msg) != MG_SSL_OK) {
    return err_msg;
  }
  return NULL;
}

/* Lazy RNG. Warning: it would be a bad idea to do this in production! */
#ifdef MG_SSL_MBED_DUMMY_RANDOM
int mg_ssl_if_mbed_random(void *ctx, unsigned char *buf, size_t len) {
  (void) ctx;
  while (len--) *buf++ = rand();
  return 0;
}
#endif

#endif /* MG_ENABLE_SSL && MG_SSL_IF == MG_SSL_IF_MBEDTLS */
