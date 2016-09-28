---
title: "mg_dns_encode_record()"
decl_name: "mg_dns_encode_record"
symbol_kind: "func"
signature: |
  int mg_dns_encode_record(struct mbuf *io, struct mg_dns_resource_record *rr,
                           const char *name, size_t nlen, const void *rdata,
                           size_t rlen);
---

Encodes and appends a DNS resource record to an IO buffer.

The record metadata is taken from the `rr` parameter, while the name and data
are taken from the parameters, encoded in the appropriate format depending on
record type and stored in the IO buffer. The encoded values might contain
offsets within the IO buffer. It's thus important that the IO buffer doesn't
get trimmed while a sequence of records are encoded while preparing a DNS
reply.

This function doesn't update the `name` and `rdata` pointers in the `rr`
struct because they might be invalidated as soon as the IO buffer grows
again.

Returns the number of bytes appened or -1 in case of error. 

