---
title: "mg_send_websocket_handshake2()"
decl_name: "mg_send_websocket_handshake2"
symbol_kind: "func"
signature: |
  void mg_send_websocket_handshake2(struct mg_connection *nc, const char *path,
                                    const char *host, const char *protocol,
                                    const char *extra_headers);
---

Send websocket handshake to the server.

`nc` must be a valid connection, connected to a server. `uri` is an URI
to fetch, `host` goes into the `Host` header, `protocol` goes into the
`Sec-WebSocket-Proto` header (NULL to omit), extra_headers` is extra HTTP
headers to send or `NULL`.

This function is intended to be used by websocket client. 

