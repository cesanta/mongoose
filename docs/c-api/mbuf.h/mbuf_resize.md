---
title: "mbuf_resize()"
decl_name: "mbuf_resize"
symbol_kind: "func"
signature: |
  void mbuf_resize(struct mbuf *, size_t new_size);
---

Resize an Mbuf.

If `new_size` is smaller than buffer's `len`, the
resize is not performed. 

