---
title: "mg_base64_decode()"
decl_name: "mg_base64_decode"
symbol_kind: "func"
signature: |
  int mg_base64_decode(const unsigned char *s, int len, char *dst);
---

Decodes base64-encoded string `s`, `len` into the destination `dst`.
The destination has to have enough space to hold the decoded buffer.
Decoding stops either when all strings have been decoded or invalid an
character appeared.
Destination is '\0'-terminated.
Returns the number of decoded characters. On success, that should be equal
to `len`. On error (invalid character) the return value is smaller then
`len`. 

