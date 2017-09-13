---
title: "mg_mqtt_vmatch_topic_expression()"
decl_name: "mg_mqtt_vmatch_topic_expression"
symbol_kind: "func"
signature: |
  int mg_mqtt_vmatch_topic_expression(const char *exp, struct mg_str topic);
---

Same as `mg_mqtt_match_topic_expression()`, but takes `exp` as a
NULL-terminated string. 

