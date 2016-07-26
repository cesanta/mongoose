---
title: "mg_hexdump_connection()"
decl_name: "mg_hexdump_connection"
symbol_kind: "func"
signature: |
  void mg_hexdump_connection(struct mg_connection *nc, const char *path,
                             const void *buf, int num_bytes, int ev);
---

Generates human-readable hexdump of the data sent or received by the
connection. `path` is a file name where hexdump should be written.
`num_bytes` is a number of bytes sent/received. `ev` is one of the `MG_*`
events sent to an event handler. This function is supposed to be called from
the event handler. 

