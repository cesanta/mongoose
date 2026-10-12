#pragma once

#if MG_ARCH == MG_ARCH_ESP32

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <esp_ota_ops.h>   // Use angle brackets to avoid
#include <esp_timer.h>     // amalgamation ditching them
#include <esp_system.h>
#include <esp_mac.h>
#include <esp_idf_version.h>
#if MG_ENABLE_HEALTH
#include <esp_memory_utils.h>
#include <esp_core_dump.h>
#endif

#define MG_PATH_MAX 128

#ifndef MG_ENABLE_POSIX_FS
#define MG_ENABLE_POSIX_FS 1
#endif

#ifndef MG_HEALTH_DATA_SIZE
#define MG_HEALTH_DATA_SIZE 20
#endif

#endif
