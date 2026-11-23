#pragma once

#define MG_ARCH MG_ARCH_ARMGCC
#define MG_TLS MG_TLS_BUILTIN
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_IPV6 0
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_DRIVER_USB_NRF52 1  // nRF52840 USB controller
#define MG_ENABLE_USB_ETHERNET 1      // USB NCM Ethernet interface
#define MG_ENABLE_USB_SERIAL 1        // USB serial log console

// Static device address; the driver serves DHCP to the USB host. Remove
// these to get an address over DHCP instead, e.g. from Internet Sharing
#define MG_TCPIP_IP MG_IPV4(192, 168, 7, 1)
#define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)
#define MG_SET_MAC_ADDRESS(mac) \
  do { mac[0] = 2; mac[1] = 3; mac[2] = 4; mac[3] = 5; mac[4] = 6; mac[5] = 7; } while (0)
