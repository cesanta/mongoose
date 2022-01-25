#include "fs.h"

#if MG_ENABLE_FATFS
#include <ff.h>

static int ff_stat(const char *path, size_t *size, time_t *mtime) {
  FILINFO fi;
  if (path[0] == '\0' || strcmp(path, MG_FATFS_ROOT) == 0) {
    if (size) *size = 0;
    if (mtime) *mtime = 0;
    return MG_FS_DIR;
  } else if (f_stat(path, &fi) == 0) {
    if (size) *size = (size_t) fi.fsize;
    if (mtime) *mtime = (fi.fdate << 16) | fi.ftime;
    return MG_FS_READ | MG_FS_WRITE | ((fi.fattrib & AM_DIR) ? MG_FS_DIR : 0);
  } else {
    return 0;
  }
}

static void ff_list(const char *dir, void (*fn)(const char *, void *),
                    void *userdata) {
  DIR d;
  FILINFO fi;
  if (f_opendir(&d, dir) == FR_OK) {
    while (f_readdir(&d, &fi) == FR_OK && fi.fname[0] != '\0') {
      if (!strcmp(fi.fname, ".") || !strcmp(fi.fname, "..")) continue;
      fn(fi.fname, userdata);
    }
    f_closedir(&d);
  }
}

static void *ff_open(const char *path, int flags) {
  FIL f;
  unsigned char mode = FA_READ;
  if (flags & MG_FS_WRITE) mode |= FA_WRITE | FA_CREATE_ALWAYS | FA_OPEN_APPEND;
  if (f_open(&f, path, mode) == 0) {
    FIL *fp = calloc(1, sizeof(*fp));
    *fp = f;
    return fp;
  } else {
    return NULL;
  }
}

static void ff_close(void *fp) {
  if (fp != NULL) {
    f_close((FIL *) fp);
    free(fp);
  }
}

static size_t ff_read(void *fp, void *buf, size_t len) {
  unsigned n = 0;
  f_read((FIL *) fp, buf, len, &n);
  return n;
}

static size_t ff_write(void *fp, const void *buf, size_t len) {
  unsigned n, sum = 0, bs = MG_FATFS_BSIZE;
  while ((size_t) sum < len &&
         f_write((FIL *) fp, (char *) buf + sum,
                 sum + bs > len ? len - sum : bs, &n) == FR_OK &&
         n > 0) {
    sum += n;
  }
  return sum;
}

static size_t ff_seek(void *fp, size_t offset) {
  f_lseek((FIL *) fp, offset);
  return offset;
}

static bool ff_rename(const char *from, const char *to) {
  return f_rename(from, to) == FR_OK;
}

static bool ff_remove(const char *path) {
  return f_unlink(path) == FR_OK;
}

static bool ff_mkdir(const char *path) {
  return f_mkdir(path) == FR_OK;
}

struct mg_fs mg_fs_fat = {ff_stat,  ff_list, ff_open,   ff_close,  ff_read,
                          ff_write, ff_seek, ff_rename, ff_remove, ff_mkdir};
#endif
