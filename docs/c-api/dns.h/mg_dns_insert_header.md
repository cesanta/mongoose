---
title: "mg_dns_insert_header()"
decl_name: "mg_dns_insert_header"
symbol_kind: "func"
signature: |
  int mg_dns_insert_header(struct mbuf *io, size_t pos,
                           struct mg_dns_message *msg);
---

Insert a DNS header to an IO buffer.

Return number of bytes inserted. 

