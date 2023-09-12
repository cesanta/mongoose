#include "fs.h"

#if MG_ENABLE_FILE

#ifndef MG_STAT_STRUCT
#define MG_STAT_STRUCT stat
#endif

#ifndef MG_STAT_FUNC
#define MG_STAT_FUNC stat
#endif

static int p_stat(const char *path, size_t *size, time_t *mtime) {
#if !defined(S_ISDIR)
  MG_ERROR(("stat() API is not supported. %p %p %p", path, size, mtime));
  return 0;
#else
#if MG_ARCH == MG_ARCH_WIN32
  struct _stati64 st;
  wchar_t tmp[MG_PATH_MAX];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, tmp, sizeof(tmp) / sizeof(tmp[0]));
  if (_wstati64(tmp, &st) != 0) return 0;
  // If path is a symlink, windows reports 0 in st.st_size.
  // Get a real file size by opening it and jumping to the end
  if (st.st_size == 0 && (st.st_mode & _S_IFREG)) {
    FILE *fp = _wfopen(tmp, L"rb");
    if (fp != NULL) {
      fseek(fp, 0, SEEK_END);
      if (ftell(fp) > 0) st.st_size = ftell(fp);  // Use _ftelli64 on win10+
      fclose(fp);
    }
  }
#else
  struct MG_STAT_STRUCT st;
  if (MG_STAT_FUNC(path, &st) != 0) return 0;
#endif
  if (size) *size = (size_t) st.st_size;
  if (mtime) *mtime = st.st_mtime;
  return MG_FS_READ | MG_FS_WRITE | (S_ISDIR(st.st_mode) ? MG_FS_DIR : 0);
#endif
}

#if MG_ARCH == MG_ARCH_WIN32
struct dirent {
  char d_name[MAX_PATH];
};

typedef struct win32_dir {
  HANDLE handle;
  WIN32_FIND_DATAW info;
  struct dirent result;
} DIR;

int gettimeofday(struct timeval *tv, void *tz) {
  FILETIME ft;
  unsigned __int64 tmpres = 0;

  if (tv != NULL) {
    GetSystemTimeAsFileTime(&ft);
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
    tmpres /= 10;  // convert into microseconds
    tmpres -= (int64_t) 11644473600000000;
    tv->tv_sec = (long) (tmpres / 1000000UL);
    tv->tv_usec = (long) (tmpres % 1000000UL);
  }
  (void) tz;
  return 0;
}

static int to_wchar(const char *path, wchar_t *wbuf, size_t wbuf_len) {
  int ret;
  char buf[MAX_PATH * 2], buf2[MAX_PATH * 2], *p;
  strncpy(buf, path, sizeof(buf));
  buf[sizeof(buf) - 1] = '\0';
  // Trim trailing slashes. Leave backslash for paths like "X:\"
  p = buf + strlen(buf) - 1;
  while (p > buf && p[-1] != ':' && (p[0] == '\\' || p[0] == '/')) *p-- = '\0';
  memset(wbuf, 0, wbuf_len * sizeof(wchar_t));
  ret = MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, (int) wbuf_len);
  // Convert back to Unicode. If doubly-converted string does not match the
  // original, something is fishy, reject.
  WideCharToMultiByte(CP_UTF8, 0, wbuf, (int) wbuf_len, buf2, sizeof(buf2),
                      NULL, NULL);
  if (strcmp(buf, buf2) != 0) {
    wbuf[0] = L'\0';
    ret = 0;
  }
  return ret;
}

DIR *opendir(const char *name) {
  DIR *d = NULL;
  wchar_t wpath[MAX_PATH];
  DWORD attrs;

  if (name == NULL) {
    SetLastError(ERROR_BAD_ARGUMENTS);
  } else if ((d = (DIR *) calloc(1, sizeof(*d))) == NULL) {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
  } else {
    to_wchar(name, wpath, sizeof(wpath) / sizeof(wpath[0]));
    attrs = GetFileAttributesW(wpath);
    if (attrs != 0Xffffffff && (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
      (void) wcscat(wpath, L"\\*");
      d->handle = FindFirstFileW(wpath, &d->info);
      d->result.d_name[0] = '\0';
    } else {
      free(d);
      d = NULL;
    }
  }
  return d;
}

int closedir(DIR *d) {
  int result = 0;
  if (d != NULL) {
    if (d->handle != INVALID_HANDLE_VALUE)
      result = FindClose(d->handle) ? 0 : -1;
    free(d);
  } else {
    result = -1;
    SetLastError(ERROR_BAD_ARGUMENTS);
  }
  return result;
}

struct dirent *readdir(DIR *d) {
  struct dirent *result = NULL;
  if (d != NULL) {
    memset(&d->result, 0, sizeof(d->result));
    if (d->handle != INVALID_HANDLE_VALUE) {
      result = &d->result;
      WideCharToMultiByte(CP_UTF8, 0, d->info.cFileName, -1, result->d_name,
                          sizeof(result->d_name), NULL, NULL);
      if (!FindNextFileW(d->handle, &d->info)) {
        FindClose(d->handle);
        d->handle = INVALID_HANDLE_VALUE;
      }
    } else {
      SetLastError(ERROR_FILE_NOT_FOUND);
    }
  } else {
    SetLastError(ERROR_BAD_ARGUMENTS);
  }
  return result;
}
#endif

static void p_list(const char *dir, void (*fn)(const char *, void *),
                   void *userdata) {
#if MG_ENABLE_DIRLIST
  struct dirent *dp;
  DIR *dirp;
  if ((dirp = (opendir(dir))) == NULL) return;
  while ((dp = readdir(dirp)) != NULL) {
    if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) continue;
    fn(dp->d_name, userdata);
  }
  closedir(dirp);
#else
  (void) dir, (void) fn, (void) userdata;
#endif
}

static void *p_open(const char *path, int flags) {
#if MG_ARCH == MG_ARCH_WIN32
  const char *mode = flags == MG_FS_READ ? "rb" : "a+b";
  wchar_t b1[MG_PATH_MAX], b2[10];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, b1, sizeof(b1) / sizeof(b1[0]));
  MultiByteToWideChar(CP_UTF8, 0, mode, -1, b2, sizeof(b2) / sizeof(b2[0]));
  return (void *) _wfopen(b1, b2);
#else
  const char *mode = flags == MG_FS_READ ? "rbe" : "a+be";  // e for CLOEXEC
  return (void *) fopen(path, mode);
#endif
}

static void p_close(void *fp) {
  fclose((FILE *) fp);
}

static size_t p_read(void *fp, void *buf, size_t len) {
  return fread(buf, 1, len, (FILE *) fp);
}

static size_t p_write(void *fp, const void *buf, size_t len) {
  return fwrite(buf, 1, len, (FILE *) fp);
}

static size_t p_seek(void *fp, size_t offset) {
#if (defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64) ||  \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600)
  if (fseeko((FILE *) fp, (off_t) offset, SEEK_SET) != 0) (void) 0;
#else
  if (fseek((FILE *) fp, (long) offset, SEEK_SET) != 0) (void) 0;
#endif
  return (size_t) ftell((FILE *) fp);
}

static bool p_rename(const char *from, const char *to) {
  return rename(from, to) == 0;
}

static bool p_remove(const char *path) {
  return remove(path) == 0;
}

static bool p_mkdir(const char *path) {
  return mkdir(path, 0775) == 0;
}

#else

static int p_stat(const char *path, size_t *size, time_t *mtime) {
  (void) path, (void) size, (void) mtime;
  return 0;
}
static void p_list(const char *path, void (*fn)(const char *, void *),
                   void *userdata) {
  (void) path, (void) fn, (void) userdata;
}
static void *p_open(const char *path, int flags) {
  (void) path, (void) flags;
  return NULL;
}
static void p_close(void *fp) {
  (void) fp;
}
static size_t p_read(void *fd, void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}
static size_t p_write(void *fd, const void *buf, size_t len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}
static size_t p_seek(void *fd, size_t offset) {
  (void) fd, (void) offset;
  return (size_t) ~0;
}
static bool p_rename(const char *from, const char *to) {
  (void) from, (void) to;
  return false;
}
static bool p_remove(const char *path) {
  (void) path;
  return false;
}
static bool p_mkdir(const char *path) {
  (void) path;
  return false;
}
#endif

struct mg_fs mg_fs_posix = {p_stat,  p_list, p_open,   p_close,  p_read,
                            p_write, p_seek, p_rename, p_remove, p_mkdir};
