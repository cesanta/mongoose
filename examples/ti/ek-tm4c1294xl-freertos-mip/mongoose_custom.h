#pragma once

#include <errno.h>	// we are not using lwIP

#define MG_ARCH MG_ARCH_FREERTOS
#define MG_ENABLE_MIP 1
#define MG_ENABLE_DRIVER_TM4C 1
#define MG_IO_SIZE 256
