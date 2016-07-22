---
title: "mg_send()"
decl_name: "mg_send"
symbol_kind: "func"
signature: |
  void mg_send(struct mg_connection *, const void *buf, int len);
---

Send data to the connection.

Note that sending functions do not actually push data to the socket.
They just append data to the output buffer. MG_EV_SEND will be delivered when
the data has actually been pushed out. 

