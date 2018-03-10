---
title: "mg_resolve_from_hosts_file()"
decl_name: "mg_resolve_from_hosts_file"
symbol_kind: "func"
signature: |
  int mg_resolve_from_hosts_file(const char *host, union socket_address *usa);
---

Resolve a name from `/etc/hosts`.

Returns 0 on success, -1 on failure. 

