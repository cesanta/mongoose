---
title: "mbuf_insert()"
decl_name: "mbuf_insert"
symbol_kind: "func"
signature: |
  size_t mbuf_insert(struct mbuf *, size_t, const void *, size_t);
---

Inserts data at a specified offset in the Mbuf.

Existing data will be shifted forwards and the buffer will
be grown if necessary.
Returns the number of bytes inserted. 

