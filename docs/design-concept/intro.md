---
title: Design Concept
---

Mongoose is a multi-protocol networking library that implements non-blocking,
asyncronous IO and provides event-based API. It has three basic data
structures:

- `struct mg_mgr` is an event manager that holds all active connections
- `struct mg_connection` describes a connection
- `struct mbuf` describes data buffer (received or sent data)

Connections could be either *listening*, *outbound* or *inbound*.  Outbound
connections are created by `mg_connect()` call.  Listening connections are
created by `mg_bind()` call.  Inbound connections are those accepted by a
listening connection.  Each connection is described by `struct mg_connection`
structure, which has a number of fields like socket, event handler function,
send/receive buffer, flags, et cetera.

Mongoose usage pattern is to declare and initialize event manager, create
connections and create an event loop by calling `mg_mgr_poll()` in a loop.
`mg_mgr_poll()` iterates over all sockets, accepts new connections, sends and
receives data, closes connections, and calls event handler functions for the
respective events.
