---
title: "struct mg_rpc_request"
decl_name: "struct mg_rpc_request"
symbol_kind: "struct"
signature: |
  struct mg_rpc_request {
    struct json_token *message; /* Whole RPC message */
    struct json_token *id;      /* Message ID */
    struct json_token *method;  /* Method name */
    struct json_token *params;  /* Method params */
  };
---

JSON-RPC request 

