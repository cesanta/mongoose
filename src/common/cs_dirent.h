
#ifndef CS_COMMON_CS_DIRENT_H_
#define CS_COMMON_CS_DIRENT_H_

#include <limits.h>

#include "common/platform.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef CS_DEFINE_DIRENT
typedef struct { int dummy; } DIR;

struct dirent {
  int d_ino;
#ifdef _WIN32
  char d_name[MAX_PATH];
#else
  /* TODO(rojer): Use PATH_MAX but make sure it's sane on every platform */
  char d_name[256];
#endif
};

DIR *opendir(const char *dir_name);
int closedir(DIR *dir);
struct dirent *readdir(DIR *dir);
#endif /* CS_DEFINE_DIRENT */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_COMMON_CS_DIRENT_H_ */
