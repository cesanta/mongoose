---
title: "mg_send_head()"
decl_name: "mg_send_head"
symbol_kind: "func"
signature: |
  void mg_send_head(struct mg_connection *n, int status_code,
                    int64_t content_length, const char *extra_headers);
---

Send response line and headers.
This function sends response line with the `status_code`, and automatically
sends one header: either "Content-Length", or "Transfer-Encoding".
If `content_length` is negative, then "Transfer-Encoding: chunked" header
is sent, otherwise, "Content-Length" header is sent.

NOTE: If `Transfer-Encoding` is `chunked`, then message body must be sent
using `mg_send_http_chunk()` or `mg_printf_http_chunk()` functions.
Otherwise, `mg_send()` or `mg_printf()` must be used.
Extra headers could be set through `extra_headers` - and note `extra_headers`
must NOT be terminated by a new line. 

