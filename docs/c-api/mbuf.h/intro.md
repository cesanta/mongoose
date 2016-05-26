---
title: "Memory Buffers"
symbol_kind: "intro"
decl_name: "mbuf.h"
items:
  - { type: file, name: mbuf_init.md }
  - { type: file, name: mbuf_free.md }
  - { type: file, name: mbuf_append.md }
  - { type: file, name: mbuf_insert.md }
  - { type: file, name: mbuf_remove.md }
  - { type: file, name: mbuf_resize.md }
  - { type: file, name: mbuf_trim.md }
  - { type: file, name: struct_mbuf.md }
---

Mbufs are mutable/growing memory buffers, like C++ strings.
Mbuf can append data to the end of a buffer, or insert data into arbitrary
position in the middle of a buffer. The buffer grows automatically when
needed.

