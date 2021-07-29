#pragma once

#include "arch.h"

enum { MG_FS_READ = 1, MG_FS_WRITE = 2, MG_FS_DIR = 4 };

// Filesystem API functions
struct mg_fs {
  char *(*realpath)(const char *path, char *resolved_path);
  int (*stat)(const char *path, size_t *size, time_t *mtime);
  void (*list)(const char *path, void (*fn)(const char *, void *), void *);
  struct mg_fd *(*open)(const char *path, int flags);
  void (*close)(struct mg_fd *fd);
  size_t (*read)(void *fd, void *buf, size_t len);
  size_t (*write)(void *fd, const void *buf, size_t len);
  size_t (*seek)(void *fd, size_t offset);
};

// File descriptor
struct mg_fd {
  void *fd;
  struct mg_fs *fs;
};

extern struct mg_fs mg_fs_posix;   // POSIX open/close/read/write/seek
extern struct mg_fs mg_fs_packed;  // Packed FS, see examples/complete
