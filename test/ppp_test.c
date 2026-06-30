#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0

#include "mongoose.c"
#include "driver_mock.c"

static int s_num_tests = 0;
static bool s_error = false;

#ifdef NO_ABORT
static int s_abort = 0;
#define ABORT() ++s_abort, s_error = true
#else
#ifdef NO_SLEEP_ABORT
#define ABORT() abort()
#else
#define ABORT()                       \
  sleep(2); /* 2s, GH print reason */ \
  abort();
#endif
#endif

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      fflush(stdout);                                           \
      ABORT();                                                  \
    }                                                           \
  } while (0)

static const uint8_t s_mac[6] = {2, 3, 4, 5, 6, 7};
static const uint8_t s_src[6] = {8, 9, 10, 11, 12, 13};

static uint8_t s_tx[2048];

static void init_if(struct mg_tcpip_if *ifp, enum mg_l2type type) {
  memset(ifp, 0, sizeof(*ifp));
  memcpy(ifp->mac, s_mac, sizeof(s_mac));
  ifp->driver = &mg_tcpip_driver_mock;
  ifp->l2type = type;
  ifp->tx.buf = (char *) s_tx;
  ifp->tx.len = sizeof(s_tx);
  mg_l2_init(ifp);
}

static size_t mk_ppp(uint8_t *buf, uint16_t proto, const void *payload,
                     size_t len) {
  struct hdlc_ *hdlc = (struct hdlc_ *) buf;
  struct ppp *ppp = (struct ppp *) (hdlc + 1);
  hdlc->addr = MG_PPP_ADDR;
  hdlc->ctrl = MG_PPP_CTRL;
  ppp->proto = mg_htons(proto);
  memcpy(ppp + 1, payload, len);
  memset((uint8_t *) (ppp + 1) + len, 0, 2);
  return sizeof(*hdlc) + sizeof(*ppp) + len + 2;
}

static void *ppp_payload(uint8_t *frame) {
  return (struct ppp *) ((struct hdlc_ *) frame + 1) + 1;
}

static size_t mk_pppoe_disc(uint8_t *buf, uint8_t code, uint16_t id,
                            const void *payload, size_t len) {
  struct eth *eth = (struct eth *) buf;
  struct pppoe *pppoe = (struct pppoe *) (eth + 1);
  memcpy(eth->dst, s_mac, sizeof(eth->dst));
  memcpy(eth->src, s_src, sizeof(eth->src));
  eth->type = mg_htons(0x8863);
  pppoe->vertype = 0x11;
  pppoe->code = code;
  pppoe->id = mg_htons(id);
  pppoe->len = mg_htons((uint16_t) len);
  if (len > 0) memcpy(pppoe + 1, payload, len);
  return sizeof(*eth) + sizeof(*pppoe) + len;
}

static size_t mk_pppoe_sess(uint8_t *buf, uint16_t id, uint16_t proto,
                            const void *payload, size_t len) {
  struct eth *eth = (struct eth *) buf;
  struct pppoe *pppoe = (struct pppoe *) (eth + 1);
  struct ppp *ppp = (struct ppp *) (pppoe + 1);
  memcpy(eth->dst, s_mac, sizeof(eth->dst));
  memcpy(eth->src, s_src, sizeof(eth->src));
  eth->type = mg_htons(0x8864);
  pppoe->vertype = 0x11;
  pppoe->code = 0;
  pppoe->id = mg_htons(id);
  pppoe->len = mg_htons((uint16_t) (sizeof(*ppp) + len));
  ppp->proto = mg_htons(proto);
  memcpy(ppp + 1, payload, len);
  return sizeof(*eth) + sizeof(*pppoe) + sizeof(*ppp) + len;
}

static void test_ppp_lcp_accept(void) {
  uint8_t frame[64];
  struct mg_tcpip_if ifp;
  struct lcp lcp = {MG_PPP_LCP_CFG_ACK, 1, mg_htons(sizeof(lcp))};
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = false;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_LCP, &lcp, sizeof(lcp)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(s_lcpup == true);
}

static void test_ppp_lcp_discard(void) {
  uint8_t frame[64], lcp[] = {MG_PPP_LCP_CFG_ACK, 1};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = false;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_LCP, lcp, sizeof(lcp)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(s_lcpup == false);
}

static void test_ppp_ip_accept(void) {
  uint8_t frame[64], payload[sizeof(struct ip)];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  memset(payload, 0, sizeof(payload));
  init_if(&ifp, MG_TCPIP_L2_PPP);
  MG_DEBUG(("MTU: %u, frame size: %u", ifp.l2mtu, ifp.framesize));
  ASSERT(ifp.l2mtu != 0);
  ASSERT(ifp.framesize != 0);
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_IP, payload, sizeof(payload)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == true);
  ASSERT(proto == MG_TCPIP_L2PROTO_IPV4);
  ASSERT(pay.len == sizeof(payload));
  ASSERT(memcmp(pay.buf, payload, sizeof(payload)) == 0);
}

static void test_ppp_ip_discard(void) {
  uint8_t frame[64], payload[sizeof(struct ip)];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  memset(payload, 0, sizeof(payload));
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = false;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_IP, payload, sizeof(payload)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
}

static void test_ppp_ipcp_accept(void) {
  uint8_t frame[64], ipcp[] = {MG_PPP_IPCP_CFG_REQ, 1, 0, 10,
                               MG_PPP_IPCP_OPT_IPADDR, 6, 192, 0, 2, 1};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  uint32_t peer;
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = true;
  memcpy(&peer, ipcp + 6, sizeof(peer));
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_IPCP, ipcp, sizeof(ipcp)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(ifp.gw == peer);
  ASSERT(((struct ipcp *) ppp_payload(frame))->code == MG_PPP_IPCP_CFG_ACK);
  ASSERT(ifp.nsent == 2);
}

static void test_ppp_ipcp_reject(void) {
  uint8_t frame[64], ipcp[] = {MG_PPP_IPCP_CFG_REQ, 1, 0, 4};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_IPCP, ipcp, sizeof(ipcp)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(ifp.gw == 0);
  ASSERT(((struct ipcp *) ppp_payload(frame))->code == MG_PPP_IPCP_CFG_REJECT);
  ASSERT(ifp.nsent == 3);
}

#if MG_ENABLE_IPV6
static void test_ppp_ipv6_accept(void) {
  uint8_t frame[96], payload[sizeof(struct ip6)];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  memset(payload, 0, sizeof(payload));
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_IPV6, payload, sizeof(payload)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == true);
  ASSERT(proto == MG_TCPIP_L2PROTO_IPV6);
  ASSERT(pay.len == sizeof(payload));
  ASSERT(memcmp(pay.buf, payload, sizeof(payload)) == 0);
}

static void test_ppp_ipv6_discard(void) {
  uint8_t frame[96], payload[sizeof(struct ip6)];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  memset(payload, 0, sizeof(payload));
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = false;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_IPV6, payload, sizeof(payload)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
}

static void test_ppp_ipv6cp_accept(void) {
  uint8_t frame[64], ipv6cp[] = {MG_PPP_IPV6CP_CFG_REQ, 1, 0, 14,
                                 MG_PPP_IPV6CP_OPT_IFCID, 10,
                                 2, 3, 4, 5, 6, 7, 8, 9};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_IPV6CP, ipv6cp, sizeof(ipv6cp)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(memcmp(&((struct mg_l2addr *) ifp.gwmac)->addr.ieee64, ipv6cp + 6,
                8) == 0);
  ASSERT(((struct ipv6cp *) ppp_payload(frame))->code ==
         MG_PPP_IPV6CP_CFG_ACK);
  ASSERT(ifp.nsent == 2);
}

static void test_ppp_ipv6cp_reject(void) {
  uint8_t frame[64], ipv6cp[] = {MG_PPP_IPV6CP_CFG_REQ, 1, 0, 14,
                                 MG_PPP_IPV6CP_OPT_IFCID, 10,
                                 0, 0, 0, 0, 0, 0, 0, 0};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, MG_PPP_PROTO_IPV6CP, ipv6cp, sizeof(ipv6cp)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(((struct ipv6cp *) ppp_payload(frame))->code ==
         MG_PPP_IPV6CP_CFG_REJECT);
  ASSERT(ifp.nsent == 1);
}
#endif

static void test_ppp_unknown_reject(void) {
  uint8_t frame[64], payload[] = {1, 2, 3, 4};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, 0x1234, payload, sizeof(payload)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(ifp.nsent == 1);
}

static void test_ppp_unknown_discard(void) {
  uint8_t frame[64], payload[] = {1, 2, 3, 4};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPP);
  s_lcpup = false;
  raw = mg_str_n((char *) frame,
                 mk_ppp(frame, 0x1234, payload, sizeof(payload)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(ifp.nsent == 0);
}

static void test_pppoe_pado_accept(void) {
  uint8_t frame[64];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPPoE);
  MG_DEBUG(("MTU: %u, frame size: %u", ifp.l2mtu, ifp.framesize));
  ASSERT(ifp.l2mtu != 0);
  ASSERT(ifp.framesize != 0);
  s_state = MG_PPPoE_ST_DISC;
  raw = mg_str_n((char *) frame,
                 mk_pppoe_disc(frame, MG_PPPoE_PADO, 0, NULL, 0));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(s_state == MG_PPPoE_ST_REQ);
  ASSERT(ifp.nsent == 1);
}

static void test_pppoe_pado_discard(void) {
  uint8_t frame[64];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPPoE);
  s_state = MG_PPPoE_ST_REQ;
  raw = mg_str_n((char *) frame,
                 mk_pppoe_disc(frame, MG_PPPoE_PADO, 0, NULL, 0));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(s_state == MG_PPPoE_ST_REQ);
  ASSERT(ifp.nsent == 0);
}

static void test_pppoe_pads_accept(void) {
  uint8_t frame[64];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPPoE);
  s_state = MG_PPPoE_ST_REQ;
  raw = mg_str_n((char *) frame,
                 mk_pppoe_disc(frame, MG_PPPoE_PADS, 0x1234, NULL, 0));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(s_state == MG_PPPoE_ST_SESS);
  ASSERT(s_id == mg_htons(0x1234));
}

static void test_pppoe_pads_discard(void) {
  uint8_t frame[64];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPPoE);
  s_state = MG_PPPoE_ST_DISC;
  raw = mg_str_n((char *) frame,
                 mk_pppoe_disc(frame, MG_PPPoE_PADS, 0x1234, NULL, 0));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(s_state == MG_PPPoE_ST_DISC);
}

static void test_pppoe_padt_accept(void) {
  uint8_t frame[64];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPPoE);
  s_state = MG_PPPoE_ST_SESS;
  s_id = mg_htons(0x1234);
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_pppoe_disc(frame, MG_PPPoE_PADT, 0x1234, NULL, 0));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(s_state == MG_PPPoE_ST_DISC);
  ASSERT(s_id == 0);
  ASSERT(s_lcpup == false);
}

static void test_pppoe_padt_discard(void) {
  uint8_t frame[64];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  init_if(&ifp, MG_TCPIP_L2_PPPoE);
  s_state = MG_PPPoE_ST_SESS;
  s_id = mg_htons(0x4321);
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_pppoe_disc(frame, MG_PPPoE_PADT, 0x1234, NULL, 0));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(s_state == MG_PPPoE_ST_SESS);
  ASSERT(s_id == mg_htons(0x4321));
  ASSERT(s_lcpup == true);
}

static void test_pppoe_session_accept(void) {
  uint8_t frame[96], payload[sizeof(struct ip)];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  memset(payload, 0, sizeof(payload));
  init_if(&ifp, MG_TCPIP_L2_PPPoE);
  s_state = MG_PPPoE_ST_SESS;
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_pppoe_sess(frame, 0x1234, MG_PPP_PROTO_IP, payload,
                               sizeof(payload)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == true);
  ASSERT(proto == MG_TCPIP_L2PROTO_IPV4);
  ASSERT(pay.len == sizeof(payload));
  ASSERT(memcmp(pay.buf, payload, sizeof(payload)) == 0);
}

static void test_pppoe_session_discard(void) {
  uint8_t frame[96], payload[sizeof(struct ip)];
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  memset(payload, 0, sizeof(payload));
  init_if(&ifp, MG_TCPIP_L2_PPPoE);
  s_state = MG_PPPoE_ST_REQ;
  s_lcpup = true;
  raw = mg_str_n((char *) frame,
                 mk_pppoe_sess(frame, 0x1234, MG_PPP_PROTO_IP, payload,
                               sizeof(payload)));
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
}

#define DASHBOARD(x) \
  printf("HEALTH_DASHBOARD\t\"%s\": %s,\n", x, s_error ? "false" : "true")

int main(void) {
  s_error = false;
  test_ppp_lcp_accept();
  test_ppp_lcp_discard();
  DASHBOARD("lcp");

  s_error = false;
  test_ppp_ipcp_accept();
  test_ppp_ipcp_reject();
  DASHBOARD("ipcp");

#if MG_ENABLE_IPV6
  s_error = false;
  test_ppp_ipv6cp_accept();
  test_ppp_ipv6cp_reject();
  DASHBOARD("ipv6cp");
#endif

  s_error = false;
  test_ppp_ip_accept();
  test_ppp_ip_discard();
#if MG_ENABLE_IPV6
  test_ppp_ipv6_accept();
  test_ppp_ipv6_discard();
#endif
  test_ppp_unknown_reject();
  test_ppp_unknown_discard();
  DASHBOARD("ppp");

  s_error = false;
  test_pppoe_pado_accept();
  test_pppoe_pado_discard();
  test_pppoe_pads_accept();
  test_pppoe_pads_discard();
  test_pppoe_padt_accept();
  test_pppoe_padt_discard();
  test_pppoe_session_accept();
  test_pppoe_session_discard();
  printf("HEALTH_DASHBOARD\t\"pppoe\": %s\n", s_error ? "false" : "true");

#ifdef NO_ABORT
  if (s_abort != 0) return EXIT_FAILURE;
#endif

  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return EXIT_SUCCESS;
}
