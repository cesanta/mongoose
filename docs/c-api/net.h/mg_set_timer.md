---
title: "mg_set_timer()"
decl_name: "mg_set_timer"
symbol_kind: "func"
signature: |
  double mg_set_timer(struct mg_connection *c, double timestamp);
---

Schedules an MG_EV_TIMER event to be delivered at `timestamp` time.
`timestamp` is UNIX time (the number of seconds since Epoch). It is
`double` instead of `time_t` to allow for sub-second precision.
Returns the old timer value.

Example: set the connect timeout to 1.5 seconds:

```
 c = mg_connect(&mgr, "cesanta.com", ev_handler);
 mg_set_timer(c, mg_time() + 1.5);
 ...

 void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
 switch (ev) {
   case MG_EV_CONNECT:
     mg_set_timer(c, 0);  // Clear connect timer
     break;
   case MG_EV_TIMER:
     log("Connect timeout");
     c->flags |= MG_F_CLOSE_IMMEDIATELY;
     break;
``` 

