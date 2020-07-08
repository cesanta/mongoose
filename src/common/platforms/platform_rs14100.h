
#ifndef CS_COMMON_PLATFORMS_PLATFORM_RS14100_H_
#define CS_COMMON_PLATFORMS_PLATFORM_RS14100_H_
#if CS_PLATFORM == CS_P_RS14100

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef MGOS_HAVE_VFS_COMMON
#include <mgos_vfs.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define to64(x) strtoll(x, NULL, 10)
#define INT64_FMT "lld"
#define SIZE_T_FMT "u"
typedef struct stat cs_stat_t;
#define DIRSEP '/'

#ifndef CS_ENABLE_STDIO
#define CS_ENABLE_STDIO 1
#endif

#ifndef MG_ENABLE_FILESYSTEM
#define MG_ENABLE_FILESYSTEM 1
#endif

#ifdef __cplusplus
}
#endif

#endif /* CS_PLATFORM == CS_P_RS14100 */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_RS14100_H_ */
