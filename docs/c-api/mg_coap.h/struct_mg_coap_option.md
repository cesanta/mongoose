---
title: "struct mg_coap_option"
decl_name: "struct mg_coap_option"
symbol_kind: "struct"
signature: |
  struct mg_coap_option {
    struct mg_coap_option *next;
    uint32_t number;
    struct mg_str value;
  };
---

CoAP options.
Use mg_coap_add_option and mg_coap_free_options
for creation and destruction. 

