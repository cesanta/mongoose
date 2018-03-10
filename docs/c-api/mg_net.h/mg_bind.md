---
title: "mg_bind()"
decl_name: "mg_bind"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_bind(struct mg_mgr *mgr, const char *address,
                                MG_CB(mg_event_handler_t handler,
                                      void *user_data);
---

Creates a listening connection.

See `mg_bind_opt` for full documentation. 

