---
title: "mg_connect_ws_opt()"
decl_name: "mg_connect_ws_opt"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_connect_ws_opt(struct mg_mgr *mgr,
                                          mg_event_handler_t ev_handler,
                                          struct mg_connect_opts opts,
                                          const char *url, const char *protocol,
                                          const char *extra_headers);
---

Helper function that creates an outbound WebSocket connection

Mostly identical to mg_connect_ws, but allows to provide extra parameters
(for example, SSL parameters 

