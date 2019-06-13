/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_SSL && MG_SSL_IF == MG_SSL_IF_OPENSSL

#ifdef __APPLE__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <openssl/ssl.h>
#ifndef KR_VERSION
#include <openssl/tls1.h>
#endif

struct mg_ssl_if_ctx {
  SSL *ssl;
  SSL_CTX *ssl_ctx;
  struct mbuf psk;
  size_t identity_len;
};

void mg_ssl_if_init() {
  SSL_library_init();
}

enum mg_ssl_if_result mg_ssl_if_conn_accept(struct mg_connection *nc,
                                            struct mg_connection *lc) {
  struct mg_ssl_if_ctx *ctx =
      (struct mg_ssl_if_ctx *) MG_CALLOC(1, sizeof(*ctx));
  struct mg_ssl_if_ctx *lc_ctx = (struct mg_ssl_if_ctx *) lc->ssl_if_data;
  nc->ssl_if_data = ctx;
  if (ctx == NULL || lc_ctx == NULL) return MG_SSL_ERROR;
  ctx->ssl_ctx = lc_ctx->ssl_ctx;
  if ((ctx->ssl = SSL_new(ctx->ssl_ctx)) == NULL) {
    return MG_SSL_ERROR;
  }
  return MG_SSL_OK;
}

static enum mg_ssl_if_result mg_use_cert(SSL_CTX *ctx, const char *cert,
                                         const char *key, const char **err_msg);
static enum mg_ssl_if_result mg_use_ca_cert(SSL_CTX *ctx, const char *cert);
static enum mg_ssl_if_result mg_set_cipher_list(SSL_CTX *ctx, const char *cl);
static enum mg_ssl_if_result mg_ssl_if_ossl_set_psk(struct mg_ssl_if_ctx *ctx,
                                                    const char *identity,
                                                    const char *key_str);

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
  if (nc->flags & MG_F_LISTENING) {
    ctx->ssl_ctx = SSL_CTX_new(SSLv23_server_method());
  } else {
    ctx->ssl_ctx = SSL_CTX_new(SSLv23_client_method());
  }
  if (ctx->ssl_ctx == NULL) {
    MG_SET_PTRPTR(err_msg, "Failed to create SSL context");
    return MG_SSL_ERROR;
  }

#ifndef KR_VERSION
  /* Disable deprecated protocols. */
  SSL_CTX_set_options(ctx->ssl_ctx, SSL_OP_NO_SSLv2);
  SSL_CTX_set_options(ctx->ssl_ctx, SSL_OP_NO_SSLv3);
  SSL_CTX_set_options(ctx->ssl_ctx, SSL_OP_NO_TLSv1);
#ifdef MG_SSL_OPENSSL_NO_COMPRESSION
  SSL_CTX_set_options(ctx->ssl_ctx, SSL_OP_NO_COMPRESSION);
#endif
#ifdef MG_SSL_OPENSSL_CIPHER_SERVER_PREFERENCE
  SSL_CTX_set_options(ctx->ssl_ctx, SSL_OP_CIPHER_SERVER_PREFERENCE);
#endif
#else
/* Krypton only supports TLSv1.2 anyway. */
#endif

  if (params->cert != NULL &&
      mg_use_cert(ctx->ssl_ctx, params->cert, params->key, err_msg) !=
          MG_SSL_OK) {
    return MG_SSL_ERROR;
  }

  if (params->ca_cert != NULL &&
      mg_use_ca_cert(ctx->ssl_ctx, params->ca_cert) != MG_SSL_OK) {
    MG_SET_PTRPTR(err_msg, "Invalid SSL CA cert");
    return MG_SSL_ERROR;
  }

  if (mg_set_cipher_list(ctx->ssl_ctx, params->cipher_suites) != MG_SSL_OK) {
    MG_SET_PTRPTR(err_msg, "Invalid cipher suite list");
    return MG_SSL_ERROR;
  }

  mbuf_init(&ctx->psk, 0);
  if (mg_ssl_if_ossl_set_psk(ctx, params->psk_identity, params->psk_key) !=
      MG_SSL_OK) {
    MG_SET_PTRPTR(err_msg, "Invalid PSK settings");
    return MG_SSL_ERROR;
  }

  if (!(nc->flags & MG_F_LISTENING) &&
      (ctx->ssl = SSL_new(ctx->ssl_ctx)) == NULL) {
    MG_SET_PTRPTR(err_msg, "Failed to create SSL session");
    return MG_SSL_ERROR;
  }

  if (params->server_name != NULL) {
#ifdef KR_VERSION
    SSL_CTX_kr_set_verify_name(ctx->ssl_ctx, params->server_name);
#else
    SSL_set_tlsext_host_name(ctx->ssl, params->server_name);
#endif
  }

  nc->flags |= MG_F_SSL;

  return MG_SSL_OK;
}

static enum mg_ssl_if_result mg_ssl_if_ssl_err(struct mg_connection *nc,
                                               int res) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  int err = SSL_get_error(ctx->ssl, res);
  if (err == SSL_ERROR_WANT_READ) return MG_SSL_WANT_READ;
  if (err == SSL_ERROR_WANT_WRITE) return MG_SSL_WANT_WRITE;
  DBG(("%p %p SSL error: %d %d", nc, ctx->ssl_ctx, res, err));
  nc->err = err;
  return MG_SSL_ERROR;
}

enum mg_ssl_if_result mg_ssl_if_handshake(struct mg_connection *nc) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  int server_side = (nc->listener != NULL);
  int res;
  /* If descriptor is not yet set, do it now. */
  if (SSL_get_fd(ctx->ssl) < 0) {
    if (SSL_set_fd(ctx->ssl, nc->sock) != 1) return MG_SSL_ERROR;
  }
  res = server_side ? SSL_accept(ctx->ssl) : SSL_connect(ctx->ssl);
  if (res != 1) return mg_ssl_if_ssl_err(nc, res);
  return MG_SSL_OK;
}

int mg_ssl_if_read(struct mg_connection *nc, void *buf, size_t buf_size) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  int n = SSL_read(ctx->ssl, buf, buf_size);
  DBG(("%p %d -> %d", nc, (int) buf_size, n));
  if (n < 0) return mg_ssl_if_ssl_err(nc, n);
  if (n == 0) nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  return n;
}

int mg_ssl_if_write(struct mg_connection *nc, const void *data, size_t len) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  int n = SSL_write(ctx->ssl, data, len);
  DBG(("%p %d -> %d", nc, (int) len, n));
  if (n <= 0) return mg_ssl_if_ssl_err(nc, n);
  return n;
}

void mg_ssl_if_conn_close_notify(struct mg_connection *nc) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  if (ctx == NULL) return;
  SSL_shutdown(ctx->ssl);
}

void mg_ssl_if_conn_free(struct mg_connection *nc) {
  struct mg_ssl_if_ctx *ctx = (struct mg_ssl_if_ctx *) nc->ssl_if_data;
  if (ctx == NULL) return;
  nc->ssl_if_data = NULL;
  if (ctx->ssl != NULL) SSL_free(ctx->ssl);
  if (ctx->ssl_ctx != NULL && nc->listener == NULL) SSL_CTX_free(ctx->ssl_ctx);
  mbuf_free(&ctx->psk);
  memset(ctx, 0, sizeof(*ctx));
  MG_FREE(ctx);
}

/*
 * Cipher suite options used for TLS negotiation.
 * https://wiki.mozilla.org/Security/Server_Side_TLS#Recommended_configurations
 */
static const char mg_s_cipher_list[] =
#if defined(MG_SSL_CRYPTO_MODERN)
    "ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:"
    "ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:"
    "DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:"
    "ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:"
    "ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:"
    "ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:"
    "DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:"
    "DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:"
    "!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK"
#elif defined(MG_SSL_CRYPTO_OLD)
    "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:"
    "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:"
    "DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:"
    "ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:"
    "ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:"
    "ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:"
    "DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:"
    "DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:ECDHE-RSA-DES-CBC3-SHA:"
    "ECDHE-ECDSA-DES-CBC3-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:"
    "AES128-SHA256:AES256-SHA256:AES128-SHA:AES256-SHA:AES:DES-CBC3-SHA:"
    "HIGH:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!aECDH:"
    "!EDH-DSS-DES-CBC3-SHA:!EDH-RSA-DES-CBC3-SHA:!KRB5-DES-CBC3-SHA"
#else /* Default - intermediate. */
    "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:"
    "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:"
    "DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:"
    "ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:"
    "ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:"
    "ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:"
    "DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:"
    "DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:"
    "AES128-SHA256:AES256-SHA256:AES128-SHA:AES256-SHA:AES:CAMELLIA:"
    "DES-CBC3-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!aECDH:"
    "!EDH-DSS-DES-CBC3-SHA:!EDH-RSA-DES-CBC3-SHA:!KRB5-DES-CBC3-SHA"
#endif
    ;

/*
 * Default DH params for PFS cipher negotiation. This is a 2048-bit group.
 * Will be used if none are provided by the user in the certificate file.
 */
#if !MG_DISABLE_PFS && !defined(KR_VERSION)
static const char mg_s_default_dh_params[] =
    "\
-----BEGIN DH PARAMETERS-----\n\
MIIBCAKCAQEAlvbgD/qh9znWIlGFcV0zdltD7rq8FeShIqIhkQ0C7hYFThrBvF2E\n\
Z9bmgaP+sfQwGpVlv9mtaWjvERbu6mEG7JTkgmVUJrUt/wiRzwTaCXBqZkdUO8Tq\n\
+E6VOEQAilstG90ikN1Tfo+K6+X68XkRUIlgawBTKuvKVwBhuvlqTGerOtnXWnrt\n\
ym//hd3cd5PBYGBix0i7oR4xdghvfR2WLVu0LgdThTBb6XP7gLd19cQ1JuBtAajZ\n\
wMuPn7qlUkEFDIkAZy59/Hue/H2Q2vU/JsvVhHWCQBL4F1ofEAt50il6ZxR1QfFK\n\
9VGKDC4oOgm9DlxwwBoC2FjqmvQlqVV3kwIBAg==\n\
-----END DH PARAMETERS-----\n";
#endif

static enum mg_ssl_if_result mg_use_ca_cert(SSL_CTX *ctx, const char *cert) {
  if (cert == NULL || strcmp(cert, "*") == 0) {
    return MG_SSL_OK;
  }
  SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, 0);
  return SSL_CTX_load_verify_locations(ctx, cert, NULL) == 1 ? MG_SSL_OK
                                                             : MG_SSL_ERROR;
}

static enum mg_ssl_if_result mg_use_cert(SSL_CTX *ctx, const char *cert,
                                         const char *key,
                                         const char **err_msg) {
  if (key == NULL) key = cert;
  if (cert == NULL || cert[0] == '\0' || key == NULL || key[0] == '\0') {
    return MG_SSL_OK;
  } else if (SSL_CTX_use_certificate_file(ctx, cert, 1) == 0) {
    MG_SET_PTRPTR(err_msg, "Invalid SSL cert");
    return MG_SSL_ERROR;
  } else if (SSL_CTX_use_PrivateKey_file(ctx, key, 1) == 0) {
    MG_SET_PTRPTR(err_msg, "Invalid SSL key");
    return MG_SSL_ERROR;
  } else if (SSL_CTX_use_certificate_chain_file(ctx, cert) == 0) {
    MG_SET_PTRPTR(err_msg, "Invalid CA bundle");
    return MG_SSL_ERROR;
  } else {
    SSL_CTX_set_mode(ctx, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
#if !MG_DISABLE_PFS && !defined(KR_VERSION)
    BIO *bio = NULL;
    DH *dh = NULL;

    /* Try to read DH parameters from the cert/key file. */
    bio = BIO_new_file(cert, "r");
    if (bio != NULL) {
      dh = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
      BIO_free(bio);
    }
    /*
     * If there are no DH params in the file, fall back to hard-coded ones.
     * Not ideal, but better than nothing.
     */
    if (dh == NULL) {
      bio = BIO_new_mem_buf((void *) mg_s_default_dh_params, -1);
      dh = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
      BIO_free(bio);
    }
    if (dh != NULL) {
      SSL_CTX_set_tmp_dh(ctx, dh);
      SSL_CTX_set_options(ctx, SSL_OP_SINGLE_DH_USE);
      DH_free(dh);
    }
#if OPENSSL_VERSION_NUMBER > 0x10002000L
    SSL_CTX_set_ecdh_auto(ctx, 1);
#endif
#endif
  }
  return MG_SSL_OK;
}

static enum mg_ssl_if_result mg_set_cipher_list(SSL_CTX *ctx, const char *cl) {
  return (SSL_CTX_set_cipher_list(ctx, cl ? cl : mg_s_cipher_list) == 1
              ? MG_SSL_OK
              : MG_SSL_ERROR);
}

#if !defined(KR_VERSION) && !defined(LIBRESSL_VERSION_NUMBER)
static unsigned int mg_ssl_if_ossl_psk_cb(SSL *ssl, const char *hint,
                                          char *identity,
                                          unsigned int max_identity_len,
                                          unsigned char *psk,
                                          unsigned int max_psk_len) {
  struct mg_ssl_if_ctx *ctx =
      (struct mg_ssl_if_ctx *) SSL_CTX_get_app_data(SSL_get_SSL_CTX(ssl));
  size_t key_len = ctx->psk.len - ctx->identity_len - 1;
  DBG(("hint: '%s'", (hint ? hint : "")));
  if (ctx->identity_len + 1 > max_identity_len) {
    DBG(("identity too long"));
    return 0;
  }
  if (key_len > max_psk_len) {
    DBG(("key too long"));
    return 0;
  }
  memcpy(identity, ctx->psk.buf, ctx->identity_len + 1);
  memcpy(psk, ctx->psk.buf + ctx->identity_len + 1, key_len);
  (void) ssl;
  return key_len;
}

static enum mg_ssl_if_result mg_ssl_if_ossl_set_psk(struct mg_ssl_if_ctx *ctx,
                                                    const char *identity,
                                                    const char *key_str) {
  unsigned char key[32];
  size_t key_len;
  size_t i = 0;
  if (identity == NULL && key_str == NULL) return MG_SSL_OK;
  if (identity == NULL || key_str == NULL) return MG_SSL_ERROR;
  key_len = strlen(key_str);
  if (key_len != 32 && key_len != 64) return MG_SSL_ERROR;
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
  ctx->identity_len = strlen(identity);
  mbuf_append(&ctx->psk, identity, ctx->identity_len + 1);
  mbuf_append(&ctx->psk, key, key_len);
  SSL_CTX_set_psk_client_callback(ctx->ssl_ctx, mg_ssl_if_ossl_psk_cb);
  SSL_CTX_set_app_data(ctx->ssl_ctx, ctx);
  return MG_SSL_OK;
}
#else
static enum mg_ssl_if_result mg_ssl_if_ossl_set_psk(struct mg_ssl_if_ctx *ctx,
                                                    const char *identity,
                                                    const char *key_str) {
  (void) ctx;
  (void) identity;
  (void) key_str;
  /* Krypton / LibreSSL does not support PSK. */
  return MG_SSL_ERROR;
}
#endif /* !defined(KR_VERSION) && !defined(LIBRESSL_VERSION_NUMBER) */

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

#endif /* MG_ENABLE_SSL && MG_SSL_IF == MG_SSL_IF_OPENSSL */
