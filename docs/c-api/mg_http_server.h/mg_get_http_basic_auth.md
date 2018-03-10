---
title: "mg_get_http_basic_auth()"
decl_name: "mg_get_http_basic_auth"
symbol_kind: "func"
signature: |
  int mg_get_http_basic_auth(struct http_message *hm, char *user, size_t user_len,
                             char *pass, size_t pass_len);
---

Gets and parses the Authorization: Basic header
Returns -1 if no Authorization header is found, or if
mg_parse_http_basic_auth
fails parsing the resulting header. 

