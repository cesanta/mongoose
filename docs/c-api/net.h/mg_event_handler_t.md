---
title: "mg_event_handler_t"
decl_name: "mg_event_handler_t"
symbol_kind: "typedef"
signature: |
  typedef void (*mg_event_handler_t)(struct mg_connection *, int ev, void *);
---

Callback function (event handler) prototype, must be defined by user.
Mongoose calls event handler, passing events defined below. 

