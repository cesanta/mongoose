#pragma once

// See https://mongoose.ws/docs/getting-started/build-environment/
#define MG_ARCH MG_ARCH_PICOSDK
#define MG_OTA MG_OTA_PICOSDK
#define MG_TLS MG_TLS_BUILTIN
#define MG_IO_SIZE 2048

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_W5500 1

// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 1, 10)
// #define MG_TCPIP_GW MG_IPV4(192, 168, 1, 1)
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)

// Construct MAC address from the unique board ID
#define MG_SET_MAC_ADDRESS(mac)              \
  do {                                       \
    pico_unique_board_id_t id_;              \
    pico_get_unique_board_id(&id_);          \
    mac[0] = 2, mac[1] = id_.id[3];          \
    mac[2] = id_.id[4], mac[3] = id_.id[5];  \
    mac[4] = id_.id[6], mac[5] = id_.id[7];  \
  } while (0)
