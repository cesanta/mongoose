---
title: "mg_send_websocket_handshake3v()"
decl_name: "mg_send_websocket_handshake3v"
symbol_kind: "func"
signature: |
  void mg_send_websocket_handshake3v(struct mg_connection *nc,
                                     const struct mg_str path,
                                     const struct mg_str host,
                                     const struct mg_str protocol,
                                     const struct mg_str extra_headers,
                                     const struct mg_str user,
                                     const struct mg_str pass);
---

Same as mg_send_websocket_handshake3 but with strings not necessarily
NUL-temrinated 

