---
title: "mg_resolve()"
decl_name: "mg_resolve"
symbol_kind: "func"
signature: |
  int mg_resolve(const char *domain_name, char *ip_addr_buf, size_t buf_len);
---

Convert domain name into IP address.

This is a utility function. If compilation flags have
`-DMG_ENABLE_GETADDRINFO`, then `getaddrinfo()` call is used for name
resolution. Otherwise, `gethostbyname()` is used.

CAUTION: this function can block.
Return 1 on success, 0 on failure. 

