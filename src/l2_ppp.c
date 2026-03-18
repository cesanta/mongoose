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
// all in network order

struct ppp {  // RFC-1661
  uint16_t proto;
};

struct lcp {  // RFC-1661
  uint8_t code, id;
  uint16_t len;
};

struct ipcp {  // RFC-1332
  uint8_t code, id;
  uint16_t len;
};

struct ipv6cp {  // RFC-5072
  uint8_t code, id;
  uint16_t len;
};

struct hdlc_ {  // RFC-1662, "PPP in HDLC-like Framing"
  uint8_t addr, ctrl;
};

struct pppoe {  // RFC-2516, "A Method for Transmitting PPP Over Ethernet
                // (PPPoE)"
  uint8_t vertype, code;
  uint16_t id, len;
};

#if defined(__DCC__)
#pragma pack(0)
#else
#pragma pack(pop)
#endif

#define MG_PPP_ADDR 0xff
#define MG_PPP_CTRL 0x03

#define MG_PPP_PROTO_IP 0x0021
#define MG_PPP_PROTO_IPV6 0x0057
#define MG_PPP_PROTO_LCP 0xc021
#define MG_PPP_PROTO_IPCP 0x8021
#define MG_PPP_PROTO_IPV6CP 0x8057
#define MG_PPP_PROTO_PAP 0xc023
#define MG_PPP_PROTO_CHAP 0xc223

#define MG_PPP_LCP_CFG_REQ 1
#define MG_PPP_LCP_CFG_ACK 2
#define MG_PPP_LCP_CFG_NACK 3
#define MG_PPP_LCP_CFG_REJECT 4
#define MG_PPP_LCP_CFG_TERM_REQ 5
#define MG_PPP_LCP_CFG_TERM_ACK 6
#define MG_PPP_LCP_REJECT 8
#define MG_PPP_LCP_ECHO_REQ 9
#define MG_PPP_LCP_ECHO_REPLY 10

#define MG_PPP_IPCP_CFG_REQ 1
#define MG_PPP_IPCP_CFG_ACK 2
#define MG_PPP_IPCP_CFG_NACK 3
#define MG_PPP_IPCP_CFG_REJECT 4
#define MG_PPP_IPCP_OPT_IPADDR 3

#define MG_PPP_IPV6CP_CFG_REQ 1
#define MG_PPP_IPV6CP_CFG_ACK 2
#define MG_PPP_IPV6CP_CFG_NACK 3
#define MG_PPP_IPV6CP_CFG_REJECT 4
#define MG_PPP_IPV6CP_OPT_IFCID 1

#define MG_PPPoE_PADI 0x09
#define MG_PPPoE_PADO 0x07
#define MG_PPPoE_PADR 0x19
#define MG_PPPoE_PADS 0x65
#define MG_PPPoE_PADT 0xa7

#define MG_PPPoE_ST_DISC 0  // Discovery phase, see what servers are out there
#define MG_PPPoE_ST_REQ 1   // Chose a server, request a session and wait
#define MG_PPPoE_ST_SESS 2  // Session established, PPP traffic is exchanged

#define PDIFF(a, b) ((size_t) (((char *) (b)) - ((char *) (a))))

static bool s_link = false;  // ************ THESE SHOULD MOVE TO A struct
                             // mg_l2data *******************************
static uint8_t s_state = MG_PPPoE_ST_DISC;
static uint16_t s_id;

void mg_l2_ppp_init(struct mg_tcpip_if *ifp) {
  ifp->mtu = 1500;
  ifp->framesize = 1500 + sizeof(struct ppp) + sizeof(struct hdlc_);
}

extern void mg_l2_eth_init(struct mg_tcpip_if *);

void mg_l2_pppoe_init(struct mg_tcpip_if *ifp) {
  mg_l2_eth_init(ifp);
  ifp->mtu = ifp->mtu - (uint16_t) (sizeof(struct pppoe) +
                                    sizeof(struct ppp));  // 1500 --> 1492
}

bool mg_l2_ppp_poll(struct mg_tcpip_if *ifp, bool expired_1000ms) {
  if (expired_1000ms && ifp->state == MG_TCPIP_STATE_DOWN) s_link = false;
  return s_link;
}

static uint8_t *hdlc_header(uint8_t *p) {
  struct hdlc_ *hdlc = (struct hdlc_ *) p;
  hdlc->addr = MG_PPP_ADDR;
  hdlc->ctrl = MG_PPP_CTRL;
  return (uint8_t *) (hdlc + 1);
}

static uint8_t *ppp_header(uint16_t proto, uint8_t *p) {
  struct ppp *ppp = (struct ppp *) p;
  ppp->proto = mg_htons(proto);
  return (uint8_t *) (ppp + 1);
}

static uint8_t *l2_ppp_header(enum mg_l2proto proto, uint8_t *p) {
  uint16_t ppp_proto = proto == MG_TCPIP_L2PROTO_IPV4   ? MG_PPP_PROTO_IP
                       : proto == MG_TCPIP_L2PROTO_IPV4 ? MG_PPP_PROTO_IPV6
                                                        : 0;
  return ppp_header(ppp_proto, p);
}

uint8_t *mg_l2_ppp_header(struct mg_tcpip_if *ifp, enum mg_l2proto proto,
                          struct mg_l2addr *src, struct mg_l2addr *dst,
                          uint8_t *frame) {
  (void) ifp;
  (void) src;
  (void) dst;
  return l2_ppp_header(proto, hdlc_header(frame));
}

extern uint8_t *mg_l2_eth_header(struct mg_tcpip_if *ifp, enum mg_l2proto proto,
                                 struct mg_l2addr *src, struct mg_l2addr *dst,
                                 uint8_t *frame);

static uint8_t *pppoe_header(struct mg_tcpip_if *ifp, enum mg_l2proto proto,
                             uint8_t code, uint16_t id, struct mg_l2addr *src,
                             struct mg_l2addr *dst, uint8_t *frame) {
  struct pppoe *pppoe =
      (struct pppoe *) mg_l2_eth_header(ifp, proto, src, dst, frame);
  pppoe->vertype = 0x11;
  pppoe->code = code;
  pppoe->id = id;
  return (uint8_t *) (pppoe + 1);
}

uint8_t *mg_l2_pppoe_header(struct mg_tcpip_if *ifp, enum mg_l2proto proto,
                            struct mg_l2addr *src, struct mg_l2addr *dst,
                            uint8_t *frame) {
  (void) dst;
  return l2_ppp_header(proto, pppoe_header(ifp, MG_TCPIP_L2PROTO_PPPoE_SESS, 0,
                                           s_id, src, dst, frame));
}

// Calculate FCS/CRC for PPP frames. Could be implemented faster using lookup
// tables.
static uint16_t fcs_do(uint8_t *frame, size_t len) {
  uint32_t fcs = 0xffff;
  unsigned int j;
  for (j = 0; j < len; j++) {
    unsigned int i;
    uint8_t x = frame[j];
    for (i = 0; i < 8; i++) {
      fcs = ((fcs ^ x) & 1) ? (fcs >> 1) ^ 0x8408 : fcs >> 1;
      x >>= 1;
    }
  }
  return (uint16_t) (fcs & 0xffff);
}

size_t mg_l2_ppp_trailer(struct mg_tcpip_if *ifp, size_t len, uint8_t *cur) {
  uint16_t crc;
  uint8_t *frame;
  len += sizeof(struct ppp) + sizeof(struct hdlc_);
  frame = cur - len;
  crc = fcs_do(frame, len);
  *cur++ = (uint8_t) ~crc;  // add CRC, note the byte order
  *cur++ = (uint8_t) (~crc >> 8);
  // there is no len field in PPP
  (void) ifp;
  return len + 2;
}

extern size_t mg_l2_eth_trailer(struct mg_tcpip_if *, size_t, uint8_t *);

static size_t pppoe_trailer(struct mg_tcpip_if *ifp, size_t len, uint8_t *cur) {
  struct pppoe *pppoe = (struct pppoe *) (cur - len - sizeof(struct pppoe));
  pppoe->len = mg_htons((uint16_t) len);
  return mg_l2_eth_trailer(ifp, PDIFF(pppoe, cur), cur);
}

size_t mg_l2_pppoe_trailer(struct mg_tcpip_if *ifp, size_t len, uint8_t *cur) {
  return pppoe_trailer(ifp, len + sizeof(struct ppp), cur);
}

size_t mg_l2_driver_output(struct mg_tcpip_if *ifp, size_t len);

static uint8_t *ppp_tx_frame_header(struct mg_tcpip_if *ifp, uint16_t proto) {
  uint8_t *l2p = (uint8_t *) ifp->tx.buf;
  if (ifp->l2type == MG_TCPIP_L2_PPP)
    return ppp_header(proto, hdlc_header(l2p));
  return ppp_header(proto, pppoe_header(ifp, MG_TCPIP_L2PROTO_PPPoE_SESS, 0,
                                        s_id, (struct mg_l2addr *) ifp->mac,
                                        (struct mg_l2addr *) ifp->gwmac, l2p));
}
static size_t ppp_tx_frame_trailer(struct mg_tcpip_if *ifp, size_t len,
                                   uint8_t *cur) {
  return mg_l2_driver_output(ifp, ifp->l2type == MG_TCPIP_L2_PPPoE
                                      ? mg_l2_pppoe_trailer(ifp, len, cur)
                                      : mg_l2_ppp_trailer(ifp, len, cur));
}

// Transmit a single PPP frame for the given protocol
static size_t ppp_tx_frame(struct mg_tcpip_if *ifp, uint16_t proto,
                           uint8_t *data, size_t datasz) {
  uint8_t *pay = ppp_tx_frame_header(ifp, proto);
  memcpy(pay, data, datasz);
  return ppp_tx_frame_trailer(ifp, datasz, pay + datasz);
}

static void ppp_handle_lcp(struct mg_tcpip_if *ifp, uint8_t *lcpp,
                           size_t lcpsz) {
  uint8_t id;
  uint16_t len;
  struct lcp *lcp = (struct lcp *) lcpp;
  if (lcpsz < sizeof(*lcp)) return;
  id = lcp->id;
  len = mg_ntohs(lcp->len);
  switch (lcp->code) {
    case MG_PPP_LCP_CFG_REQ: {
      if (len == sizeof(*lcp)) {
        MG_DEBUG(("LCP config request of %d bytes, acknowledging...", len));
        lcp->code = MG_PPP_LCP_CFG_ACK;
        ppp_tx_frame(ifp, MG_PPP_PROTO_LCP, lcpp, len);
        lcp->code = MG_PPP_LCP_CFG_REQ;
        ppp_tx_frame(ifp, MG_PPP_PROTO_LCP, lcpp, len);
      } else {
        MG_DEBUG(("LCP config request of %d bytes, rejecting...", len));
        lcp->code = MG_PPP_LCP_CFG_REJECT;
        ppp_tx_frame(ifp, MG_PPP_PROTO_LCP, lcpp, len);
      }
    } break;
    case MG_PPP_LCP_CFG_ACK:
      s_link = true;
      break;
    case MG_PPP_LCP_CFG_TERM_REQ: {
      uint8_t ack[4] = {MG_PPP_LCP_CFG_TERM_ACK, id, 0, 4};
      MG_DEBUG(("LCP termination request, acknowledging..."));
      ppp_tx_frame(ifp, MG_PPP_PROTO_LCP, ack, sizeof(ack));
      s_link = false;
    } break;
    case MG_PPP_LCP_ECHO_REQ:  // RFC-1661 5.8: must respond
      MG_DEBUG(("LCP echo request of %d bytes, replying...", len));
      lcp->code = MG_PPP_LCP_ECHO_REPLY;
      ppp_tx_frame(ifp, MG_PPP_PROTO_LCP, lcpp, len);
      break;
  }
}

static bool find_opt(const uint8_t opt, const uint8_t optlen,
                     const uint8_t *opts, size_t optslen, uint8_t *dest) {
  uint8_t *p = (uint8_t *) opts;
  while (optslen >= 2) {               // parse options for requested one
    if (p[1] > optslen) return false;  // truncated / malformed
    if (p[0] == opt && p[1] == optlen) {
      memcpy(dest, p + 2, optlen - 2);
      return true;
    }
    optslen -= p[1];
    p += p[1];
  }
  return false;
}

static void ppp_handle_ipcp(struct mg_tcpip_if *ifp, uint8_t *ipcpp,
                            size_t ipcpsz) {
  uint16_t len;
  uint8_t id;
  struct ipcp *ipcp = (struct ipcp *) ipcpp;
  uint8_t req[] = {
      MG_PPP_IPCP_CFG_REQ, 0, 0, 10, MG_PPP_IPCP_OPT_IPADDR, 6, 0, 0, 0, 0};
  if (ipcpsz < sizeof(*ipcp)) return;
  id = ipcp->id;
  len = mg_ntohs(ipcp->len);
  switch (ipcp->code) {
    case MG_PPP_IPCP_CFG_REQ:
      MG_VERBOSE(("got IPCP config request, acknowledging..."));
      if (len >= 10 &&
          find_opt(MG_PPP_IPCP_OPT_IPADDR, 6, (const uint8_t *) (ipcp + 1),
                   len - sizeof(*ipcp), (uint8_t *) &ifp->gw)) {
        MG_DEBUG(("IPCP cfg, GW IP: %M", mg_print_ip4, &ifp->gw));
        ipcp->code = MG_PPP_IPCP_CFG_ACK;
      } else if (ifp->gw == 0) {
        MG_ERROR(("Peer did not provide its IP address"));
        // NOTE: We should NACK with an added option, probably we can just store
        // the incoming address and offer a statically configured ifp->gw when
        // theirs == 0, but it is unlikely to find such a dumb PPP server and it
        // will complicate our config and this protocol state machine
        ipcp->code = MG_PPP_IPCP_CFG_REJECT;
        ppp_tx_frame(ifp, MG_PPP_PROTO_IPCP, ipcpp, len);
      }
      ppp_tx_frame(ifp, MG_PPP_PROTO_IPCP, ipcpp, len);
      req[1] = id;
      memcpy(req + 6, &ifp->ip, 4);  // Request config IP address or 0.0.0.0
      ppp_tx_frame(ifp, MG_PPP_PROTO_IPCP, req, sizeof(req));
      break;
    case MG_PPP_IPCP_CFG_ACK:
      // Our peer accepted our IP address
      MG_VERBOSE(("got IPCP config ack"));
      ifp->mask = 0xffffffff;  // send to gw
      ifp->state = MG_TCPIP_STATE_IP;
      ifp->gw_ready = true;
      break;
    case MG_PPP_IPCP_CFG_NACK:
      MG_VERBOSE(("got IPCP config nack"));
      // NACK contains our "suggested" IP address, use it
      if (len >= 10 &&
          find_opt(MG_PPP_IPCP_OPT_IPADDR, 6, (const uint8_t *) (ipcp + 1),
                   len - sizeof(*ipcp), (uint8_t *) &ifp->ip)) {
        MG_DEBUG(("IPCP cfg, IP: %M", mg_print_ip4, &ifp->ip));
        ipcp->code = MG_PPP_IPCP_CFG_REQ;
        ppp_tx_frame(ifp, MG_PPP_PROTO_IPCP, ipcpp, len);
      }
      break;
    case MG_PPP_IPCP_CFG_REJECT:
      MG_ERROR(("Peer rejected our IP address, need to properly set ifp->ip"));
      break;
  }
}

#if MG_ENABLE_IPV6
static void ppp_handle_ipv6cp(struct mg_tcpip_if *ifp, uint8_t *ipv6cpp,
                              size_t ipv6cpsz) {
  uint16_t len;
  uint8_t id;
  struct ipv6cp *ipv6cp = (struct ipv6cp *) ipv6cpp;
  uint8_t req[14];
  memset(req, 0, sizeof(req));
  req[0] = MG_PPP_IPV6CP_CFG_REQ, req[3] = 14, req[4] = MG_PPP_IPV6CP_OPT_IFCID,
  req[5] = 10;
  if (ipv6cpsz < sizeof(*ipv6cp)) return;
  id = ipv6cp->id;
  len = mg_ntohs(ipv6cp->len);
  switch (ipv6cp->code) {
    case MG_PPP_IPV6CP_CFG_REQ:
      MG_VERBOSE(("got IPV6CP config request..."));
      if (len >= 10 &&
          find_opt(
              MG_PPP_IPV6CP_OPT_IFCID, 10, (const uint8_t *) (ipv6cp + 1),
              len - sizeof(*ipv6cp),
              (uint8_t *) &((struct mg_l2addr *) (ifp->gwmac))->addr.ieee64)) {
        if (((struct mg_l2addr *) (ifp->gwmac))->addr.ieee64 != 0) {
          MG_DEBUG(("IPV6CP cfg, GW IFCID: %M", mg_print_ieee64,
                    &((struct mg_l2addr *) (ifp->gwmac))->addr.ieee64));
          ipv6cp->code = MG_PPP_IPV6CP_CFG_ACK;
          ppp_tx_frame(ifp, MG_PPP_PROTO_IPV6CP, ipv6cpp, len);
          req[1] = id;
          ifp->ip6ll[0] = 0, ifp->ip6ll[1] = 0;  // clear any former ll address
          memset(req + 6, 0, 8);                 // Inform ifc id 0
          ppp_tx_frame(ifp, MG_PPP_PROTO_IPV6CP, req, sizeof(req));
        } else {
          MG_ERROR(("Peer is not able to provide its interface id"));
          ipv6cp->code = MG_PPP_IPV6CP_CFG_REJECT;
          ppp_tx_frame(ifp, MG_PPP_PROTO_IPV6CP, ipv6cpp, len);
        }
      } else {
        MG_ERROR(("Peer did not provide its interface id"));
        // We should NACK with an added option, but we can't provide one for
        // them
        ipv6cp->code = MG_PPP_IPV6CP_CFG_REJECT;
        ppp_tx_frame(ifp, MG_PPP_PROTO_IPV6CP, ipv6cpp, len);
      }
      break;
    case MG_PPP_IPV6CP_CFG_ACK:
      // Our peer accepted our ifc id
      MG_VERBOSE(("got IPV6CP config ack"));
      break;
    case MG_PPP_IPV6CP_CFG_NACK:
      MG_VERBOSE(("got IPV6CP config nack"));
      // NACK contains our "suggested" IFC id, use it
      if (len >= 10 &&
          find_opt(
              MG_PPP_IPV6CP_OPT_IFCID, 10, (const uint8_t *) (ipv6cp + 1),
              len - sizeof(*ipv6cp),
              (uint8_t *) &((struct mg_l2addr *) (ifp->mac))->addr.ieee64)) {
        MG_DEBUG(("IPV6CP cfg, IFCID: %M", mg_print_ieee64,
                  &((struct mg_l2addr *) (ifp->mac))->addr.ieee64));
        ipv6cp->code = MG_PPP_IPV6CP_CFG_REQ;
        ppp_tx_frame(ifp, MG_PPP_PROTO_IPV6CP, ipv6cpp, len);
      } else {
        MG_ERROR(("Peer is not able to offer an interface id"));
      }
      break;
    case MG_PPP_IPV6CP_CFG_REJECT:
      MG_ERROR(("Peer rejected our interface id"));
      break;
  }
}
#endif

static bool ppp_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
                   struct mg_str *pay, struct mg_str *raw) {
  struct ppp *ppp = (struct ppp *) pay->buf;
  if (pay->len < sizeof(*ppp)) return false;  // Truncated
  pay->buf = (char *) (ppp + 1);
  pay->len = pay->len - sizeof(*ppp);
  switch (mg_ntohs(ppp->proto)) {
    case MG_PPP_PROTO_LCP:
      ppp_handle_lcp(ifp, (uint8_t *) pay->buf, pay->len);
      return false;
    case MG_PPP_PROTO_IPCP:
      if (s_link) ppp_handle_ipcp(ifp, (uint8_t *) pay->buf, pay->len);
      return false;
    case MG_PPP_PROTO_IP:
      if (!s_link) return false;
      MG_VERBOSE(("got IP packet of %d bytes", pay->len));
      *proto = MG_TCPIP_L2PROTO_IPV4;
      break;
#if MG_ENABLE_IPV6
    case MG_PPP_PROTO_IPV6CP:
      if (s_link) ppp_handle_ipv6cp(ifp, (uint8_t *) pay->buf, pay->len);
      return false;
    case MG_PPP_PROTO_IPV6:
      if (!s_link) return false;
      MG_VERBOSE(("got IPv6 packet of %d bytes", pay->len));
      *proto = MG_TCPIP_L2PROTO_IPV6;
      break;
#endif
    default: {
      struct lcp rej;
      uint8_t *p;
      size_t msglen;
      MG_DEBUG(("unknown %u-byte PPP frame with proto 0x%04x:",
                pay->len + sizeof(*ppp), mg_ntohs(ppp->proto)));
      if (mg_log_level >= MG_LL_DEBUG) mg_hexdump(ppp, sizeof(*ppp) + 20);
      if (!s_link) return false;  // RFC-1661 5.7: must reject on link up
      if (pay->len > (size_t) (ifp->mtu - 20))
        pay->len = (size_t) (ifp->mtu - 20);  // truncate to some safe limit
      rej.code = MG_PPP_LCP_REJECT;
      mg_random(&rej.id, sizeof(rej.id));
      msglen = pay->len + sizeof(*ppp);
      rej.len = mg_htons((uint16_t) (msglen + sizeof(rej)));
      p = ppp_tx_frame_header(ifp, MG_PPP_PROTO_LCP);
      memmove(p, &rej, sizeof(rej));          // LCP reject
      memmove(p + sizeof(rej), ppp, msglen);  // rejected PPP message
      ppp_tx_frame_trailer(ifp, msglen + sizeof(rej), p + msglen + sizeof(rej));
    }
      return false;
  }
  (void) raw;
  return true;
}

bool mg_l2_ppp_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
                  struct mg_str *pay, struct mg_str *raw) {
  struct hdlc_ *hdlc = (struct hdlc_ *) raw->buf;
  if (raw->len < sizeof(*hdlc) + 2) return false;  // Truncated
  if (hdlc->addr == MG_PPP_ADDR && hdlc->ctrl == MG_PPP_CTRL) {
    pay->buf = (char *) (hdlc + 1);
    pay->len = raw->len - sizeof(*hdlc) - 2;
  } else {  // Address-and-Control-Field-Compressed PPP header
    pay->buf = (char *) raw->buf;
    pay->len = raw->len - 2;
  }
  return ppp_rx(ifp, proto, pay, raw);
}

extern struct mg_l2addr s_mapip;

struct mg_l2addr *mg_l2_ppp_getaddr(struct mg_tcpip_if *ifp, uint8_t *frame) {
  (void) ifp;
  (void) frame;
  return &s_mapip;  // bogus
}

extern struct mg_l2addr *mg_l2_eth_mapip(enum mg_l2addrtype, struct mg_addr *);

struct mg_l2addr *mg_l2_ppp_mapip(enum mg_l2addrtype addrtype,
                                  struct mg_addr *addr) {
  return mg_l2_eth_mapip(addrtype, addr);
}

#if MG_ENABLE_IPV6
bool mg_l2_ppp_genip6(uint64_t *ip6, uint8_t prefix_len,
                      struct mg_l2addr *l2addr) {
  if (prefix_len > 64) {
    MG_ERROR(("Prefix length > 64, UNSUPPORTED"));
    return false;
  }
  ip6[0] = 0;
  ip6[1] = l2addr->addr.ieee64;
  return false;
}

bool mg_l2_ppp_ip6get(struct mg_l2addr *l2addr, uint8_t *opts, uint8_t len) {
  (void) l2addr;
  (void) opts;
  (void) len;
  return true;
}

uint8_t mg_l2_ppp_ip6put(struct mg_l2addr *l2addr, uint8_t *opts) {
  (void) l2addr;
  (void) opts;
  return 0;
}
#endif

// Transmit a single PPPoE frame for the discovery phase
static size_t pppoe_tx_frame(struct mg_tcpip_if *ifp, uint8_t code, uint16_t id,
                             uint8_t *data, size_t datasz,
                             struct mg_l2addr *dst) {
  uint8_t *l2p = (uint8_t *) ifp->tx.buf;
  uint8_t *p = pppoe_header(ifp, MG_TCPIP_L2PROTO_PPPoE_DISC, code, id,
                            (struct mg_l2addr *) ifp->mac, dst, l2p);
  memmove(p, data, datasz);
  return mg_l2_driver_output(ifp, pppoe_trailer(ifp, datasz, p + datasz));
}

bool mg_l2_pppoe_poll(struct mg_tcpip_if *ifp, bool expired_1000ms) {
  if (expired_1000ms && s_state == MG_PPPoE_ST_DISC &&
      ifp->state == MG_TCPIP_STATE_LINK_UP) {
    uint16_t tags[2];
    tags[0] = mg_htons(0x0101);  // Service Request
    tags[1] = mg_htons(0x0000);  // Any
    pppoe_tx_frame(ifp, MG_PPPoE_PADI, 0, (uint8_t *) tags, sizeof(tags),
                   mg_l2_eth_mapip(MG_TCPIP_L2ADDR_BCAST, NULL));
    MG_DEBUG(("Sent PADI"));
  } else if (expired_1000ms && (s_state != MG_PPPoE_ST_SESS ||
                                ifp->state == MG_TCPIP_STATE_DOWN)) {
    s_state = MG_PPPoE_ST_DISC;
  }
  return mg_l2_ppp_poll(ifp, expired_1000ms);
}

extern bool mg_l2_eth_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
                         struct mg_str *pay, struct mg_str *raw);
extern struct mg_l2addr *mg_l2_eth_getaddr(struct mg_tcpip_if *, uint8_t *);

bool mg_l2_pppoe_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
                    struct mg_str *pay, struct mg_str *raw) {
  enum mg_l2proto eth_proto;  // raw is handled by eth
  struct pppoe *pppoe;
  if (!mg_l2_eth_rx(ifp, &eth_proto, pay, raw)) return false;
  pppoe = (struct pppoe *) pay->buf;            // here we handle pay, not raw
  if (pay->len < sizeof(*pppoe)) return false;  // Truncated
  if (eth_proto == MG_TCPIP_L2PROTO_PPPoE_DISC) {
    MG_VERBOSE(("PPPoE_DISC"));
    if (s_state == MG_PPPoE_ST_DISC && pppoe->code == MG_PPPoE_PADO &&
        pppoe->id == 0) {
      uint16_t tags[2];
      bool has_cookie = false;
      size_t len = pay->len - sizeof(*pppoe);
      uint8_t *p = (uint8_t *) (pppoe + 1);
      uint16_t taglen;
      while (len >= 4) {  // parse tags for a possible AC-Cookie
        uint16_t curtag = *((uint16_t *) p);
        taglen = mg_ntohs(*(((uint16_t *) p) + 1));
        if (taglen > len - 4) return false;  // truncated / malformed
        if (curtag == mg_htons(0x0104)) {
          has_cookie = true;
          break;
        }
        len -= 4 + taglen;
        p += 4 + taglen;
      }
      tags[0] = mg_htons(0x0101);  // Service Request
      tags[1] = mg_htons(0x0000);  // Any
      if (has_cookie) {  // copy tags to before AC-Cookie tag in rx buffer
        memcpy(p -= sizeof(tags), tags, sizeof(tags));  // point to all tags
        taglen += 4 + sizeof(tags);                     // account for tags
      } else {
        p = (uint8_t *) tags;
        taglen = sizeof(tags);
      }
      pppoe_tx_frame(ifp, MG_PPPoE_PADR, 0, p, taglen,
                     mg_l2_eth_getaddr(ifp, (uint8_t *) raw->buf));
      MG_DEBUG(("Sent PADR"));
      s_state = MG_PPPoE_ST_REQ;
    } else if (s_state == MG_PPPoE_ST_REQ && pppoe->code == MG_PPPoE_PADS) {
      s_id = pppoe->id;
      memcpy(&ifp->gwmac,
             mg_l2_eth_getaddr(ifp, (uint8_t *) raw->buf)->addr.mac, 6);
      MG_DEBUG(("PPPoE session 0x%04x started", mg_ntohs(s_id)));
      s_state = MG_PPPoE_ST_SESS;
    } else if (s_state == MG_PPPoE_ST_SESS && pppoe->code == MG_PPPoE_PADT &&
               pppoe->id == s_id) {
      MG_ERROR(("Got PADT"));
      s_id = 0;
      s_link = false;
      s_state = MG_PPPoE_ST_DISC;
    }
  } else if (eth_proto == MG_TCPIP_L2PROTO_PPPoE_SESS &&
             s_state == MG_PPPoE_ST_SESS) {
    pay->buf = (char *) (pppoe + 1);
    pay->len = pay->len - sizeof(*pppoe);
    return ppp_rx(ifp, proto, pay, raw);
  }
  return false;
}

#endif
