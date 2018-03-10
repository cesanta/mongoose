/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_SIMPLELINK_SL_FS_SLFS_H_
#define CS_COMMON_PLATFORMS_SIMPLELINK_SL_FS_SLFS_H_

#if defined(MG_FS_SLFS)

#include <stdio.h>
#ifndef __TI_COMPILER_VERSION__
#include <unistd.h>
#include <sys/stat.h>
#endif

#define MAX_OPEN_SLFS_FILES 8

/* Indirect libc interface - same functions, different names. */
int fs_slfs_open(const char *pathname, int flags, mode_t mode);
int fs_slfs_close(int fd);
ssize_t fs_slfs_read(int fd, void *buf, size_t count);
ssize_t fs_slfs_write(int fd, const void *buf, size_t count);
int fs_slfs_stat(const char *pathname, struct stat *s);
int fs_slfs_fstat(int fd, struct stat *s);
off_t fs_slfs_lseek(int fd, off_t offset, int whence);
int fs_slfs_unlink(const char *filename);
int fs_slfs_rename(const char *from, const char *to);

void fs_slfs_set_new_file_size(const char *name, size_t size);

#endif /* defined(MG_FS_SLFS) */

#endif /* CS_COMMON_PLATFORMS_SIMPLELINK_SL_FS_SLFS_H_ */
