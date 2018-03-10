---
title: "mg_mqtt_publish()"
decl_name: "mg_mqtt_publish"
symbol_kind: "func"
signature: |
  void mg_mqtt_publish(struct mg_connection *nc, const char *topic,
                       uint16_t message_id, int flags, const void *data,
                       size_t len);
---

Publishes a message to a given topic. 

