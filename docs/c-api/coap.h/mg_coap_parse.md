---
title: "mg_coap_parse()"
decl_name: "mg_coap_parse"
symbol_kind: "func"
signature: |
  uint32_t mg_coap_parse(struct mbuf *io, struct mg_coap_message *cm);
---

Parse COAP message and fills mg_coap_message and returns cm->flags.
This is a helper function.

NOTE: usually CoAP work over UDP, so lack of data means format error,
but in theory it is possible to use CoAP over TCP (according to RFC)

The caller have to check results and treat COAP_NOT_ENOUGH_DATA according to
underlying protocol:

- in case of UDP COAP_NOT_ENOUGH_DATA means COAP_FORMAT_ERROR,
- in case of TCP client can try to receive more data

Return value: see `mg_coap_send_message()` 

