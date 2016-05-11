---
title: Events
---

Mongoose accepts incoming connections, reads and writes data, and calls
specified event handler for each connection when appropriate. Typical event
sequence is this:

- For outbound connection: `MG_EV_CONNECT` -> (`MG_EV_RECV`, `MG_EV_SEND`,
  `MG_EV_POLL` ...) -> `MG_EV_CLOSE`
- For inbound connection: `MG_EV_ACCEPT` ->  (`MG_EV_RECV`, `MG_EV_SEND`,
  `MG_EV_POLL` ...) -> `MG_EV_CLOSE`


Below is a list of core events triggered by Mongoose (note that each protocol
triggers protocol-specific events in addition to the core ones):

- `MG_EV_ACCEPT`: sent when new server connection is accepted by a listening
  connection. `void *ev_data` is `union socket_address` of the remote peer.

- `MG_EV_CONNECT`: sent when a new outbound connection created by `mg_connect()`
  either failed or succeeded. `void *ev_data` is `int *success`.  If `success`
  is 0, then connection has been established, otherwise it contains error code.
  See `mg_connect_opt()` function for code example.

- `MG_EV_RECV`: New data is received and appended to the end of `recv_mbuf`.
  `void *ev_data` is `int *num_received_bytes`. Typically, event handler should
  check received data in `nc->recv_mbuf`, discard processed data by calling
  `mbuf_remove()`, set connection flags `nc->flags` if necessary (see `struct
  mg_connection`), and write data the remote peer by output functions like
  `mg_send()`.

  **WARNING**: Mongoose uses `realloc()` to expand receive buffer.  It is
  user's responsibility to discard processed data from the beginning of receive
  buffer, note the `mbuf_remove()` call in the example above.

- `MG_EV_SEND`: Mongoose has written data to the remote peer and discarded
  written data from the `mg_connection::send_mbuf`. `void *ev_data` is `int
  *num_sent_bytes`.

  **NOTE**: Mongoose output functions only append data to the
  `mg_connection::send_mbuf`, they do not do any socket writes.  An actual IO
  is done by `mg_mgr_poll()`. `MG_EV_SEND` event is just a notification about
  an IO has been done.

- `MG_EV_POLL`: Sent to all connections on each invocation of `mg_mgr_poll()`.
  This event could be used to do any housekeeping, for example check whether
  certain timeout has expired and close the connection, or send heartbeet
  message, et cetera.

- `MG_EV_TIMER`: Sent to the connection if `mg_set_timer()` was called.

