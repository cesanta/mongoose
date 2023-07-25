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

static STACK_OF(X509_INFO) * load_ca_certs(const char *ca, int ca_len) {
  BIO *ca_bio = BIO_new_mem_buf(ca, ca_len);
  if (!ca_bio) return NULL;
  STACK_OF(X509_INFO) *certs = PEM_X509_INFO_read_bio(ca_bio, NULL, NULL, NULL);
  BIO_free(ca_bio);
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

static EVP_PKEY *load_key(const char *key, int key_len) {
  BIO *key_bio = BIO_new_mem_buf(key, key_len);
  if (!key_bio) return NULL;
  EVP_PKEY *priv_key = PEM_read_bio_PrivateKey(key_bio, NULL, 0, NULL);
  BIO_free(key_bio);
  return priv_key;
}

static X509 *load_cert(const char *cert, int cert_len) {
  BIO *cert_bio = BIO_new_mem_buf(cert, cert_len);
  if (!cert_bio) return NULL;
  X509 *x509 = PEM_read_bio_X509(cert_bio, NULL, 0, NULL);
  BIO_free(cert_bio);
  return x509;
}

void mg_tls_init(struct mg_connection *c, struct mg_str hostname) {
  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) c->mgr->tls_ctx;
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));

  if (ctx == NULL) {
    mg_error(c, "TLS context not initialized");
    goto fail;
  }

  if (tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }

  tls->ctx = c->is_client ? SSL_CTX_new(TLS_client_method())
                          : SSL_CTX_new(TLS_server_method());
  if ((tls->ssl = SSL_new(tls->ctx)) == NULL) {
    mg_error(c, "SSL_new");
    goto fail;
  }

  SSL_set_min_proto_version(tls->ssl, TLS1_2_VERSION);

#ifdef MG_ENABLE_OPENSSL_NO_COMPRESSION
  SSL_set_options(tls->ssl, SSL_OP_NO_COMPRESSION);
#endif
#ifdef MG_ENABLE_OPENSSL_CIPHER_SERVER_PREFERENCE
  SSL_set_options(tls->ssl, SSL_OP_CIPHER_SERVER_PREFERENCE);
#endif

  if (c->is_client) {
    if (ctx->client_ca) {
      SSL_set_verify(tls->ssl,
                     SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
      if (!add_ca_certs(tls->ctx, ctx->client_ca)) goto fail;
    }
    if (ctx->client_cert && ctx->client_key) {
      if (SSL_use_certificate(tls->ssl, ctx->client_cert) != 1) {
        mg_error(c, "SSL_CTX_use_certificate");
        goto fail;
      }
      if (SSL_use_PrivateKey(tls->ssl, ctx->client_key) != 1) {
        mg_error(c, "SSL_CTX_use_PrivateKey");
        goto fail;
      }
    }
  } else {
    if (ctx->server_ca) {
      SSL_set_verify(tls->ssl,
                     SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
      if (!add_ca_certs(tls->ctx, ctx->server_ca)) goto fail;
    }
    if (ctx->server_cert && ctx->server_key) {
      if (SSL_use_certificate(tls->ssl, ctx->server_cert) != 1) {
        mg_error(c, "SSL_CTX_use_certificate");
        goto fail;
      }
      if (SSL_use_PrivateKey(tls->ssl, ctx->server_key) != 1) {
        mg_error(c, "SSL_CTX_use_PrivateKey");
        goto fail;
      }
    }
  }

  SSL_set_mode(tls->ssl, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
#if OPENSSL_VERSION_NUMBER > 0x10002000L
  SSL_set_ecdh_auto(tls->ssl, 1);
#endif

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
  if (c->is_client && hostname.ptr && hostname.ptr[0] != '\0') {
    char *s = mg_mprintf("%.*s", (int) hostname.len, hostname.ptr);
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
  c->is_closing = 1;
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

void mg_tls_ctx_free(struct mg_mgr *mgr) {
  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) mgr->tls_ctx;
  if (ctx) {
    if (ctx->server_cert) X509_free(ctx->server_cert);
    if (ctx->server_key) EVP_PKEY_free(ctx->server_key);
    if (ctx->server_ca)
      sk_X509_INFO_pop_free(ctx->server_ca, X509_INFO_free);
    if (ctx->client_cert) X509_free(ctx->client_cert);
    if (ctx->client_key) EVP_PKEY_free(ctx->client_key);
    if (ctx->client_ca)
      sk_X509_INFO_pop_free(ctx->client_ca, X509_INFO_free);
    free(ctx);
    mgr->tls_ctx = NULL;
  }
}

void mg_tls_ctx_init(struct mg_mgr *mgr, const struct mg_tls_opts *opts) {
  static unsigned char s_initialised = 0;
  if (!s_initialised) {
    SSL_library_init();
    s_initialised++;
  }

  struct mg_tls_ctx *ctx = (struct mg_tls_ctx *) calloc(1, sizeof(*ctx));
  if (ctx == NULL) return;

  if (opts->server_cert.ptr && opts->server_cert.ptr[0] != '\0') {
    struct mg_str key = opts->server_key;
    if (!key.ptr) key = opts->server_cert;
    if (!(ctx->server_cert =
              load_cert(opts->server_cert.ptr, (int) opts->server_cert.len)))
      goto fail;
    if (!(ctx->server_key = load_key(key.ptr, (int) key.len))) goto fail;
  }

  if (opts->server_ca.ptr && opts->server_ca.ptr[0] != '\0') {
    if (!(ctx->server_ca =
              load_ca_certs(opts->server_ca.ptr, (int) opts->server_ca.len)))
      goto fail;
  }

  if (opts->client_cert.ptr && opts->client_cert.ptr[0] != '\0') {
    struct mg_str key = opts->client_key;
    if (!key.ptr) key = opts->client_cert;
    if (!(ctx->client_cert =
              load_cert(opts->client_cert.ptr, (int) opts->client_cert.len)))
      goto fail;
    if (!(ctx->client_key = load_key(key.ptr, (int) key.len))) goto fail;
  }

  if (opts->client_ca.ptr && opts->client_ca.ptr[0] != '\0') {
    if (!(ctx->client_ca =
              load_ca_certs(opts->client_ca.ptr, (int) opts->client_ca.len)))
      goto fail;
  }

  mgr->tls_ctx = ctx;
  return;
fail:
  MG_ERROR(("TLS ctx init error"));
  mg_tls_ctx_free(mgr);
}

#endif
