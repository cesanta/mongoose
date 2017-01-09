---
title: "struct mg_bind_opts"
decl_name: "struct mg_bind_opts"
symbol_kind: "struct"
signature: |
  struct mg_bind_opts {
    void *user_data;           /* Initial value for connection's user_data */
    unsigned int flags;        /* Extra connection flags */
    const char **error_string; /* Placeholder for the error string */
    struct mg_iface *iface;    /* Interface instance */
  #if MG_ENABLE_SSL
    /*
     * SSL settings.
     *
     * Server certificate to present to clients or client certificate to
     * present to tunnel dispatcher (for tunneled connections).
     */
    const char *ssl_cert;
    /* Private key corresponding to the certificate. If ssl_cert is set but
     * ssl_key is not, ssl_cert is used. */
    const char *ssl_key;
    /* CA bundle used to verify client certificates or tunnel dispatchers. */
    const char *ssl_ca_cert;
    /* Colon-delimited list of acceptable cipher suites.
     * Names depend on the library used, for example:
     *
     * ECDH-ECDSA-AES128-GCM-SHA256:DHE-RSA-AES128-SHA256 (OpenSSL)
     * TLS-ECDH-ECDSA-WITH-AES-128-GCM-SHA256:TLS-DHE-RSA-WITH-AES-128-GCM-SHA256
     *   (mbedTLS)
     *
     * For OpenSSL the list can be obtained by running "openssl ciphers".
     * For mbedTLS, names can be found in library/ssl_ciphersuites.c
     * If NULL, a reasonable default is used.
     */
    const char *ssl_cipher_suites;
  #endif
  };
---

Optional parameters to `mg_bind_opt()`.

`flags` is an initial `struct mg_connection::flags` bitmask to set,
see `MG_F_*` flags definitions. 

