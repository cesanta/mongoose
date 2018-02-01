---
title: "mg_http_create_digest_auth_header()"
decl_name: "mg_http_create_digest_auth_header"
symbol_kind: "func"
signature: |
  int mg_http_create_digest_auth_header(char *buf, size_t buf_len,
                                        const char *method, const char *uri,
                                        const char *auth_domain, const char *user,
                                        const char *passwd, const char *nonce);
---

Creates digest authentication header for a client request. 

