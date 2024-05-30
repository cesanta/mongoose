#pragma once

#include <errno.h>	// we are not using lwIP

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_FREERTOS

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_IMXRT 1
#define MG_DRIVER_IMXRT_RT11 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0
#define MG_IO_SIZE 256
#define MG_ENABLE_PACKED_FS 1
#define MG_ENABLE_TCPIP_PRINT_DEBUG_STATS 1
