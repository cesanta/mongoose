//
// Link with WolfSSL compiled with './configure --enable-debug --enable-static --with-ecc --enable-ecc --enable-psk --enable-sni --enable-harden --enable-curve25519 --enable-curve448 --enable-ed448 --enable-ed25519 --enable-ecccustcurves --enable-tlsx --enable-dsa CFLAGS="-DFP_MAX_BITS=8192 -DWOLFSSL_ALT_CERT_CHAINS"'.
// 
#include "tls.h"

#if MG_ENABLE_WOLFSSL
static int mg_tls_err(struct mg_tls *tls, int res) {
  int err = wolfSSL_get_error(tls->ssl, res);

  //wolfSSL_ERR_clear_error();
  if (err == SSL_ERROR_WANT_READ) return 0;
  if (err == SSL_ERROR_WANT_WRITE) return 0;
  return err;
}

void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  static unsigned char s_initialised = 0;
  int rc;

  if (tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }

  if (!s_initialised) {
#ifdef MG_ENABLE_WOLFSSL_DEBUG
    wolfSSL_Debugging_ON();
#endif
    wolfSSL_Init();
    s_initialised++;
  }
  MG_DEBUG(("%lu Setting TLS, CA: %s, cert: %s, key: %s", c->id,
            opts->ca == NULL ? "null" : opts->ca,
            opts->cert == NULL ? "null" : opts->cert,
            opts->certkey == NULL ? "null" : opts->certkey));
  tls->ctx = c->is_client ? wolfSSL_CTX_new(wolfTLSv1_2_client_method())
                          : wolfSSL_CTX_new(wolfTLSv1_2_server_method());
  if (tls->ctx == NULL) {
    mg_error(c, "Error in wolfSSL_CTX_new");
    goto fail;
  }

  // Disable the verification of the certificates, for debugging purpose only
  //wolfSSL_CTX_set_verify(tls->ctx, WOLFSSL_VERIFY_NONE, 0);

  if (opts->ca != NULL && opts->ca[0] != '\0') {
    wolfSSL_CTX_set_verify(tls->ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
                   NULL);
    if ((rc = wolfSSL_CTX_load_verify_locations(tls->ctx, opts->ca, NULL)) != 1) {
      mg_error(c, "load('%s') %d err %d", opts->ca, rc, mg_tls_err(tls, rc));
      goto fail;
    }
  }else{
      wolfSSL_CTX_set_verify(tls->ctx, WOLFSSL_VERIFY_NONE, 0);
  }


  if (opts->cert != NULL && opts->cert[0] != '\0') {
    const char *key = opts->certkey;
    if (key == NULL) key = opts->cert;
    if ((rc = wolfSSL_CTX_use_certificate_file(tls->ctx, opts->cert, 1)) != 1) {
      mg_error(c, "Invalid SSL cert, err %d", mg_tls_err(tls, rc));
      goto fail;
    } else if ((rc = wolfSSL_CTX_use_PrivateKey_file(tls->ctx, key, 1)) != 1) {
      mg_error(c, "Invalid SSL key, err %d", mg_tls_err(tls, rc));
      goto fail;
    } else if ((rc = wolfSSL_CTX_use_certificate_chain_file(tls->ctx, opts->cert)) !=
               1) {
      mg_error(c, "Invalid chain, err %d", mg_tls_err(tls, rc));
      goto fail;
    } else {
      //wolfSSL_CTX_set_mode(tls->ctx, WOLFSSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
    }
  }

  tls->ssl = wolfSSL_new(tls->ctx);
  if (tls->ssl == NULL) {
    mg_error(c, "Error in wolfSSL_new");
    goto fail;
  }

#ifdef HAVE_SNI
  if (opts && opts->srvname.len > 0) {
    rc = wolfSSL_UseSNI(tls->ssl, WOLFSSL_SNI_HOST_NAME, opts->srvname.ptr, (word16) opts->srvname.len);
    if (rc != SSL_SUCCESS) {
      mg_error(c, "Error in wolfSSL_CTX_UseSNI");
      goto fail;
    }
    rc = wolfSSL_check_domain_name(tls->ssl, opts->srvname.ptr);
    if (rc != SSL_SUCCESS) {
      mg_error(c, "Error in wolfSSL_check_domain_name");
      goto fail;
    }
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
  mg_error(c, "Failure in mg_tls_init");
  c->is_closing = 1;
  free(tls);
}

void mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc, err;

  rc = wolfSSL_set_fd(tls->ssl, (int) (size_t) c->fd);
  if (rc != SSL_SUCCESS) {
    mg_error(c, "Error in wolfSSL_set_fd");
  }

  if (c->is_client) {
    rc = wolfSSL_connect(tls->ssl);
    err = wolfSSL_get_error(tls->ssl, rc);

    if (rc == SSL_SUCCESS) {
      MG_DEBUG(("%lu success", c->id));
      c->is_tls_hs = 0;
    } else if (err == WOLFSSL_ERROR_WANT_READ || err == WOLFSSL_ERROR_WANT_WRITE) {
      MG_DEBUG(("%lu pending", c->id));
    } else {
      int code = mg_tls_err(tls, rc);
      if (code != 0) mg_error(c, "tls hs: rc %d, err %d", rc, code);
    }
  } else {
    rc = wolfSSL_accept(tls->ssl);
    err = wolfSSL_get_error(tls->ssl, rc);

    if (rc == SSL_SUCCESS) {
      MG_DEBUG(("%lu success", c->id));
      c->is_tls_hs = 0;
    } else if (err == WOLFSSL_ERROR_WANT_READ || err == WOLFSSL_ERROR_WANT_WRITE) {
      MG_DEBUG(("%lu pending", c->id));
    } else {
      int code = mg_tls_err(tls, rc);
    if (code != 0) mg_error(c, "tls hs: rc %d, err %d", rc, code);
    }
  }
}

void mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int ret, err;
  if (tls == NULL) return;

  do {
    ret = wolfSSL_shutdown(tls->ssl);
    err = wolfSSL_get_error(tls->ssl, ret);
  } while (ret != SSL_SUCCESS && (err == WOLFSSL_ERROR_WANT_READ || err == WOLFSSL_ERROR_WANT_WRITE));

  wolfSSL_free(tls->ssl);
  wolfSSL_CTX_free(tls->ctx);
  free(tls);
  c->tls = NULL;
}

size_t mg_tls_pending(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  return tls == NULL ? 0 : (size_t) wolfSSL_pending(tls->ssl);
}

long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int ret, err;

  ret = wolfSSL_read(tls->ssl, buf, (int) len);

  if (ret == 0) {
    err = wolfSSL_get_error(tls->ssl, ret);
    if (err == WOLFSSL_ERROR_WANT_READ) return MG_IO_WAIT;
    else return MG_IO_ERR;
  }

  return ret;
}

long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int ret, err;

  ret = wolfSSL_write(tls->ssl, buf, (int) len);

  if (ret == 0) {
    err = wolfSSL_get_error(tls->ssl, ret);
    if (err == WOLFSSL_ERROR_WANT_WRITE) return MG_IO_WAIT;
    else return MG_IO_ERR;
  }

  return ret;
}

#endif
