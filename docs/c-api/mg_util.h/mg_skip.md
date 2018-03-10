---
title: "mg_skip()"
decl_name: "mg_skip"
symbol_kind: "func"
signature: |
  const char *mg_skip(const char *s, const char *end_string,
                      const char *delimiters, struct mg_str *v);
---

Fetches substring from input string `s`, `end` into `v`.
Skips initial delimiter characters. Records first non-delimiter character
at the beginning of substring `v`. Then scans the rest of the string
until a delimiter character or end-of-string is found.
`delimiters` is a 0-terminated string containing delimiter characters.
Either one of `delimiters` or `end_string` terminates the search.
Returns an `s` pointer, advanced forward where parsing has stopped. 

