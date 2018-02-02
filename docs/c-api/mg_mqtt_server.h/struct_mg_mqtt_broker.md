---
title: "struct mg_mqtt_broker"
decl_name: "struct mg_mqtt_broker"
symbol_kind: "struct"
signature: |
  struct mg_mqtt_broker {
    LIST_HEAD(_mg_sesshead, mg_mqtt_session) sessions; /* Session list */
    void *user_data;                                   /* User data */
  };
---

MQTT broker. 

