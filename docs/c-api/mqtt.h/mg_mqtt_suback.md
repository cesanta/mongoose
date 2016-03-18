---
title: "mg_mqtt_suback()"
decl_name: "mg_mqtt_suback"
symbol_kind: "func"
signature: |
  void mg_mqtt_suback(struct mg_connection *nc, uint8_t *qoss, size_t qoss_len,
                      uint16_t message_id);
---

Send a SUBACK command with a given `message_id`
and a sequence of granted QoSs. 

