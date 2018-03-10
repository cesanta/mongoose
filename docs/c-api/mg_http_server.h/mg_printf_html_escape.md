---
title: "mg_printf_html_escape()"
decl_name: "mg_printf_html_escape"
symbol_kind: "func"
signature: |
  void mg_printf_html_escape(struct mg_connection *nc, const char *fmt, ...);
---

Sends a printf-formatted HTTP chunk, escaping HTML tags. 

