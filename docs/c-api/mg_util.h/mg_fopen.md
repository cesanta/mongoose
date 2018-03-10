---
title: "mg_fopen()"
decl_name: "mg_fopen"
symbol_kind: "func"
signature: |
  FILE *mg_fopen(const char *path, const char *mode);
---

Opens the given file and returns a file stream.

`path` and `mode` should be UTF8 encoded.

Return value is the same as for the `fopen()` call. 

