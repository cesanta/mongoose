---
title: "struct mg_ssi_call_ctx"
decl_name: "struct mg_ssi_call_ctx"
symbol_kind: "struct"
signature: |
  struct mg_ssi_call_ctx {
    struct http_message *req; /* The request being processed. */
    struct mg_str file;       /* Filesystem path of the file being processed. */
    struct mg_str arg; /* The argument passed to the tag: <!-- call arg -->. */
  };
---

SSI call context 

