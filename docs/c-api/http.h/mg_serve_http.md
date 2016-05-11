---
title: "mg_serve_http()"
decl_name: "mg_serve_http"
symbol_kind: "func"
signature: |
  void mg_serve_http(struct mg_connection *nc, struct http_message *hm,
                     struct mg_serve_http_opts opts);
---

Serve given HTTP request according to the `options`.

Example code snippet:

```c
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  struct mg_serve_http_opts opts = { .document_root = "/var/www" };  // C99

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      mg_serve_http(nc, hm, opts);
      break;
    default:
      break;
  }
}
``` 

