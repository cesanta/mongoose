---
title: "struct mg_rpc_error"
decl_name: "struct mg_rpc_error"
symbol_kind: "struct"
signature: |
  struct mg_rpc_error {
    struct json_token *message;       /* Whole RPC message */
    struct json_token *id;            /* Message ID */
    struct json_token *error_code;    /* error.code */
    struct json_token *error_message; /* error.message */
    struct json_token *error_data;    /* error.data, can be NULL */
  };
---

JSON-RPC error 

