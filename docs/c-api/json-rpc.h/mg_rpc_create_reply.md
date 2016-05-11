---
title: "mg_rpc_create_reply()"
decl_name: "mg_rpc_create_reply"
symbol_kind: "func"
signature: |
  int mg_rpc_create_reply(char *buf, int len, const struct mg_rpc_request *req,
                          const char *result_fmt, ...);
---

Create JSON-RPC reply in a given buffer.

Return length of the reply, which
can be larger then `len` that indicates an overflow.
`result_fmt` format string should conform to `json_emit()` API,
see https://github.com/cesanta/frozen 

