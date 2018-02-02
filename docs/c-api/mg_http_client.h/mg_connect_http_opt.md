---
title: "mg_connect_http_opt()"
decl_name: "mg_connect_http_opt"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_connect_http_opt(
      struct mg_mgr *mgr, MG_CB(mg_event_handler_t ev_handler, void *user_data);
---

Helper function that creates an outbound HTTP connection.

Mostly identical to mg_connect_http, but allows you to provide extra
*parameters
(for example, SSL parameters) 

