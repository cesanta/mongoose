---
title: "mg_http_send_redirect()"
decl_name: "mg_http_send_redirect"
symbol_kind: "func"
signature: |
  void mg_http_send_redirect(struct mg_connection *nc, int status_code,
                             const struct mg_str location,
                             const struct mg_str extra_headers);
---

Sends a redirect response.
`status_code` should be either 301 or 302 and `location` point to the
new location.
If `extra_headers` is not empty, then `extra_headers` are also sent
after the reponse line. `extra_headers` must NOT end end with new line.
Example:

     mg_send_response_line(nc, 200, "Access-Control-Allow-Origin: *");

Will result in:

     HTTP/1.1 200 OK\r\n
     Access-Control-Allow-Origin: *\r\n 

