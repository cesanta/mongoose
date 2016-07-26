---
title: "mg_next_comma_list_entry()"
decl_name: "mg_next_comma_list_entry"
symbol_kind: "func"
signature: |
  const char *mg_next_comma_list_entry(const char *list, struct mg_str *val,
                                       struct mg_str *eq_val);
---

A helper function for traversing a comma separated list of values.
It returns a list pointer shifted to the next value or NULL if the end
of the list found.
The value is stored in a val vector. If the value has a form "x=y", then
eq_val vector is initialised to point to the "y" part, and val vector length
is adjusted to point only to "x".
If the list is just a comma separated list of entries, like "aa,bb,cc" then
`eq_val` will contain zero-length string.

The purpose of this function is to parse comma separated string without
any copying/memory allocation. 

