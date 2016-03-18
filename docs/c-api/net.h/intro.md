---
title: "Core: TCP/UDP/SSL"
symbol_kind: "intro"
decl_name: "net.h"
---

NOTE: Mongoose manager is single threaded. It does not protect
its data structures by mutexes, therefore all functions that are dealing
with particular event manager should be called from the same thread,
with exception of `mg_broadcast()` function. It is fine to have different
event managers handled by different threads.

