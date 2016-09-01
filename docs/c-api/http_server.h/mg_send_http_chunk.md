---
title: "mg_send_http_chunk()"
decl_name: "mg_send_http_chunk"
symbol_kind: "func"
signature: |
  void mg_send_http_chunk(struct mg_connection *nc, const char *buf, size_t len);
---

Sends buffer `buf` of size `len` to the client using chunked HTTP encoding.
This function sends the buffer size as hex number + newline first, then
the buffer itself, then the newline. For example,
`mg_send_http_chunk(nc, "foo", 3)` whill append the `3\r\nfoo\r\n` string
to the `nc->send_mbuf` output IO buffer.

NOTE: The HTTP header "Transfer-Encoding: chunked" should be sent prior to
using this function.

NOTE: do not forget to send an empty chunk at the end of the response,
to tell the client that everything was sent. Example:

```
  mg_printf_http_chunk(nc, "%s", "my response!");
  mg_send_http_chunk(nc, "", 0); // Tell the client we're finished
``` 

