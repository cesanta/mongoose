---
title: "mg_mqtt_match_topic_expression()"
decl_name: "mg_mqtt_match_topic_expression"
symbol_kind: "func"
signature: |
  int mg_mqtt_match_topic_expression(struct mg_str exp, struct mg_str topic);
---

Matches a topic against a topic expression

Returns 1 if it matches; 0 otherwise. 

