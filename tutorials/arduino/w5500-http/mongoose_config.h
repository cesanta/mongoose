#pragma once

#include "Arduino.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#define MG_ARCH MG_ARCH_CUSTOM
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_W5500 1
// Fixed MAC address for the W5500 module
#define MG_SET_MAC_ADDRESS(mac)                       \
  do {                                                \
    mac[0] = 2, mac[1] = 0, mac[2] = 1, mac[3] = 2;   \
    mac[4] = 3, mac[5] = 5;                           \
  } while (0)
#define MG_ENABLE_TCPIP_PRINT_DEBUG_STATS 0
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_TLS MG_TLS_BUILTIN
#define MG_IO_SIZE 256
