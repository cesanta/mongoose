---
title: "mg_match_prefix()"
decl_name: "mg_match_prefix"
symbol_kind: "func"
signature: |
  int mg_match_prefix(const char *pattern, int pattern_len, const char *str);
---

Matches 0-terminated string (mg_match_prefix) or string with given length
mg_match_prefix_n against a glob pattern.

Match is case-insensitive. Returns number of bytes matched, or -1 if no
match. 

