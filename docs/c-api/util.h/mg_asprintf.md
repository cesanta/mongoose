---
title: "mg_asprintf()"
decl_name: "mg_asprintf"
symbol_kind: "func"
signature: |
  int mg_asprintf(char **buf, size_t size, const char *fmt, ...);
---

Prints message to the buffer. If the buffer is large enough to hold the
message, it returns buffer. If buffer is to small, it allocates a large
enough buffer on heap and returns allocated buffer.
This is a supposed use case:

   char buf[5], *p = buf;
   mg_avprintf(&p, sizeof(buf), "%s", "hi there");
   use_p_somehow(p);
   if (p != buf) {
     free(p);
   }

The purpose of this is to avoid malloc-ing if generated strings are small. 

