---
title: "mg_mqtt_subscribe()"
decl_name: "mg_mqtt_subscribe"
symbol_kind: "func"
signature: |
  void mg_mqtt_subscribe(struct mg_connection *nc,
                         const struct mg_mqtt_topic_expression *topics,
                         size_t topics_len, uint16_t message_id);
---

Subscribe to a bunch of topics. 

