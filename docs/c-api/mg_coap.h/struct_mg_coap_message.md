---
title: "struct mg_coap_message"
decl_name: "struct mg_coap_message"
symbol_kind: "struct"
signature: |
  struct mg_coap_message {
    uint32_t flags;
    uint8_t msg_type;
    uint8_t code_class;
    uint8_t code_detail;
    uint16_t msg_id;
    struct mg_str token;
    struct mg_coap_option *options;
    struct mg_str payload;
    struct mg_coap_option *optiomg_tail;
  };
---

CoAP message. See RFC 7252 for details. 

