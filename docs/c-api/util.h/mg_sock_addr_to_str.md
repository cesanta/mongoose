---
title: "mg_sock_addr_to_str()"
decl_name: "mg_sock_addr_to_str"
symbol_kind: "func"
signature: |
  void mg_sock_addr_to_str(const union socket_address *sa, char *buf, size_t len,
                           int flags);
---

Convert the socket's address into string.

`flags` is MG_SOCK_STRINGIFY_IP and/or MG_SOCK_STRINGIFY_PORT. 

