---
title: "mg_send_websocket_frame()"
decl_name: "mg_send_websocket_frame"
symbol_kind: "func"
signature: |
  void mg_send_websocket_frame(struct mg_connection *nc, int op_and_flags,
                               const void *data, size_t data_len);
---

Send WebSocket frame to the remote end.

`op_and_flags` specifies the frame's type. It's one of:

- WEBSOCKET_OP_CONTINUE
- WEBSOCKET_OP_TEXT
- WEBSOCKET_OP_BINARY
- WEBSOCKET_OP_CLOSE
- WEBSOCKET_OP_PING
- WEBSOCKET_OP_PONG

Orred with one of the flags:

- WEBSOCKET_DONT_FIN: Don't set the FIN flag on the frame to be sent.

`data` and `data_len` contain frame data. 

