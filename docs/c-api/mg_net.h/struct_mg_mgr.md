---
title: "struct mg_mgr"
decl_name: "struct mg_mgr"
symbol_kind: "struct"
signature: |
  struct mg_mgr {
    struct mg_connection *active_connections;
  #if MG_ENABLE_HEXDUMP
    const char *hexdump_file; /* Debug hexdump file path */
  #endif
  #if MG_ENABLE_BROADCAST
    sock_t ctl[2]; /* Socketpair for mg_broadcast() */
  #endif
    void *user_data; /* User data */
    int num_ifaces;
    int num_calls;
    struct mg_iface **ifaces; /* network interfaces */
    const char *nameserver;   /* DNS server to use */
  };
---

Mongoose event manager. 

