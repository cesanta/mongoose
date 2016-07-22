---
title: "mg_get_http_header()"
decl_name: "mg_get_http_header"
symbol_kind: "func"
signature: |
  struct mg_str *mg_get_http_header(struct http_message *hm, const char *name);
---

Search and return header `name` in parsed HTTP message `hm`.
If header is not found, NULL is returned. Example:

    struct mg_str *host_hdr = mg_get_http_header(hm, "Host"); 

