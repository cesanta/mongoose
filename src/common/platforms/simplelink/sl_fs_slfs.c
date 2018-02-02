/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/* Standard libc interface to TI SimpleLink FS. */

#if defined(MG_FS_SLFS) || defined(CC3200_FS_SLFS)

#include "common/platforms/simplelink/sl_fs_slfs.h"

#include <errno.h>

#if CS_PLATFORM == CS_P_CC3200
#include <inc/hw_types.h>
#endif

#include "common/cs_dbg.h"
#include "common/mg_mem.h"

#if SL_MAJOR_VERSION_NUM < 2
int slfs_open(const unsigned char *fname, uint32_t flags) {
  _i32 fh;
  _i32 r = sl_FsOpen(fname, flags, NULL /* token */, &fh);
  return (r < 0 ? r : fh);
}
#else /* SL_MAJOR_VERSION_NUM >= 2 */
int slfs_open(const unsigned char *fname, uint32_t flags) {
  return sl_FsOpen(fname, flags, NULL /* token */);
}
#endif

/* From sl_fs.c */
int set_errno(int e);
const char *drop_dir(const char *fname, bool *is_slfs);

/*
 * With SLFS, you have to pre-declare max file size. Yes. Really.
 * 64K should be enough for everyone. Right?
 */
#ifndef FS_SLFS_MAX_FILE_SIZE
#define FS_SLFS_MAX_FILE_SIZE (64 * 1024)
#endif

struct sl_file_size_hint {
  char *name;
  size_t size;
};

struct sl_fd_info {
  _i32 fh;
  _off_t pos;
  size_t size;
};

static struct sl_fd_info s_sl_fds[MAX_OPEN_SLFS_FILES];
static struct sl_file_size_hint s_sl_file_size_hints[MAX_OPEN_SLFS_FILES];

static int sl_fs_to_errno(_i32 r) {
  DBG(("SL error: %d", (int) r));
  switch (r) {
    case SL_FS_OK:
      return 0;
    case SL_ERROR_FS_FILE_NAME_EXIST:
      return EEXIST;
    case SL_ERROR_FS_WRONG_FILE_NAME:
      return EINVAL;
    case SL_ERROR_FS_NO_AVAILABLE_NV_INDEX:
    case SL_ERROR_FS_NOT_ENOUGH_STORAGE_SPACE:
      return ENOSPC;
    case SL_ERROR_FS_FAILED_TO_ALLOCATE_MEM:
      return ENOMEM;
    case SL_ERROR_FS_FILE_NOT_EXISTS:
      return ENOENT;
    case SL_ERROR_FS_NOT_SUPPORTED:
      return ENOTSUP;
  }
  return ENXIO;
}

int fs_slfs_open(const char *pathname, int flags, mode_t mode) {
  int fd;
  for (fd = 0; fd < MAX_OPEN_SLFS_FILES; fd++) {
    if (s_sl_fds[fd].fh <= 0) break;
  }
  if (fd >= MAX_OPEN_SLFS_FILES) return set_errno(ENOMEM);
  struct sl_fd_info *fi = &s_sl_fds[fd];

  /*
   * Apply path manipulations again, in case we got here directly
   * (via TI libc's "add_device").
   */
  pathname = drop_dir(pathname, NULL);

  _u32 am = 0;
  fi->size = (size_t) -1;
  int rw = (flags & 3);
  size_t new_size = FS_SLFS_MAX_FILE_SIZE;
  if (rw == O_RDONLY) {
    SlFsFileInfo_t sl_fi;
    _i32 r = sl_FsGetInfo((const _u8 *) pathname, 0, &sl_fi);
    if (r == SL_FS_OK) {
      fi->size = SL_FI_FILE_SIZE(sl_fi);
    }
    am = SL_FS_READ;
  } else {
    if (!(flags & O_TRUNC) || (flags & O_APPEND)) {
      // FailFS files cannot be opened for append and will be truncated
      // when opened for write.
      return set_errno(ENOTSUP);
    }
    if (flags & O_CREAT) {
      size_t i;
      for (i = 0; i < MAX_OPEN_SLFS_FILES; i++) {
        if (s_sl_file_size_hints[i].name != NULL &&
            strcmp(s_sl_file_size_hints[i].name, pathname) == 0) {
          new_size = s_sl_file_size_hints[i].size;
          MG_FREE(s_sl_file_size_hints[i].name);
          s_sl_file_size_hints[i].name = NULL;
          break;
        }
      }
      am = FS_MODE_OPEN_CREATE(new_size, 0);
    } else {
      am = SL_FS_WRITE;
    }
  }
  fi->fh = slfs_open((_u8 *) pathname, am);
  LOG(LL_DEBUG, ("sl_FsOpen(%s, 0x%x) sz %u = %d", pathname, (int) am,
                 (unsigned int) new_size, (int) fi->fh));
  int r;
  if (fi->fh >= 0) {
    fi->pos = 0;
    r = fd;
  } else {
    r = set_errno(sl_fs_to_errno(fi->fh));
  }
  return r;
}

int fs_slfs_close(int fd) {
  struct sl_fd_info *fi = &s_sl_fds[fd];
  if (fi->fh <= 0) return set_errno(EBADF);
  _i32 r = sl_FsClose(fi->fh, NULL, NULL, 0);
  LOG(LL_DEBUG, ("sl_FsClose(%d) = %d", (int) fi->fh, (int) r));
  s_sl_fds[fd].fh = -1;
  return set_errno(sl_fs_to_errno(r));
}

ssize_t fs_slfs_read(int fd, void *buf, size_t count) {
  struct sl_fd_info *fi = &s_sl_fds[fd];
  if (fi->fh <= 0) return set_errno(EBADF);
  /* Simulate EOF. sl_FsRead @ file_size return SL_FS_ERR_OFFSET_OUT_OF_RANGE.
   */
  if (fi->pos == fi->size) return 0;
  _i32 r = sl_FsRead(fi->fh, fi->pos, buf, count);
  DBG(("sl_FsRead(%d, %d, %d) = %d", (int) fi->fh, (int) fi->pos, (int) count,
       (int) r));
  if (r >= 0) {
    fi->pos += r;
    return r;
  }
  return set_errno(sl_fs_to_errno(r));
}

ssize_t fs_slfs_write(int fd, const void *buf, size_t count) {
  struct sl_fd_info *fi = &s_sl_fds[fd];
  if (fi->fh <= 0) return set_errno(EBADF);
  _i32 r = sl_FsWrite(fi->fh, fi->pos, (_u8 *) buf, count);
  DBG(("sl_FsWrite(%d, %d, %d) = %d", (int) fi->fh, (int) fi->pos, (int) count,
       (int) r));
  if (r >= 0) {
    fi->pos += r;
    return r;
  }
  return set_errno(sl_fs_to_errno(r));
}

int fs_slfs_stat(const char *pathname, struct stat *s) {
  SlFsFileInfo_t sl_fi;
  /*
   * Apply path manipulations again, in case we got here directly
   * (via TI libc's "add_device").
   */
  pathname = drop_dir(pathname, NULL);
  _i32 r = sl_FsGetInfo((const _u8 *) pathname, 0, &sl_fi);
  if (r == SL_FS_OK) {
    s->st_mode = S_IFREG | 0666;
    s->st_nlink = 1;
    s->st_size = SL_FI_FILE_SIZE(sl_fi);
    return 0;
  }
  return set_errno(sl_fs_to_errno(r));
}

int fs_slfs_fstat(int fd, struct stat *s) {
  struct sl_fd_info *fi = &s_sl_fds[fd];
  if (fi->fh <= 0) return set_errno(EBADF);
  s->st_mode = 0666;
  s->st_mode = S_IFREG | 0666;
  s->st_nlink = 1;
  s->st_size = fi->size;
  return 0;
}

off_t fs_slfs_lseek(int fd, off_t offset, int whence) {
  if (s_sl_fds[fd].fh <= 0) return set_errno(EBADF);
  switch (whence) {
    case SEEK_SET:
      s_sl_fds[fd].pos = offset;
      break;
    case SEEK_CUR:
      s_sl_fds[fd].pos += offset;
      break;
    case SEEK_END:
      return set_errno(ENOTSUP);
  }
  return 0;
}

int fs_slfs_unlink(const char *pathname) {
  /*
   * Apply path manipulations again, in case we got here directly
   * (via TI libc's "add_device").
   */
  pathname = drop_dir(pathname, NULL);
  return set_errno(sl_fs_to_errno(sl_FsDel((const _u8 *) pathname, 0)));
}

int fs_slfs_rename(const char *from, const char *to) {
  return set_errno(ENOTSUP);
}

void fs_slfs_set_new_file_size(const char *name, size_t size) {
  int i;
  for (i = 0; i < MAX_OPEN_SLFS_FILES; i++) {
    if (s_sl_file_size_hints[i].name == NULL) {
      DBG(("File size hint: %s %d", name, (int) size));
      s_sl_file_size_hints[i].name = strdup(name);
      s_sl_file_size_hints[i].size = size;
      break;
    }
  }
}

#endif /* defined(MG_FS_SLFS) || defined(CC3200_FS_SLFS) */
