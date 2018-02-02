---
title: "mg_add_sock()"
decl_name: "mg_add_sock"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_add_sock(struct mg_mgr *mgr, sock_t sock,
                                    MG_CB(mg_event_handler_t handler,
                                          void *user_data);
---

Creates a connection, associates it with the given socket and event handler
and adds it to the manager.

For more options see the `mg_add_sock_opt` variant. 

