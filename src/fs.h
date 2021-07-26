#pragma once

#include "arch.h"

FILE *mg_fopen_packed(const char *path, const char *mode);

#if defined(__linux__) && defined(GCC)
#define MG_ENABLE_PACKED_FS 1
#define MG_FOPENCOOKIE
#else
#define MG_ENABLE_PACKED_FS 0
#endif
