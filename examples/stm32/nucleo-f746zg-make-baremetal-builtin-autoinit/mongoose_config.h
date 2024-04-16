#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_NEWLIB

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_PACKED_FS 1 
#define MG_ENABLE_DRIVER_STM32F 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 1
#define MG_ENABLE_LINES 1
#define MG_ENABLE_TCPIP_PRINT_DEBUG_STATS 1

#define MG_UUID ((uint8_t *) 0x1FF0F420U)  // Unique 96-bit chip ID. TRM 41.1

#define MG_MAC_ADDRESS                                                 \
  {                                                                    \
    2, MG_UUID[0] ^ MG_UUID[1], MG_UUID[2] ^ MG_UUID[3],               \
        MG_UUID[4] ^ MG_UUID[5], MG_UUID[6] ^ MG_UUID[7] ^ MG_UUID[8], \
        MG_UUID[9] ^ MG_UUID[10] ^ MG_UUID[11]                         \
  }
