---
title: "mg_rpc_create_error()"
decl_name: "mg_rpc_create_error"
symbol_kind: "func"
signature: |
  int mg_rpc_create_error(char *buf, int len, struct mg_rpc_request *req,
                          int code, const char *message, const char *fmt, ...);
---

Create JSON-RPC error reply in a given buffer.

Return length of the error, which
can be larger then `len` that indicates an overflow.
`fmt` format string should conform to `json_emit()` API,
see https://github.com/cesanta/frozen 

