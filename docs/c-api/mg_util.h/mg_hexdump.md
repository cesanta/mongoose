---
title: "mg_hexdump()"
decl_name: "mg_hexdump"
symbol_kind: "func"
signature: |
  int mg_hexdump(const void *buf, int len, char *dst, int dst_len);
---

Generates a human-readable hexdump of memory chunk.

Takes a memory buffer `buf` of length `len` and creates a hex dump of that
buffer in `dst`. The generated output is a-la hexdump(1).
Returns the length of generated string, excluding terminating `\0`. If
returned length is bigger than `dst_len`, the overflow bytes are discarded. 

