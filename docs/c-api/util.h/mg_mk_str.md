---
title: "mg_mk_str()"
decl_name: "mg_mk_str"
symbol_kind: "func"
signature: |
  struct mg_str mg_mk_str(const char *s);
---

A helper function for creating mg_str struct from plain C string.
`NULL` is allowed and becomes `{NULL, 0}`. 

