#include "printf.h"
#include "tls.h"

#if MG_TLS == MG_TLS_OPENSSL
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

  if (err != 0) ERR_print_errors_fp(stderr);
  ERR_clear_error();
  if (err == SSL_ERROR_WANT_READ) return 0;
  if (err == SSL_ERROR_WANT_WRITE) return 0;
  return err;
}

static STACK_OF(X509_INFO) * load_ca_certs(struct mg_str ca) {
  BIO *bio = BIO_new_mem_buf(ca.ptr, (int) ca.len);
  STACK_OF(X509_INFO) *certs =
      bio ? PEM_X509_INFO_read_bio(bio, NULL, NULL, NULL) : NULL;
  if (bio) BIO_free(bio);
  return certs;
}

static bool add_ca_certs(SSL_CTX *ctx, STACK_OF(X509_INFO) * certs) {
  X509_STORE *cert_store = SSL_CTX_get_cert_store(ctx);
  for (int i = 0; i < sk_X509_INFO_num(certs); i++) {
    X509_INFO *cert_info = sk_X509_INFO_value(certs, i);
    if (cert_info->x509 && !X509_STORE_add_cert(cert_store, cert_info->x509))
      return false;
  }
  return true;
}

static EVP_PKEY *load_key(struct mg_str s) {
  BIO *bio = BIO_new_mem_buf(s.ptr, (int) (long) s.len);
  EVP_PKEY *key = bio ? PEM_read_bio_PrivateKey(bio, NULL, 0, NULL) : NULL;
  if (bio) BIO_free(bio);
  return key;
}

static X509 *load_cert(struct mg_str s) {
  BIO *bio = BIO_new_mem_buf(s.ptr, (int) (long) s.len);
  X509 *cert = bio == NULL ? NULL
               : s.ptr[0] == '-'
                   ? PEM_read_bio_X509(bio, NULL, NULL, NULL)  // PEM
                   : d2i_X509_bio(bio, NULL);                  // DER
  if (bio) BIO_free(bio);
  return cert;
}

void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  const char *id = "mongoose";
  static unsigned char s_initialised = 0;
  int rc;

  if (tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }

  if (!s_initialised) {
    SSL_library_init();
    s_initialised++;
  }
  MG_DEBUG(("%lu Setting TLS", c->id));
  tls->ctx = c->is_client ? SSL_CTX_new(SSLv23_client_method())
                          : SSL_CTX_new(SSLv23_server_method());
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

  if (opts->ca.ptr != NULL && opts->ca.ptr[0] != '\0') {
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
  if (opts->cert.ptr != NULL && opts->cert.ptr[0] != '\0') {
    X509 *cert = load_cert(opts->cert);
    rc = cert == NULL ? 0 : SSL_use_certificate(tls->ssl, cert);
    X509_free(cert);
    if (cert == NULL || rc != 1) {
      mg_error(c, "CERT err %d", mg_tls_err(tls, rc));
      goto fail;
    }
  }
  if (opts->key.ptr != NULL && opts->key.ptr[0] != '\0') {
    EVP_PKEY *key = load_key(opts->key);
    rc = key == NULL ? 0 : SSL_use_PrivateKey(tls->ssl, key);
    EVP_PKEY_free(key);
    if (key == NULL || rc != 1) {
      mg_error(c, "KEY err %d", mg_tls_err(tls, rc));
      goto fail;
    }
  }

  SSL_set_mode(tls->ssl, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
#if OPENSSL_VERSION_NUMBER > 0x10002000L
  SSL_set_ecdh_auto(tls->ssl, 1);
#endif
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
  if (opts->name.len > 0) {
    char *s = mg_mprintf("%.*s", (int) opts->name.len, opts->name.ptr);
    SSL_set1_host(tls->ssl, s);
    SSL_set_tlsext_host_name(tls->ssl, s);
    free(s);
  }
#endif
  c->tls = tls;
  c->is_tls = 1;
  c->is_tls_hs = 1;
  if (c->is_client && c->is_resolving == 0 && c->is_connecting == 0) {
    mg_tls_handshake(c);
  }
  MG_DEBUG(("%lu SSL %s OK", c->id, c->is_accepted ? "accept" : "client"));
  return;
fail:
  free(tls);
}

void mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc;
  SSL_set_fd(tls->ssl, (int) (size_t) c->fd);
  rc = c->is_client ? SSL_connect(tls->ssl) : SSL_accept(tls->ssl);
  if (rc == 1) {
    MG_DEBUG(("%lu success", c->id));
    c->is_tls_hs = 0;
    mg_call(c, MG_EV_TLS_HS, NULL);
  } else {
    int code = mg_tls_err(tls, rc);
    if (code != 0) mg_error(c, "tls hs: rc %d, err %d", rc, code);
  }
}

void mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls == NULL) return;
  SSL_free(tls->ssl);
  SSL_CTX_free(tls->ctx);
  free(tls);
  c->tls = NULL;
}

size_t mg_tls_pending(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  return tls == NULL ? 0 : (size_t) SSL_pending(tls->ssl);
}

long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_read(tls->ssl, buf, (int) len);
  if (n < 0 && mg_tls_err(tls, n) == 0) return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_write(tls->ssl, buf, (int) len);
  if (n < 0 && mg_tls_err(tls, n) == 0) return MG_IO_WAIT;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

void mg_tls_ctx_init(struct mg_mgr *mgr) {
  (void) mgr;
}

void mg_tls_ctx_free(struct mg_mgr *mgr) {
  (void) mgr;
}
#endif
