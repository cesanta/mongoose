---
title: "mg_rpc_parse_reply()"
decl_name: "mg_rpc_parse_reply"
symbol_kind: "func"
signature: |
  int mg_rpc_parse_reply(const char *buf, int len, struct json_token *toks,
                         int max_toks, struct mg_rpc_reply *,
                         struct mg_rpc_error *);
---

Parse JSON-RPC reply contained in `buf`, `len` into JSON tokens array
`toks`, `max_toks`. If buffer contains valid reply, `reply` structure is
populated. The result of RPC call is located in `reply.result`. On error,
`error` structure is populated. Returns: the result of calling
`parse_json(buf, len, toks, max_toks)`:

On success, an offset inside `json_string` is returned
where parsing has finished. On failure, a negative number is
returned, one of:

- `#define JSON_STRING_INVALID           -1`
- `#define JSON_STRING_INCOMPLETE        -2`
- `#define JSON_TOKEN_ARRAY_TOO_SMALL    -3` 

