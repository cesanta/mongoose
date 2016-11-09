---
title: "mg_basic_auth_header()"
decl_name: "mg_basic_auth_header"
symbol_kind: "func"
signature: |
  void mg_basic_auth_header(const char *user, const char *pass, struct mbuf *buf);
---

Generate a Basic Auth header and appends it to buf.
If pass is NULL, then user is expected to contain the credentials pair
already encoded as `user:pass`. 

