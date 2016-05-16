---
title: "mg_http_check_digest_auth()"
decl_name: "mg_http_check_digest_auth"
symbol_kind: "func"
signature: |
  int mg_http_check_digest_auth(struct http_message *hm, const char *auth_domain,
                                FILE *fp);
---

Authenticate HTTP request against opened passwords file.
Returns 1 if authenticated, 0 otherwise. 

