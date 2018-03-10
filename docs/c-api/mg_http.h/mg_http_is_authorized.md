---
title: "mg_http_is_authorized()"
decl_name: "mg_http_is_authorized"
symbol_kind: "func"
signature: |
  int mg_http_is_authorized(struct http_message *hm, struct mg_str path,
                            const char *domain, const char *passwords_file,
                            int flags);
---

Checks whether an http request is authorized. `domain` is the authentication
realm, `passwords_file` is a htdigest file (can be created e.g. with
`htdigest` utility). If either `domain` or `passwords_file` is NULL, this
function always returns 1; otherwise checks the authentication in the
http request and returns 1 only if there is a match; 0 otherwise. 

