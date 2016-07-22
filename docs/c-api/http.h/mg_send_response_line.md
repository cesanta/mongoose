---
title: "mg_send_response_line()"
decl_name: "mg_send_response_line"
symbol_kind: "func"
signature: |
  void mg_send_response_line(struct mg_connection *c, int status_code,
                             const char *extra_headers);
---

Send response status line.
If `extra_headers` is not NULL, then `extra_headers` are also sent
after the reponse line. `extra_headers` must NOT end end with new line.
Example:

     mg_send_response_line(nc, 200, "Access-Control-Allow-Origin: *");

Will result in:

     HTTP/1.1 200 OK\r\n
     Access-Control-Allow-Origin: *\r\n 

