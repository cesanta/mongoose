---
title: "mg_socketpair()"
decl_name: "mg_socketpair"
symbol_kind: "func"
signature: |
  int mg_socketpair(sock_t[2], int sock_type);
---

Create a socket pair.
`sock_type` can be either `SOCK_STREAM` or `SOCK_DGRAM`.
Return 0 on failure, 1 on success. 

