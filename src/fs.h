#pragma once

#include "arch.h"

#ifdef _WIN32
typedef struct _stati64 mg_stat_t;
#else
typedef struct stat mg_stat_t;
#endif
int mg_stat(const char *path, mg_stat_t *);
