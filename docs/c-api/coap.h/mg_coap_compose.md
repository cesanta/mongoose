---
title: "mg_coap_compose()"
decl_name: "mg_coap_compose"
symbol_kind: "func"
signature: |
  uint32_t mg_coap_compose(struct mg_coap_message *cm, struct mbuf *io);
---

Composes CoAP message from mg_coap_message structure.
This is a helper function.
Return value: see `mg_coap_send_message()` 

