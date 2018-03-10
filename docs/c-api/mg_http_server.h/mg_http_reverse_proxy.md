---
title: "mg_http_reverse_proxy()"
decl_name: "mg_http_reverse_proxy"
symbol_kind: "func"
signature: |
  void mg_http_reverse_proxy(struct mg_connection *nc,
                             const struct http_message *hm, struct mg_str mount,
                             struct mg_str upstream);
---

Proxies a given request to a given upstream http server. The path prefix
in `mount` will be stripped of the path requested to the upstream server,
e.g. if mount is /api and upstream is http://localhost:8001/foo
then an incoming request to /api/bar will cause a request to
http://localhost:8001/foo/bar

EXPERIMENTAL API. Please use http_serve_http + url_rewrites if a static
mapping is good enough. 

