#include "iobuf.h"
#include "log.h"

#include <string.h>

// Not using memset for zeroing memory, cause it can be dropped by compiler
// See https://github.com/cesanta/mongoose/pull/1265
static void zeromem(volatile unsigned char *buf, size_t len) {
  if (buf != NULL) {
    while (len--) *buf++ = 0;
  }
}

int mg_iobuf_resize(struct mg_iobuf *io, size_t new_size) {
  int ok = 1;
  if (new_size == 0) {
    zeromem(io->buf, io->size);
    free(io->buf);
    io->buf = NULL;
    io->len = io->size = 0;
  } else if (new_size != io->size) {
    // NOTE(lsm): do not use realloc here. Use calloc/free only, to ease the
    // porting to some obscure platforms like FreeRTOS
    void *p = calloc(1, new_size);
    if (p != NULL) {
      size_t len = new_size < io->len ? new_size : io->len;
      if (len > 0) memcpy(p, io->buf, len);
      zeromem(io->buf, io->size);
      free(io->buf);
      io->buf = (unsigned char *) p;
      io->size = new_size;
    } else {
      ok = 0;
      LOG(LL_ERROR,
          ("%lu->%lu", (unsigned long) io->size, (unsigned long) new_size));
    }
  }
  return ok;
}

int mg_iobuf_init(struct mg_iobuf *io, size_t size) {
  return mg_iobuf_resize(io, size);
}

size_t mg_iobuf_append(struct mg_iobuf *io, const void *buf, size_t len,
                       size_t chunk_size) {
  size_t new_size = io->len + len;
  if (new_size > io->size) {
    new_size += chunk_size;             // Make sure that io->size
    new_size -= new_size % chunk_size;  // is aligned by chunk_size boundary
    mg_iobuf_resize(io, new_size);      // Attempt to realloc
    if (new_size != io->size) len = 0;  // Realloc failure, append nothing
  }
  if (buf != NULL) memmove(io->buf + io->len, buf, len);
  io->len += len;
  return len;
}

size_t mg_iobuf_delete(struct mg_iobuf *io, size_t len) {
  if (len > io->len) len = 0;
  memmove(io->buf, io->buf + len, io->len - len);
  zeromem(io->buf + io->len - len, len);
  io->len -= len;
  return len;
}

void mg_iobuf_free(struct mg_iobuf *io) {
  mg_iobuf_resize(io, 0);
}
