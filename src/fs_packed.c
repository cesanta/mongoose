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

static char *packed_realpath(const char *path, char *resolved_path) {
  if (resolved_path == NULL) resolved_path = malloc(strlen(path) + 1);
  strcpy(resolved_path, path);
  return resolved_path;
}

static int packed_stat(const char *path, size_t *size, unsigned *mtime) {
  const char *data = mg_unpack(path, size);
  if (mtime) *mtime = 0;
  return data == NULL ? 0 : MG_FS_READ;
}

static void packed_list(const char *path, void (*fn)(const char *, void *),
                        void *userdata) {
  (void) path, (void) fn, (void) userdata;
}

static struct mg_fd *packed_open(const char *path, int flags) {
  size_t size = 0;
  const char *data = mg_unpack(path, &size);
  struct packed_file *fp = NULL;
  struct mg_fd *fd = NULL;
  if (data == NULL) return NULL;
  if (flags & MG_FS_WRITE) return NULL;
  fp = calloc(1, sizeof(*fp));
  fd = calloc(1, sizeof(*fd));
  fp->size = size;
  fp->data = data;
  fd->fd = fp;
  fd->fs = &mg_fs_packed;
  return fd;
}

static void packed_close(struct mg_fd *fd) {
  if (fd) free(fd->fd), free(fd);
}

static size_t packed_read(void *fd, void *buf, size_t len) {
  struct packed_file *fp = (struct packed_file *) fd;
  if (fp->pos + len > fp->size) len = fp->size - fp->pos;
  memcpy(buf, &fp->data[fp->pos], len);
  fp->pos += len;
  return len;
}

static size_t packed_write(void *fd, const void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}

static size_t packed_seek(void *fd, size_t offset) {
  struct packed_file *fp = (struct packed_file *) fd;
  fp->pos = offset;
  if (fp->pos > fp->size) fp->pos = fp->size;
  return fp->pos;
}

struct mg_fs mg_fs_packed = {packed_realpath, packed_stat,  packed_list,
                             packed_open,     packed_close, packed_read,
                             packed_write,    packed_seek};
