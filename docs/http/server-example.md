# HTTP server example

To create an HTTP server, follow this pattern:

1. Create a listening connection by calling `mg_bind()` or `mg_bind_opt()`
2. Call `mg_set_protocol_http_websocket()` for that listening connection.
  That attaches a built-in HTTP event handler which parses incoming
  data and triggers HTTP-specific events. For example, when an HTTP request
  is fully buffered, a built-in HTTP handler parses the request and
  calls user-defined event handler with `MG_EV_HTTP_REQUEST` event and
  parsed HTTP request as an event data.
3. Create event handler function. Note that event handler receives all
  events - low level TCP events like `MG_EV_RECV` and high-level HTTP
  events like `MG_EV_HTTP_REQUEST`. Normally, an event handler function
  should only handle `MG_EV_HTTP_REQUEST` event.

Here's an example of the simplest HTTP server. Error checking is omitted for
the sake of clarity:

```c
#include "mongoose.h"

static const char *s_http_port = "8000";

static void ev_handler(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) p;

    // We have received an HTTP request. Parsed request is contained in `hm`.
    // Send HTTP reply to the client which shows full original request.
    mg_send_head(c, 200, hm->message.len, "Content-Type: text/plain");
    mg_printf(c, "%.*s", (int)hm->message.len, hm->message.p);
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;

  mg_mgr_init(&mgr, NULL);
  c = mg_bind(&mgr, s_http_port, ev_handler);
  mg_set_protocol_http_websocket(c);

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
```

See full [HTTP server example](https://github.com/cesanta/mongoose/tree/master/examples/simplest_web_server).
