---
title: "mg_coap_free_options()"
decl_name: "mg_coap_free_options"
symbol_kind: "func"
signature: |
  void mg_coap_free_options(struct mg_coap_message *cm);
---

Free the memory allocated for options,
if cm paramater doesn't contain any option does nothing. 

