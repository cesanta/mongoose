---
title: "mg_parse_http_basic_auth()"
decl_name: "mg_parse_http_basic_auth"
symbol_kind: "func"
signature: |
  int mg_parse_http_basic_auth(struct mg_str *hdr, char *user, size_t user_len,
                               char *pass, size_t pass_len);
---

Parses the Authorization: Basic header
Returns -1 iif the authorization type is not "Basic" or any other error such
as incorrectly encoded base64 user password pair. 

