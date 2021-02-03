#pragma once

#if MG_ARCH == MG_ARCH_ESP32

#include <dirent.h>
#include <netdb.h>
#include <sys/stat.h>
#define MG_DIRSEP '/'
#define MG_INT64_FMT "%lld"
#ifndef MG_PATH_MAX
#define MG_PATH_MAX 128
#endif

#endif
