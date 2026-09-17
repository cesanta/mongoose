#define MG_ARCH MG_ARCH_ARMGCC
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_NETC 1
#define MG_OTA MG_OTA_RT1180

#define MG_IO_SIZE 256
#define MG_ENABLE_CUSTOM_MILLIS 1

#define MG_ETH_RAM __attribute__((section(".eth_ram")))

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
