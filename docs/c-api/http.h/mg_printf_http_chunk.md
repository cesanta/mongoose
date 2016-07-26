---
title: "mg_printf_http_chunk()"
decl_name: "mg_printf_http_chunk"
symbol_kind: "func"
signature: |
  void mg_printf_http_chunk(struct mg_connection *nc, const char *fmt, ...);
---

Sends a printf-formatted HTTP chunk.
Functionality is similar to `mg_send_http_chunk()`. 

