---
title: Serving files
---

API function `mg_serve_http()` makes it easy to serve files from a filesystem.
Generally speaking, that function is an implementation of the HTTP server
that serves static files, CGI and SSI. It's behavior is driven by a list
of options that are consolidated into the `struct mg_serve_http_opts`
structure. See [struct mg_serve_http_opts](#) definition for the full list
of capabilities of `mg_serve_http()`.

For example, in order to create a web server that serves static files
from the current directory, implement event handler function as follows:

```c
static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct mg_serve_http_opts opts;

    memset(&opts, 0, sizeof(opts);  // Reset all options to defaults
    opts.document_root = ".";       // Serve files from the current directory

    mg_serve_http(c, (struct http_message *) ev_data, s_http_server_opts);
  }
}
```

See working example at [simplest web server](https://github.com/cesanta/mongoose/tree/master/examples/simplest_web_server).

Sometimes there is no need to implement a full static web server, for example
if one works on a RESTful server. If certain endpoints must return the contents
of a static file, a simpler `mg_http_serve_file()` function can be used:

```c
  static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
   switch (ev) {
     case MG_EV_HTTP_REQUEST: {
       struct http_message *hm = (struct http_message *) ev_data;
       mg_http_serve_file(c, hm, "file.txt",
                          mg_mk_str("text/plain"), mg_mk_str(""));
       break;
     }
     ...
   }
  }
```
