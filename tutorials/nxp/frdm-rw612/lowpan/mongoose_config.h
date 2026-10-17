#pragma once

// See https://mongoose.ws/docs/getting-started/build-environment/
#define MG_ARCH MG_ARCH_ARMGCC

#define MG_ENABLE_TCPIP 1
// Compile both drivers; main.c initialises both interfaces itself
#define MG_ENABLE_DRIVER_RW612 1      // Ethernet
#define MG_ENABLE_DRIVER_RW612_154 1  // 802.15.4 radio, 6LoWPAN
#define MG_ENABLE_TCPIP_DRIVER_INIT 0  // Do not start one in mg_mgr_init()
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_TLS MG_TLS_BUILTIN
#define MG_ENABLE_TCPIP_TCPRTX 1  // Radio link can be lossy
