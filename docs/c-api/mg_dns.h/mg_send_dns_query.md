---
title: "mg_send_dns_query()"
decl_name: "mg_send_dns_query"
symbol_kind: "func"
signature: |
  void mg_send_dns_query(struct mg_connection *nc, const char *name,
                         int query_type);
---

Sends a DNS query to the remote end. 

