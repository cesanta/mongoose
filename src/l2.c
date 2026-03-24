#include "l2.h"
#include "net.h"
#include "net_builtin.h"

#if MG_ENABLE_TCPIP

// L2 API
void mg_l2_init(struct mg_tcpip_if *ifp);
bool mg_l2_poll(struct mg_tcpip_if *ifp, bool expired_1000ms);
uint8_t *mg_l2_header(struct mg_tcpip_if *ifp, enum mg_l2proto proto,
                      uint8_t *src, uint8_t *dst, uint8_t *frame);
size_t mg_l2_trailer(struct mg_tcpip_if *ifp, size_t len, uint8_t *cur);
bool mg_l2_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
              struct mg_str *pay, struct mg_str *raw);
uint8_t *mg_l2_getaddr(struct mg_tcpip_if *ifp, uint8_t *frame);
uint8_t *mg_l2_mapip(enum mg_l2type type, enum mg_l2addrtype addrtype,
                     struct mg_addr *ip);
#if MG_ENABLE_IPV6
bool mg_l2_genip6(enum mg_l2type type, uint64_t *ip6, uint8_t prefix_len,
                  uint8_t *addr);
bool mg_l2_ip6get(enum mg_l2type type, uint8_t *addr, uint8_t *opts,
                  uint8_t len);
uint8_t mg_l2_ip6put(enum mg_l2type type, uint8_t *addr, uint8_t *opts);
#endif
size_t mg_l2_driver_output(struct mg_tcpip_if *ifp, size_t len);

// clang-format off
extern void mg_l2_eth_init(struct mg_tcpip_if *);
extern bool mg_l2_eth_poll(struct mg_tcpip_if *, bool);
extern uint8_t *mg_l2_eth_header(struct mg_tcpip_if *, enum mg_l2proto, struct mg_l2addr *, struct mg_l2addr *, uint8_t *);
extern size_t mg_l2_eth_trailer(struct mg_tcpip_if *, size_t, uint8_t *);
extern bool mg_l2_eth_rx(struct mg_tcpip_if *, enum mg_l2proto *, struct mg_str *, struct mg_str *);
extern struct mg_l2addr *mg_l2_eth_getaddr(struct mg_tcpip_if *, uint8_t *);
extern struct mg_l2addr *mg_l2_eth_mapip(enum mg_l2addrtype, struct mg_addr *);
#if MG_ENABLE_IPV6
extern bool mg_l2_eth_genip6(uint64_t *, uint8_t, struct mg_l2addr *);
extern bool mg_l2_eth_ip6get(struct mg_l2addr *, uint8_t *, uint8_t);
extern uint8_t mg_l2_eth_ip6put(struct mg_l2addr *, uint8_t *);
#endif

extern void mg_l2_ppp_init(struct mg_tcpip_if *);
extern bool mg_l2_ppp_poll(struct mg_tcpip_if *, bool);
extern uint8_t *mg_l2_ppp_header(struct mg_tcpip_if *, enum mg_l2proto, struct mg_l2addr *, struct mg_l2addr *, uint8_t *);
extern size_t mg_l2_ppp_trailer(struct mg_tcpip_if *, size_t, uint8_t *);
extern bool mg_l2_ppp_rx(struct mg_tcpip_if *, enum mg_l2proto *, struct mg_str *, struct mg_str *);
extern struct mg_l2addr *mg_l2_ppp_getaddr(struct mg_tcpip_if *, uint8_t *);
extern struct mg_l2addr *mg_l2_ppp_mapip(enum mg_l2addrtype, struct mg_addr *);
#if MG_ENABLE_IPV6
extern bool mg_l2_ppp_genip6(uint64_t *, uint8_t, struct mg_l2addr *);
extern bool mg_l2_ppp_ip6get(struct mg_l2addr *, uint8_t *, uint8_t);
extern uint8_t mg_l2_ppp_ip6put(struct mg_l2addr *, uint8_t *);
#endif
extern void mg_l2_pppoe_init(struct mg_tcpip_if *);
extern bool mg_l2_pppoe_poll(struct mg_tcpip_if *, bool);
extern uint8_t *mg_l2_pppoe_header(struct mg_tcpip_if *, enum mg_l2proto, struct mg_l2addr *, struct mg_l2addr *, uint8_t *);
extern size_t mg_l2_pppoe_trailer(struct mg_tcpip_if *, size_t, uint8_t *);
extern bool mg_l2_pppoe_rx(struct mg_tcpip_if *, enum mg_l2proto *, struct mg_str *, struct mg_str *);

typedef void (*l2_init_fn)(struct mg_tcpip_if *);
typedef bool (*l2_poll_fn)(struct mg_tcpip_if *, bool);
typedef uint8_t *((*l2_header_fn)(struct mg_tcpip_if *, enum mg_l2proto, struct mg_l2addr *, struct mg_l2addr *, uint8_t *));
typedef size_t (*l2_trailer_fn)(struct mg_tcpip_if *, size_t, uint8_t *);
typedef bool (*l2_rx_fn)(struct mg_tcpip_if *, enum mg_l2proto *, struct mg_str *, struct mg_str *);
typedef struct mg_l2addr (*(*l2_getaddr_fn)(struct mg_tcpip_if *, uint8_t *));
typedef struct mg_l2addr (*(*l2_mapip_fn)(enum mg_l2addrtype, struct mg_addr *));
#if MG_ENABLE_IPV6
typedef bool (*l2_genip6_fn)(uint64_t *, uint8_t, struct mg_l2addr *);
typedef bool (*l2_ip6get_fn)(struct mg_l2addr *, uint8_t *, uint8_t);
typedef uint8_t (*l2_ip6put_fn)(struct mg_l2addr *, uint8_t *);
#endif
// clang-format on

static const l2_init_fn l2_init[] = {mg_l2_eth_init, mg_l2_ppp_init,
                                     mg_l2_pppoe_init};
static const l2_poll_fn l2_poll[] = {mg_l2_eth_poll, mg_l2_ppp_poll,
                                     mg_l2_pppoe_poll};
static const l2_header_fn l2_header[] = {mg_l2_eth_header, mg_l2_ppp_header,
                                         mg_l2_pppoe_header};
static const l2_trailer_fn l2_trailer[] = {mg_l2_eth_trailer, mg_l2_ppp_trailer,
                                           mg_l2_pppoe_trailer};
static const l2_rx_fn l2_rx[] = {mg_l2_eth_rx, mg_l2_ppp_rx, mg_l2_pppoe_rx};
static const l2_getaddr_fn l2_getaddr[] = {mg_l2_eth_getaddr, mg_l2_ppp_getaddr,
                                           mg_l2_ppp_getaddr};
static const l2_mapip_fn l2_mapip[] = {mg_l2_eth_mapip, mg_l2_ppp_mapip,
                                       mg_l2_ppp_mapip};
#if MG_ENABLE_IPV6
static const l2_genip6_fn l2_genip6[] = {mg_l2_eth_genip6, mg_l2_ppp_genip6,
                                         mg_l2_ppp_genip6};
static const l2_ip6get_fn l2_ip6get[] = {mg_l2_eth_ip6get, mg_l2_ppp_ip6get,
                                         mg_l2_ppp_ip6get};
static const l2_ip6put_fn l2_ip6put[] = {mg_l2_eth_ip6put, mg_l2_ppp_ip6put,
                                         mg_l2_ppp_ip6put};
#endif

void mg_l2_init(struct mg_tcpip_if *ifp) {
  l2_init[ifp->l2type](ifp);
}

bool mg_l2_poll(struct mg_tcpip_if *ifp, bool expired_1000ms) {
  return l2_poll[ifp->l2type](ifp, expired_1000ms);
}

uint8_t *mg_l2_header(struct mg_tcpip_if *ifp, enum mg_l2proto proto,
                      uint8_t *src, uint8_t *dst, uint8_t *frame) {
  return l2_header[ifp->l2type](ifp, proto, (struct mg_l2addr *) src,
                                (struct mg_l2addr *) dst, frame);
}

size_t mg_l2_trailer(struct mg_tcpip_if *ifp, size_t len, uint8_t *frame) {
  return l2_trailer[ifp->l2type](ifp, len, frame);
}

bool mg_l2_rx(struct mg_tcpip_if *ifp, enum mg_l2proto *proto,
              struct mg_str *pay, struct mg_str *raw) {
  return l2_rx[ifp->l2type](ifp, proto, pay, raw);
}

uint8_t *mg_l2_getaddr(struct mg_tcpip_if *ifp, uint8_t *frame) {
  return (uint8_t *) l2_getaddr[ifp->l2type](ifp, frame);
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

size_t mg_l2_driver_output(struct mg_tcpip_if *ifp, size_t len) {
  size_t n = ifp->driver->tx(ifp->tx.buf, len, ifp);
  if (n == len) ifp->nsent++;
  return n;
}

#endif
