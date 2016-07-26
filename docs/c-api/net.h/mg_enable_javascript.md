---
title: "mg_enable_javascript()"
decl_name: "mg_enable_javascript"
symbol_kind: "func"
signature: |
  enum v7_err mg_enable_javascript(struct mg_mgr *m, struct v7 *v7,
                                   const char *init_js_file_name);
---

Enables server-side JavaScript scripting.
Requires a `-DMG_ENABLE_JAVASCRIPT` compilation flag and V7 engine sources.
V7 instance must not be destroyed during manager's lifetime.
Returns a V7 error. 

