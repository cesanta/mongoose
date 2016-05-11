---
title: "mg_rpc_dispatch()"
decl_name: "mg_rpc_dispatch"
symbol_kind: "func"
signature: |
  int mg_rpc_dispatch(const char *buf, int, char *dst, int dst_len,
                      const char **methods, mg_rpc_handler_t *handlers);
---

Dispatches a JSON-RPC request.

Parses JSON-RPC request contained in `buf`, `len`.
Then, dispatches the request to the correct handler method.
Valid method names should be specified in NULL
terminated array `methods`, and corresponding handlers in `handlers`.
Result is put in `dst`, `dst_len`. Return: length of the result, which
can be larger then `dst_len` that indicates an overflow.
Overflown bytes are not written to the buffer.
If method is not found, an error is automatically generated. 

