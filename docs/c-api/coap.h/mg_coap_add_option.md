---
title: "mg_coap_add_option()"
decl_name: "mg_coap_add_option"
symbol_kind: "func"
signature: |
  struct mg_coap_option *mg_coap_add_option(struct mg_coap_message *cm,
                                            uint32_t number, char *value,
                                            size_t len);
---

Adds a new option to mg_coap_message structure.
Returns pointer to the newly created option.
Note: options must be freed by using mg_coap_free_options 

