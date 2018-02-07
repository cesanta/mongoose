---
title: "mg_http_parse_header()"
decl_name: "mg_http_parse_header"
symbol_kind: "func"
signature: |
  int mg_http_parse_header(struct mg_str *hdr, const char *var_name, char *buf,
                           size_t buf_size);
---

DEPRECATED: use mg_http_parse_header2() instead.

Same as mg_http_parse_header2(), but takes buffer as a `char *` (instead of
`char **`), and thus it cannot allocate a new buffer if the provided one
is not enough, and just returns 0 in that case. 

