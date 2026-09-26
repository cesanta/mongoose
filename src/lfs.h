#pragma once

#if MG_ENABLE_LFS
#include <fcntl.h>
#include <littlefs/lfs.h>

#ifndef DT_DIR
#define DT_DIR 4
#endif
#ifndef DT_REG
#define DT_REG 8
#endif

struct dirent {
  char d_name[LFS_NAME_MAX + 1];
  unsigned char d_type;
};

typedef struct mg_lfs_fd  DIR;
#endif
