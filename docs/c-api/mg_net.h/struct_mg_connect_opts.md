---
title: "struct mg_connect_opts"
decl_name: "struct mg_connect_opts"
symbol_kind: "struct"
signature: |
  struct mg_connect_opts {
    void *user_data;           /* Initial value for connection's user_data */
    unsigned int flags;        /* Extra connection flags */
    const char **error_string; /* Placeholder for the error string */
    struct mg_iface *iface;    /* Interface instance */
    const char *nameserver;    /* DNS server to use, NULL for default */
  #if MG_ENABLE_SSL
    /*
     * SSL settings.
     * Client certificate to present to the server.
     */
    const char *ssl_cert;
    /*
     * Private key corresponding to the certificate.
     * If ssl_cert is set but ssl_key is not, ssl_cert is used.
     */
    const char *ssl_key;
    /*
     * Verify server certificate using this CA bundle. If set to "*", then SSL
     * is enabled but no cert verification is performed.
     */
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
    /*
     * Server name verification. If ssl_ca_cert is set and the certificate has
     * passed verification, its subject will be verified against this string.
     * By default (if ssl_server_name is NULL) hostname part of the address will
     * be used. Wildcard matching is supported. A special value of "*" disables
     * name verification.
     */
    const char *ssl_server_name;
    /*
     * PSK identity and key. Identity is a NUL-terminated string and key is a hex
     * string. Key must be either 16 or 32 bytes (32 or 64 hex digits) for AES-128
     * or AES-256 respectively.
     * Note: Default list of cipher suites does not include PSK suites, if you
     * want to use PSK you will need to set ssl_cipher_suites as well.
     */
    const char *ssl_psk_identity;
    const char *ssl_psk_key;
  #endif
  };
---

Optional parameters to `mg_connect_opt()` 

