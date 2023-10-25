#pragma once

#include <errno.h>	// we are not using lwIP

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_FREERTOS
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_RT1020 1
#define MG_IO_SIZE 256
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_PACKED_FS 1

