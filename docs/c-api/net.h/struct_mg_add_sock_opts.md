---
title: "struct mg_add_sock_opts"
decl_name: "struct mg_add_sock_opts"
symbol_kind: "struct"
signature: |
  struct mg_add_sock_opts {
    void *user_data;           /* Initial value for connection's user_data */
    unsigned int flags;        /* Initial connection flags */
    const char **error_string; /* Placeholder for the error string */
    struct mg_iface *iface;    /* Interface instance */
  };
---

Optional parameters to `mg_add_sock_opt()`.

`flags` is an initial `struct mg_connection::flags` bitmask to set,
see `MG_F_*` flags definitions. 

