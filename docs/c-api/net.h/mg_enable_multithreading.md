---
title: "mg_enable_multithreading()"
decl_name: "mg_enable_multithreading"
symbol_kind: "func"
signature: |
  void mg_enable_multithreading(struct mg_connection *nc);
---

Enable multi-threaded handling for the given listening connection `nc`.
For each accepted connection, Mongoose will create a separate thread
and run event handler in that thread. Thus, if an event hanler is doing
a blocking call or some long computation, that will not slow down
other connections. 

