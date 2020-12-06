#pragma once

#include <stddef.h>

struct mg_iobuf {
  unsigned char *buf;
  size_t size, len;
};

void mg_iobuf_init(struct mg_iobuf *, size_t);
void mg_iobuf_resize(struct mg_iobuf *, size_t);
void mg_iobuf_free(struct mg_iobuf *);
size_t mg_iobuf_append(struct mg_iobuf *, const void *, size_t, size_t);
size_t mg_iobuf_delete(struct mg_iobuf *, size_t);
