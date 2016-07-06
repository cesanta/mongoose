---
title: "Core: TCP/UDP/SSL"
symbol_kind: "intro"
decl_name: "net.h"
items:
  - { type: file, name: mg_mgr_init.md }
  - { type: file, name: mg_mgr_free.md }
  - { type: file, name: mg_mgr_poll.md }
  - { type: file, name: mg_broadcast.md }
  - { type: file, name: mg_next.md }
  - { type: file, name: mg_add_sock.md }
  - { type: file, name: mg_add_sock_opt.md }
  - { type: file, name: mg_bind.md }
  - { type: file, name: mg_bind_opt.md }
  - { type: file, name: mg_connect.md }
  - { type: file, name: mg_connect_opt.md }
  - { type: file, name: mg_set_ssl.md }
  - { type: file, name: mg_send.md }
  - { type: file, name: mg_printf.md }
  - { type: file, name: mg_vprintf.md }
  - { type: file, name: mg_socketpair.md }
  - { type: file, name: mg_check_ip_acl.md }
  - { type: file, name: mg_enable_multithreading.md }
  - { type: file, name: mg_enable_javascript.md }
  - { type: file, name: mg_set_timer.md }
  - { type: file, name: mg_time.md }
  - { type: file, name: mg_event_handler_t.md }
  - { type: file, name: struct_mg_mgr.md }
  - { type: file, name: struct_mg_connection.md }
  - { type: file, name: struct_mg_add_sock_opts.md }
  - { type: file, name: struct_mg_bind_opts.md }
  - { type: file, name: struct_mg_connect_opts.md }
---

NOTE: Mongoose manager is single threaded. It does not protect
its data structures by mutexes, therefore all functions that are dealing
with particular event manager should be called from the same thread,
with exception of `mg_broadcast()` function. It is fine to have different
event managers handled by different threads.

