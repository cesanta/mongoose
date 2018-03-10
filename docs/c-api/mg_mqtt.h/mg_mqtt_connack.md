---
title: "mg_mqtt_connack()"
decl_name: "mg_mqtt_connack"
symbol_kind: "func"
signature: |
  void mg_mqtt_connack(struct mg_connection *nc, uint8_t return_code);
---

Sends a CONNACK command with a given `return_code`. 

