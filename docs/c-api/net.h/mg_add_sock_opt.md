---
title: "mg_add_sock_opt()"
decl_name: "mg_add_sock_opt"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_add_sock_opt(struct mg_mgr *mgr, sock_t sock,
                                        MG_CB(mg_event_handler_t handler,
                                              void *user_data);
---

Creates a connection, associates it with the given socket and event handler
and adds to the manager.

See the `mg_add_sock_opts` structure for a description of the options. 

