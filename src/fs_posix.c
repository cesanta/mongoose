#include "fs.h"

#if defined(O_READ)
static char *posix_realpath(const char *path, char *resolved_path) {
#ifdef _WIN32
  return _fullpath(path, resolved_path, PATH_MAX);
#else
  return realpath(path, resolved_path);
#endif
}

static int posix_stat(const char *path, size_t *size, unsigned *mtime) {
#ifdef _WIN32
  struct _stati64 st;
  wchar_t tmp[PATH_MAX];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, tmp, sizeof(tmp) / sizeof(tmp[0]));
  if (_wstati64(tmp, &st) != 0) return 0;
#else
  struct stat st;
  if (stat(path, &st) != 0) return 0;
#endif
  if (size) *size = (size_t) st.st_size;
  if (mtime) *mtime = (unsigned) st.st_mtime;
  return MG_FS_READ | MG_FS_WRITE | (S_ISDIR(st.st_mode) ? MG_FS_DIR : 0);
}

static void posix_list(const char *dir, void (*fn)(const char *, void *),
                       void *userdata) {
  // char path[MG_PATH_MAX], *p = &dir[strlen(dir) - 1], tmp[10];
  struct dirent *dp;
  DIR *dirp;

  // while (p > dir && *p != '/') *p-- = '\0';
  if ((dirp = (opendir(dir))) != NULL) {
    size_t off, n;
    while ((dp = readdir(dirp)) != NULL) {
      // Do not show current dir and hidden files
      if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) continue;
      fn(dp->d_name, &st);
    }
    closedir(dirp);
  }
}

static struct mg_fd *posix_open(const char *path, int flags) {
  const char *mode =
      flags & (MG_FS_READ | MG_FS_WRITE)
          ? "r+b"
          : flags & MG_FS_READ ? "rb" : flags & MG_FS_WRITE ? "wb" : "";
  void *fp = NULL;
  struct mg_fd *fd = NULL;
#ifdef _WIN32
  wchar_t b1[PATH_MAX], b2[10];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, b1, sizeof(b1) / sizeof(b1[0]));
  MultiByteToWideChar(CP_UTF8, 0, mode, -1, b2, sizeof(b2) / sizeof(b2[0]));
  fp = (void *) _wfopen(b1, b2);
#else
  fp = (void *) fopen(path, mode);
#endif
  if (fp == NULL) return NULL;
  fd = calloc(1, sizeof(*fd));
  fd->fd = fp;
  fd->fs = &mg_fs_posix;
  return fd;
}

static void posix_close(struct mg_fd *fd) {
  if (fd) fclose((FILE *) fd->fd), free(fd);
}

static size_t posix_read(void *fp, void *buf, size_t len) {
  return fread(buf, 1, len, (FILE *) fp);
}

static size_t posix_write(void *fp, const void *buf, size_t len) {
  return fwrite(buf, 1, len, (FILE *) fp);
}

static size_t posix_seek(void *fp, size_t offset) {
#if _FILE_OFFSET_BITS == 64 || _POSIX_C_SOURCE >= 200112L || \
    _XOPEN_SOURCE >= 600
  fseeko((FILE *) fp, (off_t) offset, SEEK_SET);
#else
  fseek((FILE *) fp, (long) offset, SEEK_SET);
#endif
  return (size_t) ftell((FILE *) fp);
}
#else
static char *posix_realpath(const char *path, char *resolved_path) {
  (void) path, (void) resolved_path;
  return NULL;
}

static int posix_stat(const char *path, size_t *size, unsigned *mtime) {
  (void) path, (void) size, (void) mtime;
  return 0;
}

static void posix_list(const char *path, void (*fn)(const char *, void *),
                       void *userdata) {
  (void) path, (void) fn, (void) userdata;
}

static struct mg_fd *posix_open(const char *path, int flags) {
  (void) path, (void) flags;
  return NULL;
}

static void posix_close(struct mg_fd *fd) {
  (void) fd;
}

static size_t posix_read(void *fd, void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}

static size_t posix_write(void *fd, const void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}

static size_t posix_seek(void *fd, size_t offset) {
  (void) fd, (void) offset;
  return (size_t) ~0;
}
#endif

struct mg_fs mg_fs_posix = {posix_realpath, posix_stat,  posix_list,
                            posix_open,     posix_close, posix_read,
                            posix_write,    posix_seek};
