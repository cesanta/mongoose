#include "fs.h"
#include "printf.h"
#include "str.h"

struct mg_fd *mg_fs_open(struct mg_fs *fs, const char *path, int flags) {
  struct mg_fd *fd = (struct mg_fd *) calloc(1, sizeof(*fd));
  if (fd != NULL) {
    fd->fd = fs->op(path, flags);
    fd->fs = fs;
    if (fd->fd == NULL) {
      free(fd);
      fd = NULL;
    }
  }
  return fd;
}

void mg_fs_close(struct mg_fd *fd) {
  if (fd != NULL) {
    fd->fs->cl(fd->fd);
    free(fd);
  }
}

char *mg_file_read(struct mg_fs *fs, const char *path, size_t *sizep) {
  struct mg_fd *fd;
  char *data = NULL;
  size_t size = 0;
  fs->st(path, &size, NULL);
  if ((fd = mg_fs_open(fs, path, MG_FS_READ)) != NULL) {
    data = (char *) calloc(1, size + 1);
    if (data != NULL) {
      if (fs->rd(fd->fd, data, size) != size) {
        free(data);
        data = NULL;
      } else {
        data[size] = '\0';
        if (sizep != NULL) *sizep = size;
      }
    }
    mg_fs_close(fd);
  }
  return data;
}

bool mg_file_write(struct mg_fs *fs, const char *path, const void *buf,
                   size_t len) {
  bool result = false;
  struct mg_fd *fd;
  char tmp[MG_PATH_MAX];
  mg_snprintf(tmp, sizeof(tmp), "%s..%d", path, rand());
  if ((fd = mg_fs_open(fs, tmp, MG_FS_WRITE)) != NULL) {
    result = fs->wr(fd->fd, buf, len) == len;
    mg_fs_close(fd);
    if (result) {
      fs->rm(path);
      fs->mv(tmp, path);
    } else {
      fs->rm(tmp);
    }
  }
  return result;
}

bool mg_file_printf(struct mg_fs *fs, const char *path, const char *fmt, ...) {
  va_list ap;
  char *data;
  bool result = false;
  va_start(ap, fmt);
  data = mg_vmprintf(fmt, &ap);
  va_end(ap);
  result = mg_file_write(fs, path, data, strlen(data));
  free(data);
  return result;
}

// This helper function allows to scan a filesystem in a sequential way,
// without using callback function:
//      char buf[100] = "";
//      while (mg_fs_ls(&mg_fs_posix, "./", buf, sizeof(buf))) {
//        ...
static void mg_fs_ls_fn(const char *filename, void *param) {
  struct mg_str *s = (struct mg_str *) param;
  if (s->ptr[0] == '\0') {
    mg_snprintf((char *) s->ptr, s->len, "%s", filename);
  } else if (strcmp(s->ptr, filename) == 0) {
    ((char *) s->ptr)[0] = '\0';  // Fetch next file
  }
}

bool mg_fs_ls(struct mg_fs *fs, const char *path, char *buf, size_t len) {
  struct mg_str s = {buf, len};
  fs->ls(path, mg_fs_ls_fn, &s);
  return buf[0] != '\0';
}
