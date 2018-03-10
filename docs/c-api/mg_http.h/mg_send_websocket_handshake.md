---
title: "mg_send_websocket_handshake()"
decl_name: "mg_send_websocket_handshake"
symbol_kind: "func"
signature: |
  void mg_send_websocket_handshake(struct mg_connection *nc, const char *uri,
                                   const char *extra_headers);
---

Send websocket handshake to the server.

`nc` must be a valid connection, connected to a server. `uri` is an URI
to fetch, extra_headers` is extra HTTP headers to send or `NULL`.

This function is intended to be used by websocket client.

Note that the Host header is mandatory in HTTP/1.1 and must be
included in `extra_headers`. `mg_send_websocket_handshake2` offers
a better API for that.

Deprecated in favour of `mg_send_websocket_handshake2` 

