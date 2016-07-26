---
title: "mg_bind_opt()"
decl_name: "mg_bind_opt"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_bind_opt(struct mg_mgr *mgr, const char *address,
                                    mg_event_handler_t handler,
                                    struct mg_bind_opts opts);
---

Creates a listening connection.

The `address` parameter specifies which address to bind to. It's format is
the same as for the `mg_connect()` call, where `HOST` part is optional.
`address` can be just a port number, e.g. `:8000`. To bind to a specific
interface, an IP address can be specified, e.g. `1.2.3.4:8000`. By default,
a TCP connection is created. To create UDP connection, prepend `udp://`
prefix, e.g. `udp://:8000`. To summarize, `address` paramer has following
format: `[PROTO://][IP_ADDRESS]:PORT`, where `PROTO` could be `tcp` or
`udp`.

See the `mg_bind_opts` structure for a description of the optional
parameters.

Returns a new listening connection or `NULL` on error.
NOTE: The connection remains owned by the manager, do not free(). 

