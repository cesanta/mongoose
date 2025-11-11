#include "l2.h"
#include "log.h"
#include "net.h"
#include "net_builtin.h"
#include "printf.h"
#include "util.h"

#if MG_ENABLE_TCPIP

#if defined(__DCC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct eth {
  uint8_t dst[6];  // Destination MAC address
  uint8_t src[6];  // Source MAC address
  uint16_t type;   // Ethernet type
};

#if defined(__DCC__)
#pragma pack(0)
#else
#pragma pack(pop)
#endif

static const uint16_t eth_types[] = {
    // order is vital, see l2.h
    0x800,   // IPv4
    0x86dd,  // IPv6
    0x806,   // ARP
    0x8863,  // PPPoE Discovery Stage
    0x8864   // PPPoE Session Stage
};

void mg_l2_eth_init(struct mg_l2addr *l2addr, uint16_t *mtu,
                    uint16_t *framesize) {
  // If MAC is not set, make a random one
  if (l2addr->addr.mac[0] == 0 && l2addr->addr.mac[1] == 0 &&
      l2addr->addr.mac[2] == 0 && l2addr->addr.mac[3] == 0 &&
      l2addr->addr.mac[4] == 0 && l2addr->addr.mac[5] == 0) {
    l2addr->addr.mac[0] = 0x02;  // Locally administered, unicast
    mg_random(&l2addr->addr.mac[1], sizeof(l2addr->addr.mac) - 1);
    MG_INFO(
        ("MAC not set. Generated random: %M", mg_print_mac, l2addr->addr.mac));
  }
  *mtu = 1500;
  *framesize = 1540;
}

uint8_t *mg_l2_eth_header(enum mg_l2proto proto, struct mg_l2addr *src,
                          struct mg_l2addr *dst, uint8_t *frame) {
  struct eth *eth = (struct eth *) frame;
  eth->type = mg_htons(eth_types[(unsigned int) proto]);
  memcpy(eth->src, src->addr.mac, sizeof(eth->dst));
  memcpy(eth->dst, dst->addr.mac, sizeof(eth->dst));
  return (uint8_t *) (eth + 1);
}

size_t mg_l2_eth_footer(size_t len, uint8_t *frame) {
  struct eth *eth = (struct eth *) frame;
  // nothing to do; there is no len field in Ethernet, CRC is hw-calculated
  return len + sizeof(*eth);
}

struct mg_l2addr *mg_l2_eth_mapip(enum mg_l2addrtype addrtype,
                                  struct mg_addr *addr);

bool mg_l2_eth_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
                  struct mg_str *pay, struct mg_str *raw) {
  struct eth *eth = (struct eth *) raw->buf;
  uint16_t type, len;
  unsigned int i;
  if (raw->len < sizeof(*eth)) return false;  // Truncated - runt?
  len = (uint16_t) raw->len;
  if (ifp->enable_mac_check &&
      memcmp(eth->dst, ifp->mac, sizeof(eth->dst)) != 0 &&
      memcmp(eth->dst, mg_l2_eth_mapip(MG_TCPIP_L2ADDR_BCAST, NULL),
             sizeof(eth->dst)) != 0)
    return false;  // TODO(): add multicast addresses
  if (ifp->enable_crc32_check && len > sizeof(*eth) + 4) {
    uint32_t crc;
    len -= 4;  // TODO(scaprile): check on bigendian
    crc = mg_crc32(0, (const char *) raw->buf, len);
    if (memcmp((void *) ((size_t) raw->buf + len), &crc, sizeof(crc)))
      return false;
  }
  pay->buf = (char *) (eth + 1);
  pay->len = len;

  type = mg_htons(eth->type);
  for (i = 0; i < sizeof(eth_types) / sizeof(uint16_t); i++) {
    if (type == eth_types[i]) break;
  }
  if (i == sizeof(eth_types)) {
    MG_DEBUG(("Unknown eth type %x", type));
    if (mg_log_level >= MG_LL_VERBOSE)
      mg_hexdump(raw->buf, raw->len >= 32 ? 32 : raw->len);
    return false;
  }
  *proto = (enum mg_l2proto) i;
  return true;
}

struct mg_l2addr *mg_l2_eth_getaddr(uint8_t *frame) {
  struct eth *eth = (struct eth *) frame;
  return (struct mg_l2addr *) &eth->src;
}

extern struct mg_l2addr s_mapip;

struct mg_l2addr *mg_l2_eth_mapip(enum mg_l2addrtype addrtype,
                                  struct mg_addr *addr) {
  switch (addrtype) {
    case MG_TCPIP_L2ADDR_BCAST:
      memset(s_mapip.addr.mac, 0xff, sizeof(s_mapip.addr.mac));
      break;
    case MG_TCPIP_L2ADDR_MCAST: {
      uint8_t *ip = (uint8_t *) &addr->addr.ip4;
      // IP multicast group MAC, RFC-1112 6.4
      s_mapip.addr.mac[0] = 0x01, s_mapip.addr.mac[1] = 0x00,
      s_mapip.addr.mac[2] = 0x5E;
      s_mapip.addr.mac[3] = ip[1] & 0x7F;  // 23 LSb
      s_mapip.addr.mac[4] = ip[2];
      s_mapip.addr.mac[5] = ip[3];
      break;
    }
    case MG_TCPIP_L2ADDR_MCAST6: {
      // IPv6 multicast address mapping, RFC-2464 7
      uint8_t *ip = (uint8_t *) &addr->addr.ip6;
      s_mapip.addr.mac[0] = 0x33, s_mapip.addr.mac[1] = 0x33;
      s_mapip.addr.mac[2] = ip[12], s_mapip.addr.mac[3] = ip[13],
      s_mapip.addr.mac[4] = ip[14], s_mapip.addr.mac[5] = ip[15];
      break;
    }
  }
  return &s_mapip;
}

#if MG_ENABLE_IPV6
static void meui64(uint8_t *addr, uint8_t *mac) {
  *addr++ = *mac++ ^ (uint8_t) 0x02, *addr++ = *mac++, *addr++ = *mac++;
  *addr++ = 0xff, *addr++ = 0xfe;
  *addr++ = *mac++, *addr++ = *mac++, *addr = *mac;
}

bool mg_l2_eth_genip6(uint64_t *ip6, uint8_t prefix_len,
                      struct mg_l2addr *l2addr) {
  if (prefix_len > 64) {
    MG_ERROR(("Prefix length > 64, UNSUPPORTED"));
    return false;
  }
  ip6[0] = 0;
  meui64(((uint8_t *) &ip6[1]), l2addr->addr.mac);  // RFC-4291 2.5.4, 2.5.1
  return true;
}

bool mg_l2_eth_ip6get(struct mg_l2addr *l2addr, uint8_t *opts, uint8_t len) {
  if (len != 1) return false;
  memcpy(l2addr->addr.mac, opts, 6);
  return true;
}

uint8_t mg_l2_eth_ip6put(struct mg_l2addr *l2addr, uint8_t *opts) {
  memcpy(opts, l2addr->addr.mac, 6);
  return 1;
}
#endif

#endif
