---
title: "mg_coap_send_ack()"
decl_name: "mg_coap_send_ack"
symbol_kind: "func"
signature: |
  uint32_t mg_coap_send_ack(struct mg_connection *nc, uint16_t msg_id);
---

Composes CoAP acknowledgement from `mg_coap_message`
and sends it into `nc` connection.
Return value: see `mg_coap_send_message()` 

