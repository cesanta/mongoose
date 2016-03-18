---
title: "mg_mqtt_broker()"
decl_name: "mg_mqtt_broker"
symbol_kind: "func"
signature: |
  void mg_mqtt_broker(struct mg_connection *brk, int ev, void *data);
---

Process a MQTT broker message.

Listening connection expects a pointer to an initialized `mg_mqtt_broker`
structure in the `user_data` field.

Basic usage:

```c
mg_mqtt_broker_init(&brk, NULL);

if ((nc = mg_bind(&mgr, address, mg_mqtt_broker)) == NULL) {
  // fail;
}
nc->user_data = &brk;
```

New incoming connections will receive a `mg_mqtt_session` structure
in the connection `user_data`. The original `user_data` will be stored
in the `user_data` field of the session structure. This allows the user
handler to store user data before `mg_mqtt_broker` creates the session.

Since only the MG_EV_ACCEPT message is processed by the listening socket,
for most events the `user_data` will thus point to a `mg_mqtt_session`. 

