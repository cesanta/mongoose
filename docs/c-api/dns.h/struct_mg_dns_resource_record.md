---
title: "struct mg_dns_resource_record"
decl_name: "struct mg_dns_resource_record"
symbol_kind: "struct"
signature: |
  struct mg_dns_resource_record {
    struct mg_str name; /* buffer with compressed name */
    int rtype;
    int rclass;
    int ttl;
    enum mg_dns_resource_record_kind kind;
    struct mg_str rdata; /* protocol data (can be a compressed name) */
  };
---

DNS resource record. 

