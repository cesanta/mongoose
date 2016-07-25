---
title: "mg_dns_uncompress_name()"
decl_name: "mg_dns_uncompress_name"
symbol_kind: "func"
signature: |
  size_t mg_dns_uncompress_name(struct mg_dns_message *msg, struct mg_str *name,
                                char *dst, int dst_len);
---

Uncompress a DNS compressed name.

The containing dns message is required because the compressed encoding
and reference suffixes present elsewhere in the packet.

If name is less than `dst_len` characters long, the remainder
of `dst` is terminated with `\0' characters. Otherwise, `dst` is not
*terminated.

If `dst_len` is 0 `dst` can be NULL.
Return the uncompressed name length. 

