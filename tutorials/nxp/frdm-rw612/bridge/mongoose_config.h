#pragma once

// See https://mongoose.ws/docs/getting-started/build-environment/
#define MG_ARCH MG_ARCH_ARMGCC

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_RW612 1        // Ethernet
#define MG_ENABLE_DRIVER_RW612W 1 // Mesh bridge over raw 802.15.4
#define MG_ENABLE_CUSTOM_MILLIS 1


// The bridge initializes and polls its two L2 interfaces directly.
#define MG_ENABLE_TCPIP_DRIVER_INIT 0
