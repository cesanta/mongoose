/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

void fs_slfs_set_file_size(const char *name, size_t size);
void fs_slfs_set_file_flags(const char *name, uint32_t flags, uint32_t *token);
void fs_slfs_unset_file_flags(const char *name);

#endif /* defined(MG_FS_SLFS) */

#endif /* CS_COMMON_PLATFORMS_SIMPLELINK_SL_FS_SLFS_H_ */
