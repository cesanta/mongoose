---
title: "mg_add_sock()"
decl_name: "mg_add_sock"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_add_sock(struct mg_mgr *, sock_t, mg_event_handler_t);
---

Creates a connection, associates it with the given socket and event handler
and adds it to the manager.

For more options see the `mg_add_sock_opt` variant. 

