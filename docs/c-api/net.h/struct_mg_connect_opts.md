---
title: "struct mg_connect_opts"
decl_name: "struct mg_connect_opts"
symbol_kind: "struct"
signature: |
  struct mg_connect_opts {
    void *user_data;           /* Initial value for connection's user_data */
    unsigned int flags;        /* Extra connection flags */
    const char **error_string; /* Placeholder for the error string */
  #ifdef MG_ENABLE_SSL
    /* SSL settings. */
    const char *ssl_cert;    /* Client certificate to present to the server */
    const char *ssl_ca_cert; /* Verify server certificate using this CA bundle */
  
    /*
     * Server name verification. If ssl_ca_cert is set and the certificate has
     * passed verification, its subject will be verified against this string.
     * By default (if ssl_server_name is NULL) hostname part of the address will
     * be used. Wildcard matching is supported. A special value of "*" disables
     * name verification.
     */
    const char *ssl_server_name;
  #endif
  };
---

Optional parameters to `mg_connect_opt()` 

