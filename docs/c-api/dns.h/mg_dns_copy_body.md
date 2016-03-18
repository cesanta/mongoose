---
title: "mg_dns_copy_body()"
decl_name: "mg_dns_copy_body"
symbol_kind: "func"
signature: |
  int mg_dns_copy_body(struct mbuf *io, struct mg_dns_message *msg);
---

Append already encoded body from an existing message.

This is useful when generating a DNS reply message which includes
all question records.

Return number of appened bytes. 

