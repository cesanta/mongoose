---
title: "mg_mqtt_unsubscribe()"
decl_name: "mg_mqtt_unsubscribe"
symbol_kind: "func"
signature: |
  void mg_mqtt_unsubscribe(struct mg_connection *nc, char **topics,
                           size_t topics_len, uint16_t message_id);
---

Unsubscribe from a bunch of topics. 

