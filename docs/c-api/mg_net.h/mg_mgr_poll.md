---
title: "mg_mgr_poll()"
decl_name: "mg_mgr_poll"
symbol_kind: "func"
signature: |
  int mg_mgr_poll(struct mg_mgr *mgr, int milli);
---

This function performs the actual IO and must be called in a loop
(an event loop). It returns number of user events generated (except POLLs).
`milli` is the maximum number of milliseconds to sleep.
`mg_mgr_poll()` checks all connections for IO readiness. If at least one
of the connections is IO-ready, `mg_mgr_poll()` triggers the respective
event handlers and returns. 

