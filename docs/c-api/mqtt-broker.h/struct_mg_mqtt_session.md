---
title: "struct mg_mqtt_session"
decl_name: "struct mg_mqtt_session"
symbol_kind: "struct"
signature: |
  struct mg_mqtt_session {
    struct mg_mqtt_broker *brk;          /* Broker */
    struct mg_mqtt_session *next, *prev; /* mg_mqtt_broker::sessions linkage */
    struct mg_connection *nc;            /* Connection with the client */
    size_t num_subscriptions;            /* Size of `subscriptions` array */
    struct mg_mqtt_topic_expression *subscriptions;
    void *user_data; /* User data */
  };
---

MQTT session (Broker side). 

