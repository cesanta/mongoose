#pragma once

#if MG_ARCH == MG_ARCH_RP2040
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pico/stdlib.h>
int mkdir(const char *, mode_t);
#endif
