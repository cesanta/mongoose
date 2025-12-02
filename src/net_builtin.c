#include "net_builtin.h"
#include "profile.h"

#if MG_ENABLE_TCPIP
#define MG_EPHEMERAL_PORT_BASE 32768
#define PDIFF(a, b) ((size_t) (((char *) (b)) - ((char *) (a))))

#ifndef MIP_TCP_KEEPALIVE_MS
#define MIP_TCP_KEEPALIVE_MS 45000  // TCP keep-alive period, ms
#endif

#define MIP_TCP_ACK_MS 150    // Timeout for ACKing
#define MIP_ARP_RESP_MS 100   // Timeout for ARP response
#define MIP_TCP_SYN_MS 15000  // Timeout for connection establishment
#define MIP_TCP_FIN_MS 1000   // Timeout for closing connection
#define MIP_TCP_WIN 6000      // TCP window size

struct connstate {
  uint32_t seq, ack;           // TCP seq/ack counters
  uint64_t timer;              // TCP timer (see 'ttype' below)
  uint32_t acked;              // Last ACK-ed number
  size_t unacked;              // Not acked bytes
  uint16_t dmss;               // destination MSS (from TCP opts)
  uint8_t mac[6];              // Peer MAC address
  uint8_t ttype;               // Timer type:
#define MIP_TTYPE_KEEPALIVE 0  // Connection is idle for long, send keepalive
#define MIP_TTYPE_ACK 1        // Peer sent us data, we have to ack it soon
#define MIP_TTYPE_ARP 2        // ARP resolve sent, waiting for response
#define MIP_TTYPE_SYN 3        // SYN sent, waiting for response
#define MIP_TTYPE_FIN 4  // FIN sent, waiting until terminating the connection
  uint8_t tmiss;         // Number of keep-alive misses
  struct mg_iobuf raw;   // For TLS only. Incoming raw data
  bool fin_rcvd;         // We have received FIN from the peer
  bool twclosure;        // 3-way closure done
};

#if defined(__DCC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct lcp {
  uint8_t addr, ctrl, proto[2], code, id, len[2];
};

struct eth {
  uint8_t dst[6];  // Destination MAC address
  uint8_t src[6];  // Source MAC address
  uint16_t type;   // Ethernet type
};

struct ip {
  uint8_t ver;    // Version
  uint8_t tos;    // Unused
  uint16_t len;   // Datagram length
  uint16_t id;    // Unused
  uint16_t frag;  // Fragmentation
#define IP_FRAG_OFFSET_MSK 0x1fff
#define IP_MORE_FRAGS_MSK 0x2000
  uint8_t ttl;    // Time to live
  uint8_t proto;  // Upper level protocol
  uint16_t csum;  // Checksum
  uint32_t src;   // Source IP
  uint32_t dst;   // Destination IP
};

struct ip6 {
  uint8_t ver;       // Version
  uint8_t label[3];  // Flow label
  uint16_t plen;     // Payload length
  uint8_t next;      // Upper level protocol
  uint8_t hops;      // Hop limit
  uint64_t src[2];   // Source IP
  uint64_t dst[2];   // Destination IP
};

struct icmp {
  uint8_t type;
  uint8_t code;
  uint16_t csum;
};

struct icmp6 {
  uint8_t type;
  uint8_t code;
  uint16_t csum;
};

struct ndp_na {
  uint8_t res[4];    // R S O, reserved
  uint64_t addr[2];  // Target address
};

struct ndp_ra {
  uint8_t cur_hop_limit;
  uint8_t flags;  // M,O,Prf,Resvd
  uint16_t router_lifetime;
  uint32_t reachable_time;
  uint32_t retrans_timer;
};

struct arp {
  uint16_t fmt;    // Format of hardware address
  uint16_t pro;    // Format of protocol address
  uint8_t hlen;    // Length of hardware address
  uint8_t plen;    // Length of protocol address
  uint16_t op;     // Operation
  uint8_t sha[6];  // Sender hardware address
  uint32_t spa;    // Sender protocol address
  uint8_t tha[6];  // Target hardware address
  uint32_t tpa;    // Target protocol address
};

struct tcp {
  uint16_t sport;  // Source port
  uint16_t dport;  // Destination port
  uint32_t seq;    // Sequence number
  uint32_t ack;    // Acknowledgement number
  uint8_t off;     // Data offset
  uint8_t flags;   // TCP flags
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
  uint16_t win;   // Window
  uint16_t csum;  // Checksum
  uint16_t urp;   // Urgent pointer
};

struct udp {
  uint16_t sport;  // Source port
  uint16_t dport;  // Destination port
  uint16_t len;    // UDP length
  uint16_t csum;   // UDP checksum
};

struct dhcp {
  uint8_t op, htype, hlen, hops;
  uint32_t xid;
  uint16_t secs, flags;
  uint32_t ciaddr, yiaddr, siaddr, giaddr;
  uint8_t hwaddr[208];
  uint32_t magic;
  uint8_t options[30 + sizeof(((struct mg_tcpip_if *) 0)->dhcp_name)];
};

struct dhcp6 {
  union {
    uint8_t type;
    uint32_t xid;
  };
  uint8_t options[30 + sizeof(((struct mg_tcpip_if *) 0)->dhcp_name)];
};

#if defined(__DCC__)
#pragma pack(0)
#else
#pragma pack(pop)
#endif

// pkt is 8-bit aligned, pointers to headers hint compilers to generate
// byte-copy code for micros with alignment constraints
struct pkt {
  struct mg_str raw;  // Raw packet data
  struct mg_str pay;  // Payload data
  struct eth *eth;
  struct llc *llc;
  struct arp *arp;
  struct ip *ip;
  struct ip6 *ip6;
  struct icmp *icmp;
  struct icmp6 *icmp6;
  struct tcp *tcp;
  struct udp *udp;
  struct dhcp *dhcp;
  struct dhcp6 *dhcp6;
};

static void mg_tcpip_call(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
#if MG_ENABLE_PROFILE
  const char *names[] = {"TCPIP_EV_ST_CHG",        "TCPIP_EV_DHCP_DNS",
                         "TCPIP_EV_DHCP_SNTP",     "TCPIP_EV_ARP",
                         "TCPIP_EV_TIMER_1S",      "TCPIP_EV_WIFI_SCAN_RESULT",
                         "TCPIP_EV_WIFI_SCAN_END", "TCPIP_EV_WIFI_CONNECT_ERR",
                         "TCPIP_EV_DRIVER",        "TCPIP_EV_USER"};
  if (ev != MG_TCPIP_EV_POLL && ev < (int) (sizeof(names) / sizeof(names[0]))) {
    MG_PROF_ADD(c, names[ev]);
  }
#endif
  // Fire protocol handler first, user handler second. See #2559
  if (ifp->pfn != NULL) ifp->pfn(ifp, ev, ev_data);
  if (ifp->fn != NULL) ifp->fn(ifp, ev, ev_data);
}

static void send_syn(struct mg_connection *c);

static void mkpay(struct pkt *pkt, void *p) {
  pkt->pay =
      mg_str_n((char *) p, (size_t) (&pkt->pay.buf[pkt->pay.len] - (char *) p));
}

static uint32_t csumup(uint32_t sum, const void *buf, size_t len) {
  size_t i;
  const uint8_t *p = (const uint8_t *) buf;
  for (i = 0; i < len; i++) sum += i & 1 ? p[i] : ((uint32_t) p[i]) << 8;
  return sum;
}

static uint16_t csumfin(uint32_t sum) {
  while (sum >> 16) sum = (sum & 0xffff) + (sum >> 16);
  return mg_htons((uint16_t) ((uint16_t) ~sum & 0xffff));
}

static uint16_t ipcsum(const void *buf, size_t len) {
  uint32_t sum = csumup(0, buf, len);
  return csumfin(sum);
}

#if MG_ENABLE_IPV6
static void meui64(uint8_t *addr, uint8_t *mac) {
  *addr++ = *mac++ ^ (uint8_t) 0x02, *addr++ = *mac++, *addr++ = *mac++;
  *addr++ = 0xff, *addr++ = 0xfe;
  *addr++ = *mac++, *addr++ = *mac++, *addr = *mac;
}
static void ip6gen(uint8_t *addr, uint8_t *prefix, uint8_t *mac) {
  memcpy(addr, prefix, 8);  // RFC-4291 2.5.4
  meui64(addr + 8, mac);    // 2.5.1
}
static void ip6genll(uint8_t *addr, uint8_t *mac) {
  uint8_t prefix[8] = {0xfe, 0x80, 0, 0, 0, 0, 0, 0};  // RFC-4291 2.5.6
  ip6gen(addr, prefix, mac);                           // 2.5.1
}
static void ip6sn(uint64_t *addr, uint64_t *sn_addr) {
  // Build solicited-node multicast address from a given unicast IP
  // RFC-4291 2.7
  uint8_t *sn = (uint8_t *) sn_addr;
  memset(sn_addr, 0, 16);
  sn[0] = 0xff;
  sn[1] = 0x02;
  sn[11] = 0x01;
  sn[12] = 0xff;
  sn[13] = ((uint8_t *) addr)[13];
  sn[14] = ((uint8_t *) addr)[14];
  sn[15] = ((uint8_t *) addr)[15];
}
static void ip6_mcastmac(uint8_t *mac, uint64_t *ip6) {
  // Build multicast MAC address from a solicited-node
  // multicast IPv6 address, RFC-4291 2.7
  uint8_t *ip = (uint8_t *) ip6;
  mac[0] = 0x33;
  mac[1] = 0x33;
  mac[2] = ip[12];
  mac[3] = ip[13];
  mac[4] = ip[14];
  mac[5] = ip[15];
}

union ip6addr {
  uint64_t u[2];
  uint8_t b[16];
};

static const union ip6addr ip6_allrouters = {
    .b = {0xFF, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x02}};
static const union ip6addr ip6_allnodes = {
    .b = {0xFF, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x01}};
static const uint8_t ip6mac_allnodes[6] = {0x33, 0x33, 0x00, 0x00, 0x00, 0x01};
static const uint8_t ip6mac_allrouters[6] = {0x33, 0x33, 0x00,
                                             0x00, 0x00, 0x02};

#define MG_IP6MATCH(a, b) (a[0] == b[0] && a[1] == b[1])
#endif

static void settmout(struct mg_connection *c, uint8_t type) {
  struct mg_tcpip_if *ifp = c->mgr->ifp;
  struct connstate *s = (struct connstate *) (c + 1);
  unsigned n = type == MIP_TTYPE_ACK   ? MIP_TCP_ACK_MS
               : type == MIP_TTYPE_ARP ? MIP_ARP_RESP_MS
               : type == MIP_TTYPE_SYN ? MIP_TCP_SYN_MS
               : type == MIP_TTYPE_FIN ? MIP_TCP_FIN_MS
                                       : MIP_TCP_KEEPALIVE_MS;
  if (s->ttype == MIP_TTYPE_FIN) return;  // skip if 3-way closing
  s->timer = ifp->now + n;
  s->ttype = type;
  MG_VERBOSE(("%lu %d -> %llx", c->id, type, s->timer));
}

static size_t ether_output(struct mg_tcpip_if *ifp, size_t len) {
  size_t n = ifp->driver->tx(ifp->tx.buf, len, ifp);
  if (n == len) ifp->nsent++;
  return n;
}

void mg_tcpip_arp_request(struct mg_tcpip_if *ifp, uint32_t ip, uint8_t *mac) {
  struct eth *eth = (struct eth *) ifp->tx.buf;
  struct arp *arp = (struct arp *) (eth + 1);
  memset(eth->dst, 255, sizeof(eth->dst));
  memcpy(eth->src, ifp->mac, sizeof(eth->src));
  eth->type = mg_htons(0x806);
  memset(arp, 0, sizeof(*arp));
  arp->fmt = mg_htons(1), arp->pro = mg_htons(0x800), arp->hlen = 6,
  arp->plen = 4;
  arp->op = mg_htons(1), arp->tpa = ip, arp->spa = ifp->ip;
  memcpy(arp->sha, ifp->mac, sizeof(arp->sha));
  if (mac != NULL) memcpy(arp->tha, mac, sizeof(arp->tha));
  ether_output(ifp, PDIFF(eth, arp + 1));
}

static void onstatechange(struct mg_tcpip_if *ifp) {
  if (ifp->state == MG_TCPIP_STATE_READY) {
    MG_INFO(("READY, IP: %M", mg_print_ip4, &ifp->ip));
    MG_INFO(("       GW: %M", mg_print_ip4, &ifp->gw));
    MG_INFO(("      MAC: %M", mg_print_mac, &ifp->mac));
  } else if (ifp->state == MG_TCPIP_STATE_IP) {
    if (ifp->gw != 0)
      mg_tcpip_arp_request(ifp, ifp->gw, NULL);  // unsolicited GW ARP request
  } else if (ifp->state == MG_TCPIP_STATE_UP) {
    srand((unsigned int) mg_millis());
  } else if (ifp->state == MG_TCPIP_STATE_DOWN) {
    MG_ERROR(("Link down"));
  }
  mg_tcpip_call(ifp, MG_TCPIP_EV_ST_CHG, &ifp->state);
}

static struct ip *tx_ip(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                        uint8_t proto, uint32_t ip_src, uint32_t ip_dst,
                        size_t plen) {
  // ifp->tx.buf is 8-bit aligned, keep other headers as pointers, see pkt
  struct eth *eth = (struct eth *) ifp->tx.buf;
  struct ip *ip = (struct ip *) (eth + 1);
  memcpy(eth->dst, mac_dst, sizeof(eth->dst));
  memcpy(eth->src, ifp->mac, sizeof(eth->src));  // Use our MAC
  eth->type = mg_htons(0x800);
  memset(ip, 0, sizeof(*ip));
  ip->ver = 0x45;               // Version 4, header length 5 words
  ip->frag = mg_htons(0x4000);  // Don't fragment
  ip->len = mg_htons((uint16_t) (sizeof(*ip) + plen));
  ip->ttl = 64;
  ip->proto = proto;
  ip->src = ip_src;
  ip->dst = ip_dst;
  ip->csum = ipcsum(ip, sizeof(*ip));
  return ip;
}

#if MG_ENABLE_IPV6
static struct ip6 *tx_ip6(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                          uint8_t next, uint64_t *ip_src, uint64_t *ip_dst,
                          size_t plen);
#endif

static bool tx_udp(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                   struct mg_addr *ip_src, struct mg_addr *ip_dst,
                   const void *buf, size_t len) {
  struct ip *ip = NULL;
  struct udp *udp;
  size_t eth_len;
  uint32_t cs;
#if MG_ENABLE_IPV6
  struct ip6 *ip6 = NULL;
  if (ip_dst->is_ip6) {
    ip6 = tx_ip6(ifp, mac_dst, 17, ip_src->addr.ip6, ip_dst->addr.ip6,
                 len + sizeof(struct udp));
    udp = (struct udp *) (ip6 + 1);
    eth_len = sizeof(struct eth) + sizeof(*ip6) + sizeof(*udp) + len;
  } else
#endif
  {
    ip = tx_ip(ifp, mac_dst, 17, ip_src->addr.ip4, ip_dst->addr.ip4,
               len + sizeof(struct udp));
    udp = (struct udp *) (ip + 1);
    eth_len = sizeof(struct eth) + sizeof(*ip) + sizeof(*udp) + len;
  }
  udp->sport = ip_src->port;
  udp->dport = ip_dst->port;
  udp->len = mg_htons((uint16_t) (sizeof(*udp) + len));
  udp->csum = 0;
  cs = csumup(0, udp, sizeof(*udp));
  cs = csumup(cs, buf, len);
#if MG_ENABLE_IPV6
  if (ip_dst->is_ip6) {
    cs = csumup(cs, &ip6->src, sizeof(ip6->src));
    cs = csumup(cs, &ip6->dst, sizeof(ip6->dst));
  } else
#endif
  {
    cs = csumup(cs, &ip->src, sizeof(ip->src));
    cs = csumup(cs, &ip->dst, sizeof(ip->dst));
  }
  cs += (uint32_t) (17 + sizeof(*udp) + len);
  udp->csum = csumfin(cs);
  memmove(udp + 1, buf, len);
  return (ether_output(ifp, eth_len) == eth_len);
}

static bool tx_udp4(struct mg_tcpip_if *ifp, uint8_t *mac_dst, uint32_t ip_src,
                    uint16_t sport, uint32_t ip_dst, uint16_t dport,
                    const void *buf, size_t len) {
  struct mg_addr ips, ipd;
  memset(&ips, 0, sizeof(ips));
  ips.addr.ip4 = ip_src;
  ips.port = sport;
  memset(&ipd, 0, sizeof(ipd));
  ipd.addr.ip4 = ip_dst;
  ipd.port = dport;
  return tx_udp(ifp, mac_dst, &ips, &ipd, buf, len);
}

static void tx_dhcp(struct mg_tcpip_if *ifp, uint8_t *mac_dst, uint32_t ip_src,
                    uint32_t ip_dst, uint8_t *opts, size_t optslen,
                    bool ciaddr) {
  // https://datatracker.ietf.org/doc/html/rfc2132#section-9.6
  struct dhcp dhcp = {1, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, {0}, 0, {0}};
  dhcp.magic = mg_htonl(0x63825363);
  memcpy(&dhcp.hwaddr, ifp->mac, sizeof(ifp->mac));
  memcpy(&dhcp.xid, ifp->mac + 2, sizeof(dhcp.xid));
  memcpy(&dhcp.options, opts, optslen);
  if (ciaddr) dhcp.ciaddr = ip_src;
  tx_udp4(ifp, mac_dst, ip_src, mg_htons(68), ip_dst, mg_htons(67), &dhcp,
          sizeof(dhcp));
}

static const uint8_t broadcast[] = {255, 255, 255, 255, 255, 255};

// RFC-2131 #4.3.6, #4.4.1; RFC-2132 #9.8
static void tx_dhcp_request_sel(struct mg_tcpip_if *ifp, uint32_t ip_req,
                                uint32_t ip_srv) {
  uint8_t extra = (uint8_t) ((ifp->enable_req_dns ? 1 : 0) +
                             (ifp->enable_req_sntp ? 1 : 0));
  size_t len = strlen(ifp->dhcp_name);
  size_t olen = 21 + len + extra + 2 + 1;  // Total length of options
#define OPTS_MAXLEN (21 + sizeof(ifp->dhcp_name) + 2 + 2 + 1)
  uint8_t opts[OPTS_MAXLEN];  // Allocate options (max size possible)
  uint8_t *p = opts;
  assert(olen <= sizeof(opts));
  memset(opts, 0, sizeof(opts));
  *p++ = 53, *p++ = 1, *p++ = 3;                       // Type: DHCP request
  *p++ = 54, *p++ = 4, memcpy(p, &ip_srv, 4), p += 4;  // DHCP server ID
  *p++ = 50, *p++ = 4, memcpy(p, &ip_req, 4), p += 4;  // Requested IP
  *p++ = 12, *p++ = (uint8_t) (len & 255);             // DHCP host
  memcpy(p, ifp->dhcp_name, len), p += len;            // name
  *p++ = 55, *p++ = 2 + extra, *p++ = 1, *p++ = 3;     // GW, MASK
  if (ifp->enable_req_dns) *p++ = 6;                   // DNS
  if (ifp->enable_req_sntp) *p++ = 42;                 // SNTP
  *p++ = 255;                                          // End of options
  // assert((size_t) (p - opts) < olen);
  tx_dhcp(ifp, (uint8_t *) broadcast, 0, 0xffffffff, opts, olen, 0);
  MG_DEBUG(("DHCP req sent"));
}

// RFC-2131 #4.3.6, #4.4.5 (renewing: unicast, rebinding: bcast)
static void tx_dhcp_request_re(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                               uint32_t ip_src, uint32_t ip_dst) {
  uint8_t opts[] = {
      53, 1, 3,  // Type: DHCP request
      255        // End of options
  };
  tx_dhcp(ifp, mac_dst, ip_src, ip_dst, opts, sizeof(opts), true);
  MG_DEBUG(("DHCP req sent"));
}

static void tx_dhcp_discover(struct mg_tcpip_if *ifp) {
  uint8_t opts[] = {
      53, 1, 1,     // Type: DHCP discover
      55, 2, 1, 3,  // Parameters: ip, mask
      255           // End of options
  };
  tx_dhcp(ifp, (uint8_t *) broadcast, 0, 0xffffffff, opts, sizeof(opts), false);
  MG_DEBUG(("DHCP discover sent. Our MAC: %M", mg_print_mac, ifp->mac));
}

static struct mg_connection *getpeer(struct mg_mgr *mgr, struct pkt *pkt,
                                     bool lsn) {
  struct mg_connection *c = NULL;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->is_arplooking && pkt->arp && pkt->arp->spa == c->rem.addr.ip4) break;
#if MG_ENABLE_IPV6
    if (c->is_arplooking && pkt->icmp6 && pkt->icmp6->type == 136) {
      struct ndp_na *na = (struct ndp_na *) (pkt->icmp6 + 1);
      if (MG_IP6MATCH(na->addr, c->rem.addr.ip6)) break;
    }
#endif
    if (c->is_udp && pkt->udp && c->loc.port == pkt->udp->dport &&
        (!c->loc.is_ip6 || pkt->ip6))
      break;
    if (!c->is_udp && pkt->tcp && c->loc.port == pkt->tcp->dport &&
        (!c->loc.is_ip6 || pkt->ip6) && lsn == (bool) c->is_listening &&
        (lsn || c->rem.port == pkt->tcp->sport))
      break;
  }
  return c;
}

static void mac_resolved(struct mg_connection *c);
static uint8_t *get_return_mac(struct mg_tcpip_if *ifp, struct mg_addr *rem,
                               bool is_udp, struct pkt *pkt);

static void rx_arp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  if (pkt->arp->op == mg_htons(1) && pkt->arp->tpa == ifp->ip) {
    // ARP request. Make a response, then send
    // MG_VERBOSE(("ARP req from %M", mg_print_ip4, &pkt->arp->spa));
    struct eth *eth = (struct eth *) ifp->tx.buf;
    struct arp *arp = (struct arp *) (eth + 1);
    memcpy(eth->dst, pkt->eth->src, sizeof(eth->dst));
    memcpy(eth->src, ifp->mac, sizeof(eth->src));
    eth->type = mg_htons(0x806);
    *arp = *pkt->arp;
    arp->op = mg_htons(2);
    memcpy(arp->tha, pkt->arp->sha, sizeof(pkt->arp->tha));
    memcpy(arp->sha, ifp->mac, sizeof(pkt->arp->sha));
    arp->tpa = pkt->arp->spa;
    arp->spa = ifp->ip;
    MG_DEBUG(("ARP: tell %M we're %M", mg_print_ip4, &arp->tpa, mg_print_mac,
              &ifp->mac));
    ether_output(ifp, PDIFF(eth, arp + 1));
  } else if (pkt->arp->op == mg_htons(2)) {
    if (memcmp(pkt->arp->tha, ifp->mac, sizeof(pkt->arp->tha)) != 0) return;
    // MG_VERBOSE(("ARP resp from %M", mg_print_ip4, &pkt->arp->spa));
    if (pkt->arp->spa == ifp->gw) {
      // Got response for the GW ARP request. Set ifp->gwmac and IP -> READY
      memcpy(ifp->gwmac, pkt->arp->sha, sizeof(ifp->gwmac));
      ifp->gw_ready = true;
      if (ifp->state == MG_TCPIP_STATE_IP) {
        ifp->state = MG_TCPIP_STATE_READY;
        onstatechange(ifp);
      }
    } else {
      struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
      if (c != NULL && c->is_arplooking) {
        struct connstate *s = (struct connstate *) (c + 1);
        memcpy(s->mac, pkt->arp->sha, sizeof(s->mac));
        MG_DEBUG(("%lu ARP resolved %M -> %M", c->id, mg_print_ip4,
                  c->rem.addr.ip, mg_print_mac, s->mac));
        c->is_arplooking = 0;
        mac_resolved(c);
      }
    }
  }
}

static void rx_icmp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  if (pkt->icmp->type == 8 && pkt->ip != NULL && pkt->ip->dst == ifp->ip) {
    size_t hlen = sizeof(struct eth) + sizeof(struct ip) + sizeof(struct icmp);
    size_t space = ifp->tx.len - hlen, plen = pkt->pay.len;
    struct ip *ip;
    struct icmp *icmp;
    struct mg_addr ips;
    ips.addr.ip4 = pkt->ip->src;
    ips.is_ip6 = false;
    if (get_return_mac(ifp, &ips, false, pkt) == NULL)
      return;  // safety net for lousy networks
    if (plen > space) plen = space;
    ip = tx_ip(ifp, pkt->eth->src, 1, ifp->ip, pkt->ip->src,
               sizeof(*icmp) + plen);
    icmp = (struct icmp *) (ip + 1);
    memset(icmp, 0, sizeof(*icmp));        // Set csum, type, code to 0
    memcpy(icmp + 1, pkt->pay.buf, plen);  // Copy RX payload to TX
    icmp->csum = ipcsum(icmp, sizeof(*icmp) + plen);
    ether_output(ifp, hlen + plen);
  }
}

static void setdns4(struct mg_tcpip_if *ifp, uint32_t *ip);

static void rx_dhcp_client(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  uint32_t ip = 0, gw = 0, mask = 0, lease = 0, dns = 0, sntp = 0;
  uint8_t msgtype = 0, state = ifp->state;
  // perform size check first, then access fields
  uint8_t *p = pkt->dhcp->options,
          *end = (uint8_t *) &pkt->pay.buf[pkt->pay.len];
  if (end < p) return;  // options are optional, check min header length
  if (memcmp(&pkt->dhcp->xid, ifp->mac + 2, sizeof(pkt->dhcp->xid))) return;
  while (p + 1 < end && p[0] != 255) {  // Parse options RFC-1533 #9
    if (p[0] == 1 && p[1] == sizeof(ifp->mask) && p + 6 < end) {  // Mask
      memcpy(&mask, p + 2, sizeof(mask));
    } else if (p[0] == 3 && p[1] == sizeof(ifp->gw) && p + 6 < end) {  // GW
      memcpy(&gw, p + 2, sizeof(gw));
      ip = pkt->dhcp->yiaddr;
    } else if (ifp->enable_req_dns && p[0] == 6 && p[1] == sizeof(dns) &&
               p + 6 < end) {  // DNS
      memcpy(&dns, p + 2, sizeof(dns));
    } else if (ifp->enable_req_sntp && p[0] == 42 && p[1] == sizeof(sntp) &&
               p + 6 < end) {  // SNTP
      memcpy(&sntp, p + 2, sizeof(sntp));
    } else if (p[0] == 51 && p[1] == 4 && p + 6 < end) {  // Lease
      memcpy(&lease, p + 2, sizeof(lease));
      lease = mg_ntohl(lease);
    } else if (p[0] == 53 && p[1] == 1 && p + 6 < end) {  // Msg Type
      msgtype = p[2];
    }
    p += p[1] + 2;
  }
  // Process message type, RFC-1533 (9.4); RFC-2131 (3.1, 4)
  if (msgtype == 6 && ifp->ip == ip) {  // DHCPNACK, release IP
    ifp->state = MG_TCPIP_STATE_UP, ifp->ip = 0;
  } else if (msgtype == 2 && ifp->state == MG_TCPIP_STATE_UP && ip && gw &&
             lease) {  // DHCPOFFER
    // select IP, (4.4.1) (fallback to IP source addr on foul play)
    tx_dhcp_request_sel(ifp, ip,
                        pkt->dhcp->siaddr ? pkt->dhcp->siaddr : pkt->ip->src);
    ifp->state = MG_TCPIP_STATE_REQ;  // REQUESTING state
  } else if (msgtype == 5) {          // DHCPACK
    if (ifp->state == MG_TCPIP_STATE_REQ && ip && gw && lease) {  // got an IP
      uint64_t rand;
      ifp->lease_expire = ifp->now + lease * 1000;
      MG_INFO(("Lease: %u sec (%lld)", lease, ifp->lease_expire / 1000));
      // assume DHCP server = router until ARP resolves
      memcpy(ifp->gwmac, pkt->eth->src, sizeof(ifp->gwmac));
      ifp->gw_ready = true;  // NOTE(): actual gw ARP won't retry now
      ifp->ip = ip, ifp->gw = gw, ifp->mask = mask;
      ifp->state = MG_TCPIP_STATE_IP;  // BOUND state
      mg_random(&rand, sizeof(rand));
      srand((unsigned int) (rand + mg_millis()));
      if (ifp->enable_req_dns && dns != 0) {
        setdns4(ifp, &dns);
        mg_tcpip_call(ifp, MG_TCPIP_EV_DHCP_DNS, &dns);
      }
      if (ifp->enable_req_sntp && sntp != 0)
        mg_tcpip_call(ifp, MG_TCPIP_EV_DHCP_SNTP, &sntp);
    } else if (ifp->state == MG_TCPIP_STATE_READY && ifp->ip == ip) {  // renew
      ifp->lease_expire = ifp->now + lease * 1000;
      MG_INFO(("Lease: %u sec (%lld)", lease, ifp->lease_expire / 1000));
    }  // TODO(): accept provided T1/T2 and store server IP for renewal (4.4)
  }
  if (ifp->state != state) onstatechange(ifp);
}

// Simple DHCP server that assigns a next IP address: ifp->ip + 1
static void rx_dhcp_server(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  uint8_t op = 0, *p = pkt->dhcp->options,
          *end = (uint8_t *) &pkt->pay.buf[pkt->pay.len];
  // struct dhcp *req = pkt->dhcp;
  struct dhcp res = {2, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, {0}, 0, {0}};
  if (end < p) return;  // options are optional, check min header length
  res.yiaddr = ifp->ip;
  ((uint8_t *) (&res.yiaddr))[3]++;                // Offer our IP + 1
  while (p + 1 < end && p[0] != 255) {             // Parse options
    if (p[0] == 53 && p[1] == 1 && p + 2 < end) {  // Message type
      op = p[2];
    }
    p += p[1] + 2;
  }
  if (op == 1 || op == 3) {         // DHCP Discover or DHCP Request
    uint8_t msg = op == 1 ? 2 : 5;  // Message type: DHCP OFFER or DHCP ACK
    uint8_t opts[] = {
        53, 1, 0,                   // Message type
        1,  4, 0,   0,   0,   0,    // Subnet mask
        54, 4, 0,   0,   0,   0,    // Server ID
        12, 3, 'm', 'i', 'p',       // Host name: "mip"
        51, 4, 255, 255, 255, 255,  // Lease time
        255                         // End of options
    };
    opts[2] = msg;
    memcpy(&res.hwaddr, pkt->dhcp->hwaddr, 6);
    memcpy(opts + 5, &ifp->mask, sizeof(ifp->mask));
    memcpy(opts + 11, &ifp->ip, sizeof(ifp->ip));
    memcpy(&res.options, opts, sizeof(opts));
    res.magic = pkt->dhcp->magic;
    res.xid = pkt->dhcp->xid;
    if (ifp->enable_get_gateway) {
      ifp->gw = res.yiaddr;  // set gw IP, best-effort gwmac as DHCP server's
      memcpy(ifp->gwmac, pkt->eth->src, sizeof(ifp->gwmac));
    }
    tx_udp4(ifp, pkt->eth->src, ifp->ip, mg_htons(67),
            op == 1 ? ~0U : res.yiaddr, mg_htons(68), &res, sizeof(res));
  }
}

#if MG_ENABLE_IPV6
static struct ip6 *tx_ip6(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                          uint8_t next, uint64_t *ip_src, uint64_t *ip_dst,
                          size_t plen) {
  // ifp->tx.buf is 8-bit aligned, keep other headers as pointers, see pkt
  struct eth *eth = (struct eth *) ifp->tx.buf;
  struct ip6 *ip6 = (struct ip6 *) (eth + 1);
  memcpy(eth->dst, mac_dst, sizeof(eth->dst));
  memcpy(eth->src, ifp->mac, sizeof(eth->src));  // Use our MAC
  eth->type = mg_htons(0x86dd);
  memset(ip6, 0, sizeof(*ip6));
  ip6->ver = 0x60;  // Version 6, traffic class 0
  ip6->plen = mg_htons((uint16_t) plen);
  ip6->next = next;
  ip6->hops = 255;  // NDP requires max
  ip6->src[0] = *ip_src++;
  ip6->src[1] = *ip_src;
  ip6->dst[0] = *ip_dst++;
  ip6->dst[1] = *ip_dst;
  return ip6;
}

static void tx_icmp6(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                     uint64_t *ip_src, uint64_t *ip_dst, uint8_t type,
                     uint8_t code, const void *buf, size_t len) {
  struct ip6 *ip6;
  struct icmp6 *icmp6;
  uint32_t cs;
  ip6 = tx_ip6(ifp, mac_dst, 58, ip_src, ip_dst, sizeof(*icmp6) + len);
  icmp6 = (struct icmp6 *) (ip6 + 1);
  memset(icmp6, 0, sizeof(*icmp6));  // Set csum to 0
  icmp6->type = type;
  icmp6->code = code;
  memcpy(icmp6 + 1, buf, len);  // Copy payload
  icmp6->csum = 0;              // RFC-4443 2.3, RFC-8200 8.1
  cs = csumup(0, icmp6, sizeof(*icmp6));
  cs = csumup(cs, buf, len);
  cs = csumup(cs, ip_src, 16);
  cs = csumup(cs, ip_dst, 16);
  cs += (uint32_t) (58 + sizeof(*icmp6) + len);
  icmp6->csum = csumfin(cs);
  ether_output(ifp, sizeof(struct eth) + sizeof(*ip6) + sizeof(*icmp6) + len);
}

// Neighbor Discovery Protocol, RFC-4861
// Neighbor Advertisement, 4.4
static void tx_ndp_na(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                      uint64_t *ip_src, uint64_t *ip_dst, bool solicited,
                      uint8_t *mac) {
  uint8_t data[28];
  memset(data, 0, sizeof(data));
  data[0] = solicited ? 0x60 : 0x20;  // O + S
  memcpy(data + 4, ip_src, 16);       // Target address
  data[20] = 2, data[21] = 1;         // 4.6.1, target MAC
  memcpy(data + 22, mac, 6);
  tx_icmp6(ifp, mac_dst, ip_src, ip_dst, 136, 0, data, sizeof(data));
}

static void onstate6change(struct mg_tcpip_if *ifp);

static void rx_ndp_na(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  struct ndp_na *na = (struct ndp_na *) (pkt->icmp6 + 1);
  uint8_t *opts = (uint8_t *) (na + 1);
  if ((na->res[0] & 0x40) == 0) return;  // not "solicited"
  if (*opts++ != 2) return;              // no target hwaddr
  if (*opts++ != 1) return;  // don't know what to do with this layer 2
  MG_VERBOSE(("NDP NA resp from %M", mg_print_ip6, (char *) &na->addr));
  if (MG_IP6MATCH(na->addr, ifp->gw6)) {
    // Got response for the GW NS request. Set ifp->gw6mac and IP6 -> READY
    memcpy(ifp->gw6mac, opts, sizeof(ifp->gw6mac));
    ifp->gw6_ready = true;
    if (ifp->state6 == MG_TCPIP_STATE_IP) {
      ifp->state6 = MG_TCPIP_STATE_READY;
      onstate6change(ifp);
    }
  } else {
    struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
    if (c != NULL && c->is_arplooking) {
      struct connstate *s = (struct connstate *) (c + 1);
      memcpy(s->mac, opts, sizeof(s->mac));
      MG_DEBUG(("%lu NDP resolved %M -> %M", c->id, mg_print_ip6,
                c->rem.addr.ip, mg_print_mac, s->mac));
      c->is_arplooking = 0;
      mac_resolved(c);
    }
  }
}

// Neighbor Solicitation, 4.3
static void rx_ndp_ns(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  uint64_t target[2];
  if (pkt->pay.len < sizeof(target)) return;
  memcpy(target, pkt->pay.buf + 4, sizeof(target));
  if (MG_IP6MATCH(target, ifp->ip6ll) || MG_IP6MATCH(target, ifp->ip6))
    tx_ndp_na(ifp, (uint8_t *) pkt->pay.buf + 22, target, pkt->ip6->src, true,
              ifp->mac);
}

static void tx_ndp_ns(struct mg_tcpip_if *ifp, uint64_t *ip_dst, uint8_t *mac) {
  uint8_t payload[4 + 16 + 8];
  uint64_t unspec_ip[2] = {0, 0};
  size_t payload_len;
  bool mcast_ns = true;
  uint64_t mcast_ip[2] = {0, 0};
  uint8_t mcast_mac[6];

  memset(payload, 0, sizeof(payload));
  memcpy(payload + 4, ip_dst, 16);
  for (int i = 0; i < 6; i++) {
    if (mac[i] != 0xff) {
      mcast_ns = false;
      break;
    }
  }
  if (mcast_ns) {
    ip6sn(ip_dst, mcast_ip);
    ip6_mcastmac(mcast_mac, mcast_ip);
  }
  // TODO(robertc2000): using only link-local IP addr for now
  // We might consider to add an option to use either link-local or global IP
  if (!MG_IP6MATCH(ifp->ip6ll, unspec_ip)) {
    payload[20] = payload[21] = 1;  // Type = 1; Length = 1
    memcpy(payload + 22, ifp->mac, 6);
    payload_len = sizeof(payload);
  } else {
    payload_len = sizeof(payload) - 8;
  }
  tx_icmp6(ifp, mcast_ns ? mcast_mac : mac, ifp->ip6ll,
           mcast_ns ? mcast_ip : ip_dst, 135, 0, payload, payload_len);
}

// Router Solicitation, 4.1
static void tx_ndp_rs(struct mg_tcpip_if *ifp, uint64_t *ip_dst, uint8_t *mac) {
  // Note: currently, this function only sends multicast RS packets
  (void) ip_dst;
  (void) mac;
  uint8_t payload[4 + 8];  // reserved + optional source MAC addr
  size_t payload_len = 4;
  uint64_t unspec_ip[2] = {0, 0};

  memset(payload, 0, sizeof(payload));
  if (!MG_IP6MATCH(ifp->ip6ll, unspec_ip)) {
    payload[4] = payload[5] = 1;  // Type = 1; Length = 1
    memcpy(payload + 6, ifp->mac, 6);
    payload_len += 8;
  }
  tx_icmp6(ifp, (uint8_t *) ip6mac_allrouters, ifp->ip6ll,
           (uint64_t *) ip6_allrouters.u, 133, 0, payload, payload_len);
  MG_DEBUG(("NDP Router Solicitation sent"));
}

static bool fill_global(uint64_t *ip6, uint8_t *prefix, uint8_t prefix_len,
                        uint8_t *mac) {
  uint8_t full = prefix_len / 8;
  uint8_t rem = prefix_len % 8;
  if (full >= 8 && rem != 0) {
    MG_ERROR(("Prefix length > 64, UNSUPPORTED"));
    return false;
  } else if (full == 8 && rem == 0) {
    ip6gen((uint8_t *) ip6, prefix, mac);
  } else {
    ip6[0] = ip6[1] = 0;  // already zeroed before firing RS...
    if (full) memcpy(ip6, prefix, full);
    if (rem) {
      uint8_t mask = (uint8_t) (0xFF << (8 - rem));
      ((uint8_t *) ip6)[full] = prefix[full] & mask;
    }
    meui64(((uint8_t *) &ip6[1]), mac);  // RFC-4291 2.5.4, 2.5.1
  }
  return true;
}

// Router Advertisement, 4.2
static void rx_ndp_ra(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  if (pkt->pay.len < 12) return;
  struct ndp_ra *ra = (struct ndp_ra *) (pkt->icmp6 + 1);
  uint8_t *opts = (uint8_t *) (ra + 1);
  size_t opt_left = pkt->pay.len - 12;

  if (ifp->state6 == MG_TCPIP_STATE_UP) {
    MG_DEBUG(("Received NDP RA"));
    memcpy(ifp->gw6, (uint8_t *) pkt->ip6->src, 16);  // fill gw6 address
    // parse options
    while (opt_left >= 2) {
      uint8_t type = opts[0], len = opts[1];
      size_t length = (size_t) len * 8;
      if (length == 0 || length > opt_left) break;  // malformed
      if (type == 1 && length >= 8) {
        // Received router's MAC address
        ifp->state6 = MG_TCPIP_STATE_READY;
        memcpy(ifp->gw6mac, opts + 2, 6);
        ifp->gw6_ready = true;
      } else if (type == 5 && length >= 8) {
        // process MTU if available
        uint32_t mtu = mg_ntohl(*(uint32_t *) (opts + 4));
        MG_INFO(("got a nice MTU: %u, do you care ?", mtu));
        // TODO(): **** This is an IPv6-given MTU, ifp->MTU is a LINK MTU, are
        // we talkin'bout the same ? ***
      } else if (type == 3 && length >= 32) {
        // process prefix, 4.6.2
        uint8_t prefix_len = opts[2];
        uint8_t pfx_flags = opts[3];  // L=0x80, A=0x40
        uint32_t valid = mg_ntohl(*(uint32_t *) (opts + 4));
        uint32_t pref_lifetime = mg_ntohl(*(uint32_t *) (opts + 8));
        uint8_t *prefix = opts + 16;

        // TODO (robertc2000): handle prefix options if necessary
        (void) prefix_len;
        (void) pfx_flags;
        (void) valid;
        (void) pref_lifetime;
        (void) prefix;

        // fill prefix length and global
        ifp->prefix_len = prefix_len;
        if (!fill_global(ifp->ip6, prefix, prefix_len, ifp->mac)) return;
      }
      opts += length;
      opt_left -= length;
    }

    if (ifp->state6 != MG_TCPIP_STATE_READY) {
      tx_ndp_ns(ifp, ifp->gw6, ifp->gw6mac);  // unsolicited GW MAC resolution
      ifp->state6 = MG_TCPIP_STATE_IP;
    }
    onstate6change(ifp);
  }
}

static void rx_icmp6(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  switch (pkt->icmp6->type) {
    case 128: {  // Echo Request, RFC-4443 4.1
      uint64_t target[2];
      memcpy(target, pkt->ip6->dst, sizeof(target));
      if (MG_IP6MATCH(target, ifp->ip6ll) || MG_IP6MATCH(target, ifp->ip6)) {
        size_t hlen =
            sizeof(struct eth) + sizeof(struct ip6) + sizeof(struct icmp6);
        size_t space = ifp->tx.len - hlen, plen = pkt->pay.len;
        struct mg_addr ips;
        ips.ip6[0] = pkt->ip6->src[0], ips.ip6[1] = pkt->ip6->src[1];
        ips.is_ip6 = true;
        if (get_return_mac(ifp, &ips, false, pkt) == NULL)
          return;                        // safety net for lousy networks
        if (plen > space) plen = space;  // Copy (truncated) RX payload to TX
        // Echo Reply, 4.2
        tx_icmp6(ifp, pkt->eth->src, pkt->ip6->dst, pkt->ip6->src, 129, 0,
                 pkt->pay.buf, plen);
      }
    } break;
    case 134:  // Router Advertisement
      rx_ndp_ra(ifp, pkt);
      break;
    case 135:  // Neighbor Solicitation
      rx_ndp_ns(ifp, pkt);
      break;
    case 136:  // Neighbor Advertisement
      rx_ndp_na(ifp, pkt);
      break;
  }
}

static void onstate6change(struct mg_tcpip_if *ifp) {
  if (ifp->state6 == MG_TCPIP_STATE_READY) {
    MG_INFO(("READY, IP: %M", mg_print_ip6, &ifp->ip6));
    MG_INFO(("       GW: %M", mg_print_ip6, &ifp->gw6));
    MG_INFO(("      MAC: %M", mg_print_mac, &ifp->mac));
  } else if (ifp->state6 == MG_TCPIP_STATE_IP) {
    if (ifp->gw6[0] != 0 || ifp->gw6[1] != 0)
      tx_ndp_ns(ifp, ifp->gw6, ifp->gw6mac);  // unsolicited GW MAC resolution
  } else if (ifp->state6 == MG_TCPIP_STATE_UP) {
    MG_INFO(("IP: %M", mg_print_ip6, &ifp->ip6ll));
  }
  if (ifp->state6 != MG_TCPIP_STATE_UP && ifp->state6 != MG_TCPIP_STATE_DOWN)
    mg_tcpip_call(ifp, MG_TCPIP_EV_ST6_CHG, &ifp->state6);
}
#endif

static uint8_t *get_return_mac(struct mg_tcpip_if *ifp, struct mg_addr *rem,
                               bool is_udp, struct pkt *pkt) {
#if MG_ENABLE_IPV6
  if (rem->is_ip6) {
    if (is_udp && MG_IP6MATCH(rem->ip6, ip6_allnodes.u))  // local broadcast
      return (uint8_t *) ip6mac_allnodes;
    if (rem->ip6[0] == ifp->ip6[0])  // TODO(): HANDLE PREFIX ***
      return pkt->eth->src;  // we're on the same LAN, get MAC from frame
    if (is_udp && *((uint8_t *) rem->ip6) == 0xFF)  // multicast
    {
    }  // TODO(): ip6_mcastmac(s->mac, c->rem.ip6), l2 PR handles this better
    if (ifp->gw6_ready)    // use the router
      return ifp->gw6mac;  // ignore source MAC in frame
  } else
#endif
  {
    uint32_t rem_ip = rem->addr.ip4;
    if (is_udp && (rem_ip == 0xffffffff || rem_ip == (ifp->ip | ~ifp->mask)))
      return (uint8_t *) broadcast;  // global or local broadcast
    if (ifp->ip != 0 && ((rem_ip & ifp->mask) == (ifp->ip & ifp->mask)))
      return pkt->eth->src;  // we're on the same LAN, get MAC from frame
    if (is_udp &&
        (*((uint8_t *) &rem_ip) & 0xE0) == 0xE0)  // 224 to 239, E0 to EF
    {
    }  // TODO(): ip4_mcastmac(s->mac, &rem_ip);     // multicast group, l2 PR
    if (ifp->gw_ready)  // use the router, ignore source MAC
      return ifp->gwmac;
  }
  MG_ERROR(("%M %s: No way back, can't respond", mg_print_ip_port, rem,
            is_udp ? "UDP" : "TCP"));
  return NULL;
}

static bool rx_udp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  struct mg_connection *c = getpeer(ifp->mgr, pkt, true);
  struct connstate *s;
  uint8_t *mac;
  if (c == NULL) return false;  // No UDP listener on this port
  s = (struct connstate *) (c + 1);
  c->rem.port = pkt->udp->sport;
#if MG_ENABLE_IPV6
  if (c->loc.is_ip6) {
    c->rem.addr.ip6[0] = pkt->ip6->src[0],
    c->rem.addr.ip6[1] = pkt->ip6->src[1], c->rem.is_ip6 = true;
  } else
#endif
  {
    c->rem.addr.ip4 = pkt->ip->src;
  }
  if ((mac = get_return_mac(ifp, &c->rem, true, pkt)) == NULL)
    return false;  // safety net for lousy networks
  memcpy(s->mac, mac, sizeof(s->mac));
  if (c->recv.len >= MG_MAX_RECV_SIZE) {
    mg_error(c, "max_recv_buf_size reached");
  } else if (c->recv.size - c->recv.len < pkt->pay.len &&
             !mg_iobuf_resize(&c->recv, c->recv.len + pkt->pay.len)) {
    mg_error(c, "oom");
  } else {
    memcpy(&c->recv.buf[c->recv.len], pkt->pay.buf, pkt->pay.len);
    c->recv.len += pkt->pay.len;
    mg_call(c, MG_EV_READ, &pkt->pay.len);
  }
  return true;
}

static size_t tx_tcp(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                     struct mg_addr *ip_src, struct mg_addr *ip_dst,
                     uint8_t flags, uint32_t seq, uint32_t ack, const void *buf,
                     size_t len) {
  struct ip *ip = NULL;
  struct tcp *tcp;
  uint16_t opts[4 / 2], mss;
#if MG_ENABLE_IPV6
  struct ip6 *ip6 = NULL;
  mss = (uint16_t) (ifp->mtu - 60);  // RFC-9293 3.7.1; RFC-6691 2
#else
  mss = (uint16_t) (ifp->mtu - 40);  // RFC-9293 3.7.1; RFC-6691 2
#endif
  if (flags & TH_SYN) {          // Send MSS
    opts[0] = mg_htons(0x0204);  // RFC-9293 3.2
    opts[1] = mg_htons(mss);
    buf = opts;
    len = sizeof(opts);
  }
#if MG_ENABLE_IPV6
  if (ip_dst->is_ip6) {
    ip6 = tx_ip6(ifp, mac_dst, 6, ip_src->addr.ip6, ip_dst->addr.ip6,
                 sizeof(struct tcp) + len);
    tcp = (struct tcp *) (ip6 + 1);
  } else
#endif
  {
    ip = tx_ip(ifp, mac_dst, 6, ip_src->addr.ip4, ip_dst->addr.ip4,
               sizeof(struct tcp) + len);
    tcp = (struct tcp *) (ip + 1);
  }
  memset(tcp, 0, sizeof(*tcp));
  if (buf != NULL && len) memmove(tcp + 1, buf, len);
  tcp->sport = ip_src->port;
  tcp->dport = ip_dst->port;
  tcp->seq = seq;
  tcp->ack = ack;
  tcp->flags = flags;
  tcp->win = mg_htons(MIP_TCP_WIN);
  tcp->off = (uint8_t) (sizeof(*tcp) / 4 << 4);
  if (flags & TH_SYN) tcp->off += (uint8_t) (sizeof(opts) / 4 << 4);
  {
    uint32_t cs = 0;
    uint16_t n = (uint16_t) (sizeof(*tcp) + len);
    cs = csumup(cs, tcp, n);
#if MG_ENABLE_IPV6
    if (ip_dst->is_ip6) {
      cs = csumup(cs, &ip6->src, sizeof(ip6->src));
      cs = csumup(cs, &ip6->dst, sizeof(ip6->dst));
      cs += (uint32_t) (6 + n);
    } else
#endif
    {
      uint8_t pseudo[] = {0, ip->proto, (uint8_t) (n >> 8),
                          (uint8_t) (n & 255)};
      cs = csumup(cs, &ip->src, sizeof(ip->src));
      cs = csumup(cs, &ip->dst, sizeof(ip->dst));
      cs = csumup(cs, pseudo, sizeof(pseudo));
    }
    tcp->csum = csumfin(cs);
  }
  MG_VERBOSE(("TCP %M -> %M fl %x len %u", mg_print_ip_port, ip_src,
              mg_print_ip_port, ip_dst, tcp->flags, len));
  return ether_output(ifp, PDIFF(ifp->tx.buf, tcp + 1) + len);
}

static size_t tx_tcp_ctrlresp(struct mg_tcpip_if *ifp, struct pkt *pkt,
                              uint8_t flags, uint32_t seqno) {
  uint32_t ackno = mg_htonl(mg_ntohl(pkt->tcp->seq) + (uint32_t) pkt->pay.len +
                            ((pkt->tcp->flags & (TH_SYN | TH_FIN)) ? 1 : 0));
  struct mg_addr ips, ipd;
  uint8_t *mac;
  memset(&ips, 0, sizeof(ips));
  memset(&ipd, 0, sizeof(ipd));
  if (pkt->ip != NULL) {
    ips.addr.ip4 = pkt->ip->dst;
    ipd.addr.ip4 = pkt->ip->src;
  } else {
    ips.addr.ip6[0] = pkt->ip6->dst[0], ips.addr.ip6[1] = pkt->ip6->dst[1];
    ipd.addr.ip6[0] = pkt->ip6->src[0], ipd.addr.ip6[1] = pkt->ip6->src[1];
    ips.is_ip6 = true;
    ipd.is_ip6 = true;
  }
  ips.port = pkt->tcp->dport;
  ipd.port = pkt->tcp->sport;
  if ((mac = get_return_mac(ifp, &ipd, false, pkt)) == NULL)
    return 0;  // safety net for lousy networks
  return tx_tcp(ifp, mac, &ips, &ipd, flags, seqno, ackno, NULL, 0);
}

static size_t tx_tcp_rst(struct mg_tcpip_if *ifp, struct pkt *pkt, bool toack) {
  return tx_tcp_ctrlresp(ifp, pkt, toack ? TH_RST : (TH_RST | TH_ACK),
                         toack ? pkt->tcp->ack : 0);
}

static struct mg_connection *accept_conn(struct mg_connection *lsn,
                                         struct pkt *pkt, uint16_t mss) {
  struct mg_connection *c = mg_alloc_conn(lsn->mgr);
  struct connstate *s;
  uint8_t *mac;
  if (c == NULL) {
    MG_ERROR(("OOM"));
    return NULL;
  }
  s = (struct connstate *) (c + 1);
  s->dmss = mss;  // from options in client SYN
  s->seq = mg_ntohl(pkt->tcp->ack), s->ack = mg_ntohl(pkt->tcp->seq);
#if MG_ENABLE_IPV6
  if (lsn->loc.is_ip6) {
    c->rem.addr.ip6[0] = pkt->ip6->src[0],
    c->rem.addr.ip6[1] = pkt->ip6->src[1], c->rem.is_ip6 = true;
    c->loc.addr.ip6[0] = c->mgr->ifp->ip6[0],
    c->loc.addr.ip6[1] = c->mgr->ifp->ip6[1], c->loc.is_ip6 = true;
    // TODO(): compare lsn to link-local, or rem as link-local: use ll instead
  } else
#endif
  {
    c->rem.addr.ip4 = pkt->ip->src;
    c->loc.addr.ip4 = c->mgr->ifp->ip;
  }
  c->rem.port = pkt->tcp->sport;
  c->loc.port = lsn->loc.port;
  if ((mac = get_return_mac(lsn->mgr->ifp, &c->rem, false, pkt)) == NULL) {
    free(c);      // safety net for lousy networks, not actually needed
    return NULL;  // as path has already been checked at SYN (sending SYN+ACK)
  } 
  memcpy(s->mac, mac, sizeof(s->mac));
  settmout(c, MIP_TTYPE_KEEPALIVE);
  MG_DEBUG(("%lu accepted %M", c->id, mg_print_ip_port, &c->rem));
  LIST_ADD_HEAD(struct mg_connection, &lsn->mgr->conns, c);
  c->is_accepted = 1;
  c->is_hexdumping = lsn->is_hexdumping;
  c->pfn = lsn->pfn;
  c->pfn_data = lsn->pfn_data;
  c->fn = lsn->fn;
  c->fn_data = lsn->fn_data;
  c->is_tls = lsn->is_tls;
  mg_call(c, MG_EV_OPEN, NULL);
  mg_call(c, MG_EV_ACCEPT, NULL);
  if (!c->is_tls_hs) c->is_tls = 0;  // user did not call mg_tls_init()
  return c;
}

static size_t trim_len(struct mg_connection *c, size_t len) {
  struct mg_tcpip_if *ifp = c->mgr->ifp;
  size_t eth_h_len = 14, tcp_max_h_len = 60, udp_h_len = 8;
  size_t ip_max_h_len = c->rem.is_ip6 ? 40 : 24;  // we don't send options
  size_t max_headers_len =
      eth_h_len + ip_max_h_len + (c->is_udp ? udp_h_len : tcp_max_h_len);
  size_t min_mtu = c->rem.is_ip6 ? 1280
                   : c->is_udp   ? 68 /* RFC-791 */
                                 : max_headers_len - eth_h_len;

  // If the frame exceeds the available buffer, trim the length
  if (len + max_headers_len > ifp->tx.len) {
    len = ifp->tx.len - max_headers_len;
  }
  // Ensure the MTU isn't lower than the minimum allowed value
  if (ifp->mtu < min_mtu) {
    MG_ERROR(("MTU is lower than minimum, capping to %lu", min_mtu));
    ifp->mtu = (uint16_t) min_mtu;
  }
  // If the total packet size exceeds the MTU, trim the length
  if (len + max_headers_len - eth_h_len > ifp->mtu) {
    len = ifp->mtu - max_headers_len + eth_h_len;
    if (c->is_udp) {
      MG_ERROR(("UDP datagram exceeds MTU. Truncating it."));
    }
  }

  return len;
}

static bool udp_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tcpip_if *ifp = c->mgr->ifp;
  struct connstate *s = (struct connstate *) (c + 1);
  struct mg_addr ips;
  memset(&ips, 0, sizeof(ips));
#if MG_ENABLE_IPV6
  if (c->loc.is_ip6) {
    ips.addr.ip6[0] = ifp->ip6[0], ips.addr.ip6[1] = ifp->ip6[1],
    ips.is_ip6 = true;
    // TODO(): detect link-local (c->rem) and use it
  } else
#endif
  {
    ips.addr.ip4 = ifp->ip;
  }
  ips.port = c->loc.port;
  return tx_udp(ifp, s->mac, &ips, &c->rem, buf, len);
}

long mg_io_send(struct mg_connection *c, const void *buf, size_t len) {
  struct connstate *s = (struct connstate *) (c + 1);
  len = trim_len(c, len);
  if (c->is_udp) {
    if (!udp_send(c, buf, len)) return MG_IO_WAIT;
  } else {  // TCP, cap to peer's MSS
    struct mg_tcpip_if *ifp = c->mgr->ifp;
    size_t sent;
    if (len > s->dmss) len = s->dmss;  // RFC-6691: reduce if sending opts
    sent = tx_tcp(ifp, s->mac, &c->loc, &c->rem, TH_PUSH | TH_ACK,
                  mg_htonl(s->seq), mg_htonl(s->ack), buf, len);
    if (sent == 0) {
      return MG_IO_WAIT;
    } else if (sent == (size_t) -1) {
      return MG_IO_ERR;
    } else {
      s->seq += (uint32_t) len;
      if (s->ttype == MIP_TTYPE_ACK) settmout(c, MIP_TTYPE_KEEPALIVE);
    }
  }
  return (long) len;
}

static void handle_tls_recv(struct mg_connection *c) {
  size_t avail = mg_tls_pending(c);
  size_t min = avail > MG_MAX_RECV_SIZE ? MG_MAX_RECV_SIZE : avail;
  struct mg_iobuf *io = &c->recv;
  if (io->size - io->len < min && !mg_iobuf_resize(io, io->len + min)) {
    mg_error(c, "oom");
  } else {
    // Decrypt data directly into c->recv
    long n = mg_tls_recv(c, io->buf != NULL ? &io->buf[io->len] : io->buf,
                         io->size - io->len);
    if (n == MG_IO_ERR) {
      mg_error(c, "TLS recv error");
    } else if (n > 0) {
      // Decrypted successfully - trigger MG_EV_READ
      io->len += (size_t) n;
      mg_call(c, MG_EV_READ, &n);
    }  // else n < 0: outstanding data to be moved to c->recv
  }
}

static void read_conn(struct mg_connection *c, struct pkt *pkt) {
  struct connstate *s = (struct connstate *) (c + 1);
  struct mg_iobuf *io = c->is_tls ? &c->rtls : &c->recv;
  uint32_t seq = mg_ntohl(pkt->tcp->seq);
  if (pkt->tcp->flags & TH_FIN) {
    uint8_t flags = TH_ACK;
    if (mg_ntohl(pkt->tcp->seq) != s->ack) {
      MG_VERBOSE(("ignoring FIN, %x != %x", mg_ntohl(pkt->tcp->seq), s->ack));
      tx_tcp(c->mgr->ifp, s->mac, &c->loc, &c->rem, TH_ACK, mg_htonl(s->seq),
             mg_htonl(s->ack), "", 0);
      return;
    }
    // If we initiated the closure, we reply with ACK upon receiving FIN
    // If we didn't initiate it, we reply with FIN as part of the normal TCP
    // closure process
    s->ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len + 1);
    s->fin_rcvd = true;
    if (c->is_draining && s->ttype == MIP_TTYPE_FIN) {
      if (s->seq == mg_htonl(pkt->tcp->ack)) {  // Simultaneous closure ?
        s->seq++;                               // Yes. Increment our SEQ
      } else {                                  // Otherwise,
        s->seq = mg_htonl(pkt->tcp->ack);       // Set to peer's ACK
      }
      s->twclosure = true;
    } else {
      flags |= TH_FIN;
      c->is_draining = 1;
      settmout(c, MIP_TTYPE_FIN);
    }
    tx_tcp(c->mgr->ifp, s->mac, &c->loc, &c->rem, flags, mg_htonl(s->seq),
           mg_htonl(s->ack), "", 0);
    if (pkt->pay.len == 0) return;  // if no data, we're done
  } else if (pkt->pay.len <= 1 && mg_ntohl(pkt->tcp->seq) == s->ack - 1) {
    // Keep-Alive (RFC-9293 3.8.4, allow erroneous implementations)
    MG_VERBOSE(("%lu keepalive ACK", c->id));
    tx_tcp(c->mgr->ifp, s->mac, &c->loc, &c->rem, TH_ACK, mg_htonl(s->seq),
           mg_htonl(s->ack), NULL, 0);
    return;                        // no data to process
  } else if (pkt->pay.len == 0) {  // this is an ACK
    if (s->fin_rcvd && s->ttype == MIP_TTYPE_FIN) s->twclosure = true;
    return;  // no data to process
  } else if (seq != s->ack) {
    uint32_t ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len);
    if (s->ack == ack) {
      MG_VERBOSE(("ignoring duplicate pkt"));
    } else {
      MG_VERBOSE(("SEQ != ACK: %x %x %x", seq, s->ack, ack));
      tx_tcp(c->mgr->ifp, s->mac, &c->loc, &c->rem, TH_ACK, mg_htonl(s->seq),
             mg_htonl(s->ack), "", 0);
    }
    return;  // drop it
  } else if (io->size - io->len < pkt->pay.len &&
             !mg_iobuf_resize(io, io->len + pkt->pay.len)) {
    mg_error(c, "oom");
    return;  // drop it
  }
  // Copy TCP payload into the IO buffer. If the connection is plain text,
  // we copy to c->recv. If the connection is TLS, this data is encrypted,
  // therefore we copy that encrypted data to the c->rtls iobuffer instead,
  // and then call mg_tls_recv() to decrypt it. NOTE: mg_tls_recv() will
  // call back mg_io_recv() which grabs raw data from c->rtls
  memcpy(&io->buf[io->len], pkt->pay.buf, pkt->pay.len);
  io->len += pkt->pay.len;
  MG_VERBOSE(("%lu SEQ %x -> %x", c->id, mg_htonl(pkt->tcp->seq), s->ack));
  // Advance ACK counter
  s->ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len);
  s->unacked += pkt->pay.len;
  // size_t diff = s->acked <= s->ack ? s->ack - s->acked : s->ack;
  if (s->unacked > MIP_TCP_WIN / 2 && s->acked != s->ack) {
    // Send ACK immediately
    MG_VERBOSE(("%lu imm ACK %lu", c->id, s->acked));
    tx_tcp(c->mgr->ifp, s->mac, &c->loc, &c->rem, TH_ACK, mg_htonl(s->seq),
           mg_htonl(s->ack), NULL, 0);
    s->unacked = 0;
    s->acked = s->ack;
    if (s->ttype != MIP_TTYPE_KEEPALIVE) settmout(c, MIP_TTYPE_KEEPALIVE);
  } else {
    // if not already running, setup a timer to send an ACK later
    if (s->ttype != MIP_TTYPE_ACK) settmout(c, MIP_TTYPE_ACK);
  }
  if (c->is_tls) {
    c->is_tls_hs ? mg_tls_handshake(c) : handle_tls_recv(c);
  } else {
    // Plain text connection, data is already in c->recv, trigger MG_EV_READ
    mg_call(c, MG_EV_READ, &pkt->pay.len);
  }
}

// TCP backlog
struct mg_backlog {
  uint16_t port, mss;  // use port=0 for available entries
  uint8_t age;
};

static int backlog_insert(struct mg_connection *c, uint16_t port,
                          uint16_t mss) {
  struct mg_backlog *p = (struct mg_backlog *) c->data;
  size_t i;
  for (i = 0; i < sizeof(c->data) / sizeof(*p); i++) {
    if (p[i].port != 0) continue;
    p[i].age = 2;  // remove after two calls, average 1.5 call rate
    p[i].port = port, p[i].mss = mss;
    return (int) i;
  }
  return -1;
}

static struct mg_backlog *backlog_retrieve(struct mg_connection *c,
                                           uint16_t key, uint16_t port) {
  struct mg_backlog *p = (struct mg_backlog *) c->data;
  if (key >= sizeof(c->data) / sizeof(*p)) return NULL;
  if (p[key].port != port) return NULL;
  p += key;
  return p;
}

static void backlog_remove(struct mg_connection *c, uint16_t key) {
  struct mg_backlog *p = (struct mg_backlog *) c->data;
  p[key].port = 0;
}

static void backlog_maintain(struct mg_connection *c) {
  struct mg_backlog *p = (struct mg_backlog *) c->data;
  size_t i;  // dec age and remove those where it reaches 0
  for (i = 0; i < sizeof(c->data) / sizeof(*p); i++) {
    if (p[i].port == 0) continue;
    if (p[i].age != 0) --p[i].age;
    if (p[i].age == 0) p[i].port = 0;
  }
}

static void backlog_poll(struct mg_mgr *mgr) {
  struct mg_connection *c = NULL;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (!c->is_udp && c->is_listening) backlog_maintain(c);
  }
}

// process options (MSS)
static void handle_opt(struct connstate *s, struct tcp *tcp, bool ip6) {
  uint8_t *opts = (uint8_t *) (tcp + 1);
  int len = 4 * ((int) (tcp->off >> 4) - ((int) sizeof(*tcp) / 4));
  s->dmss = ip6 ? 1220 : 536;  // assume default, RFC-9293 3.7.1
  while (len > 0) {            // RFC-9293 3.1 3.2
    uint8_t kind = opts[0], optlen = 1;
    if (kind != 1) {         // No-Operation
      if (kind == 0) break;  // End of Option List
      optlen = opts[1];
      if (kind == 2 && optlen == 4)  // set received MSS
        s->dmss = (uint16_t) (((uint16_t) opts[2] << 8) + opts[3]);
    }
    MG_VERBOSE(("kind: %u, optlen: %u, len: %d\n", kind, optlen, len));
    opts += optlen;
    len -= optlen;
  }
}

static void rx_tcp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
  struct connstate *s = c == NULL ? NULL : (struct connstate *) (c + 1);
  // Order is VERY important; RFC-9293 3.5.2
  // - check clients (Group 1) and established connections (Group 3)
  if (c != NULL && c->is_connecting && pkt->tcp->flags == (TH_SYN | TH_ACK)) {
    // client got a server connection accept
    handle_opt(s, pkt->tcp, pkt->ip6 != NULL);  // process options (MSS)
    s->seq = mg_ntohl(pkt->tcp->ack), s->ack = mg_ntohl(pkt->tcp->seq) + 1;
    tx_tcp_ctrlresp(ifp, pkt, TH_ACK, pkt->tcp->ack);
    c->is_connecting = 0;  // Client connected
    settmout(c, MIP_TTYPE_KEEPALIVE);
    mg_call(c, MG_EV_CONNECT, NULL);  // Let user know
    if (c->is_tls_hs) mg_tls_handshake(c);
    if (!c->is_tls_hs) c->is_tls = 0;  // user did not call mg_tls_init()
  } else if (c != NULL && c->is_connecting && pkt->tcp->flags != TH_ACK) {
    mg_error(c, "connection refused");
  } else if (c != NULL && pkt->tcp->flags & TH_RST) {
    // TODO(): validate RST is within window (and optional with proper ACK)
    mg_error(c, "peer RST");  // RFC-1122 4.2.2.13
  } else if (c != NULL) {
    // process segment
    s->tmiss = 0;                         // Reset missed keep-alive counter
    if (s->ttype == MIP_TTYPE_KEEPALIVE)  // Advance keep-alive timer
      settmout(c,
               MIP_TTYPE_KEEPALIVE);  // unless a former ACK timeout is pending
    read_conn(c, pkt);  // Override timer with ACK timeout if needed
  } else
    // - we don't listen on that port; RFC-9293 3.5.2 Group 1
    // - check listening connections; RFC-9293 3.5.2 Group 2
    if ((c = getpeer(ifp->mgr, pkt, true)) == NULL) {
      // not listening on that port
      if (!(pkt->tcp->flags & TH_RST)) {
        tx_tcp_rst(ifp, pkt, pkt->tcp->flags & TH_ACK);
      }  // else silently discard
    } else if (pkt->tcp->flags == TH_SYN) {
      // listener receives a connection request
      struct connstate cs;  // At this point, s = NULL, there is no connection
      int key;
      uint32_t isn;
      if (pkt->tcp->sport != 0) {
        handle_opt(&cs, pkt->tcp, pkt->ip6 != NULL);  // process options (MSS)
        key = backlog_insert(c, pkt->tcp->sport,
                             cs.dmss);  // backlog options (MSS)
        if (key < 0) return;  // no room in backlog, discard SYN, client retries
        // Use peer's src port and bl key as ISN, to later identify the
        // handshake
        isn = (mg_htonl(((uint32_t) key << 16) | mg_ntohs(pkt->tcp->sport)));
        if (tx_tcp_ctrlresp(ifp, pkt, TH_SYN | TH_ACK, isn) == 0)
          backlog_remove(c, (uint16_t) key);  // safety net for lousy networks
      }  // what should we do when port=0 ? Linux takes port 0 as any other
         // port
    } else if (pkt->tcp->flags == TH_ACK) {
      // listener receives an ACK
      struct mg_backlog *b = NULL;
      if ((uint16_t) (mg_htonl(pkt->tcp->ack) - 1) ==
          mg_htons(pkt->tcp->sport)) {
        uint16_t key = (uint16_t) ((mg_htonl(pkt->tcp->ack) - 1) >> 16);
        b = backlog_retrieve(c, key, pkt->tcp->sport);
        if (b != NULL) {                // ACK is a response to a SYN+ACK
          accept_conn(c, pkt, b->mss);  // pass options
          backlog_remove(c, key);
        }  // else not an actual match, reset
      }
      if (b == NULL) tx_tcp_rst(ifp, pkt, true);
    } else if (pkt->tcp->flags & TH_RST) {
      // silently discard
    } else if (pkt->tcp->flags & TH_ACK) {  // ACK + something else != RST
      tx_tcp_rst(ifp, pkt, true);
    } else if (pkt->tcp->flags & TH_SYN) {  // SYN + something else != ACK
      tx_tcp_rst(ifp, pkt, false);
    }  // else  silently discard
}

static void rx_ip(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  uint8_t ihl;
  uint16_t frag, len;
  if (pkt->pay.len < sizeof(*pkt->ip)) return;  // Truncated
  if ((pkt->ip->ver >> 4) != 4) return;         // Not IP
  ihl = pkt->ip->ver & 0x0F;
  if (ihl < 5) return;                              // bad IHL
  if (pkt->pay.len < (uint16_t) (ihl * 4)) return;  // Truncated / malformed
  // There can be link padding, take length from IP header
  len = mg_ntohs(pkt->ip->len);  // IP datagram length
  if (len < (uint16_t) (ihl * 4) || len > pkt->pay.len) return;  // malformed
  pkt->pay.len = len;                      // strip padding
  mkpay(pkt, (uint32_t *) pkt->ip + ihl);  // account for opts
  frag = mg_ntohs(pkt->ip->frag);
  if (frag & IP_MORE_FRAGS_MSK || frag & IP_FRAG_OFFSET_MSK) {
    struct mg_connection *c;
    if (pkt->ip->proto == 17) pkt->udp = (struct udp *) (pkt->pay.buf);
    if (pkt->ip->proto == 6) pkt->tcp = (struct tcp *) (pkt->pay.buf);
    c = getpeer(ifp->mgr, pkt, false);
    if (c) mg_error(c, "Received fragmented packet");
  } else if (pkt->ip->proto == 1) {
    pkt->icmp = (struct icmp *) (pkt->pay.buf);
    if (pkt->pay.len < sizeof(*pkt->icmp)) return;
    mkpay(pkt, pkt->icmp + 1);
    rx_icmp(ifp, pkt);
  } else if (pkt->ip->proto == 17) {
    pkt->udp = (struct udp *) (pkt->pay.buf);
    if (pkt->pay.len < sizeof(*pkt->udp)) return;  // truncated
    // Take length from UDP header
    len = mg_ntohs(pkt->udp->len);  // UDP datagram length
    if (len < sizeof(*pkt->udp) || len > pkt->pay.len) return;  // malformed
    pkt->pay.len = len;  // strip excess data
    mkpay(pkt, pkt->udp + 1);
    MG_VERBOSE(("UDP %M:%hu -> %M:%hu len %u", mg_print_ip4, &pkt->ip->src,
                mg_ntohs(pkt->udp->sport), mg_print_ip4, &pkt->ip->dst,
                mg_ntohs(pkt->udp->dport), (int) pkt->pay.len));
    if (ifp->enable_dhcp_client && pkt->udp->dport == mg_htons(68)) {
      pkt->dhcp = (struct dhcp *) (pkt->udp + 1);
      mkpay(pkt, &pkt->dhcp->options);
      rx_dhcp_client(ifp, pkt);
    } else if (ifp->enable_dhcp_server && pkt->udp->dport == mg_htons(67)) {
      pkt->dhcp = (struct dhcp *) (pkt->udp + 1);
      mkpay(pkt, &pkt->dhcp->options);
      rx_dhcp_server(ifp, pkt);
    } else if (!rx_udp(ifp, pkt)) {
      // Should send ICMP Destination Unreachable for unicasts, but keep
      // silent
    }
  } else if (pkt->ip->proto == 6) {
    uint8_t off;
    pkt->tcp = (struct tcp *) (pkt->pay.buf);
    if (pkt->pay.len < sizeof(*pkt->tcp)) return;
    off = pkt->tcp->off >> 4;  // account for opts
    if (pkt->pay.len < (uint16_t) (4 * off)) return;
    mkpay(pkt, (uint32_t *) pkt->tcp + off);
    MG_VERBOSE(("TCP %M:%hu -> %M:%hu len %u", mg_print_ip4, &pkt->ip->src,
                mg_ntohs(pkt->tcp->sport), mg_print_ip4, &pkt->ip->dst,
                mg_ntohs(pkt->tcp->dport), (int) pkt->pay.len));
    rx_tcp(ifp, pkt);
  } else {
    MG_DEBUG(("Unknown IP proto %x", (int) pkt->ip->proto));
    if (mg_log_level >= MG_LL_VERBOSE)
      mg_hexdump(pkt->ip, pkt->pay.len >= 32 ? 32 : pkt->pay.len);
  }
}

#if MG_ENABLE_IPV6
static void rx_ip6(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  uint16_t len = 0, plen;
  uint8_t next, *nhdr;
  bool loop = true;
  if (pkt->pay.len < sizeof(*pkt->ip6)) return;  // Truncated
  if ((pkt->ip6->ver >> 4) != 0x6) return;       // Not IPv6
  plen = mg_ntohs(pkt->ip6->plen);
  if (plen > (pkt->pay.len - sizeof(*pkt->ip6))) return;  // malformed
  next = pkt->ip6->next;
  nhdr = (uint8_t *) (pkt->ip6 + 1);
  while (loop) {
    switch (next) {
      case 0:   // Hop-by-Hop 4.3
      case 43:  // Routing 4.4
      case 60:  // Destination Options 4.6
      case 51:  // Authentication RFC-4302
        MG_INFO(("IPv6 extension header %d", (int) next));
        next = nhdr[0];
        len += (uint16_t) (8 * (nhdr[1] + 1));
        nhdr += 8 * (nhdr[1] + 1);
        break;
      case 44:  // Fragment 4.5
      {
        struct mg_connection *c;
        if (nhdr[0] == 17) pkt->udp = (struct udp *) (pkt->pay.buf);
        if (nhdr[0] == 6) pkt->tcp = (struct tcp *) (pkt->pay.buf);
        c = getpeer(ifp->mgr, pkt, false);
        if (c) mg_error(c, "Received fragmented packet");
      }
        return;
      case 59:  // No Next Header 4.7
        return;
      case 50:  // IPsec ESP RFC-4303, unsupported
      default:
        loop = false;
        break;
    }
  }
  if (len >= plen) return;
  // There can be link padding, take payload length from IPv6 header - options
  pkt->pay.buf = (char *) nhdr;
  pkt->pay.len = plen - len;
  if (next == 58) {
    pkt->icmp6 = (struct icmp6 *) (pkt->pay.buf);
    if (pkt->pay.len < sizeof(*pkt->icmp6)) return;
    mkpay(pkt, pkt->icmp6 + 1);
    MG_DEBUG(("ICMPv6 %M -> %M len %u", mg_print_ip6, &pkt->ip6->src,
              mg_print_ip6, &pkt->ip6->dst, (int) pkt->pay.len));
    rx_icmp6(ifp, pkt);
  } else if (next == 17) {
    pkt->udp = (struct udp *) (pkt->pay.buf);
    if (pkt->pay.len < sizeof(*pkt->udp)) return;
    // Take length from UDP header
    len = mg_ntohs(pkt->udp->len);  // UDP datagram length
    if (len < sizeof(*pkt->udp) || len > pkt->pay.len) return;  // malformed
    pkt->pay.len = len;  // strip excess data
    mkpay(pkt, pkt->udp + 1);
    MG_DEBUG(("UDP %M:%hu -> %M:%hu len %u", mg_print_ip6, &pkt->ip6->src,
              mg_ntohs(pkt->udp->sport), mg_print_ip6, &pkt->ip6->dst,
              mg_ntohs(pkt->udp->dport), (int) pkt->pay.len));
    if (ifp->enable_dhcp6_client && pkt->udp->dport == mg_htons(546)) {
      pkt->dhcp6 = (struct dhcp6 *) (pkt->udp + 1);
      mkpay(pkt, pkt->dhcp6 + 1);
      // rx_dhcp6_client(ifp, pkt);
#if 0
    } else if (ifp->enable_dhcp_server && pkt->udp->dport == mg_htons(547)) {
      pkt->dhcp6 = (struct dhcp6 *) (pkt->udp + 1);
      mkpay(pkt, pkt->dhcp6 + 1);
      rx_dhcp6_server(ifp, pkt);
#endif
    } else if (!rx_udp(ifp, pkt)) {
      // Should send ICMPv6 Destination Unreachable for unicasts, keep silent
    }
  } else if (next == 6) {
    uint8_t off;
    pkt->tcp = (struct tcp *) (pkt->pay.buf);
    if (pkt->pay.len < sizeof(*pkt->tcp)) return;
    off = pkt->tcp->off >> 4;  // account for opts
    if (pkt->pay.len < (uint16_t) (4 * off)) return;
    mkpay(pkt, (uint32_t *) pkt->tcp + off);
    MG_DEBUG(("TCP %M:%hu -> %M:%hu len %u", mg_print_ip6, &pkt->ip6->src,
              mg_ntohs(pkt->tcp->sport), mg_print_ip6, &pkt->ip6->dst,
              mg_ntohs(pkt->tcp->dport), (int) pkt->pay.len));
    rx_tcp(ifp, pkt);
  } else {
    MG_DEBUG(("Unknown IPv6 next hdr %x", (int) next));
    if (mg_log_level >= MG_LL_VERBOSE)
      mg_hexdump(pkt->ip6, pkt->pay.len >= 32 ? 32 : pkt->pay.len);
  }
}
#else
#define rx_ip6(x, y)
#endif

static void mg_tcpip_rx(struct mg_tcpip_if *ifp, void *buf, size_t len) {
  struct pkt pkt;
  memset(&pkt, 0, sizeof(pkt));
  pkt.pay.buf = pkt.raw.buf = (char *) buf;
  pkt.pay.len = pkt.raw.len = len;             // payload = raw
  pkt.eth = (struct eth *) buf;                // Ethernet = raw
  if (pkt.raw.len < sizeof(*pkt.eth)) return;  // Truncated - runt?
  if (ifp->enable_mac_check &&
      memcmp(pkt.eth->dst, ifp->mac, sizeof(pkt.eth->dst)) != 0 &&
      memcmp(pkt.eth->dst, broadcast, sizeof(pkt.eth->dst)) != 0)
    return;
  if (ifp->enable_crc32_check && len > 4) {
    uint32_t crc;
    len -= 4;  // TODO(scaprile): check on bigendian
    crc = mg_crc32(0, (const char *) buf, len);
    if (memcmp((void *) ((size_t) buf + len), &crc, sizeof(crc))) return;
    pkt.pay.len = len;
  }
  mkpay(&pkt, pkt.eth + 1);
  if (pkt.eth->type == mg_htons(0x806)) {
    pkt.arp = (struct arp *) (pkt.pay.buf);
    if (pkt.pay.len < sizeof(*pkt.arp)) return;  // Truncated
    mg_tcpip_call(ifp, MG_TCPIP_EV_ARP, &pkt.raw);
    rx_arp(ifp, &pkt);
  } else if (pkt.eth->type == mg_htons(0x86dd)) {
    pkt.ip6 = (struct ip6 *) (pkt.pay.buf);
    rx_ip6(ifp, &pkt);
  } else if (pkt.eth->type == mg_htons(0x800)) {
    pkt.ip = (struct ip *) (pkt.pay.buf);
    rx_ip(ifp, &pkt);
  } else {
    MG_DEBUG(("Unknown eth type %x", mg_htons(pkt.eth->type)));
    if (mg_log_level >= MG_LL_VERBOSE) mg_hexdump(buf, len >= 32 ? 32 : len);
  }
}

static void mg_ip_poll(struct mg_tcpip_if *ifp, bool s1) {
  if (ifp->state == MG_TCPIP_STATE_DOWN) return;
  // DHCP RFC-2131 (4.4)
  if (ifp->enable_dhcp_client && s1) {
    if (ifp->state == MG_TCPIP_STATE_UP) {
      tx_dhcp_discover(ifp);  // INIT (4.4.1)
    } else if (ifp->state == MG_TCPIP_STATE_READY &&
               ifp->lease_expire > 0) {  // BOUND / RENEWING / REBINDING
      if (ifp->now >= ifp->lease_expire) {
        ifp->state = MG_TCPIP_STATE_UP, ifp->ip = 0;  // expired, release IP
        onstatechange(ifp);
      } else if (ifp->now + 30UL * 60UL * 1000UL > ifp->lease_expire &&
                 ((ifp->now / 1000) % 60) == 0) {
        // hack: 30 min before deadline, try to rebind (4.3.6) every min
        tx_dhcp_request_re(ifp, (uint8_t *) broadcast, ifp->ip, 0xffffffff);
      }  // TODO(): Handle T1 (RENEWING) and T2 (REBINDING) (4.4.5)
    }
  }
}
static void mg_ip_link(struct mg_tcpip_if *ifp, bool up) {
  bool current = ifp->state != MG_TCPIP_STATE_DOWN;
  if (!up && ifp->enable_dhcp_client) ifp->ip = 0;
  if (up != current) {  // link state has changed
    ifp->state = up == false                               ? MG_TCPIP_STATE_DOWN
                 : ifp->enable_dhcp_client || ifp->ip == 0 ? MG_TCPIP_STATE_UP
                                                           : MG_TCPIP_STATE_IP;
    onstatechange(ifp);
  } else if (!ifp->enable_dhcp_client && ifp->state == MG_TCPIP_STATE_UP &&
             ifp->ip) {
    ifp->state = MG_TCPIP_STATE_IP;  // ifp->fn has set an IP
    onstatechange(ifp);
  }
}

#if MG_ENABLE_IPV6
static void mg_ip6_poll(struct mg_tcpip_if *ifp, bool s1) {
  if (ifp->state6 == MG_TCPIP_STATE_DOWN) return;
  if (ifp->enable_slaac && s1 && ifp->state6 == MG_TCPIP_STATE_UP)
    tx_ndp_rs(ifp, ifp->gw6, ifp->gw6mac);
}
static void mg_ip6_link(struct mg_tcpip_if *ifp, bool up) {
  bool current = ifp->state6 != MG_TCPIP_STATE_DOWN;
  if (!up && ifp->enable_slaac) ifp->ip6[0] = ifp->ip6[1] = 0;
  if (up != current) {  // link state has changed
    ifp->state6 = !up                                     ? MG_TCPIP_STATE_DOWN
                  : ifp->enable_slaac || ifp->ip6[0] == 0 ? MG_TCPIP_STATE_UP
                                                          : MG_TCPIP_STATE_IP;
    onstate6change(ifp);
  } else if (!ifp->enable_slaac && ifp->state6 == MG_TCPIP_STATE_UP &&
             ifp->ip6[0]) {
    ifp->state6 = MG_TCPIP_STATE_IP;  // ifp->fn has set an IP
    onstate6change(ifp);
  }
}
#else
#define mg_ip6_poll(x, y)
#define mg_ip6_link(x, y)
#endif

static void mg_tcpip_poll(struct mg_tcpip_if *ifp, uint64_t now) {
  struct mg_connection *c;
  bool expired_1000ms = mg_timer_expired(&ifp->timer_1000ms, 1000, now);
  ifp->now = now;

  if (expired_1000ms) {
#if MG_ENABLE_TCPIP_PRINT_DEBUG_STATS
    const char *names[] = {"down", "up", "req", "ip", "ready"};
    size_t max = sizeof(names) / sizeof(char *);
    unsigned int state = ifp->state >= max ? max - 1 : ifp->state;
    MG_INFO(("Status: %s, IP: %M, rx:%u, tx:%u, dr:%u, er:%u", names[state],
             mg_print_ip4, &ifp->ip, ifp->nrecv, ifp->nsent, ifp->ndrop,
             ifp->nerr));
#if MG_ENABLE_IPV6
    state = ifp->state6 >= max ? max - 1 : ifp->state6;
    if (state > MG_TCPIP_STATE_UP)
      MG_INFO(("Status: %s, IPv6: %M", names[state], mg_print_ip6, &ifp->ip6));
#endif
#endif
    backlog_poll(ifp->mgr);
  }
  // Handle gw ARP request timeout, order is important
  if (expired_1000ms && ifp->state == MG_TCPIP_STATE_IP) {
    ifp->state = MG_TCPIP_STATE_READY;  // keep best-effort MAC or poison mark
    onstatechange(ifp);
  }
  if (expired_1000ms && ifp->state == MG_TCPIP_STATE_READY && !ifp->gw_ready &&
      ifp->gw != 0)
    mg_tcpip_arp_request(ifp, ifp->gw, NULL);  // retry GW ARP request
#if MG_ENABLE_IPV6
  // Handle gw NS/NA req/resp timeout, order is important
  if (expired_1000ms && ifp->state6 == MG_TCPIP_STATE_IP) {
    ifp->state6 = MG_TCPIP_STATE_READY;  // keep best-effort MAC or poison mark
    onstate6change(ifp);
  }
  if (expired_1000ms && ifp->state == MG_TCPIP_STATE_READY && !ifp->gw6_ready &&
      (ifp->gw6[0] != 0 || ifp->gw6[1] != 0))
    tx_ndp_ns(ifp, ifp->gw6, ifp->gw6mac);  // retry GW MAC resolution
#endif

  // poll driver
  if (ifp->driver->poll) {
    bool up = ifp->driver->poll(ifp, expired_1000ms);
    // Handle physical interface up/down status, ifp->state rules over state6
    if (expired_1000ms) {
      mg_ip_link(ifp, up);   // Handle IPv4
      mg_ip6_link(ifp, up);  // Handle IPv6
      if (ifp->state == MG_TCPIP_STATE_DOWN) MG_ERROR(("Network is down"));
      mg_tcpip_call(ifp, MG_TCPIP_EV_TIMER_1S, NULL);
    }
  }

  mg_ip_poll(ifp, expired_1000ms);   // Handle IPv4
  mg_ip6_poll(ifp, expired_1000ms);  // Handle IPv6

  if (ifp->state == MG_TCPIP_STATE_DOWN) return;
  // Read data from the network
  if (ifp->driver->rx != NULL) {  // Simple polling driver, returns one frame
    size_t len =
        ifp->driver->rx(ifp->recv_queue.buf, ifp->recv_queue.size, ifp);
    if (len > 0) {
      ifp->nrecv++;
      mg_tcpip_rx(ifp, ifp->recv_queue.buf, len);
    }
  } else {  // Complex poll / Interrupt-based driver. Queues recvd frames
    char *buf;
    size_t len = mg_queue_next(&ifp->recv_queue, &buf);
    if (len > 0) {
      mg_tcpip_rx(ifp, buf, len);
      mg_queue_del(&ifp->recv_queue, len);
    }
  }

  // Process timeouts
  for (c = ifp->mgr->conns; c != NULL; c = c->next) {
    struct connstate *s = (struct connstate *) (c + 1);
    if ((c->is_udp && !c->is_arplooking) || c->is_listening || c->is_resolving)
      continue;
    if (ifp->now > s->timer) {
      if (s->ttype == MIP_TTYPE_ARP) {
        mg_error(c, "ARP timeout");
      } else if (c->is_udp) {
        continue;
      } else if (s->ttype == MIP_TTYPE_ACK && s->acked != s->ack) {
        MG_VERBOSE(("%lu ack %x %x", c->id, s->seq, s->ack));
        tx_tcp(ifp, s->mac, &c->loc, &c->rem, TH_ACK, mg_htonl(s->seq),
               mg_htonl(s->ack), NULL, 0);
        s->acked = s->ack;
      } else if (s->ttype == MIP_TTYPE_SYN) {
        mg_error(c, "Connection timeout");
      } else if (s->ttype == MIP_TTYPE_FIN) {
        c->is_closing = 1;
        continue;
      } else {
        if (s->tmiss++ > 2) {
          mg_error(c, "keepalive");
        } else {
          MG_VERBOSE(("%lu keepalive", c->id));
          tx_tcp(ifp, s->mac, &c->loc, &c->rem, TH_ACK, mg_htonl(s->seq - 1),
                 mg_htonl(s->ack), NULL, 0);
        }
      }

      settmout(c, MIP_TTYPE_KEEPALIVE);
    }
  }
}

// This function executes in interrupt context, thus it should copy data
// somewhere fast. Note that newlib's malloc is not thread safe, thus use
// our lock-free queue with preallocated buffer to copy data and return asap
void mg_tcpip_qwrite(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  char *p;
  if (mg_queue_book(&ifp->recv_queue, &p, len) >= len) {
    memcpy(p, buf, len);
    mg_queue_add(&ifp->recv_queue, len);
    ifp->nrecv++;
  } else {
    ifp->ndrop++;
  }
}

void mg_tcpip_init(struct mg_mgr *mgr, struct mg_tcpip_if *ifp) {
  // If MAC address is not set, make a random one
  if (ifp->mac[0] == 0 && ifp->mac[1] == 0 && ifp->mac[2] == 0 &&
      ifp->mac[3] == 0 && ifp->mac[4] == 0 && ifp->mac[5] == 0) {
    ifp->mac[0] = 0x02;  // Locally administered, unicast
    mg_random(&ifp->mac[1], sizeof(ifp->mac) - 1);
    MG_INFO(("MAC not set. Generated random: %M", mg_print_mac, ifp->mac));
  }

  // If DHCP name is not set, use "mip"
  if (ifp->dhcp_name[0] == '\0') {
    memcpy(ifp->dhcp_name, "mip", 4);
  }
  ifp->dhcp_name[sizeof(ifp->dhcp_name) - 1] = '\0';  // Just in case

  if (ifp->driver->init && !ifp->driver->init(ifp)) {
    MG_ERROR(("driver init failed"));
  } else {
    size_t framesize = 1540;
    ifp->tx.buf = (char *) mg_calloc(1, framesize), ifp->tx.len = framesize;
    if (ifp->recv_queue.size == 0)
      ifp->recv_queue.size = ifp->driver->rx ? framesize : 8192;
    ifp->recv_queue.buf = (char *) mg_calloc(1, ifp->recv_queue.size);
    ifp->timer_1000ms = mg_millis();
    mgr->ifp = ifp;
    ifp->mgr = mgr;
    ifp->mtu = MG_TCPIP_MTU_DEFAULT;
    mgr->extraconnsize = sizeof(struct connstate);
    if (ifp->ip == 0) ifp->enable_dhcp_client = true;
    mg_random(&ifp->eport, sizeof(ifp->eport));  // Random from 0 to 65535
    ifp->eport |= MG_EPHEMERAL_PORT_BASE;        // Random from
                                           // MG_EPHEMERAL_PORT_BASE to 65535
    if (ifp->tx.buf == NULL || ifp->recv_queue.buf == NULL) MG_ERROR(("OOM"));
#if MG_ENABLE_IPV6
    // If static configuration is used, link-local and global addresses,
    // prefix length, and gw are already filled at this point.
    if (ifp->ip6[0] == 0 && ifp->ip6[1] == 0) {
      ifp->enable_slaac = true;
      ip6genll((uint8_t *) ifp->ip6ll, ifp->mac);  // build link-local address
    }
#endif
  }
}

void mg_tcpip_free(struct mg_tcpip_if *ifp) {
  mg_free(ifp->recv_queue.buf);
  mg_free(ifp->tx.buf);
  mg_free(ifp->dns4_url);
}

static void send_syn(struct mg_connection *c) {
  struct connstate *s = (struct connstate *) (c + 1);
  uint32_t isn = mg_htonl((uint32_t) mg_ntohs(c->loc.port));
  tx_tcp(c->mgr->ifp, s->mac, &c->loc, &c->rem, TH_SYN, isn, 0, NULL, 0);
}

static void mac_resolved(struct mg_connection *c) {
  if (c->is_udp) {
    c->is_connecting = 0;
    mg_call(c, MG_EV_CONNECT, NULL);
  } else {
    send_syn(c);
    settmout(c, MIP_TTYPE_SYN);
  }
}

static void ip4_mcastmac(uint8_t *mac, uint32_t *ip) {
  uint8_t mcastp[3] = {0x01, 0x00, 0x5E};  // multicast group MAC
  memcpy(mac, mcastp, 3);
  memcpy(mac + 3, ((uint8_t *) ip) + 1, 3);  // 23 LSb
  mac[3] &= 0x7F;
}

void mg_connect_resolved(struct mg_connection *c) {
  struct mg_tcpip_if *ifp = c->mgr->ifp;
  c->is_resolving = 0;
  if (ifp->eport < MG_EPHEMERAL_PORT_BASE) ifp->eport = MG_EPHEMERAL_PORT_BASE;
  c->loc.port = mg_htons(ifp->eport++);
#if MG_ENABLE_IPV6
  if (c->rem.is_ip6) {
    c->loc.addr.ip6[0] = ifp->ip6[0], c->loc.addr.ip6[1] = ifp->ip6[1],
    c->loc.is_ip6 = true;
  } else
#endif
  {
    c->loc.addr.ip4 = ifp->ip;
  }
  MG_DEBUG(("%lu %M -> %M", c->id, mg_print_ip_port, &c->loc, mg_print_ip_port,
            &c->rem));
  mg_call(c, MG_EV_RESOLVE, NULL);
  c->is_connecting = 1;
#if MG_ENABLE_IPV6
  if (c->rem.is_ip6) {
    if (c->is_udp &&
        MG_IP6MATCH(c->rem.addr.ip6, ip6_allnodes.u)) {  // local broadcast
      struct connstate *s = (struct connstate *) (c + 1);
      memcpy(s->mac, ip6mac_allnodes, sizeof(s->mac));
      mac_resolved(c);
    } else if (c->rem.addr.ip6[0] == ifp->ip6[0] &&  // TODO(): HANDLE PREFIX ***
               !MG_IP6MATCH(c->rem.addr.ip6,
                            ifp->gw6)) {  // skip if gw (onstate6change -> NS)
      // If we're in the same LAN, fire a Neighbor Solicitation
      MG_DEBUG(("%lu NS lookup...", c->id));
      tx_ndp_ns(ifp, c->rem.addr.ip6, ifp->mac);
      settmout(c, MIP_TTYPE_ARP);
      c->is_arplooking = 1;
    } else if (c->is_udp && *((uint8_t *) c->rem.addr.ip6) == 0xFF) {  // multicast
      struct connstate *s = (struct connstate *) (c + 1);
      ip6_mcastmac(s->mac, c->rem.addr.ip6);
      mac_resolved(c);
    } else if (ifp->gw6_ready) {
      struct connstate *s = (struct connstate *) (c + 1);
      memcpy(s->mac, ifp->gw6mac, sizeof(s->mac));
      mac_resolved(c);
    } else {
      MG_ERROR(("No IPv6 gateway, can't connect"));
    }
  } else
#endif
  {
    uint32_t rem_ip = c->rem.addr.ip4;
    if (c->is_udp &&
        (rem_ip == 0xffffffff || rem_ip == (ifp->ip | ~ifp->mask))) {
      struct connstate *s = (struct connstate *) (c + 1);
      memset(s->mac, 0xFF, sizeof(s->mac));  // global or local broadcast
      mac_resolved(c);
    } else if (ifp->ip && ((rem_ip & ifp->mask) == (ifp->ip & ifp->mask)) &&
               rem_ip != ifp->gw) {  // skip if gw (onstatechange -> ARP)
      // If we're in the same LAN, fire an ARP lookup.
      MG_DEBUG(("%lu ARP lookup...", c->id));
      mg_tcpip_arp_request(ifp, rem_ip, NULL);
      settmout(c, MIP_TTYPE_ARP);
      c->is_arplooking = 1;
    } else if (c->is_udp && (*((uint8_t *) &rem_ip) & 0xE0) == 0xE0) {
      struct connstate *s =
          (struct connstate *) (c + 1);  // 224 to 239, E0 to EF
      ip4_mcastmac(s->mac, &rem_ip);     // multicast group
      mac_resolved(c);
    } else if (ifp->gw_ready) {
      struct connstate *s = (struct connstate *) (c + 1);
      memcpy(s->mac, ifp->gwmac, sizeof(s->mac));
      mac_resolved(c);
    } else {
      MG_ERROR(("No gateway, can't connect"));
    }
  }
}

bool mg_open_listener(struct mg_connection *c, const char *url) {
  c->loc.port = mg_htons(mg_url_port(url));
  if (!mg_aton(mg_url_host(url), &c->loc)) {
    MG_ERROR(("invalid listening URL: %s", url));
    return false;
  }
  return true;
}

static void write_conn(struct mg_connection *c) {
  long len = c->is_tls ? mg_tls_send(c, c->send.buf, c->send.len)
                       : mg_io_send(c, c->send.buf, c->send.len);
  // TODO(): mg_tls_send() may return 0 forever on steady OOM
  if (len == MG_IO_ERR) {
    mg_error(c, "tx err");
  } else if (len > 0) {
    mg_iobuf_del(&c->send, 0, (size_t) len);
    mg_call(c, MG_EV_WRITE, &len);
  }
}

static void init_closure(struct mg_connection *c) {
  struct connstate *s = (struct connstate *) (c + 1);
  if (c->is_udp == false && c->is_listening == false &&
      c->is_connecting == false) {  // For TCP conns,
    tx_tcp(c->mgr->ifp, s->mac, &c->loc, &c->rem, TH_FIN | TH_ACK,
           mg_htonl(s->seq), mg_htonl(s->ack), NULL, 0);
    settmout(c, MIP_TTYPE_FIN);
  }
}

static void close_conn(struct mg_connection *c) {
  struct connstate *s = (struct connstate *) (c + 1);
  mg_iobuf_free(&s->raw);  // For TLS connections, release raw data
  mg_close_conn(c);
}

static bool can_write(struct mg_connection *c) {
  return c->is_connecting == 0 && c->is_resolving == 0 && c->send.len > 0 &&
         c->is_tls_hs == 0 && c->is_arplooking == 0;
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  struct mg_connection *c, *tmp;
  uint64_t now = mg_millis();
  mg_timer_poll(&mgr->timers, now);
  if (mgr->ifp == NULL || mgr->ifp->driver == NULL) return;
  mg_tcpip_poll(mgr->ifp, now);
  for (c = mgr->conns; c != NULL; c = tmp) {
    struct connstate *s = (struct connstate *) (c + 1);
    bool is_tls = c->is_tls && !c->is_resolving && !c->is_arplooking &&
                  !c->is_listening && !c->is_connecting;
    tmp = c->next;
    mg_call(c, MG_EV_POLL, &now);
    MG_VERBOSE(("%lu .. %c%c%c%c%c %lu %lu", c->id, c->is_tls ? 'T' : 't',
                c->is_connecting ? 'C' : 'c', c->is_tls_hs ? 'H' : 'h',
                c->is_resolving ? 'R' : 'r', c->is_closing ? 'C' : 'c',
                mg_tls_pending(c), c->rtls.len));
    // order is important, TLS conn close with > 1 record in buffer (below)
    if (is_tls && (c->rtls.len > 0 || mg_tls_pending(c) > 0))
      c->is_tls_hs ? mg_tls_handshake(c) : handle_tls_recv(c);
    if (can_write(c)) write_conn(c);
    if (is_tls && c->send.len == 0) mg_tls_flush(c);
    if (c->is_draining && c->send.len == 0 && s->ttype != MIP_TTYPE_FIN)
      init_closure(c);
    // For non-TLS, close immediately upon completing the 3-way closure
    // For TLS, handle any pending data (above) until MIP_TTYPE_FIN expires
    if (s->twclosure &&
        (!c->is_tls || (c->rtls.len == 0 && mg_tls_pending(c) == 0)))
      c->is_closing = 1;
    if (c->is_closing) close_conn(c);
  }
  (void) ms;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tcpip_if *ifp = c->mgr->ifp;
  bool res = false;
  if (!c->loc.is_ip6 && (ifp->ip == 0 || ifp->state != MG_TCPIP_STATE_READY)) {
    mg_error(c, "net down");
#if MG_ENABLE_IPV6
  } else if (c->loc.is_ip6 && ifp->state6 != MG_TCPIP_STATE_READY) {
    mg_error(c, "net down");
#endif
  } else if (c->is_udp && (c->is_arplooking || c->is_resolving)) {
    // Fail to send, no target MAC or IP
    MG_VERBOSE(("still resolving..."));
  } else if (c->is_udp) {
    len = trim_len(c, len);  // Trimming length if necessary
    res = udp_send(c, buf, len);
  } else {
    res = len == 0 || mg_iobuf_add(&c->send, c->send.len, buf, len) > 0;
    // returning 0 means an OOM condition (iobuf couldn't resize), yet this is
    // so far recoverable, let the caller decide
  }
  return res;
}

uint8_t mcast_addr[6] = {0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb};
void mg_multicast_add(struct mg_connection *c, char *ip) {
  (void) ip;  // ip4/6_mcastmac(mcast_mac, &ip); ipv6 param
  // TODO(): actual IP -> MAC; check database, update
  c->mgr->ifp->update_mac_hash_table = true;  // mark dirty
}

bool mg_dnsc_init(struct mg_mgr *mgr, struct mg_dns *dnsc);

static void setdns4(struct mg_tcpip_if *ifp, uint32_t *ip) {
  struct mg_dns *dnsc;
  mg_free(ifp->dns4_url);
  ifp->dns4_url = mg_mprintf("udp://%M:53", mg_print_ip4, ip);
  dnsc = &ifp->mgr->dns4;
  dnsc->url = (const char *) ifp->dns4_url;
  MG_DEBUG(("Set DNS URL to %s", dnsc->url));
  if (ifp->mgr->use_dns6) return;
  if (dnsc->c != NULL) mg_close_conn(dnsc->c);
  if (!mg_dnsc_init(ifp->mgr, dnsc))  // create DNS connection
    MG_ERROR(("DNS connection creation failed"));
}

#endif  // MG_ENABLE_TCPIP
