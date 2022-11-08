#pragma once

#if MG_ARCH == MG_ARCH_RTX

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if !defined MG_ENABLE_RL && (!defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP)
#define MG_ENABLE_RL 1
#endif

#endif
