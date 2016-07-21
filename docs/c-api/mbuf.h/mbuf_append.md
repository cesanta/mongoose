---
title: "mbuf_append()"
decl_name: "mbuf_append"
symbol_kind: "func"
signature: |
  size_t mbuf_append(struct mbuf *, const void *data, size_t data_size);
---

Appends data to the Mbuf.

Returns the number of bytes appended or 0 if out of memory. 

