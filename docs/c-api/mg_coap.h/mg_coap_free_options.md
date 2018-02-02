---
title: "mg_coap_free_options()"
decl_name: "mg_coap_free_options"
symbol_kind: "func"
signature: |
  void mg_coap_free_options(struct mg_coap_message *cm);
---

Frees the memory allocated for options.
If the cm parameter doesn't contain any option it does nothing. 

