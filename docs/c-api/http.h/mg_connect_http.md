---
title: "mg_connect_http()"
decl_name: "mg_connect_http"
symbol_kind: "func"
signature: |
  struct mg_connection *mg_connect_http(struct mg_mgr *mgr,
                                        mg_event_handler_t event_handler,
                                        const char *url,
                                        const char *extra_headers,
                                        const char *post_data);
---

Helper function that creates outbound HTTP connection.

`url` is a URL to fetch. It must be properly URL-encoded, e.g. have
no spaces, etc. By default, `mg_connect_http()` sends Connection and
Host headers. `extra_headers` is an extra HTTP headers to send, e.g.
`"User-Agent: my-app\r\n"`.
If `post_data` is NULL, then GET request is created. Otherwise, POST request
is created with the specified POST data. Note that if the data being posted
is a form submission, the `Content-Type` header should be set accordingly
(see example below).

Examples:

```c
  nc1 = mg_connect_http(mgr, ev_handler_1, "http://www.google.com", NULL,
                        NULL);
  nc2 = mg_connect_http(mgr, ev_handler_1, "https://github.com", NULL, NULL);
  nc3 = mg_connect_http(
      mgr, ev_handler_1, "my_server:8000/form_submit/",
      "Content-Type: application/x-www-form-urlencoded\r\n",
      "var_1=value_1&var_2=value_2");
``` 

