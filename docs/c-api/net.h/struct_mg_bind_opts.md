---
title: "struct mg_bind_opts"
decl_name: "struct mg_bind_opts"
symbol_kind: "struct"
signature: |
  struct mg_bind_opts {
    void *user_data;           /* Initial value for connection's user_data */
    unsigned int flags;        /* Extra connection flags */
    const char **error_string; /* Placeholder for the error string */
  #ifdef MG_ENABLE_SSL
    /* SSL settings. */
    const char *ssl_cert;    /* Server certificate to present to clients */
    const char *ssl_ca_cert; /* Verify client certificates with this CA bundle */
  #endif
  };
---

Optional parameters to `mg_bind_opt()`.

`flags` is an initial `struct mg_connection::flags` bitmask to set,
see `MG_F_*` flags definitions. 

