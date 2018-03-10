---
title: "mg_check_digest_auth()"
decl_name: "mg_check_digest_auth"
symbol_kind: "func"
signature: |
  int mg_check_digest_auth(struct mg_str method, struct mg_str uri,
                           struct mg_str username, struct mg_str cnonce,
                           struct mg_str response, struct mg_str qop,
                           struct mg_str nc, struct mg_str nonce,
                           struct mg_str auth_domain, FILE *fp);
---

Authenticates given response params against an opened password file.
Returns 1 if authenticated, 0 otherwise.

It's used by mg_http_check_digest_auth(). 

