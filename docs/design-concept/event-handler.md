---
title: Event handler function
---

Each connection has an event handler function associated with it. That function
must be implemented by user. Event handler is the key element of the Mongoose
application, since it defines application's behavior. This is how an event
handler function looks like:

```c
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    /* Event handler code that defines behavior of the connection */
    ...
  }
}
```

- `struct mg_connection *nc`: Connection that has received an event.
- `int ev`: Event number, defined in `mongoose.h`. For example, when data
  arrives on inbound connection `ev` would be `MG_EV_RECV`.
- `void *ev_data`: This pointer points to the event-specific data, and it has
  different meaning for different events. For example, for `MG_EV_RECV` event,
  `ev_data` is an `int *` pointer, pointing to the number of bytes received
  from the remote peer and saved into the receive IO buffer. Exact meaning of
  `ev_data` is described for each event. Protocol-specific events usually have
  `ev_data` pointing to structures that hold protocol-specific information.

NOTE: `struct mg_connection` has `void *user_data` which is a placeholder for
an application-specific data. Mongoose does not use that pointer. Event handler
can store any kind of information there.
