#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_ARMGCC
#define MG_TLS MG_TLS_BUILTIN
#define MG_OTA MG_OTA_STM32H7_DUAL_CORE

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_PACKED_FS 1 
#define MG_ENABLE_DRIVER_CYW_SDIO 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0
