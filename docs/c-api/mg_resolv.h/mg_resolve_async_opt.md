---
title: "mg_resolve_async_opt()"
decl_name: "mg_resolve_async_opt"
symbol_kind: "func"
signature: |
  int mg_resolve_async_opt(struct mg_mgr *mgr, const char *name, int query,
                           mg_resolve_callback_t cb, void *data,
                           struct mg_resolve_async_opts opts);
---

Resolved a DNS name asynchronously.

Upon successful resolution, the user callback will be invoked
with the full DNS response message and a pointer to the user's
context `data`.

In case of timeout while performing the resolution the callback
will receive a NULL `msg`.

The DNS answers can be extracted with `mg_next_record` and
`mg_dns_parse_record_data`:

[source,c]
----
struct in_addr ina;
struct mg_dns_resource_record *rr = mg_next_record(msg, MG_DNS_A_RECORD,
  NULL);
mg_dns_parse_record_data(msg, rr, &ina, sizeof(ina));
---- 

