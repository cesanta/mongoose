---
title: "mg_conn_addr_to_str()"
decl_name: "mg_conn_addr_to_str"
symbol_kind: "func"
signature: |
  void mg_conn_addr_to_str(struct mg_connection *nc, char *buf, size_t len,
                           int flags);
---

Converts a connection's local or remote address into string.

The `flags` parameter is a bit mask that controls the behaviour,
see `MG_SOCK_STRINGIFY_*` definitions.

- MG_SOCK_STRINGIFY_IP - print IP address
- MG_SOCK_STRINGIFY_PORT - print port number
- MG_SOCK_STRINGIFY_REMOTE - print remote peer's IP/port, not local address

If both port number and IP address are printed, they are separated by `:`.
If compiled with `-DMG_ENABLE_IPV6`, IPv6 addresses are supported. 

