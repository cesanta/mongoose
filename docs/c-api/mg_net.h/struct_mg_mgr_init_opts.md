---
title: "struct mg_mgr_init_opts"
decl_name: "struct mg_mgr_init_opts"
symbol_kind: "struct"
signature: |
  struct mg_mgr_init_opts {
    const struct mg_iface_vtable *main_iface;
    int num_ifaces;
    const struct mg_iface_vtable **ifaces;
    const char *nameserver;
  };
---

Optional parameters to `mg_mgr_init_opt()`.

If `main_iface` is not NULL, it will be used as the main interface in the
default interface set. The pointer will be free'd by `mg_mgr_free`.
Otherwise, the main interface will be autodetected based on the current
platform.

If `num_ifaces` is 0 and `ifaces` is NULL, the default interface set will be
used.
This is an advanced option, as it requires you to construct a full interface
set, including special networking interfaces required by some optional
features such as TCP tunneling. Memory backing `ifaces` and each of the
`num_ifaces` pointers it contains will be reclaimed by `mg_mgr_free`. 

