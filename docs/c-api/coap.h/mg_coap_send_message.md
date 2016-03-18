---
title: "mg_coap_send_message()"
decl_name: "mg_coap_send_message"
symbol_kind: "func"
signature: |
  uint32_t mg_coap_send_message(struct mg_connection *nc,
                                struct mg_coap_message *cm);
---

Compose CoAP message from `mg_coap_message`
and send it into `nc` connection.
Return 0 on success. On error, it is a bitmask:

- `#define MG_COAP_ERROR 0x10000`
- `#define MG_COAP_FORMAT_ERROR (MG_COAP_ERROR | 0x20000)`
- `#define MG_COAP_IGNORE (MG_COAP_ERROR | 0x40000)`
- `#define MG_COAP_NOT_ENOUGH_DATA (MG_COAP_ERROR | 0x80000)`
- `#define MG_COAP_NETWORK_ERROR (MG_COAP_ERROR | 0x100000)` 

