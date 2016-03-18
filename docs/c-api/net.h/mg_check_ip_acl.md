---
title: "mg_check_ip_acl()"
decl_name: "mg_check_ip_acl"
symbol_kind: "func"
signature: |
  int mg_check_ip_acl(const char *acl, uint32_t remote_ip);
---

Verify given IP address against the ACL.

`remote_ip` - an IPv4 address to check, in host byte order
`acl` - a comma separated list of IP subnets: `x.x.x.x/x` or `x.x.x.x`.
Each subnet is
prepended by either a - or a + sign. A plus sign means allow, where a
minus sign means deny. If a subnet mask is omitted, such as `-1.2.3.4`,
this means to deny only that single IP address.
Subnet masks may vary from 0 to 32, inclusive. The default setting
is to allow all accesses. On each request the full list is traversed,
and the last match wins. Example:

`-0.0.0.0/0,+192.168/16` - deny all acccesses, only allow 192.168/16 subnet

To learn more about subnet masks, see the
link:https://en.wikipedia.org/wiki/Subnetwork[Wikipedia page on Subnetwork]

Return -1 if ACL is malformed, 0 if address is disallowed, 1 if allowed. 

