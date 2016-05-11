---
title: "struct mg_rpc_reply"
decl_name: "struct mg_rpc_reply"
symbol_kind: "struct"
signature: |
  struct mg_rpc_reply {
    struct json_token *message; /* Whole RPC message */
    struct json_token *id;      /* Message ID */
    struct json_token *result;  /* Remote call result */
  };
---

JSON-RPC response 

