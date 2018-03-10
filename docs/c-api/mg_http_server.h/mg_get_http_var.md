---
title: "mg_get_http_var()"
decl_name: "mg_get_http_var"
symbol_kind: "func"
signature: |
  int mg_get_http_var(const struct mg_str *buf, const char *name, char *dst,
                      size_t dst_len);
---

Fetches a HTTP form variable.

Fetches a variable `name` from a `buf` into a buffer specified by `dst`,
`dst_len`. The destination is always zero-terminated. Returns the length of
a fetched variable. If not found, 0 is returned. `buf` must be valid
url-encoded buffer. If destination is too small or an error occured,
negative number is returned. 

