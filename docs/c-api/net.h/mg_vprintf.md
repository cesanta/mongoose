---
title: "mg_vprintf()"
decl_name: "mg_vprintf"
symbol_kind: "func"
signature: |
  int mg_vprintf(struct mg_connection *, const char *fmt, va_list ap);
---

Same as `mg_printf()`, but takes `va_list ap` as an argument. 

