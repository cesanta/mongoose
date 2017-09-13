---
title: "mg_url_encode()"
decl_name: "mg_url_encode"
symbol_kind: "func"
signature: |
  struct mg_str mg_url_encode(const struct mg_str src);
---

URL-escape the specified string.
All non-printable characters are escaped, plus `._-$,;~()/`.
Input need not be NUL-terminated, but the returned string is.
Returned string is heap-allocated and must be free()'d. 

