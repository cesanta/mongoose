#pragma once

#include "arch.h"

// Dynamically-sized I/O buffer. len <= size always holds.
struct mg_iobuf {
  unsigned char *buf;  // Heap-allocated data buffer
  size_t size;         // Allocated capacity (rounded up to align)
  size_t len;          // Number of bytes currently stored
  size_t align;        // Allocation granularity; capacity is always a multiple of this
};

// Initializes io to empty, then allocates size bytes with the given alignment.
// Returns false on allocation failure.
bool mg_iobuf_init(struct mg_iobuf *io, size_t size, size_t align);

// Resizes the buffer to new_size (rounded up to io->align). new_size=0 frees
// the buffer. Uses mg_calloc+mg_free (not realloc) so old memory is zeroed on release.
// Returns false on allocation failure.
bool mg_iobuf_resize(struct mg_iobuf *io, size_t new_size);

// Frees the buffer and zeroes the struct. Equivalent to mg_iobuf_resize(io, 0).
void mg_iobuf_free(struct mg_iobuf *io);

// Inserts len bytes from buf at offset ofs, shifting existing data right.
// Pass buf=NULL to reserve space without writing. Returns bytes inserted, 0 on OOM.
size_t mg_iobuf_add(struct mg_iobuf *io, size_t ofs, const void *buf, size_t len);

// Removes len bytes at ofs, shifting remaining data left. Clamps to available data.
// Returns bytes actually removed.
size_t mg_iobuf_del(struct mg_iobuf *io, size_t ofs, size_t len);
