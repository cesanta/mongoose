---
title: "mg_send_websocket_framev()"
decl_name: "mg_send_websocket_framev"
symbol_kind: "func"
signature: |
  void mg_send_websocket_framev(struct mg_connection *nc, int op_and_flags,
                                const struct mg_str *strings, int num_strings);
---

Like `mg_send_websocket_frame()`, but composes a single frame from multiple
buffers. 

