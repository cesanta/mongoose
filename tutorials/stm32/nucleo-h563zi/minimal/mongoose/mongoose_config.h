#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_ARMGCC
#define MG_OTA MG_OTA_STM32H5
#define MG_TLS MG_TLS_BUILTIN
#define MG_IO_SIZE 2048

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_PACKED_FS 1
#define MG_ENABLE_DRIVER_STM32H 1

#define MG_OTA_PUBLIC_KEY { \
  0x19, 0x7f, 0xed, 0x81, 0x8b, 0xb7, 0xb2, 0xa5, \
  0xde, 0xe0, 0xde, 0x04, 0x83, 0x60, 0xc8, 0x69, \
  0x61, 0x39, 0x30, 0x1f, 0xc1, 0x52, 0x11, 0x4b, \
  0xb7, 0xa4, 0xd7, 0x52, 0x03, 0x36, 0xc6, 0xc1, \
  0xbf, 0x22, 0x75, 0x04, 0x49, 0xf8, 0x46, 0x47, \
  0xec, 0x20, 0xd6, 0x8d, 0x48, 0x29, 0x7d, 0x63, \
  0xb3, 0xf7, 0x1f, 0x58, 0x44, 0x82, 0xcc, 0x7a, \
  0x4b, 0x5e, 0x68, 0x98, 0x24, 0x1f, 0x86, 0xf4, \
}

// #define MG_DRIVER_MDC_CR 4   // RMII MDC clock divider, from 0 to 4
// #define MG_TCPIP_PHY_ADDR 0  // PHY address

// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 0, 10)
// #define MG_TCPIP_GW MG_IPV4(192, 168, 0, 1)
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)

// Set custom MAC address. By default, it is randomly generated
// Using a build-time constant:
// #define MG_SET_MAC_ADDRESS(mac) do { mac[0] = 2; mac[1] = 3; mac[2] = 4; mac[3] = 5; mac[4] = 6; mac[5] = 7; } while (0)
//
// Using custom function:
// extern void my_function(unsigned char *mac);
// #define MG_SET_MAC_ADDRESS(mac) my_function(mac)

// Construct MAC address from the MCU unique ID. It is defined in the
// ST CMSIS header as UID_BASE
#define MGUID ((uint32_t *) 0x08fff800)  // Unique 96-bit chip ID
#define MG_SET_MAC_ADDRESS(mac)      \
  do {                               \
    mac[0] = 2;                      \
    mac[1] = MGUID[0] & 255;         \
    mac[2] = (MGUID[0] >> 10) & 255; \
    mac[3] = (MGUID[0] >> 19) & 255; \
    mac[4] = MGUID[1] & 255;         \
    mac[5] = MGUID[2] & 255;         \
  } while (0)
