#pragma once

#include "Arduino.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#define MG_ARCH MG_ARCH_CUSTOM
#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_PPP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0
#define MG_ENABLE_TCPIP_PRINT_DEBUG_STATS 0
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_TLS MG_TLS_BUILTIN
#define MG_IO_SIZE 128
