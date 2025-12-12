#include "l2.h"
#include "net.h"
#include "net_builtin.h"

#if MG_ENABLE_TCPIP

// L2 API
void mg_l2_init(enum mg_l2type type, uint8_t *addr, uint16_t *mtu,
                uint16_t *framesize);
uint8_t *mg_l2_header(enum mg_l2type type, enum mg_l2proto proto, uint8_t *src,
                      uint8_t *dst, uint8_t *frame);
size_t mg_l2_footer(enum mg_l2type type, size_t len, uint8_t *frame);
bool mg_l2_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
              struct mg_str *pay, struct mg_str *raw);
// TODO(): ? bool mg_l2_rx(enum mg_l2type type, struct mg_l2opts *opts, uint8_t
// *addr, enum mg_l2proto *proto, struct mg_str *pay, struct mg_str *raw);
uint8_t *mg_l2_getaddr(enum mg_l2type type, uint8_t *frame);
uint8_t *mg_l2_mapip(enum mg_l2type type, enum mg_l2addrtype addrtype,
                     struct mg_addr *ip);
bool mg_l2_genip6(enum mg_l2type type, uint64_t *ip6, uint8_t prefix_len,
                  uint8_t *addr);
bool mg_l2_ip6get(enum mg_l2type type, uint8_t *addr, uint8_t *opts,
                  uint8_t len);
uint8_t mg_l2_ip6put(enum mg_l2type type, uint8_t *addr, uint8_t *opts);

// clang-format off
extern void mg_l2_eth_init(struct mg_l2addr *, uint16_t *, uint16_t *);
extern uint8_t *mg_l2_eth_header(enum mg_l2proto, struct mg_l2addr *, struct mg_l2addr *, uint8_t *);
extern size_t mg_l2_eth_footer(size_t, uint8_t *);
extern bool mg_l2_eth_rx(struct mg_tcpip_if *, enum mg_l2proto *, struct mg_str *, struct mg_str *);
extern struct mg_l2addr *mg_l2_eth_getaddr(uint8_t *);
extern struct mg_l2addr *mg_l2_eth_mapip(enum mg_l2addrtype, struct mg_addr *);
extern bool mg_l2_eth_genip6(uint64_t *, uint8_t, struct mg_l2addr *);
extern bool mg_l2_eth_ip6get(struct mg_l2addr *, uint8_t *, uint8_t);
extern uint8_t mg_l2_eth_ip6put(struct mg_l2addr *, uint8_t *);

extern void mg_l2_ppp_init(struct mg_l2addr *, uint16_t *, uint16_t *);
extern uint8_t *mg_l2_ppp_header(enum mg_l2proto, struct mg_l2addr *, struct mg_l2addr *, uint8_t *);
extern size_t mg_l2_ppp_footer(size_t, uint8_t *);
extern bool mg_l2_ppp_rx(struct mg_tcpip_if *, enum mg_l2proto *, struct mg_str *, struct mg_str *);
extern struct mg_l2addr *mg_l2_ppp_getaddr(uint8_t *);
extern struct mg_l2addr *mg_l2_ppp_mapip(enum mg_l2addrtype, struct mg_addr *);
#if MG_ENABLE_IPV6
extern bool mg_l2_ppp_genip6(uint64_t *, uint8_t, struct mg_l2addr *);
extern bool mg_l2_ppp_ip6get(struct mg_l2addr *, uint8_t *, uint8_t);
extern uint8_t mg_l2_ppp_ip6put(struct mg_l2addr *, uint8_t *);
#endif

typedef void (*l2_init_fn)(struct mg_l2addr *, uint16_t *, uint16_t *);
typedef uint8_t *((*l2_header_fn)(enum mg_l2proto, struct mg_l2addr *, struct mg_l2addr *, uint8_t *));
typedef size_t (*l2_footer_fn)(size_t, uint8_t *);
typedef bool (*l2_rx_fn)(struct mg_tcpip_if *, enum mg_l2proto *, struct mg_str *, struct mg_str *);
typedef struct mg_l2addr (*(*l2_getaddr_fn)(uint8_t *));
typedef struct mg_l2addr (*(*l2_mapip_fn)(enum mg_l2addrtype, struct mg_addr *));
#if MG_ENABLE_IPV6
typedef bool (*l2_genip6_fn)(uint64_t *, uint8_t, struct mg_l2addr *);
typedef bool (*l2_ip6get_fn)(struct mg_l2addr *, uint8_t *, uint8_t);
typedef uint8_t (*l2_ip6put_fn)(struct mg_l2addr *, uint8_t *);
#endif
// clang-format on

static const l2_init_fn l2_init[] = {mg_l2_eth_init, mg_l2_ppp_init};
static const l2_header_fn l2_header[] = {mg_l2_eth_header, mg_l2_ppp_header};
static const l2_footer_fn l2_footer[] = {mg_l2_eth_footer, mg_l2_ppp_footer};
static const l2_rx_fn l2_rx[] = {mg_l2_eth_rx, mg_l2_ppp_rx};
static const l2_getaddr_fn l2_getaddr[] = {mg_l2_eth_getaddr,
                                           mg_l2_ppp_getaddr};
static const l2_mapip_fn l2_mapip[] = {mg_l2_eth_mapip, mg_l2_ppp_mapip};
#if MG_ENABLE_IPV6
static const l2_genip6_fn l2_genip6[] = {mg_l2_eth_genip6, mg_l2_ppp_genip6};
static const l2_ip6get_fn l2_ip6get[] = {mg_l2_eth_ip6get, mg_l2_ppp_ip6get};
static const l2_ip6put_fn l2_ip6put[] = {mg_l2_eth_ip6put, mg_l2_ppp_ip6put};
#endif

void mg_l2_init(enum mg_l2type type, uint8_t *addr, uint16_t *mtu,
                uint16_t *framesize) {
  l2_init[type]((struct mg_l2addr *) addr, mtu, framesize);
}

uint8_t *mg_l2_header(enum mg_l2type type, enum mg_l2proto proto, uint8_t *src,
                      uint8_t *dst, uint8_t *frame) {
  return l2_header[type](proto, (struct mg_l2addr *) src,
                         (struct mg_l2addr *) dst, frame);
}

size_t mg_l2_footer(enum mg_l2type type, size_t len, uint8_t *frame) {
  return l2_footer[type](len, frame);
}

bool mg_l2_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
              struct mg_str *pay, struct mg_str *raw) {
  return l2_rx[ifp->l2type](ifp, proto, pay, raw);
}

uint8_t *mg_l2_getaddr(enum mg_l2type type, uint8_t *frame) {
  return (uint8_t *) l2_getaddr[type](frame);
}

struct mg_l2addr s_mapip;

uint8_t *mg_l2_mapip(enum mg_l2type type, enum mg_l2addrtype addrtype,
                     struct mg_addr *ip) {
  return (uint8_t *) l2_mapip[type](addrtype, ip);
}

#if MG_ENABLE_IPV6
bool mg_l2_genip6(enum mg_l2type type, uint64_t *ip6, uint8_t prefix_len,
                  uint8_t *addr) {
  return l2_genip6[type](ip6, prefix_len, (struct mg_l2addr *) addr);
}

bool mg_l2_ip6get(enum mg_l2type type, uint8_t *addr, uint8_t *opts,
                  uint8_t len) {
  return l2_ip6get[type]((struct mg_l2addr *) addr, opts, len);
}
uint8_t mg_l2_ip6put(enum mg_l2type type, uint8_t *addr, uint8_t *opts) {
  return l2_ip6put[type]((struct mg_l2addr *) addr, opts);
}
#endif

#endif
