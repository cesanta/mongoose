---
title: "mg_event_handler_t"
decl_name: "mg_event_handler_t"
symbol_kind: "typedef"
signature: |
  typedef void (*mg_event_handler_t)(struct mg_connection *nc, int ev,
                                     void *ev_data);
---

Callback function (event handler) prototype. Must be defined by the user.
Mongoose calls the event handler, passing the events defined below. 

