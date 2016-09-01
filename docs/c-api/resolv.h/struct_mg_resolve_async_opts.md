---
title: "struct mg_resolve_async_opts"
decl_name: "struct mg_resolve_async_opts"
symbol_kind: "struct"
signature: |
  struct mg_resolve_async_opts {
    const char *nameserver_url;
    int max_retries;    /* defaults to 2 if zero */
    int timeout;        /* in seconds; defaults to 5 if zero */
    int accept_literal; /* pseudo-resolve literal ipv4 and ipv6 addrs */
    int only_literal;   /* only resolves literal addrs; sync cb invocation */
    struct mg_connection **dns_conn; /* return DNS connection */
  };
---

Options for `mg_resolve_async_opt`. 

