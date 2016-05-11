---
title: "mg_mqtt_next()"
decl_name: "mg_mqtt_next"
symbol_kind: "func"
signature: |
  struct mg_mqtt_session *mg_mqtt_next(struct mg_mqtt_broker *brk,
                                       struct mg_mqtt_session *s);
---

Iterate over all mqtt sessions connections. Example:

```c
struct mg_mqtt_session *s;
for (s = mg_mqtt_next(brk, NULL); s != NULL; s = mg_mqtt_next(brk, s)) {
  // Do something
}
``` 

