---
title: "Memory Buffers"
symbol_kind: "intro"
decl_name: "mbuf.h"
items:
  - { name: mbuf_append.md }
  - { name: mbuf_free.md }
  - { name: mbuf_init.md }
  - { name: mbuf_insert.md }
  - { name: mbuf_remove.md }
  - { name: mbuf_resize.md }
  - { name: mbuf_trim.md }
  - { name: struct_mbuf.md }
---

Mbufs are mutable/growing memory buffers, like C++ strings.
Mbuf can append data to the end of a buffer or insert data into arbitrary
position in the middle of a buffer. The buffer grows automatically when
needed.

