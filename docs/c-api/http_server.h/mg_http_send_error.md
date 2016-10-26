---
title: "mg_http_send_error()"
decl_name: "mg_http_send_error"
symbol_kind: "func"
signature: |
  void mg_http_send_error(struct mg_connection *nc, int code, const char *reason);
---

Sends an error response. If reason is NULL, the message will be inferred
from the error code (if supported). 

