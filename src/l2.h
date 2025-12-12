#pragma once

#include "arch.h"
#include "config.h"

#if MG_ENABLE_TCPIP

// no config defaults to 0 => Ethernet
enum mg_l2type { MG_TCPIP_L2_ETH = 0, MG_TCPIP_L2_PPP };  // MG_TCPIP_L2_PPPoE

#if defined(__DCC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct mg_l2addr {
  union {
    uint8_t mac[6];
  } addr;
};

#if defined(__DCC__)
#pragma pack(0)
#else
#pragma pack(pop)
#endif

#if 0
TODO(): ?
struct eth_opts {
  bool enable_crc32_check;         // Do a CRC check on RX frames and strip it
  bool enable_mac_check;           // Do a MAC check on RX frames
};
struct mg_l2opts {
  union {
    struct eth_opts eth;
  };
};
#endif

enum mg_l2proto {
  MG_TCPIP_L2PROTO_IPV4 = 0,
  MG_TCPIP_L2PROTO_IPV6,
  MG_TCPIP_L2PROTO_ARP,
  MG_TCPIP_L2PROTO_PPPoE_DISC,
  MG_TCPIP_L2PROTO_PPPoE_SESS
};
enum mg_l2addrtype {
  MG_TCPIP_L2ADDR_BCAST,
  MG_TCPIP_L2ADDR_MCAST,
  MG_TCPIP_L2ADDR_MCAST6
};

#endif
