---
title: "mg_register_http_endpoint()"
decl_name: "mg_register_http_endpoint"
symbol_kind: "func"
signature: |
  void mg_register_http_endpoint(struct mg_connection *nc, const char *uri_path,
                                 MG_CB(mg_event_handler_t handler,
                                       void *user_data);
---

Registers a callback for a specified http endpoint
Note: if callback is registered it is called instead of the
callback provided in mg_bind

Example code snippet:

```c
static void handle_hello1(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev; (void) ev_data;
  mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello1]");
 nc->flags |= MG_F_SEND_AND_CLOSE;
}

static void handle_hello2(struct mg_connection *nc, int ev, void *ev_data) {
 (void) ev; (void) ev_data;
  mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello2]");
 nc->flags |= MG_F_SEND_AND_CLOSE;
}

void init() {
  nc = mg_bind(&mgr, local_addr, cb1);
  mg_register_http_endpoint(nc, "/hello1", handle_hello1);
  mg_register_http_endpoint(nc, "/hello1/hello2", handle_hello2);
}
``` 

