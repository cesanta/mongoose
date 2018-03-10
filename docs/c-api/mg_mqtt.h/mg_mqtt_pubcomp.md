---
title: "mg_mqtt_pubcomp()"
decl_name: "mg_mqtt_pubcomp"
symbol_kind: "func"
signature: |
  void mg_mqtt_pubcomp(struct mg_connection *nc, uint16_t message_id);
---

Sends a PUBCOMP command with a given `message_id`. 

