#pragma once
#define MG_ARCH MG_ARCH_CUBE
#define MG_ENABLE_HEALTH 1    // Mem stats and crash reports
#define MG_ENABLE_FREERTOS 1  // Set configRECORD_STACK_HIGH_ADDRESS

#define MG_ENABLE_MDASH 0   // mdash.net device management service support
#define MG_MDASH_KEY "..."  // See https://mongoose.ws/docs/guides/mdash/
#define MG_FIRMWARE_VERSION "h723_cube.1.0.0"

// Ethernet driver tunables:
// #define MG_DRIVER_MDC_CR 4   // RMII MDC clock divider, from 0 to 5
// #define MG_TCPIP_PHY_ADDR 0  // PHY address

// Uncomment this for static IP configuration:
// #define MG_TCPIP_IP MG_IPV4(192, 168, 0, 10)     // IP
// #define MG_TCPIP_GW MG_IPV4(192, 168, 0, 1)      // Gateway
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)  // Netmask
