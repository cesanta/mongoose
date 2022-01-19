#pragma once

#include "arch.h"
#include "config.h"

enum { MG_FS_READ = 1, MG_FS_WRITE = 2, MG_FS_DIR = 4 };

// Filesystem API functions
// stat() returns MG_FS_* flags and populates file size and modification time
// list() calls fn() for every directory entry, allowing to list a directory
struct mg_fs {
  int (*stat)(const char *path, size_t *size, time_t *mtime);
  void (*list)(const char *path, void (*fn)(const char *, void *), void *);
  void *(*open)(const char *path, int flags);              // Open file
  void (*close)(void *fd);                                 // Close file
  size_t (*read)(void *fd, void *buf, size_t len);         // Read file
  size_t (*write)(void *fd, const void *buf, size_t len);  // Write file
  size_t (*seek)(void *fd, size_t offset);                 // Set file position
  bool (*rename)(const char *from, const char *to);        // Rename
  bool (*remove)(const char *path);                        // Delete file
  bool (*mkd)(const char *path);                           // Create directory
};

extern struct mg_fs mg_fs_posix;   // POSIX open/close/read/write/seek
extern struct mg_fs mg_fs_packed;  // Packed FS, see examples/complete
extern struct mg_fs mg_fs_fat;     // FAT FS

// File descriptor
struct mg_fd {
  void *fd;
  struct mg_fs *fs;
};

struct mg_fd *mg_fs_open(struct mg_fs *fs, const char *path, int flags);
void mg_fs_close(struct mg_fd *fd);
char *mg_file_read(struct mg_fs *fs, const char *path, size_t *size);
bool mg_file_write(struct mg_fs *fs, const char *path, const void *, size_t);
bool mg_file_printf(struct mg_fs *fs, const char *path, const char *fmt, ...);
