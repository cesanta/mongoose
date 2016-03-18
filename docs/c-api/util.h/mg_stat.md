---
title: "mg_stat()"
decl_name: "mg_stat"
symbol_kind: "func"
signature: |
  int mg_stat(const char *path, cs_stat_t *st);
---

Perform a 64-bit `stat()` call against given file.

`path` should be UTF8 encoded.

Return value is the same as for `stat()` syscall. 

