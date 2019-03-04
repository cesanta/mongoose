---
title: "mbuf_append_and_free()"
decl_name: "mbuf_append_and_free"
symbol_kind: "func"
signature: |
  size_t mbuf_append_and_free(struct mbuf *, void *data, size_t data_size);
---

Appends data to the Mbuf and frees it (data must be heap-allocated).

Returns the number of bytes appended or 0 if out of memory.
data is freed irrespective of return value. 

