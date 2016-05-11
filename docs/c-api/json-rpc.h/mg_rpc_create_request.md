---
title: "mg_rpc_create_request()"
decl_name: "mg_rpc_create_request"
symbol_kind: "func"
signature: |
  int mg_rpc_create_request(char *buf, int len, const char *method,
                            const char *id, const char *params_fmt, ...);
---

Create JSON-RPC request in a given buffer.

Return length of the request, which
can be larger then `len` that indicates an overflow.
`params_fmt` format string should conform to `json_emit()` API,
see https://github.com/cesanta/frozen 

