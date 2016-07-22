---
title: "mg_enable_javascript()"
decl_name: "mg_enable_javascript"
symbol_kind: "func"
signature: |
  enum v7_err mg_enable_javascript(struct mg_mgr *m, struct v7 *v7,
                                   const char *init_js_file_name);
---

Enable server-side JavaScript scripting.
Requires `-DMG_ENABLE_JAVASCRIPT` compilation flag, and V7 engine sources.
v7 instance must not be destroyed during manager's lifetime.
Return V7 error. 

