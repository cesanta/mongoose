#include "fs.h"

static int d_stat(const char *path, size_t *size, time_t *mtime) {
  (void) path, (void) size, (void) mtime;
  return 0;
}
static void d_list(const char *path, void (*fn)(const char *, void *),
                   void *userdata) {
  (void) path, (void) fn, (void) userdata;
}
static void *d_open(const char *path, int flags) {
  (void) path, (void) flags;
  return NULL;
}
static void d_close(void *fp) {
  (void) fp;
}
static size_t d_read(void *fd, void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}
static size_t d_write(void *fd, const void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}
static size_t d_seek(void *fd, size_t offset) {
  (void) fd, (void) offset;
  return (size_t) ~0;
}
static bool d_rename(const char *from, const char *to) {
  (void) from, (void) to;
  return false;
}
static bool d_remove(const char *path) {
  (void) path;
  return false;
}
static bool d_mkdir(const char *path) {
  (void) path;
  return false;
}

struct mg_fs mg_fs_dummy = {d_stat,  d_list, d_open,   d_close,  d_read,
                            d_write, d_seek, d_rename, d_remove, d_mkdir};
