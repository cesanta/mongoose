---
title: "mg_mqtt_pubrel()"
decl_name: "mg_mqtt_pubrel"
symbol_kind: "func"
signature: |
  void mg_mqtt_pubrel(struct mg_connection *nc, uint16_t message_id);
---

Sends a PUBREL command with a given `message_id`. 

