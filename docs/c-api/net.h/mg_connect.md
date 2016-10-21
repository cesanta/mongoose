---
title: "mg_connect()"
decl_name: "mg_connect"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *address,
                                   mg_event_handler_t handler);
---

Connects to a remote host.

See `mg_connect_opt()` for full documentation. 

