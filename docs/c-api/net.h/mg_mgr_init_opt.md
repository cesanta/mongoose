---
title: "mg_mgr_init_opt()"
decl_name: "mg_mgr_init_opt"
symbol_kind: "func"
signature: |
  void mg_mgr_init_opt(struct mg_mgr *mgr, void *user_data,
                       struct mg_mgr_init_opts opts);
---

Like `mg_mgr_init` but with more options.

Notably, this allows you to create a manger and choose
dynamically which networking interface implementation to use. 

