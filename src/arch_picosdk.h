#pragma once

#if MG_ARCH == MG_ARCH_PICOSDK
#if !defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP
#include <errno.h>
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pico/stdlib.h>
#include <pico/rand.h>
int mkdir(const char *, mode_t);

#if MG_OTA == MG_OTA_PICOSDK
#include <hardware/flash.h>
#include <pico/bootrom.h>
#endif

#endif
