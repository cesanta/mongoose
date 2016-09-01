---
title: "CoAP API reference"
symbol_kind: "intro"
decl_name: "coap.h"
items:
  - { name: mg_set_protocol_coap.md }
  - { name: mg_coap_add_option.md }
  - { name: mg_coap_free_options.md }
  - { name: mg_coap_send_message.md }
  - { name: mg_coap_send_ack.md }
  - { name: mg_coap_parse.md }
  - { name: mg_coap_compose.md }
  - { name: struct_mg_coap_option.md }
  - { name: struct_mg_coap_message.md }
---

CoAP message format:

```
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|Ver| T | TKL | Code | Message ID | Token (if any, TKL bytes) ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
| Options (if any) ...            |1 1 1 1 1 1 1 1| Payload (if any) ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
```

