---
title: "mg_dns_parse_record_data()"
decl_name: "mg_dns_parse_record_data"
symbol_kind: "func"
signature: |
  int mg_dns_parse_record_data(struct mg_dns_message *msg,
                               struct mg_dns_resource_record *rr, void *data,
                               size_t data_len);
---

Parse the record data from a DNS resource record.

 - A:     struct in_addr *ina
 - AAAA:  struct in6_addr *ina
 - CNAME: char buffer

Returns -1 on error.

TODO(mkm): MX 

