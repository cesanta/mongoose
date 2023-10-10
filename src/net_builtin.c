#include "net_builtin.h"

#if defined(MG_ENABLE_TCPIP) && MG_ENABLE_TCPIP
#define MG_EPHEMERAL_PORT_BASE 32768
#define PDIFF(a, b) ((size_t) (((char *) (b)) - ((char *) (a))))

#ifndef MIP_TCP_KEEPALIVE_MS
#define MIP_TCP_KEEPALIVE_MS 45000  // TCP keep-alive period, ms
#endif

#define MIP_TCP_ACK_MS 150    // Timeout for ACKing
#define MIP_TCP_ARP_MS 100    // Timeout for ARP response
#define MIP_TCP_SYN_MS 15000  // Timeout for connection establishment
#define MIP_TCP_FIN_MS 1000   // Timeout for closing connection

struct connstate {
  uint32_t seq, ack;           // TCP seq/ack counters
  uint64_t timer;              // TCP keep-alive / ACK timer
  uint8_t mac[6];              // Peer MAC address
  uint8_t ttype;               // Timer type. 0: ack, 1: keep-alive
#define MIP_TTYPE_KEEPALIVE 0  // Connection is idle for long, send keepalive
#define MIP_TTYPE_ACK 1        // Peer sent us data, we have to ack it soon
#define MIP_TTYPE_ARP 2        // ARP resolve sent, waiting for response
#define MIP_TTYPE_SYN 3        // SYN sent, waiting for response
#define MIP_TTYPE_FIN 4  // FIN sent, waiting until terminating the connection
  uint8_t tmiss;         // Number of keep-alive misses
  struct mg_iobuf raw;   // For TLS only. Incoming raw data
};

#pragma pack(push, 1)

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
  uint16_t len;   // Length
  uint16_t id;    // Unused
  uint16_t frag;  // Fragmentation
#define IP_FRAG_OFFSET_MSK 0xFF1F
#define IP_MORE_FRAGS_MSK 0x20
  uint8_t ttl;    // Time to live
  uint8_t proto;  // Upper level protocol
  uint16_t csum;  // Checksum
  uint32_t src;   // Source IP
  uint32_t dst;   // Destination IP
};

struct ip6 {
  uint8_t ver;      // Version
  uint8_t opts[3];  // Options
  uint16_t len;     // Length
  uint8_t proto;    // Upper level protocol
  uint8_t ttl;      // Time to live
  uint8_t src[16];  // Source IP
  uint8_t dst[16];  // Destination IP
};

struct icmp {
  uint8_t type;
  uint8_t code;
  uint16_t csum;
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
  uint8_t options[32];
};

#pragma pack(pop)

struct pkt {
  struct mg_str raw;  // Raw packet data
  struct mg_str pay;  // Payload data
  struct eth *eth;
  struct llc *llc;
  struct arp *arp;
  struct ip *ip;
  struct ip6 *ip6;
  struct icmp *icmp;
  struct tcp *tcp;
  struct udp *udp;
  struct dhcp *dhcp;
};

static void send_syn(struct mg_connection *c);

static void mkpay(struct pkt *pkt, void *p) {
  pkt->pay =
      mg_str_n((char *) p, (size_t) (&pkt->raw.ptr[pkt->raw.len] - (char *) p));
}

static uint32_t csumup(uint32_t sum, const void *buf, size_t len) {
  const uint8_t *p = (const uint8_t *) buf;
  for (size_t i = 0; i < len; i++) sum += i & 1 ? p[i] : (uint32_t) (p[i] << 8);
  return sum;
}

static uint16_t csumfin(uint32_t sum) {
  while (sum >> 16) sum = (sum & 0xffff) + (sum >> 16);
  return mg_htons(~sum & 0xffff);
}

static uint16_t ipcsum(const void *buf, size_t len) {
  uint32_t sum = csumup(0, buf, len);
  return csumfin(sum);
}

static void settmout(struct mg_connection *c, uint8_t type) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  struct connstate *s = (struct connstate *) (c + 1);
  unsigned n = type == MIP_TTYPE_ACK   ? MIP_TCP_ACK_MS
               : type == MIP_TTYPE_ARP ? MIP_TCP_ARP_MS
               : type == MIP_TTYPE_SYN ? MIP_TCP_SYN_MS
               : type == MIP_TTYPE_FIN ? MIP_TCP_FIN_MS
                                       : MIP_TCP_KEEPALIVE_MS;
  s->timer = ifp->now + n;
  s->ttype = type;
  MG_VERBOSE(("%lu %d -> %llx", c->id, type, s->timer));
}

static size_t ether_output(struct mg_tcpip_if *ifp, size_t len) {
  // size_t min = 64;  // Pad short frames to 64 bytes (minimum Ethernet size)
  // if (len < min) memset(ifp->tx.ptr + len, 0, min - len), len = min;
  // mg_hexdump(ifp->tx.ptr, len);
  size_t n = ifp->driver->tx(ifp->tx.ptr, len, ifp);
  if (n == len) ifp->nsent++;
  return n;
}

static void arp_ask(struct mg_tcpip_if *ifp, uint32_t ip) {
  struct eth *eth = (struct eth *) ifp->tx.ptr;
  struct arp *arp = (struct arp *) (eth + 1);
  memset(eth->dst, 255, sizeof(eth->dst));
  memcpy(eth->src, ifp->mac, sizeof(eth->src));
  eth->type = mg_htons(0x806);
  memset(arp, 0, sizeof(*arp));
  arp->fmt = mg_htons(1), arp->pro = mg_htons(0x800), arp->hlen = 6,
  arp->plen = 4;
  arp->op = mg_htons(1), arp->tpa = ip, arp->spa = ifp->ip;
  memcpy(arp->sha, ifp->mac, sizeof(arp->sha));
  ether_output(ifp, PDIFF(eth, arp + 1));
}

static void onstatechange(struct mg_tcpip_if *ifp) {
  if (ifp->state == MG_TCPIP_STATE_READY) {
    MG_INFO(("READY, IP: %M", mg_print_ip4, &ifp->ip));
    MG_INFO(("       GW: %M", mg_print_ip4, &ifp->gw));
    MG_INFO(("      MAC: %M", mg_print_mac, &ifp->mac));
    arp_ask(ifp, ifp->gw);
  } else if (ifp->state == MG_TCPIP_STATE_UP) {
    MG_ERROR(("Link up"));
    srand((unsigned int) mg_millis());
  } else if (ifp->state == MG_TCPIP_STATE_DOWN) {
    MG_ERROR(("Link down"));
  }
}

static struct ip *tx_ip(struct mg_tcpip_if *ifp, uint8_t *mac_dst,
                        uint8_t proto, uint32_t ip_src, uint32_t ip_dst,
                        size_t plen) {
  struct eth *eth = (struct eth *) ifp->tx.ptr;
  struct ip *ip = (struct ip *) (eth + 1);
  memcpy(eth->dst, mac_dst, sizeof(eth->dst));
  memcpy(eth->src, ifp->mac, sizeof(eth->src));  // Use our MAC
  eth->type = mg_htons(0x800);
  memset(ip, 0, sizeof(*ip));
  ip->ver = 0x45;   // Version 4, header length 5 words
  ip->frag = 0x40;  // Don't fragment
  ip->len = mg_htons((uint16_t) (sizeof(*ip) + plen));
  ip->ttl = 64;
  ip->proto = proto;
  ip->src = ip_src;
  ip->dst = ip_dst;
  ip->csum = ipcsum(ip, sizeof(*ip));
  return ip;
}

static void tx_udp(struct mg_tcpip_if *ifp, uint8_t *mac_dst, uint32_t ip_src,
                   uint16_t sport, uint32_t ip_dst, uint16_t dport,
                   const void *buf, size_t len) {
  struct ip *ip =
      tx_ip(ifp, mac_dst, 17, ip_src, ip_dst, len + sizeof(struct udp));
  struct udp *udp = (struct udp *) (ip + 1);
  // MG_DEBUG(("UDP XX LEN %d %d", (int) len, (int) ifp->tx.len));
  udp->sport = sport;
  udp->dport = dport;
  udp->len = mg_htons((uint16_t) (sizeof(*udp) + len));
  udp->csum = 0;
  uint32_t cs = csumup(0, udp, sizeof(*udp));
  cs = csumup(cs, buf, len);
  cs = csumup(cs, &ip->src, sizeof(ip->src));
  cs = csumup(cs, &ip->dst, sizeof(ip->dst));
  cs += (uint32_t) (ip->proto + sizeof(*udp) + len);
  udp->csum = csumfin(cs);
  memmove(udp + 1, buf, len);
  // MG_DEBUG(("UDP LEN %d %d", (int) len, (int) ifp->frame_len));
  ether_output(ifp, sizeof(struct eth) + sizeof(*ip) + sizeof(*udp) + len);
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
  tx_udp(ifp, mac_dst, ip_src, mg_htons(68), ip_dst, mg_htons(67), &dhcp,
         sizeof(dhcp));
}

static const uint8_t broadcast[] = {255, 255, 255, 255, 255, 255};

// RFC-2131 #4.3.6, #4.4.1
static void tx_dhcp_request_sel(struct mg_tcpip_if *ifp, uint32_t ip_req,
                                uint32_t ip_srv) {
  uint8_t opts[] = {
      53, 1, 3,                 // Type: DHCP request
      55, 2, 1,   3,            // GW and mask
      12, 3, 'm', 'i', 'p',     // Host name: "mip"
      54, 4, 0,   0,   0,   0,  // DHCP server ID
      50, 4, 0,   0,   0,   0,  // Requested IP
      255                       // End of options
  };
  memcpy(opts + 14, &ip_srv, sizeof(ip_srv));
  memcpy(opts + 20, &ip_req, sizeof(ip_req));
  tx_dhcp(ifp, (uint8_t *) broadcast, 0, 0xffffffff, opts, sizeof(opts), false);
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
    if (c->is_arplooking && pkt->arp &&
        memcmp(&pkt->arp->spa, c->rem.ip, sizeof(pkt->arp->spa)) == 0)
      break;
    if (c->is_udp && pkt->udp && c->loc.port == pkt->udp->dport) break;
    if (!c->is_udp && pkt->tcp && c->loc.port == pkt->tcp->dport &&
        lsn == c->is_listening && (lsn || c->rem.port == pkt->tcp->sport))
      break;
  }
  return c;
}

static void rx_arp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  if (pkt->arp->op == mg_htons(1) && pkt->arp->tpa == ifp->ip) {
    // ARP request. Make a response, then send
    // MG_DEBUG(("ARP op %d %M: %M", mg_ntohs(pkt->arp->op), mg_print_ip4,
    //          &pkt->arp->spa, mg_print_ip4, &pkt->arp->tpa));
    struct eth *eth = (struct eth *) ifp->tx.ptr;
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
    if (pkt->arp->spa == ifp->gw) {
      // Got response for the GW ARP request. Set ifp->gwmac
      memcpy(ifp->gwmac, pkt->arp->sha, sizeof(ifp->gwmac));
    } else {
      struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
      if (c != NULL && c->is_arplooking) {
        struct connstate *s = (struct connstate *) (c + 1);
        memcpy(s->mac, pkt->arp->sha, sizeof(s->mac));
        MG_DEBUG(("%lu ARP resolved %M -> %M", c->id, mg_print_ip4, c->rem.ip,
                  mg_print_mac, s->mac));
        c->is_arplooking = 0;
        send_syn(c);
        settmout(c, MIP_TTYPE_SYN);
      }
    }
  }
}

static void rx_icmp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  // MG_DEBUG(("ICMP %d", (int) len));
  if (pkt->icmp->type == 8 && pkt->ip != NULL && pkt->ip->dst == ifp->ip) {
    size_t hlen = sizeof(struct eth) + sizeof(struct ip) + sizeof(struct icmp);
    size_t space = ifp->tx.len - hlen, plen = pkt->pay.len;
    if (plen > space) plen = space;
    struct ip *ip = tx_ip(ifp, pkt->eth->src, 1, ifp->ip, pkt->ip->src,
                          sizeof(struct icmp) + plen);
    struct icmp *icmp = (struct icmp *) (ip + 1);
    memset(icmp, 0, sizeof(*icmp));        // Set csum to 0
    memcpy(icmp + 1, pkt->pay.ptr, plen);  // Copy RX payload to TX
    icmp->csum = ipcsum(icmp, sizeof(*icmp) + plen);
    ether_output(ifp, hlen + plen);
  }
}

static void rx_dhcp_client(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  uint32_t ip = 0, gw = 0, mask = 0, lease = 0;
  uint8_t msgtype = 0, state = ifp->state;
  // perform size check first, then access fields
  uint8_t *p = pkt->dhcp->options,
          *end = (uint8_t *) &pkt->raw.ptr[pkt->raw.len];
  if (end < (uint8_t *) (pkt->dhcp + 1)) return;
  if (memcmp(&pkt->dhcp->xid, ifp->mac + 2, sizeof(pkt->dhcp->xid))) return;
  while (p + 1 < end && p[0] != 255) {  // Parse options RFC-1533 #9
    if (p[0] == 1 && p[1] == sizeof(ifp->mask) && p + 6 < end) {  // Mask
      memcpy(&mask, p + 2, sizeof(mask));
    } else if (p[0] == 3 && p[1] == sizeof(ifp->gw) && p + 6 < end) {  // GW
      memcpy(&gw, p + 2, sizeof(gw));
      ip = pkt->dhcp->yiaddr;
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
             lease) {                                 // DHCPOFFER
    tx_dhcp_request_sel(ifp, ip, pkt->dhcp->siaddr);  // select IP, (4.4.1)
    ifp->state = MG_TCPIP_STATE_REQ;                  // REQUESTING state
  } else if (msgtype == 5) {                          // DHCPACK
    if (ifp->state == MG_TCPIP_STATE_REQ && ip && gw && lease) {  // got an IP
      ifp->lease_expire = ifp->now + lease * 1000;
      MG_INFO(("Lease: %u sec (%lld)", lease, ifp->lease_expire / 1000));
      // assume DHCP server = router until ARP resolves
      memcpy(ifp->gwmac, pkt->eth->src, sizeof(ifp->gwmac));
      ifp->ip = ip, ifp->gw = gw, ifp->mask = mask;
      ifp->state = MG_TCPIP_STATE_READY;  // BOUND state
      uint64_t rand;
      mg_random(&rand, sizeof(rand));
      srand((unsigned int) (rand + mg_millis()));
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
          *end = (uint8_t *) &pkt->raw.ptr[pkt->raw.len];
  if (end < (uint8_t *) (pkt->dhcp + 1)) return;
  // struct dhcp *req = pkt->dhcp;
  struct dhcp res = {2, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, {0}, 0, {0}};
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
        53, 1, msg,                 // Message type
        1,  4, 0,   0,   0,   0,    // Subnet mask
        54, 4, 0,   0,   0,   0,    // Server ID
        12, 3, 'm', 'i', 'p',       // Host name: "mip"
        51, 4, 255, 255, 255, 255,  // Lease time
        255                         // End of options
    };
    memcpy(&res.hwaddr, pkt->dhcp->hwaddr, 6);
    memcpy(opts + 5, &ifp->mask, sizeof(ifp->mask));
    memcpy(opts + 11, &ifp->ip, sizeof(ifp->ip));
    memcpy(&res.options, opts, sizeof(opts));
    res.magic = pkt->dhcp->magic;
    res.xid = pkt->dhcp->xid;
    // memcpy(ifp->gwmac, pkt->eth->src, sizeof(ifp->gwmac));
    tx_udp(ifp, pkt->eth->src, ifp->ip, mg_htons(67),
           op == 1 ? ~0U : res.yiaddr, mg_htons(68), &res, sizeof(res));
  }
}

static void rx_udp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  struct mg_connection *c = getpeer(ifp->mgr, pkt, true);
  if (c == NULL) {
    // No UDP listener on this port. Should send ICMP, but keep silent.
  } else {
    c->rem.port = pkt->udp->sport;
    memcpy(c->rem.ip, &pkt->ip->src, sizeof(uint32_t));
    struct connstate *s = (struct connstate *) (c + 1);
    memcpy(s->mac, pkt->eth->src, sizeof(s->mac));
    if (c->recv.len >= MG_MAX_RECV_SIZE) {
      mg_error(c, "max_recv_buf_size reached");
    } else if (c->recv.size - c->recv.len < pkt->pay.len &&
               !mg_iobuf_resize(&c->recv, c->recv.len + pkt->pay.len)) {
      mg_error(c, "oom");
    } else {
      memcpy(&c->recv.buf[c->recv.len], pkt->pay.ptr, pkt->pay.len);
      c->recv.len += pkt->pay.len;
      mg_call(c, MG_EV_READ, &pkt->pay.len);
    }
  }
}

static size_t tx_tcp(struct mg_tcpip_if *ifp, uint8_t *dst_mac, uint32_t dst_ip,
                     uint8_t flags, uint16_t sport, uint16_t dport,
                     uint32_t seq, uint32_t ack, const void *buf, size_t len) {
  struct ip *ip =
      tx_ip(ifp, dst_mac, 6, ifp->ip, dst_ip, sizeof(struct tcp) + len);
  struct tcp *tcp = (struct tcp *) (ip + 1);
  memset(tcp, 0, sizeof(*tcp));
  if (buf != NULL && len) memmove(tcp + 1, buf, len);
  tcp->sport = sport;
  tcp->dport = dport;
  tcp->seq = seq;
  tcp->ack = ack;
  tcp->flags = flags;
  tcp->win = mg_htons(8192);
  tcp->off = (uint8_t) (sizeof(*tcp) / 4 << 4);
  uint32_t cs = 0;
  uint16_t n = (uint16_t) (sizeof(*tcp) + len);
  uint8_t pseudo[] = {0, ip->proto, (uint8_t) (n >> 8), (uint8_t) (n & 255)};
  cs = csumup(cs, tcp, n);
  cs = csumup(cs, &ip->src, sizeof(ip->src));
  cs = csumup(cs, &ip->dst, sizeof(ip->dst));
  cs = csumup(cs, pseudo, sizeof(pseudo));
  tcp->csum = csumfin(cs);
  MG_VERBOSE(("TCP %M:%hu -> %M:%hu fl %x len %u", mg_print_ip4, &ip->src,
              mg_ntohs(tcp->sport), mg_print_ip4, &ip->dst,
              mg_ntohs(tcp->dport), tcp->flags, (int) len));
  // mg_hexdump(ifp->tx.ptr, PDIFF(ifp->tx.ptr, tcp + 1) + len);
  return ether_output(ifp, PDIFF(ifp->tx.ptr, tcp + 1) + len);
}

static size_t tx_tcp_pkt(struct mg_tcpip_if *ifp, struct pkt *pkt,
                         uint8_t flags, uint32_t seq, const void *buf,
                         size_t len) {
  uint32_t delta = (pkt->tcp->flags & (TH_SYN | TH_FIN)) ? 1 : 0;
  return tx_tcp(ifp, pkt->eth->src, pkt->ip->src, flags, pkt->tcp->dport,
                pkt->tcp->sport, seq, mg_htonl(mg_ntohl(pkt->tcp->seq) + delta),
                buf, len);
}

static struct mg_connection *accept_conn(struct mg_connection *lsn,
                                         struct pkt *pkt) {
  struct mg_connection *c = mg_alloc_conn(lsn->mgr);
  if (c == NULL) {
    MG_ERROR(("OOM"));
    return NULL;
  }
  struct connstate *s = (struct connstate *) (c + 1);
  s->seq = mg_ntohl(pkt->tcp->ack), s->ack = mg_ntohl(pkt->tcp->seq);
  memcpy(s->mac, pkt->eth->src, sizeof(s->mac));
  settmout(c, MIP_TTYPE_KEEPALIVE);
  memcpy(c->rem.ip, &pkt->ip->src, sizeof(uint32_t));
  c->rem.port = pkt->tcp->sport;
  MG_DEBUG(("%lu accepted %M", c->id, mg_print_ip_port, &c->rem));
  LIST_ADD_HEAD(struct mg_connection, &lsn->mgr->conns, c);
  c->is_accepted = 1;
  c->is_hexdumping = lsn->is_hexdumping;
  c->pfn = lsn->pfn;
  c->loc = lsn->loc;
  c->pfn_data = lsn->pfn_data;
  c->fn = lsn->fn;
  c->fn_data = lsn->fn_data;
  mg_call(c, MG_EV_OPEN, NULL);
  mg_call(c, MG_EV_ACCEPT, NULL);
  return c;
}

static size_t trim_len(struct mg_connection *c, size_t len) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  size_t eth_h_len = 14, ip_max_h_len = 24, tcp_max_h_len = 60, udp_h_len = 8;
  size_t max_headers_len = eth_h_len + ip_max_h_len +
                          (c->is_udp ? udp_h_len : tcp_max_h_len);
  size_t min_mtu = c->is_udp ? 68 /* RFC-791 */ : max_headers_len - eth_h_len;

  // If the frame exceeds the available buffer, trim the length
  if (len + max_headers_len > ifp->tx.len) {
    len = ifp->tx.len - max_headers_len;
  }
  // Ensure the MTU isn't lower than the minimum allowed value
  if (ifp->mtu < min_mtu) {
    MG_ERROR(("MTU is lower than minimum possible value. Setting it to %d.",
              min_mtu));
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

long mg_io_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  struct connstate *s = (struct connstate *) (c + 1);
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  len = trim_len(c, len);
  if (c->is_udp) {
    tx_udp(ifp, s->mac, ifp->ip, c->loc.port, rem_ip, c->rem.port, buf, len);
  } else {
    if (tx_tcp(ifp, s->mac, rem_ip, TH_PUSH | TH_ACK, c->loc.port, c->rem.port,
               mg_htonl(s->seq), mg_htonl(s->ack), buf, len) > 0) {
      s->seq += (uint32_t) len;
      if (s->ttype == MIP_TTYPE_ACK) settmout(c, MIP_TTYPE_KEEPALIVE);
    } else {
      return MG_IO_ERR;
    }
  }
  return (long) len;
}

long mg_io_recv(struct mg_connection *c, void *buf, size_t len) {
  struct connstate *s = (struct connstate *) (c + 1);
  if (s->raw.len == 0) return MG_IO_WAIT;
  if (len > s->raw.len) len = s->raw.len;
  memcpy(buf, s->raw.buf, len);
  mg_iobuf_del(&s->raw, 0, len);
  return (long) len;
}

static void read_conn(struct mg_connection *c, struct pkt *pkt) {
  struct connstate *s = (struct connstate *) (c + 1);
  struct mg_iobuf *io = c->is_tls ? &s->raw : &c->recv;
  uint32_t seq = mg_ntohl(pkt->tcp->seq);
  s->raw.align = c->recv.align;
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  if (pkt->tcp->flags & TH_FIN) {
    // If we initiated the closure, we reply with ACK upon receiving FIN
    // If we didn't initiate it, we reply with FIN as part of the normal TCP
    // closure process
    uint8_t flags = TH_ACK;
    s->ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len + 1);
    if (c->is_draining && s->ttype == MIP_TTYPE_FIN) {
      if (s->seq == mg_htonl(pkt->tcp->ack)) {  // Simultaneous closure ?
        s->seq++;                               // Yes. Increment our SEQ
      } else {                                  // Otherwise,
        s->seq = mg_htonl(pkt->tcp->ack);       // Set to peer's ACK
      }
    } else {
      flags |= TH_FIN;
      c->is_draining = 1;
      settmout(c, MIP_TTYPE_FIN);
    }
    tx_tcp((struct mg_tcpip_if *) c->mgr->priv, s->mac, rem_ip, flags,
           c->loc.port, c->rem.port, mg_htonl(s->seq), mg_htonl(s->ack), "", 0);
  } else if (pkt->pay.len == 0) {
    // TODO(cpq): handle this peer's ACK
  } else if (seq != s->ack) {
    uint32_t ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len);
    if (s->ack == ack) {
      MG_VERBOSE(("ignoring duplicate pkt"));
    } else {
      MG_VERBOSE(("SEQ != ACK: %x %x %x", seq, s->ack, ack));
      tx_tcp((struct mg_tcpip_if *) c->mgr->priv, s->mac, rem_ip, TH_ACK,
             c->loc.port, c->rem.port, mg_htonl(s->seq), mg_htonl(s->ack), "",
             0);
    }
  } else if (io->size - io->len < pkt->pay.len &&
             !mg_iobuf_resize(io, io->len + pkt->pay.len)) {
    mg_error(c, "oom");
  } else {
    // Copy TCP payload into the IO buffer. If the connection is plain text,
    // we copy to c->recv. If the connection is TLS, this data is encrypted,
    // therefore we copy that encrypted data to the s->raw iobuffer instead,
    // and then call mg_tls_recv() to decrypt it. NOTE: mg_tls_recv() will
    // call back mg_io_recv() which grabs raw data from s->raw
    memcpy(&io->buf[io->len], pkt->pay.ptr, pkt->pay.len);
    io->len += pkt->pay.len;

    MG_VERBOSE(("%lu SEQ %x -> %x", c->id, mg_htonl(pkt->tcp->seq), s->ack));
    // Advance ACK counter
    s->ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len);
#if 0
    // Send ACK immediately
    uint32_t rem_ip;
    memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
    MG_DEBUG(("  imm ACK", c->id, mg_htonl(pkt->tcp->seq), s->ack));
    tx_tcp((struct mg_tcpip_if *) c->mgr->priv, s->mac, rem_ip, TH_ACK, c->loc.port,
           c->rem.port, mg_htonl(s->seq), mg_htonl(s->ack), "", 0);
#else
    // if not already running, setup a timer to send an ACK later
    if (s->ttype != MIP_TTYPE_ACK) settmout(c, MIP_TTYPE_ACK);
#endif

    if (c->is_tls) {
      // TLS connection. Make room for decrypted data in c->recv
      io = &c->recv;
      if (io->size - io->len < pkt->pay.len &&
          !mg_iobuf_resize(io, io->len + pkt->pay.len)) {
        mg_error(c, "oom");
      } else {
        // Decrypt data directly into c->recv
        long n = mg_tls_recv(c, &io->buf[io->len], io->size - io->len);
        if (n == MG_IO_ERR) {
          mg_error(c, "TLS recv error");
        } else if (n > 0) {
          // Decrypted successfully - trigger MG_EV_READ
          io->len += (size_t) n;
          mg_call(c, MG_EV_READ, &n);
        }
      }
    } else {
      // Plain text connection, data is already in c->recv, trigger
      // MG_EV_READ
      mg_call(c, MG_EV_READ, &pkt->pay.len);
    }
  }
}

static void rx_tcp(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
  struct connstate *s = c == NULL ? NULL : (struct connstate *) (c + 1);
#if 0
  MG_INFO(("%lu %hhu %d", c ? c->id : 0, pkt->tcp->flags, (int) pkt->pay.len));
#endif
  if (c != NULL && c->is_connecting && pkt->tcp->flags == (TH_SYN | TH_ACK)) {
    s->seq = mg_ntohl(pkt->tcp->ack), s->ack = mg_ntohl(pkt->tcp->seq) + 1;
    tx_tcp_pkt(ifp, pkt, TH_ACK, pkt->tcp->ack, NULL, 0);
    c->is_connecting = 0;  // Client connected
    settmout(c, MIP_TTYPE_KEEPALIVE);
    mg_call(c, MG_EV_CONNECT, NULL);  // Let user know
  } else if (c != NULL && c->is_connecting && pkt->tcp->flags != TH_ACK) {
    // mg_hexdump(pkt->raw.ptr, pkt->raw.len);
    tx_tcp_pkt(ifp, pkt, TH_RST | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else if (c != NULL && pkt->tcp->flags & TH_RST) {
    mg_error(c, "peer RST");  // RFC-1122 4.2.2.13
  } else if (c != NULL) {
#if 0
    MG_DEBUG(("%lu %d %M:%hu -> %M:%hu", c->id, (int) pkt->raw.len,
              mg_print_ip4, &pkt->ip->src, mg_ntohs(pkt->tcp->sport),
              mg_print_ip4, &pkt->ip->dst, mg_ntohs(pkt->tcp->dport)));
    mg_hexdump(pkt->pay.ptr, pkt->pay.len);
#endif
    s->tmiss = 0;                         // Reset missed keep-alive counter
    if (s->ttype == MIP_TTYPE_KEEPALIVE)  // Advance keep-alive timer
      settmout(c,
               MIP_TTYPE_KEEPALIVE);  // unless a former ACK timeout is pending
    read_conn(c, pkt);  // Override timer with ACK timeout if needed
  } else if ((c = getpeer(ifp->mgr, pkt, true)) == NULL) {
    tx_tcp_pkt(ifp, pkt, TH_RST | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else if (pkt->tcp->flags & TH_RST) {
    if (c->is_accepted) mg_error(c, "peer RST");  // RFC-1122 4.2.2.13
    // ignore RST if not connected
  } else if (pkt->tcp->flags & TH_SYN) {
    // Use peer's source port as ISN, in order to recognise the handshake
    uint32_t isn = mg_htonl((uint32_t) mg_ntohs(pkt->tcp->sport));
    tx_tcp_pkt(ifp, pkt, TH_SYN | TH_ACK, isn, NULL, 0);
  } else if (pkt->tcp->flags & TH_FIN) {
    tx_tcp_pkt(ifp, pkt, TH_FIN | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else if (mg_htonl(pkt->tcp->ack) == mg_htons(pkt->tcp->sport) + 1U) {
    accept_conn(c, pkt);
  } else if (!c->is_accepted) {  // no peer
    tx_tcp_pkt(ifp, pkt, TH_RST | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else {
    // MG_VERBOSE(("dropped silently.."));
  }
}

static void rx_ip(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  if (pkt->ip->frag & IP_MORE_FRAGS_MSK ||
        pkt->ip->frag & IP_FRAG_OFFSET_MSK) {
    if (pkt->ip->proto == 17) pkt->udp = (struct udp *) (pkt->ip + 1);
    if (pkt->ip->proto == 6) pkt->tcp = (struct tcp *) (pkt->ip + 1);
    struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
    if (c)  mg_error(c, "Received fragmented packet");
  } else if (pkt->ip->proto == 1) {
    pkt->icmp = (struct icmp *) (pkt->ip + 1);
    if (pkt->pay.len < sizeof(*pkt->icmp)) return;
    mkpay(pkt, pkt->icmp + 1);
    rx_icmp(ifp, pkt);
  } else if (pkt->ip->proto == 17) {
    pkt->udp = (struct udp *) (pkt->ip + 1);
    if (pkt->pay.len < sizeof(*pkt->udp)) return;
    mkpay(pkt, pkt->udp + 1);
    MG_VERBOSE(("UDP %M:%hu -> %M:%hu len %u", mg_print_ip4, &pkt->ip->src,
                mg_ntohs(pkt->udp->sport), mg_print_ip4, &pkt->ip->dst,
                mg_ntohs(pkt->udp->dport), (int) pkt->pay.len));
    if (ifp->enable_dhcp_client && pkt->udp->dport == mg_htons(68)) {
      pkt->dhcp = (struct dhcp *) (pkt->udp + 1);
      mkpay(pkt, pkt->dhcp + 1);
      rx_dhcp_client(ifp, pkt);
    } else if (ifp->enable_dhcp_server && pkt->udp->dport == mg_htons(67)) {
      pkt->dhcp = (struct dhcp *) (pkt->udp + 1);
      mkpay(pkt, pkt->dhcp + 1);
      rx_dhcp_server(ifp, pkt);
    } else {
      rx_udp(ifp, pkt);
    }
  } else if (pkt->ip->proto == 6) {
    pkt->tcp = (struct tcp *) (pkt->ip + 1);
    if (pkt->pay.len < sizeof(*pkt->tcp)) return;
    mkpay(pkt, pkt->tcp + 1);
    uint16_t iplen = mg_ntohs(pkt->ip->len);
    uint16_t off = (uint16_t) (sizeof(*pkt->ip) + ((pkt->tcp->off >> 4) * 4U));
    if (iplen >= off) pkt->pay.len = (size_t) (iplen - off);
    MG_VERBOSE(("TCP %M:%hu -> %M:%hu len %u", mg_print_ip4, &pkt->ip->src,
                mg_ntohs(pkt->tcp->sport), mg_print_ip4, &pkt->ip->dst,
                mg_ntohs(pkt->tcp->dport), (int) pkt->pay.len));
    rx_tcp(ifp, pkt);
  }
}

static void rx_ip6(struct mg_tcpip_if *ifp, struct pkt *pkt) {
  // MG_DEBUG(("IP %d", (int) len));
  if (pkt->ip6->proto == 1 || pkt->ip6->proto == 58) {
    pkt->icmp = (struct icmp *) (pkt->ip6 + 1);
    if (pkt->pay.len < sizeof(*pkt->icmp)) return;
    mkpay(pkt, pkt->icmp + 1);
    rx_icmp(ifp, pkt);
  } else if (pkt->ip6->proto == 17) {
    pkt->udp = (struct udp *) (pkt->ip6 + 1);
    if (pkt->pay.len < sizeof(*pkt->udp)) return;
    // MG_DEBUG(("  UDP %u %u -> %u", len, mg_htons(udp->sport),
    // mg_htons(udp->dport)));
    mkpay(pkt, pkt->udp + 1);
  }
}

static void mg_tcpip_rx(struct mg_tcpip_if *ifp, void *buf, size_t len) {
  struct pkt pkt;
  memset(&pkt, 0, sizeof(pkt));
  pkt.raw.ptr = (char *) buf;
  pkt.raw.len = len;
  pkt.eth = (struct eth *) buf;
  // mg_hexdump(buf, len > 16 ? 16: len);
  if (pkt.raw.len < sizeof(*pkt.eth)) return;  // Truncated - runt?
  if (ifp->enable_mac_check &&
      memcmp(pkt.eth->dst, ifp->mac, sizeof(pkt.eth->dst)) != 0 &&
      memcmp(pkt.eth->dst, broadcast, sizeof(pkt.eth->dst)) != 0)
    return;
  if (ifp->enable_crc32_check && len > 4) {
    len -= 4;  // TODO(scaprile): check on bigendian
    uint32_t crc = mg_crc32(0, (const char *) buf, len);
    if (memcmp((void *) ((size_t) buf + len), &crc, sizeof(crc))) return;
  }
  if (pkt.eth->type == mg_htons(0x806)) {
    pkt.arp = (struct arp *) (pkt.eth + 1);
    if (sizeof(*pkt.eth) + sizeof(*pkt.arp) > pkt.raw.len) return;  // Truncated
    rx_arp(ifp, &pkt);
  } else if (pkt.eth->type == mg_htons(0x86dd)) {
    pkt.ip6 = (struct ip6 *) (pkt.eth + 1);
    if (pkt.raw.len < sizeof(*pkt.eth) + sizeof(*pkt.ip6)) return;  // Truncated
    if ((pkt.ip6->ver >> 4) != 0x6) return;                         // Not IP
    mkpay(&pkt, pkt.ip6 + 1);
    rx_ip6(ifp, &pkt);
  } else if (pkt.eth->type == mg_htons(0x800)) {
    pkt.ip = (struct ip *) (pkt.eth + 1);
    if (pkt.raw.len < sizeof(*pkt.eth) + sizeof(*pkt.ip)) return;  // Truncated
    // Truncate frame to what IP header tells us
    if ((size_t) mg_ntohs(pkt.ip->len) + sizeof(struct eth) < pkt.raw.len) {
      pkt.raw.len = (size_t) mg_ntohs(pkt.ip->len) + sizeof(struct eth);
    }
    if (pkt.raw.len < sizeof(*pkt.eth) + sizeof(*pkt.ip)) return;  // Truncated
    if ((pkt.ip->ver >> 4) != 4) return;                           // Not IP
    mkpay(&pkt, pkt.ip + 1);
    rx_ip(ifp, &pkt);
  } else {
    MG_DEBUG(("Unknown eth type %x", mg_htons(pkt.eth->type)));
    mg_hexdump(buf, len >= 32 ? 32 : len);
  }
}

static void mg_tcpip_poll(struct mg_tcpip_if *ifp, uint64_t uptime_ms) {
  if (ifp == NULL || ifp->driver == NULL) return;
  bool expired_1000ms = mg_timer_expired(&ifp->timer_1000ms, 1000, uptime_ms);
  ifp->now = uptime_ms;

  // Handle physical interface up/down status
  if (expired_1000ms && ifp->driver->up) {
    bool up = ifp->driver->up(ifp);
    bool current = ifp->state != MG_TCPIP_STATE_DOWN;
    if (up != current) {
      ifp->state = up == false               ? MG_TCPIP_STATE_DOWN
                   : ifp->enable_dhcp_client ? MG_TCPIP_STATE_UP
                                             : MG_TCPIP_STATE_READY;
      if (!up && ifp->enable_dhcp_client) ifp->ip = 0;
      onstatechange(ifp);
    }
  }
  if (ifp->state == MG_TCPIP_STATE_DOWN) return;

  // DHCP RFC-2131 (4.4)
  if (ifp->state == MG_TCPIP_STATE_UP && expired_1000ms) {
    tx_dhcp_discover(ifp);  // INIT (4.4.1)
  } else if (expired_1000ms && ifp->state == MG_TCPIP_STATE_READY &&
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

  // Read data from the network
  if (ifp->driver->rx != NULL) {  // Polling driver. We must call it
    size_t len =
        ifp->driver->rx(ifp->recv_queue.buf, ifp->recv_queue.size, ifp);
    if (len > 0) {
      ifp->nrecv++;
      mg_tcpip_rx(ifp, ifp->recv_queue.buf, len);
    }
  } else {  // Interrupt-based driver. Fills recv queue itself
    char *buf;
    size_t len = mg_queue_next(&ifp->recv_queue, &buf);
    if (len > 0) {
      mg_tcpip_rx(ifp, buf, len);
      mg_queue_del(&ifp->recv_queue, len);
    }
  }

  // Process timeouts
  for (struct mg_connection *c = ifp->mgr->conns; c != NULL; c = c->next) {
    if (c->is_udp || c->is_listening || c->is_resolving) continue;
    struct connstate *s = (struct connstate *) (c + 1);
    uint32_t rem_ip;
    memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
    if (uptime_ms > s->timer) {
      if (s->ttype == MIP_TTYPE_ACK) {
        MG_VERBOSE(("%lu ack %x %x", c->id, s->seq, s->ack));
        tx_tcp(ifp, s->mac, rem_ip, TH_ACK, c->loc.port, c->rem.port,
               mg_htonl(s->seq), mg_htonl(s->ack), "", 0);
      } else if (s->ttype == MIP_TTYPE_ARP) {
        mg_error(c, "ARP timeout");
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
          tx_tcp(ifp, s->mac, rem_ip, TH_ACK, c->loc.port, c->rem.port,
                 mg_htonl(s->seq - 1), mg_htonl(s->ack), "", 0);
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

  if (ifp->driver->init && !ifp->driver->init(ifp)) {
    MG_ERROR(("driver init failed"));
  } else {
    size_t framesize = 1540;
    ifp->tx.ptr = (char *) calloc(1, framesize), ifp->tx.len = framesize;
    if (ifp->recv_queue.size == 0)
      ifp->recv_queue.size = ifp->driver->rx ? framesize : 8192;
    ifp->recv_queue.buf = (char *) calloc(1, ifp->recv_queue.size);
    ifp->timer_1000ms = mg_millis();
    mgr->priv = ifp;
    ifp->mgr = mgr;
    ifp->mtu = MG_TCPIP_MTU_DEFAULT;
    mgr->extraconnsize = sizeof(struct connstate);
    if (ifp->ip == 0) ifp->enable_dhcp_client = true;
    memset(ifp->gwmac, 255, sizeof(ifp->gwmac));  // Set to broadcast
    mg_random(&ifp->eport, sizeof(ifp->eport));   // Random from 0 to 65535
    ifp->eport |= MG_EPHEMERAL_PORT_BASE;         // Random from
                                           // MG_EPHEMERAL_PORT_BASE to 65535
    if (ifp->tx.ptr == NULL || ifp->recv_queue.buf == NULL) MG_ERROR(("OOM"));
  }
}

void mg_tcpip_free(struct mg_tcpip_if *ifp) {
  free(ifp->recv_queue.buf);
  free((char *) ifp->tx.ptr);
}

static void send_syn(struct mg_connection *c) {
  struct connstate *s = (struct connstate *) (c + 1);
  uint32_t isn = mg_htonl((uint32_t) mg_ntohs(c->loc.port));
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  tx_tcp(ifp, s->mac, rem_ip, TH_SYN, c->loc.port, c->rem.port, isn, 0, NULL,
         0);
}

void mg_connect_resolved(struct mg_connection *c) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  c->is_resolving = 0;
  if (ifp->eport < MG_EPHEMERAL_PORT_BASE) ifp->eport = MG_EPHEMERAL_PORT_BASE;
  memcpy(c->loc.ip, &ifp->ip, sizeof(uint32_t));
  c->loc.port = mg_htons(ifp->eport++);
  MG_DEBUG(("%lu %M -> %M", c->id, mg_print_ip_port, &c->loc, mg_print_ip_port,
            &c->rem));
  mg_call(c, MG_EV_RESOLVE, NULL);
  if (c->is_udp && (rem_ip == 0xffffffff || rem_ip == (ifp->ip | ~ifp->mask))) {
    struct connstate *s = (struct connstate *) (c + 1);
    memset(s->mac, 0xFF, sizeof(s->mac));  // global or local broadcast
  } else if (((rem_ip & ifp->mask) == (ifp->ip & ifp->mask))) {
    // If we're in the same LAN, fire an ARP lookup.
    MG_DEBUG(("%lu ARP lookup...", c->id));
    arp_ask(ifp, rem_ip);
    settmout(c, MIP_TTYPE_ARP);
    c->is_arplooking = 1;
    c->is_connecting = 1;
  } else if ((*((uint8_t *) &rem_ip) & 0xE0) == 0xE0) {
    struct connstate *s = (struct connstate *) (c + 1);  // 224 to 239, E0 to EF
    uint8_t mcastp[3] = {0x01, 0x00, 0x5E};              // multicast group
    memcpy(s->mac, mcastp, 3);
    memcpy(s->mac + 3, ((uint8_t *) &rem_ip) + 1, 3);  // 23 LSb
    s->mac[3] &= 0x7F;
  } else {
    struct connstate *s = (struct connstate *) (c + 1);
    memcpy(s->mac, ifp->gwmac, sizeof(ifp->gwmac));
    if (c->is_udp) {
      mg_call(c, MG_EV_CONNECT, NULL);
    } else {
      send_syn(c);
      settmout(c, MIP_TTYPE_SYN);
      c->is_connecting = 1;
    }
  }
}

bool mg_open_listener(struct mg_connection *c, const char *url) {
  c->loc.port = mg_htons(mg_url_port(url));
  return true;
}

static void write_conn(struct mg_connection *c) {
  long len = c->is_tls ? mg_tls_send(c, c->send.buf, c->send.len)
                       : mg_io_send(c, c->send.buf, c->send.len);
  if (len > 0) {
    mg_iobuf_del(&c->send, 0, (size_t) len);
    mg_call(c, MG_EV_WRITE, &len);
  }
}

static void init_closure(struct mg_connection *c) {
  struct connstate *s = (struct connstate *) (c + 1);
  if (c->is_udp == false && c->is_listening == false &&
      c->is_connecting == false) {  // For TCP conns,
    struct mg_tcpip_if *ifp =
        (struct mg_tcpip_if *) c->mgr->priv;  // send TCP FIN
    uint32_t rem_ip;
    memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
    tx_tcp(ifp, s->mac, rem_ip, TH_FIN | TH_ACK, c->loc.port, c->rem.port,
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
  mg_tcpip_poll((struct mg_tcpip_if *) mgr->priv, now);
  mg_timer_poll(&mgr->timers, now);
  for (c = mgr->conns; c != NULL; c = tmp) {
    tmp = c->next;
    struct connstate *s = (struct connstate *) (c + 1);
    mg_call(c, MG_EV_POLL, &now);
    MG_VERBOSE(("%lu .. %c%c%c%c%c", c->id, c->is_tls ? 'T' : 't',
                c->is_connecting ? 'C' : 'c', c->is_tls_hs ? 'H' : 'h',
                c->is_resolving ? 'R' : 'r', c->is_closing ? 'C' : 'c'));
    if (c->is_tls_hs) mg_tls_handshake(c);
    if (can_write(c)) write_conn(c);
    if (c->is_draining && c->send.len == 0 && s->ttype != MIP_TTYPE_FIN)
      init_closure(c);
    if (c->is_closing) close_conn(c);
  }
  (void) ms;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->mgr->priv;
  bool res = false;
  uint32_t rem_ip;
  memcpy(&rem_ip, c->rem.ip, sizeof(uint32_t));
  if (ifp->ip == 0 || ifp->state != MG_TCPIP_STATE_READY) {
    mg_error(c, "net down");
  } else if (c->is_udp) {
    struct connstate *s = (struct connstate *) (c + 1);
    len = trim_len(c, len);   // Trimming length if necessary
    tx_udp(ifp, s->mac, ifp->ip, c->loc.port, rem_ip, c->rem.port, buf, len);
    res = true;
  } else {
    res = mg_iobuf_add(&c->send, c->send.len, buf, len);
  }
  return res;
}
#endif  // MG_ENABLE_TCPIP
