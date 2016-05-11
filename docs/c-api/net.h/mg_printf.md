---
title: "mg_printf()"
decl_name: "mg_printf"
symbol_kind: "func"
signature: |
  int mg_printf(struct mg_connection *, const char *fmt, ...);
---

Send `printf`-style formatted data to the connection.

See `mg_send` for more details on send semantics. 

