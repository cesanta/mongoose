---
title: "mg_connect()"
decl_name: "mg_connect"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *address,
                                   MG_CB(mg_event_handler_t handler,
                                         void *user_data);
---

Connects to a remote host.

See `mg_connect_opt()` for full documentation. 

