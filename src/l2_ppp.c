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

struct ppp {  // RFC-1662
  uint8_t addr, ctrl;
  uint16_t proto;
};

struct lcp {  // RFC-1661
  uint8_t code, id, len[2];
};

struct ipcp { // RFC-1332
  uint8_t code;
};

struct ipv6cp { // RFC-5072
  uint8_t code;
};

#if defined(__DCC__)
#pragma pack(0)
#else
#pragma pack(pop)
#endif


void mg_l2_ppp_init(struct mg_l2addr *addr, uint16_t *mtu,
                    uint16_t *framesize) {
  (void) addr;
  *mtu = 1500;        // 1492 for PPPoE
  *framesize = 1540;  // *** TODO(scaprile): actual value, check for PPPoE too
}

uint8_t *mg_l2_ppp_header(enum mg_l2proto proto, struct mg_l2addr *src,
                          struct mg_l2addr *dst, uint8_t *frame) {
  (void) src;
  (void) dst;
  (void) proto;
  return frame;
}

size_t mg_l2_ppp_footer(size_t len, uint8_t *frame) {
  (void) frame;
  return len;
}

bool mg_l2_ppp_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
                  struct mg_str *pay, struct mg_str *raw) {
#if 0
if (ppp->addr == MG_PPP_ADDR && ppp->ctrl == MG_PPP_CTRL) {
  code = ntohs(ppp->proto);
  payload = (uint8_t *) (ppp + 1);
} else { // Address-and-Control-Field-Compressed PPP header
  uint16_t *cppp = (uint16_t *) ppp;
  code = ntohs(*cppp);
  payload = (uint8_t *) (cppp + 1);
}
#endif
  *pay = *raw;
  *proto = MG_TCPIP_L2PROTO_IPV4;
  (void) ifp;
  return true;
}

struct mg_l2addr *mg_l2_ppp_getaddr(uint8_t *frame) {
  (void) frame;
  return &s_mapip;  // bogus
}

extern struct mg_l2addr s_mapip;

struct mg_l2addr *mg_l2_ppp_mapip(enum mg_l2addrtype addrtype,
                                  struct mg_addr *addr) {
  (void) addrtype;
  (void) addr;
  return &s_mapip;  // bogus
}

#if MG_ENABLE_IPV6
bool mg_l2_ppp_genip6(uint64_t *ip6, uint8_t prefix_len,
                      struct mg_l2addr *addr) {
  (void) ip6;
  (void) prefix_len;
  (void) addr;
  return false;
}

bool mg_l2_ppp_ip6get(struct mg_l2addr *addr, uint8_t *opts, uint8_t len) {
  (void) addr;
  (void) opts;
  (void) len;
  return false;
}

uint8_t mg_l2_ppp_ip6put(struct mg_l2addr *addr, uint8_t *opts) {
  (void) addr;
  (void) opts;
  return 0;
}
#endif

#endif
