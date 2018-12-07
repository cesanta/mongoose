---
title: "mg_broadcast()"
decl_name: "mg_broadcast"
symbol_kind: "func"
signature: |
  void mg_broadcast(struct mg_mgr *mgr, mg_event_handler_t cb, void *data,
                    size_t len);
---

Passes a message of a given length to all connections.

Must be called from a thread that does NOT call `mg_mgr_poll()`.
Note that `mg_broadcast()` is the only function
that can be, and must be, called from a different (non-IO) thread.

`func` callback function will be called by the IO thread for each
connection. When called, the event will be `MG_EV_POLL`, and a message will
be passed as the `ev_data` pointer. Maximum message size is capped
by `MG_CTL_MSG_MESSAGE_SIZE` which is set to 8192 bytes by default. 

