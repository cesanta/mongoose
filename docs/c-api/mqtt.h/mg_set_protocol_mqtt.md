---
title: "mg_set_protocol_mqtt()"
decl_name: "mg_set_protocol_mqtt"
symbol_kind: "func"
signature: |
  void mg_set_protocol_mqtt(struct mg_connection *nc);
---

Attach built-in MQTT event handler to the given connection.

The user-defined event handler will receive following extra events:

- MG_EV_MQTT_CONNACK
- MG_EV_MQTT_PUBLISH
- MG_EV_MQTT_PUBACK
- MG_EV_MQTT_PUBREC
- MG_EV_MQTT_PUBREL
- MG_EV_MQTT_PUBCOMP
- MG_EV_MQTT_SUBACK 

