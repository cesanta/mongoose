---
title: "struct mbuf"
decl_name: "struct mbuf"
symbol_kind: "struct"
signature: |
  struct mbuf {
    char *buf;   /* Buffer pointer */
    size_t len;  /* Data length. Data is located between offset 0 and len. */
    size_t size; /* Buffer size allocated by realloc(1). Must be >= len */
  };
---

Memory buffer descriptor 

