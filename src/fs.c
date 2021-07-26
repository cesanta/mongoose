#include "fs.h"

struct packed_file {
  const char *data;
  size_t size;
  size_t pos;
};

const char *mg_unpack(const char *, size_t *) WEAK;
const char *mg_unpack(const char *path, size_t *size) {
  (void) path, (void) size;
  return NULL;
}

#if defined(MG_FOPENCOOKIE)
ssize_t packed_read(void *cookie, char *buf, size_t size) {
  struct packed_file *fp = (struct packed_file *) cookie;
  if (size > fp->size - fp->pos) size = fp->size - fp->pos;
  memcpy(buf, &fp->data[fp->pos], size);
  fp->pos += size;
  return (ssize_t) size;
}

ssize_t packed_write(void *cookie, const char *buf, size_t size) {
  (void) cookie, (void) buf, (void) size;
  return -1;
}

int packed_seek(void *cookie, off64_t *offset, int whence) {
  struct packed_file *fp = (struct packed_file *) cookie;
  if (whence == SEEK_SET) fp->pos = (size_t) *offset;
  if (whence == SEEK_END) fp->pos = (size_t)((off64_t) fp->size + *offset);
  if (whence == SEEK_CUR) fp->pos = (size_t)((off64_t) fp->pos + *offset);
  if (fp->pos > fp->size) fp->pos = fp->size;
  *offset = (off64_t) fp->pos;
  return 0;
}

int packed_close(void *cookie) {
  free(cookie);
  return 0;
}

FILE *mg_fopen_packed(const char *path, const char *mode) {
  cookie_io_functions_t funcs = {
      .read = packed_read,
      .write = packed_write,
      .seek = packed_seek,
      .close = packed_close,
  };
  struct packed_file *cookie = NULL;
  size_t size = 0;
  const char *data = unpack(path, &size);
  if (data == NULL) return NULL;
  if ((cookie = calloc(1, sizeof(*cookie))) == NULL) return NULL;
  cookie->data = data;
  cookie->size = size;
  return fopencookie(cookie, mode, funcs);
}
#else
FILE *mg_fopen_packed(const char *path, const char *mode) {
  (void) path, (void) mode;
  return NULL;
}
#endif
