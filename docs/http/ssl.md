---
title: Enabling HTTPS
---

To enable SSL on the server side, please follow these steps:

- Obtain SSL certificate file and private key file
- Declare `struct mg_bind_opts`, initialize `ssl_cert` and `ssl_key`
- Use `mg_bind_opt()` to create listening socket

Example:

```c
int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;
  struct mg_bind_opts bind_opts;

  mg_mgr_init(&mgr, NULL);

  memset(&bind_opts, 0, sizeof(bind_opts));
  bind_opts.ssl_cert = "server.pem";
  bind_opts.ssl_key = "key.pem";

  // Use bind_opts to specify SSL certificate & key file
  c = mg_bind_opt(&mgr, "443", ev_handler, bind_opts);
  mg_set_protocol_http_websocket(c);

  ...
}
```

For the full example, please see the [Simplest HTTPS server example](https://github.com/cesanta/dev/tree/master/mongoose/examples/simplest_web_server_ssl).
