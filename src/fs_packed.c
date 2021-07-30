#include "fs.h"

struct packed_file {
  const char *data;
  size_t size;
  size_t pos;
};

const char *mg_unpack(const char *path, size_t *size, time_t *mtime);
const char *mg_unlist(size_t no);
#if MG_ENABLE_PACKED_FS
#else
const char *mg_unpack(const char *path, size_t *size, time_t *mtime) {
  (void) path, (void) size, (void) mtime;
  return NULL;
}
const char *mg_unlist(size_t no) {
  (void) no;
  return NULL;
}
#endif

static char *packed_realpath(const char *path, char *resolved_path) {
  if (resolved_path == NULL) resolved_path = (char *) malloc(strlen(path) + 1);
  // while (*path == '.' || *path == '/') path++;
  strcpy(resolved_path, path);
  return resolved_path;
}

static int is_dir_prefix(const char *prefix, size_t n, const char *path) {
  return n < strlen(path) && memcmp(prefix, path, n) == 0 && path[n] == '/';
  //(n == 0 || path[n] == MG_DIRSEP);
}

static int packed_stat(const char *path, size_t *size, time_t *mtime) {
  const char *p;
  size_t i, n = strlen(path);
  if (mg_unpack(path, size, mtime)) return MG_FS_READ;  // Regular file
  // Scan all files. If `path` is a dir prefix for any of them, it's a dir
  for (i = 0; (p = mg_unlist(i)) != NULL; i++) {
    if (is_dir_prefix(path, n, p)) return MG_FS_DIR;
  }
  return 0;
}

static void packed_list(const char *dir, void (*fn)(const char *, void *),
                        void *userdata) {
  char buf[256], tmp[sizeof(buf)];
  const char *path, *begin, *end;
  size_t i, n = strlen(dir);
  tmp[0] = '\0';  // Previously listed entry
  for (i = 0; (path = mg_unlist(i)) != NULL; i++) {
    if (!is_dir_prefix(dir, n, path)) continue;
    begin = &path[n + 1];
    end = strchr(begin, '/');
    if (end == NULL) end = begin + strlen(begin);
    snprintf(buf, sizeof(buf), "%.*s", (int) (end - begin), begin);
    buf[sizeof(buf) - 1] = '\0';
    // If this entry has been already listed, skip
    // NOTE: we're assuming that file list is sorted alphabetically
    if (strcmp(buf, tmp) == 0) continue;
    fn(buf, userdata);  // Not yet listed, call user function
    strcpy(tmp, buf);   // And save this entry as listed
  }
}

static struct mg_fd *packed_open(const char *path, int flags) {
  size_t size = 0;
  const char *data = mg_unpack(path, &size, NULL);
  struct packed_file *fp = NULL;
  struct mg_fd *fd = NULL;
  if (data == NULL) return NULL;
  if (flags & MG_FS_WRITE) return NULL;
  fp = (struct packed_file *) calloc(1, sizeof(*fp));
  fd = (struct mg_fd *) calloc(1, sizeof(*fd));
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
