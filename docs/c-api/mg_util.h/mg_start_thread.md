---
title: "mg_start_thread()"
decl_name: "mg_start_thread"
symbol_kind: "func"
signature: |
  void *mg_start_thread(void *(*thread_func);
---

Starts a new detached thread.
Arguments and semantics are the same as pthead's `pthread_create()`.
`thread_func` is a thread function, `thread_func_param` is a parameter
that is passed to the thread function. 

