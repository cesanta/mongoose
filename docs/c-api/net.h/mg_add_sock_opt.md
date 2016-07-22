---
title: "mg_add_sock_opt()"
decl_name: "mg_add_sock_opt"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_add_sock_opt(struct mg_mgr *, sock_t,
                                        mg_event_handler_t,
                                        struct mg_add_sock_opts);
---

Create a connection, associate it with the given socket and event handler,
and add to the manager.

See the `mg_add_sock_opts` structure for a description of the options. 

