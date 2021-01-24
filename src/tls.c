#include "tls.h"

#if MG_ENABLE_MBEDTLS  ///////////////////////////////////////// MBEDTLS
#include "log.h"
#include "url.h"
#include "util.h"

#include <mbedtls/debug.h>
#include <mbedtls/ssl.h>

// Different versions have those in different files, so declare here
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C int mbedtls_net_recv(void *, unsigned char *, size_t);
EXTERN_C int mbedtls_net_send(void *, const unsigned char *, size_t);

struct mg_tls {
  char *cafile;             // CA certificate path
  mbedtls_x509_crt ca;      // Parsed CA certificate
  mbedtls_x509_crt cert;    // Parsed certificate
  mbedtls_ssl_context ssl;  // SSL/TLS context
  mbedtls_ssl_config conf;  // SSL-TLS config
  mbedtls_pk_context pk;    // Private key context
};

int mg_tls_handshake(struct mg_connection *c) {
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
  return rc == 0;
}

static int mbed_rng(void *ctx, unsigned char *buf, size_t len) {
  mg_random(buf, len);
  (void) ctx;
  return 0;
}

static void debug_cb(void *c, int lev, const char *s, int n, const char *s2) {
  n = strlen(s2) - 1;
  LOG(LL_VERBOSE_DEBUG, ("%p %.*s", ((struct mg_connection *) c)->fd, n, s2));
  (void) s;
  (void) c;
  (void) lev;
}

int mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  int rc = 0;
  LOG(LL_DEBUG, ("%lu Setting TLS, CA: %s, cert: %s, key: %s", c->id,
                 opts->ca == NULL ? "null" : opts->ca,
                 opts->cert == NULL ? "null" : opts->cert,
                 opts->certkey == NULL ? "null" : opts->certkey));
  mbedtls_ssl_init(&tls->ssl);
  mbedtls_ssl_config_init(&tls->conf);
  mbedtls_ssl_conf_dbg(&tls->conf, debug_cb, c);
#if !defined(ESP_PLATFORM)
  mbedtls_debug_set_threshold(5);
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
  }
  if (opts->ca != NULL && opts->ca[0] != '\0') {
#if defined(MBEDTLS_X509_CA_CHAIN_ON_DISK)
    tls->cafile = strdup(opts->ca);
    rc = mbedtls_ssl_conf_ca_chain_file(&tls->conf, tls->cafile, NULL);
    if (rc != 0) {
      mg_error(c, "parse on-disk chain(%s) err %#x", opts->ca, -rc);
      goto fail;
    }
#else
    mbedtls_x509_crt_init(&tls->ca);
    if ((rc = mbedtls_x509_crt_parse_file(&tls->ca, opts->ca)) != 0) {
      mg_error(c, "parse(%s) err %#x", opts->ca, -rc);
      goto fail;
    }
    mbedtls_ssl_conf_ca_chain(&tls->conf, &tls->ca, NULL);
#endif
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
  }
  if (opts->cert != NULL && opts->cert[0] != '\0') {
    const char *key = opts->certkey;
    if (key == NULL) key = opts->cert;
    mbedtls_x509_crt_init(&tls->cert);
    mbedtls_pk_init(&tls->pk);
    if ((rc = mbedtls_x509_crt_parse_file(&tls->cert, opts->cert)) != 0) {
      mg_error(c, "parse(%s) err %#x", opts->cert, -rc);
      goto fail;
    }
    if ((rc = mbedtls_pk_parse_keyfile(&tls->pk, key, NULL)) != 0) {
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
  return 1;
fail:
  c->is_closing = 1;
  free(tls);
  return 0;
}

int mg_tls_recv(struct mg_connection *c, void *buf, size_t len, int *fail) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = mbedtls_ssl_read(&tls->ssl, (unsigned char *) buf, len);
  *fail = (n == 0) || (n < 0 && n != MBEDTLS_ERR_SSL_WANT_READ);
  return n;
}

int mg_tls_send(struct mg_connection *c, const void *buf, size_t len,
                int *fail) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = mbedtls_ssl_write(&tls->ssl, (unsigned char *) buf, len);
  *fail = (n == 0) || (n < 0 && n != MBEDTLS_ERR_SSL_WANT_WRITE);
  return n;
}

int mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls == NULL) return 0;
  free(tls->cafile);
  mbedtls_x509_crt_free(&tls->ca);
  mbedtls_x509_crt_free(&tls->cert);
  mbedtls_ssl_free(&tls->ssl);
  mbedtls_pk_free(&tls->pk);
  mbedtls_ssl_config_free(&tls->conf);
  free(tls);
  return 1;
}
#elif MG_ENABLE_OPENSSL  ///////////////////////////////////////// OPENSSL

#include <openssl/ssl.h>
#if defined(_MSC_VER) && _MSC_VER < 1700
typedef long ssize_t;
#endif

extern void ERR_clear_error(void);          // Defined in openssl/err.h, but
extern void ERR_print_errors_fp(FILE *fp);  // declare here for krypton

struct mg_tls {
  SSL_CTX *ctx;
  SSL *ssl;
};

static int mg_tls_err(struct mg_tls *tls, int res) {
  int err = SSL_get_error(tls->ssl, res);
  // We've just fetched the last error from the queue.
  // Now we need to clear the error queue. If we do not, then the following
  // can happen (actually reported):
  //  - A new connection is accept()-ed with cert error (e.g. self-signed cert)
  //  - Since all accept()-ed connections share listener's context,
  //  - *ALL* SSL accepted connection report read error on the next poll cycle.
  //    Thus a single errored connection can close all the rest, unrelated ones.
  // Clearing the error keeps the shared SSL_CTX in an OK state.
  ERR_clear_error();
  if (err == SSL_ERROR_WANT_READ) return 0;
  if (err == SSL_ERROR_WANT_WRITE) return 0;
  return err;
}

int mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  const char *id = "mongoose";
  static unsigned char s_initialised = 0;
  int rc;
  if (!s_initialised) {
    SSL_library_init();
    s_initialised++;
  }
  LOG(LL_DEBUG, ("%lu Setting TLS, CA: %s, cert: %s, key: %s", c->id,
                 opts->ca == NULL ? "null" : opts->ca,
                 opts->cert == NULL ? "null" : opts->cert,
                 opts->certkey == NULL ? "null" : opts->certkey));
  tls->ctx = c->is_client ? SSL_CTX_new(SSLv23_client_method())
                          : SSL_CTX_new(SSLv23_server_method());
  if ((tls->ssl = SSL_new(tls->ctx)) == NULL) {
    mg_error(c, "SSL_new");
    goto fail;
  }
  SSL_set_session_id_context(tls->ssl, (const uint8_t *) id, strlen(id));
  // Disable deprecated protocols
  SSL_set_options(tls->ssl, SSL_OP_NO_SSLv2);
  SSL_set_options(tls->ssl, SSL_OP_NO_SSLv3);
  SSL_set_options(tls->ssl, SSL_OP_NO_TLSv1);
#ifdef MG_ENABLE_OPENSSL_NO_COMPRESSION
  SSL_set_options(tls->ssl, SSL_OP_NO_COMPRESSION);
#endif
#ifdef MG_ENABLE_OPENSSL_CIPHER_SERVER_PREFERENCE
  SSL_set_options(tls->ssl, SSL_OP_CIPHER_SERVER_PREFERENCE);
#endif

  SSL_set_verify(tls->ssl, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
                 0);
  if (opts->ca != NULL && opts->ca[0] != '\0') {
    if ((rc = SSL_CTX_load_verify_locations(tls->ctx, opts->ca, NULL)) != 1) {
      mg_error(c, "parse(%s): err %d", opts->ca, mg_tls_err(tls, rc));
      goto fail;
    }
  }
  if (opts->cert != NULL && opts->cert[0] != '\0') {
    const char *key = opts->certkey;
    if (key == NULL) key = opts->cert;
    if ((rc = SSL_use_certificate_file(tls->ssl, opts->cert, 1)) != 1) {
      mg_error(c, "Invalid SSL cert, err %d", mg_tls_err(tls, rc));
      goto fail;
    } else if ((rc = SSL_use_PrivateKey_file(tls->ssl, key, 1)) != 1) {
      mg_error(c, "Invalid SSL key, err %d", mg_tls_err(tls, rc));
      goto fail;
#if OPENSSL_VERSION_NUMBER > 0x10002000L
    } else if ((rc = SSL_use_certificate_chain_file(tls->ssl, opts->cert)) !=
               1) {
      mg_error(c, "Invalid CA, err %d", mg_tls_err(tls, rc));
      goto fail;
#endif
    } else {
      SSL_set_mode(tls->ssl, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
#if OPENSSL_VERSION_NUMBER > 0x10002000L
      SSL_set_ecdh_auto(tls->ssl, 1);
#endif
    }
  }
  if (opts->ciphers != NULL) SSL_set_cipher_list(tls->ssl, opts->ciphers);
  if (opts->srvname != NULL) SSL_set_tlsext_host_name(tls->ssl, opts->srvname);
  c->tls = tls;
  c->is_tls = 1;
  c->is_tls_hs = 1;
  if (c->is_client && c->is_resolving == 0 && c->is_connecting == 0) {
    mg_tls_handshake(c);
  }
  c->is_hexdumping = 1;
  LOG(LL_DEBUG, ("%lu SSL %s OK", c->id, c->is_accepted ? "accept" : "client"));
  return 1;
fail:
  c->is_closing = 1;
  free(tls);
  return 0;
}

int mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc;
  SSL_set_fd(tls->ssl, (ssize_t) c->fd);
  rc = c->is_client ? SSL_connect(tls->ssl) : SSL_accept(tls->ssl);
  if (rc == 1) {
    LOG(LL_DEBUG, ("%lu success", c->id));
    c->is_tls_hs = 0;
    return 1;
  } else {
    int code;
    ERR_print_errors_fp(stderr);
    code = mg_tls_err(tls, rc);
    if (code != 0) mg_error(c, "tls hs: rc %d, err %d", rc, code);
    return 0;
  }
}

int mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls == NULL) return 0;
  SSL_free(tls->ssl);
  SSL_CTX_free(tls->ctx);
  free(tls);
  return 1;
}

int mg_tls_recv(struct mg_connection *c, void *buf, size_t len, int *fail) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_read(tls->ssl, buf, len);
  *fail = (n == 0) || (n < 0 && mg_tls_err(tls, n) != 0);
  return n;
}

int mg_tls_send(struct mg_connection *c, const void *buf, size_t len,
                int *fail) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_write(tls->ssl, buf, len);
  *fail = (n == 0) || (n < 0 && mg_tls_err(tls, n) != 0);
  return n;
}

#else  //////////////////////////////////////////   NO TLS

int mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts) {
  (void) opts;
  mg_error(c, "TLS is not enabled");
  return 0;
}
int mg_tls_handshake(struct mg_connection *c) {
  return c != NULL;
}
int mg_tls_free(struct mg_connection *c) {
  return c != NULL;
}
int mg_tls_recv(struct mg_connection *c, void *buf, size_t len, int *fail) {
  *fail = 1;
  return c == NULL || buf == NULL || len == 0 ? 0 : -1;
}
int mg_tls_send(struct mg_connection *c, const void *buf, size_t len,
                int *fail) {
  *fail = 1;
  return c == NULL || buf == NULL || len == 0 ? 0 : -1;
}

#endif
