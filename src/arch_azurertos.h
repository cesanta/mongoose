#pragma once

#if MG_ARCH == MG_ARCH_AZURERTOS

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <fx_api.h>
#include <tx_api.h>

#include <nx_api.h>
#include <nx_bsd.h>
#include <nx_port.h>
#include <tx_port.h>

#define PATH_MAX FX_MAXIMUM_PATH
#define MG_DIRSEP '\\'

#define socklen_t int
#define closesocket(x) soc_close(x)

#undef FOPEN_MAX

#endif
