---
title: "mg_http_serve_file()"
decl_name: "mg_http_serve_file"
symbol_kind: "func"
signature: |
  void mg_http_serve_file(struct mg_connection *nc, struct http_message *hm,
                          const char *path, const struct mg_str mime_type,
                          const struct mg_str extra_headers);
---

Serves a specific file with a given MIME type and optional extra headers.

Example code snippet:

```c
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    case MG_EV_HTTP_REQUEST: {
      struct http_message *hm = (struct http_message *) ev_data;
      mg_http_serve_file(nc, hm, "file.txt",
                         mg_mk_str("text/plain"), mg_mk_str(""));
      break;
    }
    ...
  }
}
``` 

