#include "printf.h"
#include "tls.h"
#include "util.h"

#if MG_TLS == MG_TLS_OPENSSL || MG_TLS == MG_TLS_WOLFSSL

static int tls_err_cb(const char *s, size_t len, void *c) {
  int n = (int) len - 1;
  MG_ERROR(("%lu %.*s", ((struct mg_connection *) c)->id, n, s));
  return 0;  // undocumented
}

static int mg_tls_err(struct mg_connection *c, struct mg_tls *tls, int res) {
  int err = SSL_get_error(tls->ssl, res);
  // We've just fetched the last error from the queue.
  // Now we need to clear the error queue. If we do not, then the following
  // can happen (actually reported):
  //  - A new connection is accept()-ed with cert error (e.g. self-signed cert)
  //  - Since all accept()-ed connections share listener's context,
  //  - *ALL* SSL accepted connection report read error on the next poll cycle.
  //    Thus a single errored connection can close all the rest, unrelated ones.
  // Clearing the error keeps the shared SSL_CTX in an OK state.

  if (err != 0) ERR_print_errors_cb(tls_err_cb, c);
  ERR_clear_error();
  if (err == SSL_ERROR_WANT_READ) return 0;
  if (err == SSL_ERROR_WANT_WRITE) return 0;
  return err;
}

static STACK_OF(X509_INFO) * load_ca_certs(struct mg_str ca) {
  BIO *bio = BIO_new_mem_buf(ca.buf, (int) ca.len);
  STACK_OF(X509_INFO) *certs =
      bio ? PEM_X509_INFO_read_bio(bio, NULL, NULL, NULL) : NULL;
  if (bio) BIO_free(bio);
  return certs;
}

static bool add_ca_certs(SSL_CTX *ctx, STACK_OF(X509_INFO) * certs) {
  int i;
  X509_STORE *cert_store = SSL_CTX_get_cert_store(ctx);
  if (cert_store == NULL) return false;
  for (i = 0; i < sk_X509_INFO_num(certs); i++) {
    X509_INFO *cert_info = sk_X509_INFO_value(certs, i);
    if (cert_info->x509 && !X509_STORE_add_cert(cert_store, cert_info->x509))
      return false;
  }
  return true;
}

static EVP_PKEY *load_key(struct mg_str s) {
  BIO *bio = BIO_new_mem_buf(s.buf, (int) (long) s.len);
  EVP_PKEY *key = bio ? PEM_read_bio_PrivateKey(bio, NULL, 0, NULL) : NULL;
  if (bio) BIO_free(bio);
  return key;
}

static X509 *load_cert(struct mg_str s) {
  BIO *bio = BIO_new_mem_buf(s.buf, (int) (long) s.len);
  X509 *cert = bio == NULL ? NULL
               : s.buf[0] == '-'
                   ? PEM_read_bio_X509(bio, NULL, NULL, NULL)  // PEM
                   : d2i_X509_bio(bio, NULL);                  // DER
  if (bio) BIO_free(bio);
  return cert;
}

static long mg_bio_ctrl(BIO *b, int cmd, long larg, void *pargs) {
  long ret = 0;
  if (cmd == BIO_CTRL_PUSH) ret = 1;
  if (cmd == BIO_CTRL_POP) ret = 1;
  if (cmd == BIO_CTRL_FLUSH) ret = 1;
#if MG_TLS == MG_TLS_OPENSSL
  if (cmd == BIO_C_SET_NBIO) ret = 1;
#endif
  // MG_DEBUG(("%d -> %ld", cmd, ret));
  (void) b, (void) cmd, (void) larg, (void) pargs;
  return ret;
}

static int mg_bio_read(BIO *bio, char *buf, int len) {
  struct mg_connection *c = (struct mg_connection *) BIO_get_data(bio);
  long res = mg_io_recv(c, buf, (size_t) len);
  // MG_DEBUG(("%p %d %ld", buf, len, res));
  len = res > 0 ? (int) res : -1;
  if (res == MG_IO_WAIT) BIO_set_retry_read(bio);
  return len;
}

static int mg_bio_write(BIO *bio, const char *buf, int len) {
  struct mg_connection *c = (struct mg_connection *) BIO_get_data(bio);
  long res = mg_io_send(c, buf, (size_t) len);
  // MG_DEBUG(("%p %d %ld", buf, len, res));
  len = res > 0 ? (int) res : -1;
  if (res == MG_IO_WAIT) BIO_set_retry_write(bio);
  return len;
}

#ifdef MG_TLS_SSLKEYLOGFILE
static void ssl_keylog_cb(const SSL *ssl, const char *line) {
  char *keylogfile = getenv("SSLKEYLOGFILE");
  if (keylogfile == NULL) {
    return;
  }
  FILE *f = fopen(keylogfile, "a");
  fprintf(f, "%s\n", line);
  fflush(f);
  fclose(f);
  (void) ssl;
}
#endif

void mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls == NULL) return;
  SSL_free(tls->ssl);
  SSL_CTX_free(tls->ctx);
  BIO_meth_free(tls->bm);
  mg_free(tls);
  c->tls = NULL;
}

void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) mg_calloc(1, sizeof(*tls));
  const char *id = "mongoose";
  static unsigned char s_initialised = 0;
  BIO *bio = NULL;
  int rc;
  c->tls = tls;
  if (tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }

  if (!s_initialised) {
    SSL_library_init();
    s_initialised++;
  }
  MG_DEBUG(("%lu Setting TLS", c->id));
  tls->ctx = c->is_client ? SSL_CTX_new(TLS_client_method())
                          : SSL_CTX_new(TLS_server_method());
  if (tls->ctx == NULL) {
    mg_error(c, "SSL_CTX_new");
    goto fail;
  }
#ifdef MG_TLS_SSLKEYLOGFILE
  SSL_CTX_set_keylog_callback(tls->ctx, ssl_keylog_cb);
#endif
  if ((tls->ssl = SSL_new(tls->ctx)) == NULL) {
    mg_error(c, "SSL_new");
    goto fail;
  }
  SSL_set_session_id_context(tls->ssl, (const uint8_t *) id,
                             (unsigned) strlen(id));
  // Disable deprecated protocols
  SSL_set_options(tls->ssl, SSL_OP_NO_SSLv2);
  SSL_set_options(tls->ssl, SSL_OP_NO_SSLv3);
  SSL_set_options(tls->ssl, SSL_OP_NO_TLSv1);
  SSL_set_options(tls->ssl, SSL_OP_NO_TLSv1_1);
#ifdef MG_ENABLE_OPENSSL_NO_COMPRESSION
  SSL_set_options(tls->ssl, SSL_OP_NO_COMPRESSION);
#endif
#ifdef MG_ENABLE_OPENSSL_CIPHER_SERVER_PREFERENCE
  SSL_set_options(tls->ssl, SSL_OP_CIPHER_SERVER_PREFERENCE);
#endif

#if MG_TLS == MG_TLS_WOLFSSL && !defined(OPENSSL_COMPATIBLE_DEFAULTS)
  if (opts->ca.len == 0 || mg_strcmp(opts->ca, mg_str("*")) == 0) {
    // Older versions require that either the CA is loaded or SSL_VERIFY_NONE
    // explicitly set
    SSL_set_verify(tls->ssl, SSL_VERIFY_NONE, NULL);
  }
#endif
  if (opts->ca.buf != NULL && opts->ca.buf[0] != '\0') {
    SSL_set_verify(tls->ssl, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
                   NULL);
    STACK_OF(X509_INFO) *certs = load_ca_certs(opts->ca);
    rc = add_ca_certs(tls->ctx, certs);
    sk_X509_INFO_pop_free(certs, X509_INFO_free);
    if (!rc) {
      mg_error(c, "CA err");
      goto fail;
    }
  }
  if (opts->cert.buf != NULL && opts->cert.buf[0] != '\0') {
    X509 *cert = load_cert(opts->cert);
    rc = cert == NULL ? 0 : SSL_use_certificate(tls->ssl, cert);
    X509_free(cert);
    if (cert == NULL || rc != 1) {
      mg_error(c, "CERT err %d", mg_tls_err(c, tls, rc));
      goto fail;
    }
  }
  if (opts->key.buf != NULL && opts->key.buf[0] != '\0') {
    EVP_PKEY *key = load_key(opts->key);
    rc = key == NULL ? 0 : SSL_use_PrivateKey(tls->ssl, key);
    EVP_PKEY_free(key);
    if (key == NULL || rc != 1) {
      mg_error(c, "KEY err %d", mg_tls_err(c, tls, rc));
      goto fail;
    }
  }

  SSL_set_mode(tls->ssl, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
#if MG_TLS == MG_TLS_OPENSSL && OPENSSL_VERSION_NUMBER > 0x10002000L
  (void) SSL_set_ecdh_auto(tls->ssl, 1);
#endif
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
  if (opts->name.len > 0) {
    char *s = mg_mprintf("%.*s", (int) opts->name.len, opts->name.buf);
#if MG_TLS != MG_TLS_WOLFSSL || LIBWOLFSSL_VERSION_HEX >= 0x05005002
    SSL_set1_host(tls->ssl, s);
#else
    X509_VERIFY_PARAM_set1_host(SSL_get0_param(tls->ssl), s, 0);
#endif
    SSL_set_tlsext_host_name(tls->ssl, s);
    mg_free(s);
  }
#endif
#if MG_TLS == MG_TLS_WOLFSSL
  tls->bm = BIO_meth_new(0, "bio_mg");
#else
  tls->bm = BIO_meth_new(BIO_get_new_index() | BIO_TYPE_SOURCE_SINK, "bio_mg");
#endif
  BIO_meth_set_write(tls->bm, mg_bio_write);
  BIO_meth_set_read(tls->bm, mg_bio_read);
  BIO_meth_set_ctrl(tls->bm, mg_bio_ctrl);

  bio = BIO_new(tls->bm);
  BIO_set_data(bio, c);
  SSL_set_bio(tls->ssl, bio, bio);

  c->is_tls = 1;
  c->is_tls_hs = 1;
  MG_DEBUG(("%lu SSL %s OK", c->id, c->is_accepted ? "accept" : "client"));
  return;
fail:
  mg_tls_free(c);
}

void mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc = c->is_client ? SSL_connect(tls->ssl) : SSL_accept(tls->ssl);
  if (rc == 1) {
    MG_DEBUG(("%lu success", c->id));
    c->is_tls_hs = 0;
    mg_call(c, MG_EV_TLS_HS, NULL);
  } else {
    int code = mg_tls_err(c, tls, rc);
    if (code != 0) mg_error(c, "tls hs: rc %d, err %d", rc, code);
  }
}

size_t mg_tls_pending(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  return tls == NULL ? 0 : (size_t) SSL_pending(tls->ssl);
}

long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_read(tls->ssl, buf, (int) len);
  if (!c->is_tls_hs && buf == NULL && n == 0) return 0; // TODO(): MIP
  if (n < 0 && mg_tls_err(c, tls, n) == 0) return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_write(tls->ssl, buf, (int) len);
  if (n < 0 && mg_tls_err(c, tls, n) == 0) return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

void mg_tls_flush(struct mg_connection *c) {
  (void) c;
}

void mg_tls_ctx_init(struct mg_mgr *mgr) {
  (void) mgr;
}

void mg_tls_ctx_free(struct mg_mgr *mgr) {
  (void) mgr;
}
#endif
