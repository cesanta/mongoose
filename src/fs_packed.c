#include "fs.h"
#include "printf.h"
#include "str.h"

const struct mg_mem_file *mg_mem_files;

static const char *mg_unlist(size_t no) {
  return mg_mem_files == NULL ? NULL : mg_mem_files[no].path;
}

static const char *mg_unpack(const char *path, size_t *size, time_t *mtime) {
  const struct mg_mem_file *p;
  for (p = mg_mem_files; p != NULL && p->path != NULL; p++) {
    if (strcmp(p->path, path) != 0) continue;
    if (size != NULL) *size = p->size;
    if (mtime != NULL) *mtime = p->mtime;
    return (const char *) p->data;
  }
  return NULL;
}

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

static int mem_info(const char *path, size_t *size, time_t *mtime) {
  int result = MG_FS_FAIL;
  const char *p;
  size_t i, n = strlen(path);
  if (mg_unpack(path, size, mtime) != NULL) {
    result = MG_FS_FILE;
  } else {
    // Scan all files. If `path` is a dir prefix for any of them, it's a dir
    for (i = 0; (p = mg_unlist(i)) != NULL; i++) {
      if (is_dir_prefix(path, n, p)) {
        if (size != NULL) *size = 0;
        if (mtime != NULL) *mtime = 0;
        result = MG_FS_DIR;
        break;
      }
    }
  }
  return result;
}

static size_t mem_list(const char *dir, bool recursive,
                       void (*fn)(const char *, void *), void *userdata) {
  char buf[MG_PATH_MAX], tmp[sizeof(buf)];
  const char *path, *begin, *end;
  size_t result = 0, i, n = strlen(dir);
  tmp[0] = '\0';  // Previously listed entry
  for (i = 0; (path = mg_unlist(i)) != NULL; i++) {
    if (!is_dir_prefix(dir, n, path)) continue;
    begin = &path[n + 1];
    if (recursive) {
      mg_snprintf(buf, sizeof(buf), "%s", begin);
    } else {
      end = strchr(begin, '/');
      if (end == NULL) end = begin + strlen(begin);
      mg_snprintf(buf, sizeof(buf), "%.*s", (end - begin), begin);
    }
    buf[sizeof(buf) - 1] = '\0';
    // If this entry has been already listed, skip
    // NOTE: we're assuming that file list is sorted alphabetically
    if (strcmp(buf, tmp) == 0) continue;
    fn(buf, userdata);  // Not yet listed, call user function
    strcpy(tmp, buf);   // And save this entry as listed
    result++;
  }
  return result;
}

static size_t mem_read(const char *path, size_t ofs, void *buf, size_t len) {
  size_t size = 0, n = 0;
  const char *p = mg_unpack(path, &size, NULL);
  if (p != NULL && ofs < size) {
    n = size - ofs;
    if (n > len) n = len;
    memcpy(buf, p + ofs, n);
  }
  return n;
}

static size_t mem_write(const char *path, size_t ofs, const void *buf,
                        size_t len) {
  (void) path, (void) ofs, (void) buf, (void) len;
  return 0;
}

static bool mem_del(const char *path) {
  (void) path;
  return false;
}

struct mg_fs mg_fs_mem = {mem_info, mem_list, mem_read, mem_write, mem_del};
