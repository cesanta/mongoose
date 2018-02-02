---
title: "mg_mqtt_next_subscribe_topic()"
decl_name: "mg_mqtt_next_subscribe_topic"
symbol_kind: "func"
signature: |
  int mg_mqtt_next_subscribe_topic(struct mg_mqtt_message *msg,
                                   struct mg_str *topic, uint8_t *qos, int pos);
---

Extracts the next topic expression from a SUBSCRIBE command payload.

The topic expression name will point to a string in the payload buffer.
Returns the pos of the next topic expression or -1 when the list
of topics is exhausted. 

