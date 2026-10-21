#pragma once

// See https://mongoose.ws/docs/getting-started/build-environment/
#define MG_ARCH MG_ARCH_ARMGCC

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_RW612 1       // Ethernet
#define MG_ENABLE_DRIVER_RW612_154 1   // 802.15.4 radio
#define MG_ENABLE_TCPIP_DRIVER_INIT 0  // main.c drives both, no TCP/IP stack
#define MG_ENABLE_CUSTOM_MILLIS 1
