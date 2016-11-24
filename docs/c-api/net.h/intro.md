---
title: "Core API: TCP/UDP/SSL"
symbol_kind: "intro"
decl_name: "net.h"
items:
  - { name: mg_add_sock.md }
  - { name: mg_add_sock_opt.md }
  - { name: mg_bind.md }
  - { name: mg_bind_opt.md }
  - { name: mg_broadcast.md }
  - { name: mg_check_ip_acl.md }
  - { name: mg_connect.md }
  - { name: mg_connect_opt.md }
  - { name: mg_enable_javascript.md }
  - { name: mg_enable_multithreading.md }
  - { name: mg_mgr_free.md }
  - { name: mg_mgr_init.md }
  - { name: mg_mgr_init_opt.md }
  - { name: mg_mgr_poll.md }
  - { name: mg_next.md }
  - { name: mg_printf.md }
  - { name: mg_resolve.md }
  - { name: mg_send.md }
  - { name: mg_set_ssl.md }
  - { name: mg_set_timer.md }
  - { name: mg_socketpair.md }
  - { name: mg_time.md }
  - { name: mg_vprintf.md }
  - { name: mg_event_handler_t.md }
  - { name: struct_mg_add_sock_opts.md }
  - { name: struct_mg_bind_opts.md }
  - { name: struct_mg_connect_opts.md }
  - { name: struct_mg_connection.md }
  - { name: struct_mg_mgr.md }
  - { name: struct_mg_mgr_init_opts.md }
  - { name: struct_mg_multithreading_opts.md }
---

NOTE: Mongoose manager is single threaded. It does not protect
its data structures by mutexes, therefore all functions that are dealing
with a particular event manager should be called from the same thread,
with exception of the `mg_broadcast()` function. It is fine to have different
event managers handled by different threads.

