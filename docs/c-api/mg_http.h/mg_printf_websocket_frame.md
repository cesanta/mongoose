---
title: "mg_printf_websocket_frame()"
decl_name: "mg_printf_websocket_frame"
symbol_kind: "func"
signature: |
  void mg_printf_websocket_frame(struct mg_connection *nc, int op_and_flags,
                                 const char *fmt, ...);
---

Sends WebSocket frame to the remote end.

Like `mg_send_websocket_frame()`, but allows to create formatted messages
with `printf()`-like semantics. 

