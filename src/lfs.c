// Copyright (c) 2023 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "flash.h"
#include "util.h"

#if MG_ENABLE_LFS

#include <fcntl.h>
#include <littlefs/lfs.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef LFS_USE_RAM
#define LFS_USE_RAM 0  // If 1, use RAM FS. If 0, use flash
#endif

#ifndef MG_LFS_BUF_SIZE
#define MG_LFS_BUF_SIZE 64  // Buffer size used for reads, writes, and cache
#endif

#ifndef MG_LFS_FD_BASE
#define MG_LFS_FD_BASE 12000  // Starting file descriptor number
#endif

#if LFS_USE_RAM && !defined(MG_LFS_BLOCK_SIZE)
#define MG_LFS_BLOCK_SIZE 8192
#endif

#ifndef DT_DIR
#define DT_DIR 4
#endif
#ifndef DT_REG
#define DT_REG 8
#endif

typedef struct mg_lfs_fd DIR;
struct dirent {
  char d_name[LFS_NAME_MAX + 1];
  unsigned char d_type;
};

struct mg_lfs_fd {
  struct mg_lfs_fd *next;
  bool isopen;
  lfs_file_t file;
  lfs_dir_t dir;
  int fd;
};

static lfs_t s_lfs;
static struct mg_lfs_fd *s_fds;
static uint8_t *s_fs;
static int s_next_fd = MG_LFS_FD_BASE;
static bool s_lfs_ready;
#if LFS_USE_RAM
static uint8_t *s_ram_fs;
#endif

static int lfs_driver_read(const struct lfs_config *cfg, lfs_block_t block,
                           lfs_off_t off, void *buf, lfs_size_t len);
static int lfs_driver_prog(const struct lfs_config *cfg, lfs_block_t block,
                           lfs_off_t off, const void *buf, lfs_size_t len);
static int lfs_driver_erase(const struct lfs_config *cfg, lfs_block_t block);
static int lfs_driver_sync(const struct lfs_config *cfg);

static struct lfs_config s_cfg = {
    .read = lfs_driver_read,
    .prog = lfs_driver_prog,
    .erase = lfs_driver_erase,
    .sync = lfs_driver_sync,
    .block_cycles = 200,
    .cache_size = MG_LFS_BUF_SIZE,
    .read_size = MG_LFS_BUF_SIZE,
    .prog_size = MG_LFS_BUF_SIZE,
    .lookahead_size = MG_LFS_BUF_SIZE / 8,
};

static bool flash_write_buf(void *addr, const void *buf, size_t len) {
#if LFS_USE_RAM
  memmove(addr, buf, len);
  return true;
#else
  return mg_flash != NULL && mg_flash->write_fn(addr, buf, len);
#endif
}

static int lfs_driver_read(const struct lfs_config *cfg, lfs_block_t block,
                           lfs_off_t off, void *buf, lfs_size_t len) {
  memmove(buf, &s_fs[block * cfg->block_size + off], len);
  return 0;
}

static int lfs_driver_prog(const struct lfs_config *cfg, lfs_block_t block,
                           lfs_off_t off, const void *buf, lfs_size_t len) {
  uint8_t *dst = &s_fs[block * cfg->block_size + off];
  if (!flash_write_buf(dst, buf, len)) return LFS_ERR_IO;
  return 0;
}

static int lfs_driver_erase(const struct lfs_config *cfg, lfs_block_t block) {
  (void) cfg, (void) block;
  return 0;
}

static int lfs_driver_sync(const struct lfs_config *cfg) {
  (void) cfg;
  return 0;
}

bool mg_lfs_init(size_t size) {
  int result = 0;
  if (s_lfs_ready) return true;
  if (size == 0) return false;
#if LFS_USE_RAM
  s_cfg.block_size = MG_LFS_BLOCK_SIZE;
  if (s_ram_fs == NULL) s_ram_fs = (uint8_t *) mg_calloc(1, size);
  s_fs = s_ram_fs;
#else
  if (mg_flash == NULL || mg_flash->secsz == 0 || mg_flash->size < size) return false;
  if (mg_flash->write_fn == NULL) return false;
  s_cfg.block_size = mg_flash->secsz;
  s_fs = (uint8_t *) mg_flash->start + mg_flash->size - size;
#endif
  if (s_fs == NULL || s_cfg.block_size == 0 || size % s_cfg.block_size != 0) {
    return false;
  }
  s_cfg.block_count = (lfs_size_t) (size / s_cfg.block_size);
  if (s_cfg.block_count == 0) return false;
  if (lfs_mount(&s_lfs, &s_cfg) != 0) {
    lfs_format(&s_lfs, &s_cfg);
    if (lfs_mount(&s_lfs, &s_cfg) != 0) result = -1;
  }
  s_lfs_ready = result == 0;
  return s_lfs_ready;
}

static struct mg_lfs_fd *find_fd(int fd) {
  struct mg_lfs_fd *f;
  for (f = s_fds; f != NULL; f = f->next) {
    if (f->isopen && f->fd == fd) return f;
  }
  return NULL;
}

static struct mg_lfs_fd *open_fd(void) {
  struct mg_lfs_fd *f = NULL;
  if (s_lfs_ready) {
    f = (struct mg_lfs_fd *) mg_calloc(1, sizeof(*f));
    if (f != NULL) {
      f->isopen = true;
      f->fd = s_next_fd++;
      f->next = s_fds;
      s_fds = f;
      if (s_next_fd < MG_LFS_FD_BASE) s_next_fd = MG_LFS_FD_BASE;
    }
  }
  return f;
}

static int close_fd(struct mg_lfs_fd *fd) {
  struct mg_lfs_fd **f = &s_fds;
  while (*f != NULL && *f != fd) f = &(*f)->next;
  if (*f == NULL) return -1;
  *f = fd->next;
  fd->isopen = false;
  mg_free(fd);
  return 0;
}

int _open(const char *path, int flags, mode_t mode) {
  int err, lfs_flags = 0, fd = -1;
  struct mg_lfs_fd *f = open_fd();
  (void) mode;
  if (f == NULL) return -1;
  fd = f->fd;
  if ((flags & 3) == O_RDONLY) lfs_flags |= LFS_O_RDONLY;
  if ((flags & 3) == O_WRONLY) lfs_flags |= LFS_O_WRONLY;
  if ((flags & 3) == O_RDWR) lfs_flags |= LFS_O_RDWR;
  if (flags & O_CREAT) lfs_flags |= LFS_O_CREAT;
  if (flags & O_TRUNC) lfs_flags |= LFS_O_TRUNC;
  if (flags & O_APPEND) lfs_flags |= LFS_O_APPEND;
  err = lfs_file_open(&s_lfs, &f->file, path, lfs_flags);
  if (err < 0) close_fd(f), fd = -1;
  return fd;
}

int _close(int fd) {
  struct mg_lfs_fd *f = find_fd(fd);
  if (fd < 3) return 0;
  if (f == NULL) return -1;
  lfs_file_close(&s_lfs, &f->file);
  close_fd(f);
  return 0;
}

int _write(int fd, char *ptr, int len) {
  struct mg_lfs_fd *f = find_fd(fd);
  return fd < 3 ? len
                : f == NULL ? -1 : lfs_file_write(&s_lfs, &f->file, ptr, len);
}

int _read(int fd, char *ptr, int len) {
  struct mg_lfs_fd *f = find_fd(fd);
  return fd < 3 ? 0 : f == NULL ? -1 : lfs_file_read(&s_lfs, &f->file, ptr, len);
}

int _lseek(int fd, int offset, int whence) {
  struct mg_lfs_fd *f = find_fd(fd);
  return fd < 3 ? 0
                : f == NULL ? -1 : lfs_file_seek(&s_lfs, &f->file, offset,
                                                  whence);
}

int _rename(const char *oldname, const char *newname) {
  return s_lfs_ready ? lfs_rename(&s_lfs, oldname, newname) : -1;
}

int _unlink_r(void *r, const char *a) {
  (void) r;
  return s_lfs_ready ? lfs_remove(&s_lfs, a) : -1;
}

DIR *opendir(const char *name) {
  struct mg_lfs_fd *f = open_fd();
  if (f == NULL) return NULL;
  if (lfs_dir_open(&s_lfs, &f->dir, name) != 0) {
    close_fd(f);
    return NULL;
  }
  return (DIR *) f;
}

int closedir(DIR *dir) {
  struct mg_lfs_fd *f = (struct mg_lfs_fd *) dir;
  if (f == NULL || find_fd(f->fd) != f) return -1;
  lfs_dir_close(&s_lfs, &f->dir);
  return close_fd(f);
}

struct dirent *readdir(DIR *dir) {
  static struct dirent dirent;
  struct mg_lfs_fd *f = (struct mg_lfs_fd *) dir;
  struct lfs_info info;
  if (f == NULL || !f->isopen) return NULL;
  if (lfs_dir_read(&s_lfs, &f->dir, &info) < 1) return NULL;
  memset(&dirent, 0, sizeof(dirent));
  strncpy(dirent.d_name, info.name, sizeof(dirent.d_name) - 1);
  if (info.type == LFS_TYPE_DIR) dirent.d_type |= DT_DIR;
  if (info.type == LFS_TYPE_REG) dirent.d_type |= DT_REG;
  return &dirent;
}

int _stat(const char *path, struct stat *st) {
  struct lfs_info info;
  if (!s_lfs_ready || lfs_stat(&s_lfs, path, &info) != 0) return -1;
  st->st_mode = info.type == LFS_TYPE_DIR ? S_IFDIR : S_IFREG;
  st->st_size = info.size;
  return 0;
}

int _fstat(int fd, struct stat *st) {
  if (fd >= 3 && find_fd(fd) == NULL) return -1;
  st->st_mode = S_IFCHR;
  return 0;
}

int mkdir(const char *path, mode_t mode) {
  (void) mode;
  return s_lfs_ready ? lfs_mkdir(&s_lfs, path) : -1;
}

#endif
