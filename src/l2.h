#pragma once

#include "arch.h"
#include "config.h"

#if MG_ENABLE_TCPIP

// no config defaults to 0 => Ethernet
enum mg_l2type { MG_TCPIP_L2_ETH = 0, MG_TCPIP_L2_PPP, MG_TCPIP_L2_PPPoE };

#if defined(__DCC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct mg_l2addr {
  union {
    uint8_t mac[6];   // ieee48
    uint64_t ieee64;  // PPP ifcid
    uint16_t ieee16;
  } addr;
};

// L2 using L2 (PPPoE) must inherit the lower layer first, so they overlap
struct eth_data {
  uint16_t vlan_id;
};
struct pppoe_data {  // (struct eth_data *) &pppoe_data = pppoedata.eth
  struct eth_data eth;
};

union mg_l2data {
  struct eth_data eth;
  struct pppoe_data pppoe;
};

#if defined(__DCC__)
#pragma pack(0)
#else
#pragma pack(pop)
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
