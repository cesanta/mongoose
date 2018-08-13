---
title: "mg_url_encode()"
decl_name: "mg_url_encode"
symbol_kind: "func"
signature: |
  struct mg_str mg_url_encode(const struct mg_str src);
---

Same as `mg_url_encode_opt(src, "._-$,;~()/", 0)`. 

