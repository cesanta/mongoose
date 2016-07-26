---
title: "mg_dns_reply_record()"
decl_name: "mg_dns_reply_record"
symbol_kind: "func"
signature: |
  int mg_dns_reply_record(struct mg_dns_reply *reply,
                          struct mg_dns_resource_record *question,
                          const char *name, int rtype, int ttl, const void *rdata,
                          size_t rdata_len);
---

Appends a DNS reply record to the IO buffer and to the DNS message.

The message's num_answers field will be incremented. It's the caller's duty
to ensure num_answers is properly initialised.

Returns -1 on error. 

