/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_PLATFORM_CC3200_H_
#define CS_COMMON_PLATFORMS_PLATFORM_CC3200_H_
#if CS_PLATFORM == CS_P_CC3200

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifndef __TI_COMPILER_VERSION__
#include <fcntl.h>
#include <sys/time.h>
#endif

#define MG_NET_IF MG_NET_IF_SIMPLELINK
#define MG_SSL_IF MG_SSL_IF_SIMPLELINK

/* Only SPIFFS supports directories, SLFS does not. */
#if defined(CC3200_FS_SPIFFS) && !defined(MG_ENABLE_DIRECTORY_LISTING)
#define MG_ENABLE_DIRECTORY_LISTING 1
#endif

#include "common/platforms/simplelink/cs_simplelink.h"

typedef int sock_t;
#define INVALID_SOCKET (-1)
#define SIZE_T_FMT "u"
typedef struct stat cs_stat_t;
#define DIRSEP '/'
#define to64(x) strtoll(x, NULL, 10)
#define INT64_FMT PRId64
#define INT64_X_FMT PRIx64
#define __cdecl

#define fileno(x) -1

/* Some functions we implement for Mongoose. */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __TI_COMPILER_VERSION__
struct SlTimeval_t;
#define timeval SlTimeval_t
int gettimeofday(struct timeval *t, void *tz);
int settimeofday(const struct timeval *tv, const void *tz);

int asprintf(char **strp, const char *fmt, ...);

#endif

/* TI's libc does not have stat & friends, add them. */
#ifdef __TI_COMPILER_VERSION__

#include <file.h>

typedef unsigned int mode_t;
typedef size_t _off_t;
typedef long ssize_t;

struct stat {
  int st_ino;
  mode_t st_mode;
  int st_nlink;
  time_t st_mtime;
  off_t st_size;
};

int _stat(const char *pathname, struct stat *st);
int stat(const char *pathname, struct stat *st);

#define __S_IFMT 0170000

#define __S_IFDIR 0040000
#define __S_IFCHR 0020000
#define __S_IFREG 0100000

#define __S_ISTYPE(mode, mask) (((mode) &__S_IFMT) == (mask))

#define S_IFDIR __S_IFDIR
#define S_IFCHR __S_IFCHR
#define S_IFREG __S_IFREG
#define S_ISDIR(mode) __S_ISTYPE((mode), __S_IFDIR)
#define S_ISREG(mode) __S_ISTYPE((mode), __S_IFREG)

/* 5.x series compilers don't have va_copy, 16.x do. */
#if __TI_COMPILER_VERSION__ < 16000000
#define va_copy(apc, ap) ((apc) = (ap))
#endif

#endif /* __TI_COMPILER_VERSION__ */

#ifdef CC3200_FS_SLFS
#define MG_FS_SLFS
#endif

#if (defined(CC3200_FS_SPIFFS) || defined(CC3200_FS_SLFS)) && \
    !defined(MG_ENABLE_FILESYSTEM)
#define MG_ENABLE_FILESYSTEM 1
#define CS_DEFINE_DIRENT
#endif

#ifndef CS_ENABLE_STDIO
#define CS_ENABLE_STDIO 1
#endif

#ifdef __cplusplus
}
#endif

#endif /* CS_PLATFORM == CS_P_CC3200 */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_CC3200_H_ */
