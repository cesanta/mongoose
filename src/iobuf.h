#pragma once

#include <stddef.h>

struct mg_iobuf {
  unsigned char *buf;  // Pointer to stored data
  size_t size;         // Total size available
  size_t len;          // Current number of bytes
};

int mg_iobuf_init(struct mg_iobuf *, size_t);
int mg_iobuf_resize(struct mg_iobuf *, size_t);
void mg_iobuf_free(struct mg_iobuf *);
size_t mg_iobuf_add(struct mg_iobuf *, size_t, const void *, size_t, size_t);
size_t mg_iobuf_del(struct mg_iobuf *, size_t ofs, size_t len);
