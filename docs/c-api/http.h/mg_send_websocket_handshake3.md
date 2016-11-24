---
title: "mg_send_websocket_handshake3()"
decl_name: "mg_send_websocket_handshake3"
symbol_kind: "func"
signature: |
  void mg_send_websocket_handshake3(struct mg_connection *nc, const char *path,
                                    const char *host, const char *protocol,
                                    const char *extra_headers, const char *user,
                                    const char *pass);
---

Like mg_send_websocket_handshake2 but also passes basic auth header 

