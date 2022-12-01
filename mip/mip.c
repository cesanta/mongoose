#include "mip.h"

#if MG_ENABLE_MIP

#define MIP_ETHEMERAL_PORT 49152
#define U16(ptr) ((((uint16_t) (ptr)[0]) << 8) | (ptr)[1])
#define PDIFF(a, b) ((size_t) (((char *) (b)) - ((char *) (a))))

#ifndef MIP_QSIZE
#define MIP_QSIZE (16 * 1024)  // Queue size
#endif

#ifndef MIP_TCP_KEEPALIVE_MS
#define MIP_TCP_KEEPALIVE_MS 45000  // TCP keep-alive period, ms
#endif

#define MIP_TCP_ACK_MS 150  // Timeout for ACKing

struct connstate {
  uint32_t seq, ack;           // TCP seq/ack counters
  uint64_t timer;              // TCP keep-alive / ACK timer
  uint8_t mac[6];              // Peer MAC address
  uint8_t ttype;               // Timer type. 0: ack, 1: keep-alive
#define MIP_TTYPE_KEEPALIVE 0  // Connection is idle for long, send keepalive
#define MIP_TTYPE_ACK 1        // Peer sent us data, we have to ack it soon
  uint8_t tmiss;               // Number of keep-alive misses
  struct mg_iobuf raw;         // For TLS only. Incoming raw data
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

static void q_copyin(struct queue *q, const uint8_t *buf, size_t len,
                     size_t head) {
  size_t left = q->len - head;
  memcpy(&q->buf[head], buf, left < len ? left : len);
  if (left < len) memcpy(q->buf, &buf[left], len - left);
}

static void q_copyout(struct queue *q, uint8_t *buf, size_t len, size_t tail) {
  size_t left = q->len - tail;
  memcpy(buf, &q->buf[tail], left < len ? left : len);
  if (left < len) memcpy(&buf[left], q->buf, len - left);
}

static bool q_write(struct queue *q, const void *buf, size_t len) {
  bool success = false;
  size_t left = (q->len - q->head + q->tail - 1) % q->len;
  if (len + sizeof(size_t) <= left) {
    q_copyin(q, (uint8_t *) &len, sizeof(len), q->head);
    q_copyin(q, (uint8_t *) buf, len, (q->head + sizeof(size_t)) % q->len);
    q->head = (q->head + sizeof(len) + len) % q->len;
    success = true;
  }
  return success;
}

#ifdef MIP_QPROFILE
static inline size_t q_space(struct queue *q) {
  return q->tail > q->head ? q->tail - q->head : q->tail + (q->len - q->head);
}
#endif

static inline size_t q_avail(struct queue *q) {
  size_t n = 0;
  if (q->tail != q->head) q_copyout(q, (uint8_t *) &n, sizeof(n), q->tail);
  return n;
}

static size_t q_read(struct queue *q, void *buf) {
  size_t n = q_avail(q);
  if (n > 0) {
    q_copyout(q, (uint8_t *) buf, n, (q->tail + sizeof(n)) % q->len);
    q->tail = (q->tail + sizeof(n) + n) % q->len;
  }
  return n;
}

static struct mg_str mkstr(void *buf, size_t len) {
  struct mg_str str = {(char *) buf, len};
  return str;
}

static void mkpay(struct pkt *pkt, void *p) {
  pkt->pay = mkstr(p, (size_t) (&pkt->raw.ptr[pkt->raw.len] - (char *) p));
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

// ARP cache is organised as a doubly linked list. A successful cache lookup
// moves an entry to the head of the list. New entries are added by replacing
// the last entry in the list with a new IP/MAC.
// ARP cache format: | prev | next | Entry0 | Entry1 | .... | EntryN |
// ARP entry format: | prev | next | IP (4bytes) | MAC (6bytes) |
// prev and next are 1-byte offsets in the cache, so cache size is max 256 bytes
// ARP entry size is 12 bytes
static void arp_cache_init(uint8_t *p, int n, int size) {
  for (int i = 0; i < n; i++) p[2 + i * size] = (uint8_t) (2 + (i - 1) * size);
  for (int i = 0; i < n; i++) p[3 + i * size] = (uint8_t) (2 + (i + 1) * size);
  p[0] = p[2] = (uint8_t) (2 + (n - 1) * size);
  p[1] = p[3 + (n - 1) * size] = 2;
}

#if 0
static inline void arp_cache_dump(const uint8_t *p) {
  MG_INFO(("ARP cache:"));
  for (uint8_t i = 0, j = p[1]; i < MIP_ARP_ENTRIES; i++, j = p[j + 1]) {
    MG_INFO(("  %d.%d.%d.%d -> %x:%x:%x:%x:%x:%x", p[j + 2], p[j + 3], p[j + 4],
             p[j + 5], p[j + 6], p[j + 7], p[j + 8], p[j + 9], p[j + 10],
             p[j + 11]));
  }
}
#endif

static const uint8_t bcastmac[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static uint8_t *arp_cache_find(struct mip_if *ifp, uint32_t ip) {
  uint8_t *p = ifp->arp_cache;
  if (ip == 0) return NULL;
  // use broadcast MAC for local and global broadcast IP
  if (ip == 0xffffffffU || ip == (ifp->ip | ~ifp->mask))
    return (uint8_t *) bcastmac;  
  for (uint8_t i = 0, j = p[1]; i < MIP_ARP_ENTRIES; i++, j = p[j + 1]) {
    if (memcmp(p + j + 2, &ip, sizeof(ip)) == 0) {
      p[1] = j, p[0] = p[j];  // Found entry! Point list head to us
      // MG_DEBUG(("ARP find: %#lx @ %x:%x:%x:%x:%x:%x", (long) ip, p[j + 6],
      //          p[j + 7], p[j + 8], p[j + 9], p[j + 10], p[j + 11]));
      return p + j + 6;  // And return MAC address
    }
  }
  return NULL;
}

static void arp_cache_add(struct mip_if *ifp, uint32_t ip, uint8_t mac[6]) {
  uint8_t *p = ifp->arp_cache;
  if (ip == 0 || ip == ~0U) return;             // Bad IP
  if (arp_cache_find(ifp, ip) != NULL) return;  // Already exists, do nothing
  memcpy(p + p[0] + 2, &ip, sizeof(ip));  // Replace last entry: IP address
  memcpy(p + p[0] + 6, mac, 6);           // And MAC address
  p[1] = p[0], p[0] = p[p[1]];            // Point list head to us
  MG_DEBUG(("ARP cache: added %#lx @ %x:%x:%x:%x:%x:%x", (long) mg_htonl(ip),
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]));
}

static size_t ether_output(struct mip_if *ifp, size_t len) {
  // size_t min = 64;  // Pad short frames to 64 bytes (minimum Ethernet size)
  // if (len < min) memset(ifp->tx.ptr + len, 0, min - len), len = min;
  // mg_hexdump(ifp->tx.ptr, len);
  return ifp->driver->tx(ifp->tx.ptr, len, ifp->driver_data);
}

static void arp_ask(struct mip_if *ifp, uint32_t ip) {
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

static size_t mg_print_ipv4(mg_pfn_t fn, void *fn_data, va_list *ap) {
  uint32_t ip = mg_ntohl(va_arg(*ap, uint32_t));
  return mg_xprintf(fn, fn_data, "%d.%d.%d.%d", ip >> 24, (ip >> 16) & 255,
                    (ip >> 8) & 255, ip & 255);
}

static void onstatechange(struct mip_if *ifp) {
  if (ifp->state == MIP_STATE_READY) {
    MG_INFO(("READY, IP: %M", mg_print_ipv4, ifp->ip));
    MG_INFO(("       GW: %M", mg_print_ipv4, ifp->gw));
    MG_INFO(("       Lease: %lld sec", (ifp->lease_expire - ifp->now) / 1000));
    arp_ask(ifp, ifp->gw);
  } else if (ifp->state == MIP_STATE_UP) {
    MG_ERROR(("Link up"));
  } else if (ifp->state == MIP_STATE_DOWN) {
    MG_ERROR(("Link down"));
  }
}

static struct ip *tx_ip(struct mip_if *ifp, uint8_t proto, uint32_t ip_src,
                        uint32_t ip_dst, size_t plen) {
  struct eth *eth = (struct eth *) ifp->tx.ptr;
  struct ip *ip = (struct ip *) (eth + 1);
  uint8_t *mac = arp_cache_find(ifp, ip_dst);  // Dst IP in ARP cache ?
  if (!mac && ((ip_dst & ifp->mask) == (ifp->ip & ifp->mask)))
    arp_ask(ifp, ip_dst);                             // Same net, lookup
  if (!mac) mac = arp_cache_find(ifp, ifp->gw);       // Use gateway MAC
  if (mac) memcpy(eth->dst, mac, sizeof(eth->dst));   // Found? Use it
  if (!mac) memset(eth->dst, 255, sizeof(eth->dst));  // No? Use broadcast
  memcpy(eth->src, ifp->mac, sizeof(eth->src));       // TODO(cpq): ARP lookup
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

static void tx_udp(struct mip_if *ifp, uint32_t ip_src, uint16_t sport,
                   uint32_t ip_dst, uint16_t dport, const void *buf,
                   size_t len) {
  struct ip *ip = tx_ip(ifp, 17, ip_src, ip_dst, len + sizeof(struct udp));
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

static void tx_dhcp(struct mip_if *ifp, uint32_t src, uint32_t dst,
                    uint8_t *opts, size_t optslen) {
  struct dhcp dhcp = {1, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, {0}, 0, {0}};
  dhcp.magic = mg_htonl(0x63825363);
  memcpy(&dhcp.hwaddr, ifp->mac, sizeof(ifp->mac));
  memcpy(&dhcp.xid, ifp->mac + 2, sizeof(dhcp.xid));
  memcpy(&dhcp.options, opts, optslen);
  tx_udp(ifp, src, mg_htons(68), dst, mg_htons(67), &dhcp, sizeof(dhcp));
}

static void tx_dhcp_request(struct mip_if *ifp, uint32_t src, uint32_t dst) {
  uint8_t opts[] = {
      53, 1, 3,                 // Type: DHCP request
      55, 2, 1,   3,            // GW and mask
      12, 3, 'm', 'i', 'p',     // Host name: "mip"
      54, 4, 0,   0,   0,   0,  // DHCP server ID
      50, 4, 0,   0,   0,   0,  // Requested IP
      255                       // End of options
  };
  memcpy(opts + 14, &dst, sizeof(dst));
  memcpy(opts + 20, &src, sizeof(src));
  tx_dhcp(ifp, src, dst, opts, sizeof(opts));
}

static void tx_dhcp_discover(struct mip_if *ifp) {
  uint8_t opts[] = {
      53, 1, 1,     // Type: DHCP discover
      55, 2, 1, 3,  // Parameters: ip, mask
      255           // End of options
  };
  tx_dhcp(ifp, 0, 0xffffffff, opts, sizeof(opts));
  MG_DEBUG(("DHCP discover sent"));
}

static void rx_arp(struct mip_if *ifp, struct pkt *pkt) {
  if (pkt->arp->op == mg_htons(1) && pkt->arp->tpa == ifp->ip) {
    // ARP request. Make a response, then send
    struct eth *eth = (struct eth *) ifp->tx.ptr;
    struct arp *arp = (struct arp *) (eth + 1);
    MG_DEBUG(("ARP op %d %#x %#x", mg_htons(arp->op), arp->spa, arp->tpa));
    memcpy(eth->dst, pkt->eth->src, sizeof(eth->dst));
    memcpy(eth->src, ifp->mac, sizeof(eth->src));
    eth->type = mg_htons(0x806);
    *arp = *pkt->arp;
    arp->op = mg_htons(2);
    memcpy(arp->tha, pkt->arp->sha, sizeof(pkt->arp->tha));
    memcpy(arp->sha, ifp->mac, sizeof(pkt->arp->sha));
    arp->tpa = pkt->arp->spa;
    arp->spa = ifp->ip;
    MG_DEBUG(("ARP response: we're %#lx", (long) mg_ntohl(ifp->ip)));
    ether_output(ifp, PDIFF(eth, arp + 1));
  } else if (pkt->arp->op == mg_htons(2)) {
    if (memcmp(pkt->arp->tha, ifp->mac, sizeof(pkt->arp->tha)) != 0) return;
    // MG_INFO(("ARP RESPONSE"));
    arp_cache_add(ifp, pkt->arp->spa, pkt->arp->sha);
  }
}

static void rx_icmp(struct mip_if *ifp, struct pkt *pkt) {
  // MG_DEBUG(("ICMP %d", (int) len));
  if (pkt->icmp->type == 8 && pkt->ip != NULL && pkt->ip->dst == ifp->ip) {
    size_t hlen = sizeof(struct eth) + sizeof(struct ip) + sizeof(struct icmp);
    size_t space = ifp->tx.len - hlen, plen = pkt->pay.len;
    if (plen > space) plen = space;
    struct ip *ip =
        tx_ip(ifp, 1, ifp->ip, pkt->ip->src, sizeof(struct icmp) + plen);
    struct icmp *icmp = (struct icmp *) (ip + 1);
    memset(icmp, 0, sizeof(*icmp));        // Set csum to 0
    memcpy(icmp + 1, pkt->pay.ptr, plen);  // Copy RX payload to TX
    icmp->csum = ipcsum(icmp, sizeof(*icmp) + plen);
    ether_output(ifp, hlen + plen);
  }
}

static void rx_dhcp(struct mip_if *ifp, struct pkt *pkt) {
  uint32_t ip = 0, gw = 0, mask = 0;
  uint8_t *p = pkt->dhcp->options,
          *end = (uint8_t *) &pkt->raw.ptr[pkt->raw.len];
  if (end < (uint8_t *) (pkt->dhcp + 1)) return;
  while (p + 1 < end && p[0] != 255) {  // Parse options
    if (p[0] == 1 && p[1] == sizeof(ifp->mask) && p + 6 < end) {  // Mask
      memcpy(&mask, p + 2, sizeof(mask));
    } else if (p[0] == 3 && p[1] == sizeof(ifp->gw) && p + 6 < end) {  // GW
      memcpy(&gw, p + 2, sizeof(gw));
      ip = pkt->dhcp->yiaddr;
    } else if (p[0] == 51 && p[1] == 4 && p + 6 < end) {  // Lease
      uint32_t lease = 0;
      memcpy(&lease, p + 2, sizeof(lease));
      ifp->lease_expire = ifp->now + mg_ntohl(lease) * 1000;
    }
    p += p[1] + 2;
  }
  if (ip && mask && gw && ifp->ip == 0) {
    arp_cache_add(ifp, pkt->dhcp->siaddr, ((struct eth *) pkt->raw.ptr)->src);
    ifp->ip = ip, ifp->gw = gw, ifp->mask = mask;
    ifp->state = MIP_STATE_READY;
    onstatechange(ifp);
    tx_dhcp_request(ifp, ip, pkt->dhcp->siaddr);
  }
}

static struct mg_connection *getpeer(struct mg_mgr *mgr, struct pkt *pkt,
                                     bool lsn) {
  struct mg_connection *c = NULL;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->is_udp && pkt->udp && c->loc.port == pkt->udp->dport) break;
    if (!c->is_udp && pkt->tcp && c->loc.port == pkt->tcp->dport &&
        lsn == c->is_listening && (lsn || c->rem.port == pkt->tcp->sport))
      break;
  }
  return c;
}

static void rx_udp(struct mip_if *ifp, struct pkt *pkt) {
  struct mg_connection *c = getpeer(ifp->mgr, pkt, true);
  if (c == NULL) {
    // No UDP listener on this port. Should send ICMP, but keep silent.
  } else if (c != NULL) {
    c->rem.port = pkt->udp->sport;
    c->rem.ip = pkt->ip->src;
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

static size_t tx_tcp(struct mip_if *ifp, uint32_t dst_ip, uint8_t flags,
                     uint16_t sport, uint16_t dport, uint32_t seq, uint32_t ack,
                     const void *buf, size_t len) {
  struct ip *ip = tx_ip(ifp, 6, ifp->ip, dst_ip, sizeof(struct tcp) + len);
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
  return ether_output(ifp, PDIFF(ifp->tx.ptr, tcp + 1) + len);
}

static size_t tx_tcp_pkt(struct mip_if *ifp, struct pkt *pkt, uint8_t flags,
                         uint32_t seq, const void *buf, size_t len) {
  uint32_t delta = (pkt->tcp->flags & (TH_SYN | TH_FIN)) ? 1 : 0;
  return tx_tcp(ifp, pkt->ip->src, flags, pkt->tcp->dport, pkt->tcp->sport, seq,
                mg_htonl(mg_ntohl(pkt->tcp->seq) + delta), buf, len);
}

static struct mg_connection *accept_conn(struct mg_connection *lsn,
                                         struct pkt *pkt) {
  struct mg_connection *c = mg_alloc_conn(lsn->mgr);
  struct connstate *s = (struct connstate *) (c + 1);
  s->seq = mg_ntohl(pkt->tcp->ack), s->ack = mg_ntohl(pkt->tcp->seq);
  s->timer = ((struct mip_if *) c->mgr->priv)->now + MIP_TCP_KEEPALIVE_MS;
  c->rem.ip = pkt->ip->src;
  c->rem.port = pkt->tcp->sport;
  MG_DEBUG(("%lu accepted %lx:%hx", c->id, mg_ntohl(c->rem.ip), c->rem.port));
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

static void settmout(struct mg_connection *c, uint8_t type) {
  struct mip_if *ifp = (struct mip_if *) c->mgr->priv;
  struct connstate *s = (struct connstate *) (c + 1);
  unsigned n = type == MIP_TTYPE_ACK ? MIP_TCP_ACK_MS : MIP_TCP_KEEPALIVE_MS;
  s->timer = ifp->now + n;
  s->ttype = type;
  MG_VERBOSE(("%lu %d -> %llx", c->id, type, s->timer));
}

long mg_io_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mip_if *ifp = (struct mip_if *) c->mgr->priv;
  struct connstate *s = (struct connstate *) (c + 1);
  size_t max_headers_len = 14 + 24 /* max IP */ + 60 /* max TCP */;
  if (len + max_headers_len > ifp->tx.len) len = ifp->tx.len - max_headers_len;
  if (tx_tcp(ifp, c->rem.ip, TH_PUSH | TH_ACK, c->loc.port, c->rem.port,
             mg_htonl(s->seq), mg_htonl(s->ack), buf, len) > 0) {
    s->seq += (uint32_t) len;
    if (s->ttype == MIP_TTYPE_KEEPALIVE) settmout(c, MIP_TTYPE_KEEPALIVE);
  } else {
    return MG_IO_ERR;
  }
  return (long) len;
}

long mg_io_recv(struct mg_connection *c, void *buf, size_t len) {
  struct connstate *s = (struct connstate *) (c + 1);
  if (s->raw.len == 0) return MG_IO_WAIT;
  if (len > s->raw.len) len = s->raw.len;
  memcpy(buf, s->raw.buf, len);
  mg_iobuf_del(&s->raw, 0, len);
  MG_DEBUG(("%lu", len));
  return (long) len;
}

static void read_conn(struct mg_connection *c, struct pkt *pkt) {
  struct connstate *s = (struct connstate *) (c + 1);
  struct mg_iobuf *io = c->is_tls ? &s->raw : &c->recv;
  uint32_t seq = mg_ntohl(pkt->tcp->seq);
  s->raw.align = c->recv.align;
  if (pkt->tcp->flags & TH_FIN) {
    s->ack = mg_htonl(pkt->tcp->seq) + 1, s->seq = mg_htonl(pkt->tcp->ack);
    c->is_closing = 1;
  } else if (pkt->pay.len == 0) {
    // TODO(cpq): handle this peer's ACK
  } else if (seq != s->ack) {
    // TODO(cpq): peer sent us SEQ which we don't expect. Retransmit rather
    // than close this connection
    mg_error(c, "SEQ != ACK: %x %x", seq, s->ack);
  } else if (io->size - io->len < pkt->pay.len &&
             !mg_iobuf_resize(io, io->len + pkt->pay.len)) {
    mg_error(c, "oom");
  } else {
    // Copy TCP payload into the IO buffer. If the connection is plain text, we
    // copy to c->recv. If the connection is TLS, this data is encrypted,
    // therefore we copy that encrypted data to the s->raw iobuffer instead,
    // and then call mg_tls_recv() to decrypt it. NOTE: mg_tls_recv() will
    // call back mg_io_recv() which grabs raw data from s->raw
    memcpy(&io->buf[io->len], pkt->pay.ptr, pkt->pay.len);
    io->len += pkt->pay.len;

    MG_DEBUG(("%lu SEQ %x -> %x", c->id, mg_htonl(pkt->tcp->seq), s->ack));
    s->ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len);
#if 0
    // Send ACK immediately
    MG_DEBUG(("  imm ACK", c->id, mg_htonl(pkt->tcp->seq), s->ack));
    tx_tcp((struct mip_if *) c->mgr->priv, c->rem.ip, TH_ACK, c->loc.port,
           c->rem.port, mg_htonl(s->seq), mg_htonl(s->ack), "", 0);
#else
    // Advance ACK counter and setup a timer to send an ACK back
    settmout(c, MIP_TTYPE_ACK);
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
      // Plain text connection, data is already in c->recv, trigger MG_EV_READ
      mg_call(c, MG_EV_READ, &pkt->pay.len);
    }
  }
}

static void rx_tcp(struct mip_if *ifp, struct pkt *pkt) {
  struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
  struct connstate *s = c == NULL ? NULL : (struct connstate *) (c + 1);

  if (c != NULL && s->ttype == MIP_TTYPE_KEEPALIVE) {
    s->tmiss = 0;                      // Reset missed keep-alive counter
    settmout(c, MIP_TTYPE_KEEPALIVE);  // Advance keep-alive timer
  }
#if 0
  MG_INFO(("%lu %hhu %d", c ? c->id : 0, pkt->tcp->flags, (int) pkt->pay.len));
#endif
  if (c != NULL && c->is_connecting && pkt->tcp->flags & (TH_SYN | TH_ACK)) {
    s->seq = mg_ntohl(pkt->tcp->ack), s->ack = mg_ntohl(pkt->tcp->seq) + 1;
    tx_tcp_pkt(ifp, pkt, TH_ACK, pkt->tcp->ack, NULL, 0);
    c->is_connecting = 0;             // Client connected
    mg_call(c, MG_EV_CONNECT, NULL);  // Let user know
  } else if (c != NULL && c->is_connecting) {
    tx_tcp_pkt(ifp, pkt, TH_RST | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else if (c != NULL) {
#if 0
    MG_DEBUG(("%lu %d %lx:%hu -> %lx:%hu", c->id, (int) pkt->raw.len,
              mg_ntohl(pkt->ip->src), mg_ntohs(pkt->tcp->sport),
              mg_ntohl(pkt->ip->dst), mg_ntohs(pkt->tcp->dport)));
    mg_hexdump(pkt->pay.buf, pkt->pay.len);
#endif
    read_conn(c, pkt);
  } else if ((c = getpeer(ifp->mgr, pkt, true)) == NULL) {
    tx_tcp_pkt(ifp, pkt, TH_RST | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else if (pkt->tcp->flags & TH_SYN) {
    // Use peer's source port as ISN, in order to recognise the handshake
    uint32_t isn = mg_htonl((uint32_t) mg_ntohs(pkt->tcp->sport));
    tx_tcp_pkt(ifp, pkt, TH_SYN | TH_ACK, isn, NULL, 0);
  } else if (pkt->tcp->flags & TH_FIN) {
    tx_tcp_pkt(ifp, pkt, TH_FIN | TH_ACK, pkt->tcp->ack, NULL, 0);
  } else if (mg_htonl(pkt->tcp->ack) == mg_htons(pkt->tcp->sport) + 1U) {
    accept_conn(c, pkt);
  } else {
    // MG_DEBUG(("dropped silently.."));
  }
}

static void rx_ip(struct mip_if *ifp, struct pkt *pkt) {
  //  MG_DEBUG(("IP %d", (int) pkt->pay.len));
  if (pkt->ip->proto == 1) {
    pkt->icmp = (struct icmp *) (pkt->ip + 1);
    if (pkt->pay.len < sizeof(*pkt->icmp)) return;
    mkpay(pkt, pkt->icmp + 1);
    rx_icmp(ifp, pkt);
  } else if (pkt->ip->proto == 17) {
    pkt->udp = (struct udp *) (pkt->ip + 1);
    if (pkt->pay.len < sizeof(*pkt->udp)) return;
    // MG_DEBUG(("  UDP %u %u -> %u", len, mg_htons(udp->sport),
    // mg_htons(udp->dport)));
    mkpay(pkt, pkt->udp + 1);
    if (pkt->udp->dport == mg_htons(68)) {
      pkt->dhcp = (struct dhcp *) (pkt->udp + 1);
      mkpay(pkt, pkt->dhcp + 1);
      rx_dhcp(ifp, pkt);
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
    rx_tcp(ifp, pkt);
  }
}

static void rx_ip6(struct mip_if *ifp, struct pkt *pkt) {
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

static void mip_rx(struct mip_if *ifp, void *buf, size_t len) {
  const uint8_t broadcast[] = {255, 255, 255, 255, 255, 255};
  // struct pkt pkt = {.raw = {.buf = (uint8_t *) buf, .len = len}};
  struct pkt pkt;
  memset(&pkt, 0, sizeof(pkt));
  pkt.raw.ptr = (char *) buf;
  pkt.raw.len = len;
  pkt.eth = (struct eth *) buf;
  if (pkt.raw.len < sizeof(*pkt.eth)) return;  // Truncated - runt?
  if (memcmp(pkt.eth->dst, ifp->mac, sizeof(pkt.eth->dst)) != 0 &&
      memcmp(pkt.eth->dst, broadcast, sizeof(pkt.eth->dst)) != 0) {
    // Not for us. Drop silently
  } else if (pkt.eth->type == mg_htons(0x806)) {
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
    MG_DEBUG(("  Unknown eth type %x", mg_htons(pkt.eth->type)));
  }
}

static void mip_poll(struct mip_if *ifp, uint64_t uptime_ms) {
  if (ifp == NULL || ifp->driver == NULL) return;
  bool expired_1000ms = mg_timer_expired(&ifp->timer_1000ms, 1000, uptime_ms);
  ifp->now = uptime_ms;

  // Handle physical interface up/down status
  if (expired_1000ms && ifp->driver->up) {
    bool up = ifp->driver->up(ifp->driver_data);
    bool current = ifp->state != MIP_STATE_DOWN;
    if (up != current) {
      ifp->state = up == false     ? MIP_STATE_DOWN
                   : ifp->use_dhcp ? MIP_STATE_UP
                                   : MIP_STATE_READY;
      if (!up && ifp->use_dhcp) ifp->ip = 0;
      onstatechange(ifp);
    }
  }
  if (ifp->state == MIP_STATE_DOWN) return;
  // if (expired_1000ms) arp_cache_dump(ifp->arp_cache);

  if (ifp->ip == 0 && expired_1000ms) {
    tx_dhcp_discover(ifp);  // If IP not configured, send DHCP
  } else if (ifp->use_dhcp == false && expired_1000ms &&
             arp_cache_find(ifp, ifp->gw) == NULL) {
    arp_ask(ifp, ifp->gw);  // If GW's MAC address in not in ARP cache
  }

  // Read data from the network
  size_t len = ifp->queue.len > 0
                   ? q_read(&ifp->queue, (void *) ifp->rx.ptr)
                   : ifp->driver->rx((void *) ifp->rx.ptr, ifp->rx.len,
                                     ifp->driver_data);
  qp_mark(QP_FRAMEPOPPED, (int) q_space(&ifp->queue));
  mip_rx(ifp, (void *) ifp->rx.ptr, len);
  qp_mark(QP_FRAMEDONE, (int) q_space(&ifp->queue));

  // Process timeouts
  for (struct mg_connection *c = ifp->mgr->conns; c != NULL; c = c->next) {
    if (c->is_udp || c->is_listening) continue;
    if (c->is_connecting || c->is_resolving) continue;
    struct connstate *s = (struct connstate *) (c + 1);
    if (uptime_ms > s->timer) {
      if (s->ttype == MIP_TTYPE_ACK) {
        MG_DEBUG(("%lu ack %x %x", c->id, s->seq, s->ack));
        tx_tcp(ifp, c->rem.ip, TH_ACK, c->loc.port, c->rem.port,
               mg_htonl(s->seq), mg_htonl(s->ack), "", 0);
      } else {
        MG_DEBUG(("%lu keepalive", c->id));
        tx_tcp(ifp, c->rem.ip, TH_ACK, c->loc.port, c->rem.port,
               mg_htonl(s->seq - 1), mg_htonl(s->ack), "", 0);
        if (s->tmiss++ > 2) mg_error(c, "keepalive");
      }
      settmout(c, MIP_TTYPE_KEEPALIVE);
    }
  }
#ifdef MIP_QPROFILE
  qp_log();
#endif
}

// This function executes in interrupt context, thus it should copy data
// somewhere fast. Note that newlib's malloc is not thread safe, thus use
// our lock-free queue with preallocated buffer to copy data and return asap
static void on_rx(void *buf, size_t len, void *userdata) {
  struct mip_if *ifp = (struct mip_if *) userdata;
  if (q_write(&ifp->queue, buf, len)) {
    qp_mark(QP_FRAMEPUSHED, (int) q_space(&ifp->queue));
  } else {
    ifp->dropped++;
    qp_mark(QP_FRAMEDROPPED, ifp->dropped);
    MG_ERROR(("dropped %d", (int) len));
  }
}

void mip_init(struct mg_mgr *mgr, struct mip_if *ifp) {
  if (ifp->driver->init && !ifp->driver->init(ifp->mac, ifp->driver_data)) {
    MG_ERROR(("driver init failed"));
  } else {
    size_t maxpktsize = 1540;
    ifp->rx.ptr = (char *) calloc(1, maxpktsize), ifp->rx.len = maxpktsize;
    ifp->tx.ptr = (char *) calloc(1, maxpktsize), ifp->tx.len = maxpktsize;
    if (ifp->driver->setrx) {
      ifp->queue.len = MIP_QSIZE;
      ifp->queue.buf = (uint8_t *) calloc(1, ifp->queue.len);
      ifp->driver->setrx(on_rx, ifp);
    }
    ifp->timer_1000ms = mg_millis();
    arp_cache_init(ifp->arp_cache, MIP_ARP_ENTRIES, 12);
    mgr->priv = ifp;
    ifp->mgr = mgr;
    mgr->extraconnsize = sizeof(struct connstate);
#ifdef MIP_QPROFILE
    qp_init();
#endif
  }
}

void mip_free(struct mip_if *ifp) {
  free((char *) ifp->rx.ptr);
  free((char *) ifp->tx.ptr);
}

int mg_mkpipe(struct mg_mgr *m, mg_event_handler_t fn, void *d, bool udp) {
  (void) m, (void) fn, (void) d, (void) udp;
  MG_ERROR(("Not implemented"));
  return -1;
}

#if 0
static uint16_t mkeport(void) {
  uint16_t a = 0, b = mg_millis() & 0xffffU, c = MIP_ETHEMERAL_PORT;
  mg_random(&a, sizeof(a));
  c += (a ^ b) % (0xffffU - MIP_ETHEMERAL_PORT);
  return c;
}
#endif

void mg_connect_resolved(struct mg_connection *c) {
  struct mip_if *ifp = (struct mip_if *) c->mgr->priv;
  c->is_resolving = 0;
  if (ifp->eport < MIP_ETHEMERAL_PORT) ifp->eport = MIP_ETHEMERAL_PORT;
  c->loc.ip = ifp->ip;
  c->loc.port = mg_htons(ifp->eport++);
  MG_DEBUG(("%lu %08lx:%hu->%08lx:%hu", c->id, mg_ntohl(c->loc.ip),
            mg_ntohs(c->loc.port), mg_ntohl(c->rem.ip), mg_ntohs(c->rem.port)));
  mg_call(c, MG_EV_RESOLVE, NULL);
  if (c->is_udp) {
    mg_call(c, MG_EV_CONNECT, NULL);
  } else {
    uint32_t isn = mg_htonl((uint32_t) mg_ntohs(c->loc.port));
    tx_tcp(ifp, c->rem.ip, TH_SYN, c->loc.port, c->rem.port, isn, 0, NULL, 0);
    c->is_connecting = 1;
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

static void close_conn(struct mg_connection *c) {
  struct connstate *s = (struct connstate *) (c + 1);
  mg_iobuf_free(&s->raw);  // For TLS connections, release raw data
  if (c->is_udp == false && c->is_listening == false) {   // For TCP conns,
    struct mip_if *ifp = (struct mip_if *) c->mgr->priv;  // send TCP FIN
    tx_tcp(ifp, c->rem.ip, TH_FIN | TH_ACK, c->loc.port, c->rem.port,
           mg_htonl(s->seq), mg_htonl(s->ack), NULL, 0);
  }
  mg_close_conn(c);
}

static bool can_write(struct mg_connection *c) {
  return c->is_connecting == 0 && c->is_resolving == 0 && c->send.len > 0 &&
         c->is_tls_hs == 0;
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  struct mg_connection *c, *tmp;
  uint64_t now = mg_millis();
  mip_poll((struct mip_if *) mgr->priv, now);
  mg_timer_poll(&mgr->timers, now);
  for (c = mgr->conns; c != NULL; c = tmp) {
    tmp = c->next;
    mg_call(c, MG_EV_POLL, &now);
    MG_VERBOSE(("%lu .. %c%c%c%c%c", c->id, c->is_tls ? 'T' : 't',
                c->is_connecting ? 'C' : 'c', c->is_tls_hs ? 'H' : 'h',
                c->is_resolving ? 'R' : 'r', c->is_closing ? 'C' : 'c'));
    if (c->is_tls_hs) mg_tls_handshake(c);
    if (can_write(c)) write_conn(c);
    if (c->is_draining && c->send.len == 0) c->is_closing = 1;
    if (c->is_closing) close_conn(c);
  }
  (void) ms;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mip_if *ifp = (struct mip_if *) c->mgr->priv;
  bool res = false;
  if (ifp->ip == 0 || ifp->state != MIP_STATE_READY) {
    mg_error(c, "net down");
  } else if (c->is_udp) {
    tx_udp(ifp, ifp->ip, c->loc.port, c->rem.ip, c->rem.port, buf, len);
    res = true;
  } else {
    res = mg_iobuf_add(&c->send, c->send.len, buf, len);
  }
  return res;
}

#ifdef MIP_QPROFILE

#pragma pack(push, 1)
struct qpentry {
  uint32_t timestamp;
  uint16_t type;
  uint16_t len;
};
#pragma pack(pop)

static struct queue qp;

// This is called from IRQ and main contexts; two producers, single consumer
// TODO(scaprile): avoid concurrency issues (2 queues ?)
void qp_mark(unsigned int type, int len) {
  static bool ovf = false;
  static uint16_t irq_ctr = 0, drop_ctr = 0;
  struct qpentry e = {.timestamp = (uint32_t) mg_millis(),
                      .type = (uint16_t) type,
                      .len = (uint16_t) len};
  if (type == QP_IRQTRIGGERED) e.len = ++irq_ctr;
  if (ovf) {
    e.type = (uint16_t) QP_QUEUEOVF;
    e.len = drop_ctr;
  }
  ovf = !q_write(&qp, &e, sizeof(e));
}

void qp_log(void) {
  struct qpentry e;
  const char *titles[] = {"IRQ ", "PUSH", "POP ", "DONE", "DROP", "OVFL"};
  for (int i = 0; i < 10 && q_read(&qp, &e); i++) {
    MG_INFO(("%lx %s %u", e.timestamp, titles[e.type], e.len));
  }
}

void qp_init(void) {
  qp.len = 500 * (sizeof(size_t) + sizeof(struct qpentry));
  qp.buf = calloc(1, qp.len);  // THERE IS NO FREE
}
#endif  // MIP_QPROFILE

#endif  // MG_ENABLE_MIP
