#include "tls.h"

#if MG_ENABLE_MBEDTLS
void mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc;
  mbedtls_ssl_set_bio(&tls->ssl, &c->fd, mbedtls_net_send, mbedtls_net_recv, 0);
  rc = mbedtls_ssl_handshake(&tls->ssl);
  if (rc == 0) {  // Success
    LOG(LL_DEBUG, ("%lu success", c->id));
    c->is_tls_hs = 0;
  } else if (rc == MBEDTLS_ERR_SSL_WANT_READ ||
             rc == MBEDTLS_ERR_SSL_WANT_WRITE) {  // Still pending
    LOG(LL_VERBOSE_DEBUG, ("%lu pending, %d%d %d (-%#x)", c->id,
                           c->is_connecting, c->is_tls_hs, rc, -rc));
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
  LOG(LL_VERBOSE_DEBUG, ("%p %.*s", ((struct mg_connection *) c)->fd, n, s2));
  (void) s;
  (void) c;
  (void) lev;
}

#if defined(MBEDTLS_VERSION_NUMBER) && MBEDTLS_VERSION_NUMBER >= 0x03000000
static int rng_get(void *p_rng, unsigned char *buf, size_t len) {
  (void) p_rng;
  mg_random(buf, len);
  return 0;
}
#endif

void mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  int rc = 0;
  const char *ca = opts->ca == NULL     ? "-"
                   : opts->ca[0] == '-' ? "(emb)"
                                        : opts->ca;
  const char *crl = opts->crl == NULL     ? "-"
                    : opts->crl[0] == '-' ? "(emb)"
                                          : opts->crl;
  const char *cert = opts->cert == NULL     ? "-"
                     : opts->cert[0] == '-' ? "(emb)"
                                            : opts->cert;
  const char *certkey = opts->certkey == NULL     ? "-"
                        : opts->certkey[0] == '-' ? "(emb)"
                                                  : opts->certkey;
  if (tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  } else if (opts->ifn != NULL && opts->ifn(opts) != 0) {
    mg_error(c, "TLS custom init failed");
    goto fail;
  }
  LOG(LL_DEBUG, ("%lu Setting TLS, CA: %s, CRL: %s, cert: %s, key: %s", c->id,
                 ca, crl, cert, certkey));
  mbedtls_ssl_init(&tls->ssl);
  mbedtls_ssl_config_init(&tls->conf);
  mbedtls_x509_crt_init(&tls->ca);
  mbedtls_x509_crl_init(&tls->crl);
  mbedtls_x509_crt_init(&tls->cert);
  mbedtls_pk_init(&tls->pk);
  mbedtls_ssl_conf_dbg(&tls->conf, debug_cb, c);
  //#if !defined(ESP_PLATFORM)
  // mbedtls_debug_set_threshold(5);
  //#endif
  if ((rc = mbedtls_ssl_config_defaults(
           &tls->conf,
           c->is_client ? MBEDTLS_SSL_IS_CLIENT : MBEDTLS_SSL_IS_SERVER,
           MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
    mg_error(c, "tls defaults %#x", -rc);
    goto fail;
  }
  mbedtls_ssl_conf_rng(&tls->conf, mbed_rng, c);
  if (opts->ca == NULL || strcmp(opts->ca, "*") == 0) {
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_NONE);
  }
  if (opts->ca != NULL && opts->ca[0] != '\0') {
    if (opts->crl != NULL && opts->crl[0] != '\0') {
      rc = opts->crl[0] == '-'
               ? mbedtls_x509_crl_parse(&tls->crl, (uint8_t *) opts->crl,
                                        strlen(opts->crl) + 1)
               : mbedtls_x509_crl_parse_file(&tls->crl, opts->crl);
      if (rc != 0) {
        mg_error(c, "parse(%s) err %#x", crl, -rc);
        goto fail;
      }
    }
#if defined(MBEDTLS_X509_CA_CHAIN_ON_DISK)
    tls->cafile = strdup(opts->ca);
    rc = mbedtls_ssl_conf_ca_chain_file(&tls->conf, tls->cafile, &tls->crl);
    if (rc != 0) {
      mg_error(c, "parse on-disk chain(%s) err %#x", ca, -rc);
      goto fail;
    }
#else
    rc = opts->ca[0] == '-'
             ? mbedtls_x509_crt_parse(&tls->ca, (uint8_t *) opts->ca,
                                      strlen(opts->ca) + 1)
             : mbedtls_x509_crt_parse_file(&tls->ca, opts->ca);
    if (rc != 0) {
      mg_error(c, "parse(%s) err %#x", ca, -rc);
      goto fail;
    }
    mbedtls_ssl_conf_ca_chain(&tls->conf, &tls->ca, &tls->crl);
#endif
    if (opts->srvname.len > 0) {
      char mem[128], *buf = mem;
      mg_asprintf(&buf, sizeof(mem), "%.*s", (int) opts->srvname.len,
                  opts->srvname.ptr);
      mbedtls_ssl_set_hostname(&tls->ssl, buf);
      if (buf != mem) free(buf);
    }
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
  }
  if (opts->cert != NULL && opts->cert[0] != '\0') {
    const char *key = opts->certkey;
    if (key == NULL) {
      key = opts->cert;
      certkey = cert;
    }
    rc = opts->cert[0] == '-'
             ? mbedtls_x509_crt_parse(&tls->cert, (uint8_t *) opts->cert,
                                      strlen(opts->cert) + 1)
             : mbedtls_x509_crt_parse_file(&tls->cert, opts->cert);
    if (rc != 0) {
      mg_error(c, "parse(%s) err %#x", cert, -rc);
      goto fail;
    }
    rc = key[0] == '-' ? mbedtls_pk_parse_key(&tls->pk, (uint8_t *) key,
                                              strlen(key) + 1, NULL, 0 RNG)
                       : mbedtls_pk_parse_keyfile(&tls->pk, key, NULL RNG);
    if (rc != 0) {
      mg_error(c, "tls key(%s) %#x", certkey, -rc);
      goto fail;
    }
    rc = mbedtls_ssl_conf_own_cert(&tls->conf, &tls->cert, &tls->pk);
    if (rc != 0) {
      mg_error(c, "own cert %#x", -rc);
      goto fail;
    }
  }
  if ((rc = mbedtls_ssl_setup(&tls->ssl, &tls->conf)) != 0) {
    mg_error(c, "setup err %#x", -rc);
    goto fail;
  }
  c->tls = tls;
  c->is_tls = 1;
  c->is_tls_hs = 1;
  if (c->is_client && c->is_resolving == 0 && c->is_connecting == 0) {
    mg_tls_handshake(c);
  }
  return;
fail:
  c->is_closing = 1;
  free(tls);
}

long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  long n = mbedtls_ssl_read(&tls->ssl, (unsigned char *) buf, len);
  return n == 0 ? -1 : n == MBEDTLS_ERR_SSL_WANT_READ ? 0 : n;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  long n = mbedtls_ssl_write(&tls->ssl, (unsigned char *) buf, len);
  return n == 0 ? -1 : n == MBEDTLS_ERR_SSL_WANT_WRITE ? 0 : n;
}

void mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls == NULL) return;
  free(tls->cafile);
  mbedtls_ssl_free(&tls->ssl);
  mbedtls_pk_free(&tls->pk);
  mbedtls_x509_crt_free(&tls->ca);
  mbedtls_x509_crl_free(&tls->crl);
  mbedtls_x509_crt_free(&tls->cert);
  mbedtls_ssl_config_free(&tls->conf);
  free(tls);
  c->tls = NULL;
}
#endif
