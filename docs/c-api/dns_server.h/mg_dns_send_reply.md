---
title: "mg_dns_send_reply()"
decl_name: "mg_dns_send_reply"
symbol_kind: "func"
signature: |
  void mg_dns_send_reply(struct mg_connection *nc, struct mg_dns_reply *r);
---

Sends a DNS reply through a connection.

The DNS data is stored in an IO buffer pointed by reply structure in `r`.
This function mutates the content of that buffer in order to ensure that
the DNS header reflects the size and flags of the message, that might have
been updated either with `mg_dns_reply_record` or by direct manipulation of
`r->message`.

Once sent, the IO buffer will be trimmed unless the reply IO buffer
is the connection's send buffer and the connection is not in UDP mode. 

