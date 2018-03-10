---
title: "mg_dns_uncompress_name()"
decl_name: "mg_dns_uncompress_name"
symbol_kind: "func"
signature: |
  size_t mg_dns_uncompress_name(struct mg_dns_message *msg, struct mg_str *name,
                                char *dst, int dst_len);
---

Uncompresses a DNS compressed name.

The containing DNS message is required because of the compressed encoding
and reference suffixes present elsewhere in the packet.

If the name is less than `dst_len` characters long, the remainder
of `dst` is terminated with `\0` characters. Otherwise, `dst` is not
terminated.

If `dst_len` is 0 `dst` can be NULL.
Returns the uncompressed name length. 

