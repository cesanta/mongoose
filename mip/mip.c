#include "mip.h"

#if MG_ENABLE_MIP

#if defined(_MSC_VER) || defined(ARDUINO)
#define _Atomic
#else
#include <stdatomic.h>
#endif

#define MIP_ETHEMERAL_PORT 49152
#define U16(ptr) ((((uint16_t) (ptr)[0]) << 8) | (ptr)[1])
#define PDIFF(a, b) ((size_t) (((char *) (b)) - ((char *) (a))))

#ifndef MIP_ARP_ENTRIES
#define MIP_ARP_ENTRIES 5  // Number of ARP cache entries. Maximum 21
#endif
#define MIP_ARP_CS (2 + 12 * MIP_ARP_ENTRIES)  // ARP cache size

struct connstate {
  uint32_t seq, ack;  // TCP seq/ack counters
  uint64_t timer;     // Used to send ACKs
  uint8_t mac[6];     // Peer MAC address
};

struct str {
  uint8_t *buf;
  size_t len;
};

// Receive queue - single producer, single consumer queue.  Interrupt-based
// drivers copy received frames to the queue in interrupt context. mip_poll()
// function runs in event loop context, reads from the queue
struct queue {
  uint8_t *buf;
  size_t len;
  volatile _Atomic size_t tail, head;
};

// Network interface
struct mip_if {
  uint8_t mac[6];             // MAC address. Must be set to a valid MAC
  uint32_t ip, mask, gw;      // IP address, mask, default gateway. Can be 0
  struct str rx;              // Output (TX) buffer
  struct str tx;              // Input (RX) buffer
  bool use_dhcp;              // Enable DCHP
  struct mip_driver *driver;  // Low level driver
  void *driver_data;          // Driver-specific data
  struct mg_mgr *mgr;         // Mongoose event manager

  // Internal state, user can use it but should not change it
  uint64_t timer_1000ms;          // 1000 ms timer: for DHCP and link state
  uint8_t arp_cache[MIP_ARP_CS];  // Each entry is 12 bytes
  uint16_t eport;                 // Next ephemeral port
  int state;                      // Current state
#define MIP_STATE_DOWN 0          // Interface is down
#define MIP_STATE_UP 1            // Interface is up
#define MIP_STATE_READY 2         // Interface is up and has IP
  struct queue queue;             // Receive queue
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
  struct str raw;  // Raw packet data
  struct str pay;  // Payload data
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
  size_t left = (q->len - q->head + q->tail -1) % q->len;
  if (len + sizeof(size_t) <= left) {
    q_copyin(q, (uint8_t *) &len, sizeof(len), q->head);
    q_copyin(q, (uint8_t *) buf, len, (q->head + sizeof(size_t)) % q->len);
    q->head = (q->head + sizeof(len) + len) % q->len;
    success = true;
  }
  return success;
}

static size_t q_avail(struct queue *q) {
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

static struct str mkstr(void *buf, size_t len) {
  struct str str = {(uint8_t *) buf, len};
  return str;
}

static void mkpay(struct pkt *pkt, void *p) {
  pkt->pay = mkstr(p, (size_t) (&pkt->raw.buf[pkt->raw.len] - (uint8_t *) p));
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

static inline void arp_cache_dump(const uint8_t *p) {
  MG_INFO(("ARP cache:"));
  for (uint8_t i = 0, j = p[1]; i < MIP_ARP_ENTRIES; i++, j = p[j + 1]) {
    MG_INFO(("  %d.%d.%d.%d -> %x:%x:%x:%x:%x:%x", p[j + 2], p[j + 3], p[j + 4],
             p[j + 5], p[j + 6], p[j + 7], p[j + 8], p[j + 9], p[j + 10],
             p[j + 11]));
  }
}

static uint8_t *arp_cache_find(struct mip_if *ifp, uint32_t ip) {
  uint8_t *p = ifp->arp_cache;
  if (ip == 0 || ip == 0xffffffffU) return NULL;
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

static void arp_ask(struct mip_if *ifp, uint32_t ip) {
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
  ifp->driver->tx(eth, PDIFF(eth, arp + 1), ifp->driver_data);
}

static void onstatechange(struct mip_if *ifp) {
  if (ifp->state == MIP_STATE_READY) {
    char buf[40];
    struct mg_addr addr = {.ip = ifp->ip};
    MG_INFO(("READY, IP: %s", mg_ntoa(&addr, buf, sizeof(buf))));
    arp_ask(ifp, ifp->gw);
  } else if (ifp->state == MIP_STATE_UP) {
    MG_ERROR(("Link up"));
  } else if (ifp->state == MIP_STATE_DOWN) {
    MG_ERROR(("Link down"));
  }
}

static struct ip *tx_ip(struct mip_if *ifp, uint8_t proto, uint32_t ip_src,
                        uint32_t ip_dst, size_t plen) {
  struct eth *eth = (struct eth *) ifp->tx.buf;
  struct ip *ip = (struct ip *) (eth + 1);
  uint8_t *mac = arp_cache_find(ifp, ip_dst);         // Dst IP in ARP cache ?
  if (!mac) mac = arp_cache_find(ifp, ifp->gw);       // No, use gateway
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
  ifp->driver->tx(ifp->tx.buf,
                  sizeof(struct eth) + sizeof(*ip) + sizeof(*udp) + len,
                  ifp->driver_data);
}

static void tx_dhcp(struct mip_if *ifp, uint32_t src, uint32_t dst,
                    uint8_t *opts, size_t optslen) {
  struct dhcp dhcp = {.op = 1,
                      .htype = 1,
                      .hlen = 6,
                      .ciaddr = src,
                      .magic = mg_htonl(0x63825363)};
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
  MG_DEBUG(("DHCP request sent"));
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
    struct eth *eth = (struct eth *) ifp->tx.buf;
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
    ifp->driver->tx(ifp->tx.buf, PDIFF(eth, arp + 1), ifp->driver_data);
  } else if (pkt->arp->op == mg_htons(2)) {
    if (memcmp(pkt->arp->tha, ifp->mac, sizeof(pkt->arp->tha)) != 0) return;
    // MG_INFO(("ARP RESPONSE"));
    arp_cache_add(ifp, pkt->arp->spa, pkt->arp->sha);
  }
}

static void rx_icmp(struct mip_if *ifp, struct pkt *pkt) {
  // MG_DEBUG(("ICMP %d", (int) len));
  if (pkt->icmp->type == 8 && pkt->ip->dst == ifp->ip) {
    struct ip *ip = tx_ip(ifp, 1, ifp->ip, pkt->ip->src,
                          sizeof(struct icmp) + pkt->pay.len);
    struct icmp *icmp = (struct icmp *) (ip + 1);
    memset(icmp, 0, sizeof(*icmp));  // Important - set csum to 0
    memcpy(icmp + 1, pkt->pay.buf, pkt->pay.len);
    icmp->csum = ipcsum(icmp, sizeof(*icmp) + pkt->pay.len);
    ifp->driver->tx(ifp->tx.buf, PDIFF(ifp->tx.buf, icmp + 1) + pkt->pay.len,
                    ifp->driver_data);
  }
}

static void rx_dhcp(struct mip_if *ifp, struct pkt *pkt) {
  uint32_t ip = 0, gw = 0, mask = 0;
  uint8_t *p = pkt->dhcp->options, *end = &pkt->raw.buf[pkt->raw.len];
  if (end < (uint8_t *) (pkt->dhcp + 1)) return;
  MG_DEBUG(("DHCP %u", (unsigned) pkt->raw.len));
  while (p < end && p[0] != 255) {
    if (p[0] == 1 && p[1] == sizeof(ifp->mask)) {
      memcpy(&mask, p + 2, sizeof(mask));
      // MG_DEBUG(("MASK %x", mask));
    } else if (p[0] == 3 && p[1] == sizeof(ifp->gw)) {
      memcpy(&gw, p + 2, sizeof(gw));
      ip = pkt->dhcp->yiaddr;
      // MG_DEBUG(("IP %x GW %x", ip, gw));
    }
    p += p[1] + 2;
  }
  if (ip && mask && gw && ifp->ip == 0) {
    // MG_DEBUG(("DHCP offer ip %#08lx mask %#08lx gw %#08lx", (long) ip,
    //           (long) mask, (long) gw));
    arp_cache_add(ifp, pkt->dhcp->siaddr, ((struct eth *) pkt->raw.buf)->src);
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
      memcpy(&c->recv.buf[c->recv.len], pkt->pay.buf, pkt->pay.len);
      c->recv.len += pkt->pay.len;
      struct mg_str evd = mg_str_n((char *) pkt->pay.buf, pkt->pay.len);
      mg_call(c, MG_EV_READ, &evd);
    }
  }
}

static size_t tx_tcp(struct mip_if *ifp, uint32_t dst_ip, uint8_t flags,
                     uint16_t sport, uint16_t dport, uint32_t seq, uint32_t ack,
                     const void *buf, size_t len) {
  struct ip *ip = tx_ip(ifp, 6, ifp->ip, dst_ip, sizeof(struct tcp) + len);
  struct tcp *tcp = (struct tcp *) (ip + 1);
  memset(tcp, 0, sizeof(*tcp));
  memmove(tcp + 1, buf, len);
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
  return ifp->driver->tx(ifp->tx.buf, PDIFF(ifp->tx.buf, tcp + 1) + len,
                         ifp->driver_data);
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
  c->rem.ip = pkt->ip->src;
  c->rem.port = pkt->tcp->sport;
  MG_DEBUG(("%lu accepted %lx:%hx", c->id, c->rem.ip, c->rem.port));
  LIST_ADD_HEAD(struct mg_connection, &lsn->mgr->conns, c);
  c->fd = (void *) (size_t) mg_ntohl(pkt->tcp->ack);
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

static void read_conn(struct mg_connection *c, struct pkt *pkt) {
  struct connstate *s = (struct connstate *) (c + 1);
  if (pkt->tcp->flags & TH_FIN) {
    s->ack = mg_htonl(pkt->tcp->seq) + 1, s->seq = mg_htonl(pkt->tcp->ack);
    c->is_closing = 1;
  } else if (pkt->pay.len == 0) {
  } else if (c->recv.size - c->recv.len < pkt->pay.len &&
             !mg_iobuf_resize(&c->recv, c->recv.len + pkt->pay.len)) {
    mg_error(c, "oom");
  } else if (mg_ntohl(pkt->tcp->seq) != s->ack) {
    mg_error(c, "oob: %x %x", mg_ntohl(pkt->tcp->seq), s->ack);
  } else {
    s->ack = (uint32_t) (mg_htonl(pkt->tcp->seq) + pkt->pay.len);
    memcpy(&c->recv.buf[c->recv.len], pkt->pay.buf, pkt->pay.len);
    c->recv.len += pkt->pay.len;
    struct mg_str evd = mg_str_n((char *) pkt->pay.buf, pkt->pay.len);
    mg_call(c, MG_EV_READ, &evd);
  }
}

static void rx_tcp(struct mip_if *ifp, struct pkt *pkt) {
  struct mg_connection *c = getpeer(ifp->mgr, pkt, false);
#if 0
  MG_INFO(("%lu %hhu %d", c ? c->id : 0, pkt->tcp->flags, (int) pkt->pay.len));
#endif
  if (c != NULL && c->is_connecting && pkt->tcp->flags & (TH_SYN | TH_ACK)) {
    struct connstate *s = (struct connstate *) (c + 1);
    s->seq = mg_ntohl(pkt->tcp->ack), s->ack = mg_ntohl(pkt->tcp->seq) + 1;
    tx_tcp_pkt(ifp, pkt, TH_ACK, pkt->tcp->ack, NULL, 0);
    c->is_connecting = 0;  // Client connected
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
  // MG_DEBUG(("IP %d", (int) pkt->pay.len));
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
  } else if (pkt->ip->proto == 17) {
    pkt->udp = (struct udp *) (pkt->ip6 + 1);
    if (pkt->pay.len < sizeof(*pkt->udp)) return;
    // MG_DEBUG(("  UDP %u %u -> %u", len, mg_htons(udp->sport),
    // mg_htons(udp->dport)));
    mkpay(pkt, pkt->udp + 1);
  }
}

static void mip_rx(struct mip_if *ifp, void *buf, size_t len) {
  const uint8_t broadcast[] = {255, 255, 255, 255, 255, 255};
  struct pkt pkt = {.raw = {.buf = (uint8_t *) buf, .len = len}};
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
  for (;;) {
    size_t len = ifp->queue.len > 0 ? q_read(&ifp->queue, ifp->rx.buf)
                                    : ifp->driver->rx(ifp->rx.buf, ifp->rx.len,
                                                      ifp->driver_data);
    if (len == 0) break;
    mip_rx(ifp, ifp->rx.buf, len);
  }
}

// This function executes in interrupt context, thus it should copy data
// somewhere fast. Note that newlib's malloc is not thread safe, thus use
// our lock-free queue with preallocated buffer to copy data and return asap
static void on_rx(void *buf, size_t len, void *userdata) {
  struct mip_if *ifp = (struct mip_if *) userdata;
  if (!q_write(&ifp->queue, buf, len)) MG_ERROR(("dropped %d", (int) len));
}

void mip_init(struct mg_mgr *mgr, struct mip_cfg *ipcfg,
              struct mip_driver *driver, void *driver_data) {
  if (driver->init && !driver->init(ipcfg->mac, driver_data)) {
    MG_ERROR(("driver init failed"));
  } else {
    size_t maxpktsize = 1518, qlen = driver->setrx ? 1024 * 16 : 0;
    struct mip_if *ifp =
        (struct mip_if *) calloc(1, sizeof(*ifp) + 2 * maxpktsize + qlen);
    memcpy(ifp->mac, ipcfg->mac, sizeof(ifp->mac));
    ifp->use_dhcp = ipcfg->ip == 0;
    ifp->ip = ipcfg->ip, ifp->mask = ipcfg->mask, ifp->gw = ipcfg->gw;
    ifp->rx.buf = (uint8_t *) (ifp + 1), ifp->rx.len = maxpktsize;
    ifp->tx.buf = ifp->rx.buf + maxpktsize, ifp->tx.len = maxpktsize;
    ifp->driver = driver;
    ifp->driver_data = driver_data;
    ifp->mgr = mgr;
    ifp->queue.buf = ifp->tx.buf + maxpktsize;
    ifp->queue.len = qlen;
    ifp->timer_1000ms = mg_millis();
    arp_cache_init(ifp->arp_cache, MIP_ARP_ENTRIES, 12);
    if (driver->setrx) driver->setrx(on_rx, ifp);
    mgr->priv = ifp;
    mgr->extraconnsize = sizeof(struct connstate);
  }
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
  MG_DEBUG(("%lu %08lx.%hu->%08lx.%hu", c->id, mg_ntohl(c->loc.ip),
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
  struct mip_if *ifp = (struct mip_if *) c->mgr->priv;
  struct connstate *s = (struct connstate *) (c + 1);
  size_t sent, n = c->send.len, hdrlen = 14 + 24 /*max IP*/ + 60 /*max TCP*/;
  if (n + hdrlen > ifp->tx.len) n = ifp->tx.len - hdrlen;
  sent = tx_tcp(ifp, c->rem.ip, TH_PUSH | TH_ACK, c->loc.port, c->rem.port,
                mg_htonl(s->seq), mg_htonl(s->ack), c->send.buf, n);
  if (sent > 0) {
    mg_iobuf_del(&c->send, 0, n);
    s->seq += (uint32_t) n;
    mg_call(c, MG_EV_WRITE, &n);
  }
}

static void fin_conn(struct mg_connection *c) {
  struct mip_if *ifp = (struct mip_if *) c->mgr->priv;
  struct connstate *s = (struct connstate *) (c + 1);
  MG_INFO(("AAA"));
  tx_tcp(ifp, c->rem.ip, TH_FIN | TH_ACK, c->loc.port, c->rem.port,
         mg_htonl(s->seq), mg_htonl(s->ack), NULL, 0);
}

static bool can_write(struct mg_connection *c) {
  return c->is_connecting == 0 && c->is_resolving == 0 && c->send.len > 0;
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  struct mg_connection *c, *tmp;
  uint64_t now = mg_millis();
  mip_poll((struct mip_if *) mgr->priv, now);
  mg_timer_poll(&mgr->timers, now);
  for (c = mgr->conns; c != NULL; c = tmp) {
    tmp = c->next;
    if (can_write(c)) write_conn(c);
    if (c->is_draining && c->send.len == 0) c->is_closing = 1;
    if (c->is_closing) {
      if (c->is_udp == false && c->is_listening == false) fin_conn(c);
      mg_close_conn(c);
    }
  }
  (void) ms;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  struct mip_if *ifp = (struct mip_if *) c->mgr->priv;
  bool res = false;
  if (ifp->ip == 0) {
    mg_error(c, "net down");
  } else if (c->is_udp) {
    tx_udp(ifp, ifp->ip, c->loc.port, c->rem.ip, c->rem.port, buf, len);
    res = true;
  } else {
    // tx_tdp(ifp, ifp->ip, c->loc.port, c->rem.ip, c->rem.port, buf, len);
    return mg_iobuf_add(&c->send, c->send.len, buf, len);
  }
  return res;
}
#endif  // MG_ENABLE_MIP
