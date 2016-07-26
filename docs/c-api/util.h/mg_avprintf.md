---
title: "mg_avprintf()"
decl_name: "mg_avprintf"
symbol_kind: "func"
signature: |
  int mg_avprintf(char **buf, size_t size, const char *fmt, va_list ap);
---

Prints message to the buffer. If the buffer is large enough to hold the
message, it returns buffer. If buffer is to small, it allocates a large
enough buffer on heap and returns allocated buffer.
This is a supposed use case:

   char buf[5], *p = buf;
   p = mg_avprintf(&p, sizeof(buf), "%s", "hi there");
   use_p_somehow(p);
   if (p != buf) {
     free(p);
   }

The purpose of this is to avoid malloc-ing if generated strings are small. 

