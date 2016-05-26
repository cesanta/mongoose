---
title: "CoAP"
symbol_kind: "intro"
decl_name: "coap.h"
items:
  - { type: file, name: mg_set_protocol_coap.md }
  - { type: file, name: mg_coap_add_option.md }
  - { type: file, name: mg_coap_free_options.md }
  - { type: file, name: mg_coap_send_message.md }
  - { type: file, name: mg_coap_send_ack.md }
  - { type: file, name: mg_coap_parse.md }
  - { type: file, name: mg_coap_compose.md }
  - { type: file, name: struct_mg_coap_option.md }
  - { type: file, name: struct_mg_coap_message.md }
---

CoAP message format:

```
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
|Ver| T | TKL | Code | Message ID | Token (if any, TKL bytes) ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
| Options (if any) ...            |1 1 1 1 1 1 1 1| Payload (if any) ...
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
```

