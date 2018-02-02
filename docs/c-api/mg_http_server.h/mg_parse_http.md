---
title: "mg_parse_http()"
decl_name: "mg_parse_http"
symbol_kind: "func"
signature: |
  int mg_parse_http(const char *s, int n, struct http_message *hm, int is_req);
---

Parses a HTTP message.

`is_req` should be set to 1 if parsing a request, 0 if reply.

Returns the number of bytes parsed. If HTTP message is
incomplete `0` is returned. On parse error, a negative number is returned. 

