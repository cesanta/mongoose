#pragma once

#include "arch.h"
#include "config.h"

#if MG_ENABLE_TCPIP

// no config defaults to 0 => Ethernet
enum mg_l2type {
  MG_TCPIP_L2_ETH = 0,
  MG_TCPIP_L2_PPP,
  MG_TCPIP_L2_PPPoE,
  MG_TCPIP_L2_LOWPAN,
  MG_TCPIP_L2_USER
};

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
struct lowpan_data {
  uint16_t pan_id;           // User-set PAN ID
  uint8_t channel;           // User-set channel, 11..26
  uint16_t short_addr;       // Our short address, 0xfffe if none
  uint8_t seq;               // MAC sequence number
  uint8_t proto;      // Protocol of the frame being sent, enum mg_l2proto
  uint8_t dst[8];     // Destination of the frame being sent
  uint16_t tag;       // Next fragmentation datagram tag
  uint8_t *buf;       // Reassembly and decompression buffer
  uint16_t size;      // Datagram being reassembled: size, 0 if none
  uint16_t received;  // Bytes received so far
  uint16_t tag_rx;    // Datagram tag
  uint8_t src[8];     // Source address
  bool udp;           // UDP header was compressed
  uint64_t expire;    // Reassembly deadline, ms
};

union mg_l2data {
  struct eth_data eth;
  struct pppoe_data pppoe;
  struct lowpan_data lowpan;
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
