#pragma once

// See https://mongoose.ws/documentation/#build-options

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0
#define MG_ENABLE_BSD_SOCKETS 1
#define MG_ENABLE_LINES 1 
#define MG_ENABLE_CUSTOM_RANDOM 1

#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#if MG_ENABLE_FREERTOS


#else

#define MG_ENABLE_CUSTOM_CALLOC 1
#define MG_ENABLE_CUSTOM_MILLIS 1

#endif
