#include "fs.h"
#include "tls.h"

#if MG_ENABLE_MBEDTLS

#if defined(MBEDTLS_VERSION_NUMBER) && MBEDTLS_VERSION_NUMBER >= 0x03000000
#define MGRNG , rng_get, NULL
#else
#define MGRNG
#endif

void mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls != NULL) {
    free(tls->cafile);
    mbedtls_ssl_free(&tls->ssl);
    mbedtls_pk_free(&tls->pk);
    mbedtls_x509_crt_free(&tls->ca);
    mbedtls_x509_crt_free(&tls->cert);
    mbedtls_ssl_config_free(&tls->conf);
    free(tls);
    c->tls = NULL;
  }
}

bool mg_sock_would_block(void);
bool mg_sock_conn_reset(void);

static int mg_net_send(void *ctx, const unsigned char *buf, size_t len) {
  struct mg_connection *c = (struct mg_connection *) ctx;
  int fd = (int) (size_t) c->fd;
  int n = (int) send(fd, buf, len, 0);
  MG_VERBOSE(("%lu n=%d, errno=%d", c->id, n, errno));
  if (n < 0) {
    if (mg_sock_would_block()) return MBEDTLS_ERR_SSL_WANT_WRITE;
    if (mg_sock_conn_reset()) return MBEDTLS_ERR_NET_CONN_RESET;
    return MBEDTLS_ERR_NET_SEND_FAILED;
  }
  return n;
}

static int mg_net_recv(void *ctx, unsigned char *buf, size_t len) {
  struct mg_connection *c = (struct mg_connection *) ctx;
  int n, fd = (int) (size_t) c->fd;
  n = (int) recv(fd, buf, len, 0);
  MG_VERBOSE(("%lu n=%d, errno=%d", c->id, n, errno));
  if (n < 0) {
    if (mg_sock_would_block()) return MBEDTLS_ERR_SSL_WANT_READ;
    if (mg_sock_conn_reset()) return MBEDTLS_ERR_NET_CONN_RESET;
    return MBEDTLS_ERR_NET_RECV_FAILED;
  }
  return n;
}

void mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc;
  mbedtls_ssl_set_bio(&tls->ssl, c, mg_net_send, mg_net_recv, 0);
  rc = mbedtls_ssl_handshake(&tls->ssl);
  if (rc == 0) {  // Success
    MG_DEBUG(("%lu success", c->id));
    c->is_tls_hs = 0;
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
  MG_VERBOSE(("%lu %d %.*s", ((struct mg_connection *) c)->id, lev, n, s2));
  (void) s;
}

#if defined(MBEDTLS_VERSION_NUMBER) && MBEDTLS_VERSION_NUMBER >= 0x03000000
static int rng_get(void *p_rng, unsigned char *buf, size_t len) {
  (void) p_rng;
  mg_random(buf, len);
  return 0;
}
#endif

static struct mg_str mg_loadfile(struct mg_fs *fs, const char *path) {
  size_t n = 0;
  if (path[0] == '-') return mg_str(path);
  char *p = mg_file_read(fs, path, &n);
  return mg_str_n(p, n);
}

void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  struct mg_fs *fs = opts->fs == NULL ? &mg_fs_posix : opts->fs;
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
    mg_error(c, "tls defaults %#x", -rc);
    goto fail;
  }
  mbedtls_ssl_conf_rng(&tls->conf, mbed_rng, c);
  if (opts->ca == NULL || strcmp(opts->ca, "*") == 0) {
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_NONE);
  } else if (opts->ca != NULL && opts->ca[0] != '\0') {
#if defined(MBEDTLS_X509_CA_CHAIN_ON_DISK)
    tls->cafile = strdup(opts->ca);
    rc = mbedtls_ssl_conf_ca_chain_file(&tls->conf, tls->cafile, NULL);
    if (rc != 0) {
      mg_error(c, "parse on-disk chain(%s) err %#x", tls->cafile, -rc);
      goto fail;
    }
#else
    struct mg_str s = mg_loadfile(fs, opts->ca);
    rc = mbedtls_x509_crt_parse(&tls->ca, (uint8_t *) s.ptr, s.len + 1);
    if (opts->ca[0] != '-') free((char *) s.ptr);
    if (rc != 0) {
      mg_error(c, "parse(%s) err %#x", opts->ca, -rc);
      goto fail;
    }
    mbedtls_ssl_conf_ca_chain(&tls->conf, &tls->ca, NULL);
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
    struct mg_str s = mg_loadfile(fs, opts->cert);
    const char *key = opts->certkey == NULL ? opts->cert : opts->certkey;
    rc = mbedtls_x509_crt_parse(&tls->cert, (uint8_t *) s.ptr, s.len + 1);
    if (opts->cert[0] != '-') free((char *) s.ptr);
    if (rc != 0) {
      mg_error(c, "parse(%s) err %#x", opts->cert, -rc);
      goto fail;
    }
    s = mg_loadfile(fs, key);
    rc = mbedtls_pk_parse_key(&tls->pk, (uint8_t *) s.ptr, s.len + 1, NULL,
                              0 MGRNG);
    if (key[0] != '-') free((char *) s.ptr);
    if (rc != 0) {
      mg_error(c, "tls key(%s) %#x", key, -rc);
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
  mg_tls_free(c);
}

size_t mg_tls_pending(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  return tls == NULL ? 0 : mbedtls_ssl_get_bytes_avail(&tls->ssl);
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
#endif
