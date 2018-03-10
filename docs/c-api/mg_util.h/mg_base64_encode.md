---
title: "mg_base64_encode()"
decl_name: "mg_base64_encode"
symbol_kind: "func"
signature: |
  void mg_base64_encode(const unsigned char *src, int src_len, char *dst);
---

Base64-encode chunk of memory `src`, `src_len` into the destination `dst`.
Destination has to have enough space to hold encoded buffer.
Destination is '\0'-terminated. 

