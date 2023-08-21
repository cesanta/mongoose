#include "fs.h"
#include "printf.h"
#include "str.h"

struct packed_file {
  const char *data;
  size_t size;
  size_t pos;
};

#if MG_ENABLE_PACKED_FS
#else
const char *mg_unpack(const char *path, size_t *size, time_t *mtime) {
  *size = 0, *mtime = 0;
  (void) path;
  return NULL;
}
const char *mg_unlist(size_t no) {
  (void) no;
  return NULL;
}
#endif

struct mg_str mg_unpacked(const char *path) {
  size_t len = 0;
  const char *buf = mg_unpack(path, &len, NULL);
  return mg_str_n(buf, len);
}

static int is_dir_prefix(const char *prefix, size_t n, const char *path) {
  // MG_INFO(("[%.*s] [%s] %c", (int) n, prefix, path, path[n]));
  return n < strlen(path) && strncmp(prefix, path, n) == 0 &&
         (n == 0 || path[n] == '/' || path[n - 1] == '/');
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
  char buf[MG_PATH_MAX], tmp[sizeof(buf)];
  const char *path, *begin, *end;
  size_t i, n = strlen(dir);
  tmp[0] = '\0';  // Previously listed entry
  for (i = 0; (path = mg_unlist(i)) != NULL; i++) {
    if (!is_dir_prefix(dir, n, path)) continue;
    begin = &path[n + 1];
    end = strchr(begin, '/');
    if (end == NULL) end = begin + strlen(begin);
    mg_snprintf(buf, sizeof(buf), "%.*s", (int) (end - begin), begin);
    buf[sizeof(buf) - 1] = '\0';
    // If this entry has been already listed, skip
    // NOTE: we're assuming that file list is sorted alphabetically
    if (strcmp(buf, tmp) == 0) continue;
    fn(buf, userdata);  // Not yet listed, call user function
    strcpy(tmp, buf);   // And save this entry as listed
  }
}

static void *packed_open(const char *path, int flags) {
  size_t size = 0;
  const char *data = mg_unpack(path, &size, NULL);
  struct packed_file *fp = NULL;
  if (data == NULL) return NULL;
  if (flags & MG_FS_WRITE) return NULL;
  if ((fp = (struct packed_file *) calloc(1, sizeof(*fp))) != NULL) {
    fp->size = size;
    fp->data = data;
  }
  return (void *) fp;
}

static void packed_close(void *fp) {
  if (fp != NULL) free(fp);
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

static bool packed_rename(const char *from, const char *to) {
  (void) from, (void) to;
  return false;
}

static bool packed_remove(const char *path) {
  (void) path;
  return false;
}

static bool packed_mkdir(const char *path) {
  (void) path;
  return false;
}

struct mg_fs mg_fs_packed = {
    packed_stat,  packed_list, packed_open,   packed_close,  packed_read,
    packed_write, packed_seek, packed_rename, packed_remove, packed_mkdir};
