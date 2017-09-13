---
title: "mg_connect_ws()"
decl_name: "mg_connect_ws"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_connect_ws(struct mg_mgr *mgr,
                                      MG_CB(mg_event_handler_t event_handler,
                                            void *user_data);
---

Helper function that creates an outbound WebSocket connection.

`url` is a URL to connect to. It must be properly URL-encoded, e.g. have
no spaces, etc. By default, `mg_connect_ws()` sends Connection and
Host headers. `extra_headers` is an extra HTTP header to send, e.g.
`"User-Agent: my-app\r\n"`.
If `protocol` is not NULL, then a `Sec-WebSocket-Protocol` header is sent.

Examples:

```c
  nc1 = mg_connect_ws(mgr, ev_handler_1, "ws://echo.websocket.org", NULL,
                      NULL);
  nc2 = mg_connect_ws(mgr, ev_handler_1, "wss://echo.websocket.org", NULL,
                      NULL);
  nc3 = mg_connect_ws(mgr, ev_handler_1, "ws://api.cesanta.com",
                      "clubby.cesanta.com", NULL);
``` 

