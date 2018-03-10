---
title: "mg_mbuf_append_base64()"
decl_name: "mg_mbuf_append_base64"
symbol_kind: "func"
signature: |
  void mg_mbuf_append_base64(struct mbuf *mbuf, const void *data, size_t len);
---

Encode `len` bytes starting at `data` as base64 and append them to an mbuf. 

