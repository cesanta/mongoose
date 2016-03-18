---
title: "mg_rpc_create_std_error()"
decl_name: "mg_rpc_create_std_error"
symbol_kind: "func"
signature: |
  int mg_rpc_create_std_error(char *buf, int len, struct mg_rpc_request *req,
                              int code);
---

Create JSON-RPC error in a given buffer.

Return length of the error, which
can be larger then `len` that indicates an overflow. See
JSON_RPC_*_ERROR definitions for standard error values:

- `#define JSON_RPC_PARSE_ERROR (-32700)`
- `#define JSON_RPC_INVALID_REQUEST_ERROR (-32600)`
- `#define JSON_RPC_METHOD_NOT_FOUND_ERROR (-32601)`
- `#define JSON_RPC_INVALID_PARAMS_ERROR (-32602)`
- `#define JSON_RPC_INTERNAL_ERROR (-32603)`
- `#define JSON_RPC_SERVER_ERROR (-32000)` 

