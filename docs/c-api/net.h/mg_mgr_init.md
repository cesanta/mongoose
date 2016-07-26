---
title: "mg_mgr_init()"
decl_name: "mg_mgr_init"
symbol_kind: "func"
signature: |
  void mg_mgr_init(struct mg_mgr *mgr, void *user_data);
---

Initialise Mongoose manager. Side effect: ignores SIGPIPE signal.
`mgr->user_data` field will be initialised with a `user_data` parameter.
That is an arbitrary pointer, where the user code can associate some data
with the particular Mongoose manager. For example, a C++ wrapper class
could be written in which case `user_data` can hold a pointer to the
class instance. 

