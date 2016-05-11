---
title: "mg_get_http_var()"
decl_name: "mg_get_http_var"
symbol_kind: "func"
signature: |
  int mg_get_http_var(const struct mg_str *buf, const char *name, char *dst,
                      size_t dst_len);
---

Fetch an HTTP form variable.

Fetch a variable `name` from a `buf` into a buffer specified by
`dst`, `dst_len`. Destination is always zero-terminated. Return length
of a fetched variable. If not found, 0 is returned. `buf` must be
valid url-encoded buffer. If destination is too small, `-1` is returned. 

