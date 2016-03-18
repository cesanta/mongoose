---
title: "mg_vcasecmp()"
decl_name: "mg_vcasecmp"
symbol_kind: "func"
signature: |
  int mg_vcasecmp(const struct mg_str *str2, const char *str1);
---

Cross-platform version of `strncasecmp()` where first string is
specified by `struct mg_str`. 

