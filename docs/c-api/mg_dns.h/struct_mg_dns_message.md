---
title: "struct mg_dns_message"
decl_name: "struct mg_dns_message"
symbol_kind: "struct"
signature: |
  struct mg_dns_message {
    struct mg_str pkt; /* packet body */
    uint16_t flags;
    uint16_t transaction_id;
    int num_questions;
    int num_answers;
    struct mg_dns_resource_record questions[MG_MAX_DNS_QUESTIONS];
    struct mg_dns_resource_record answers[MG_MAX_DNS_ANSWERS];
  };
---

DNS message (request and response). 

