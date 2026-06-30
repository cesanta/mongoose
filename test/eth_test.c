#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0

#include "mongoose.c"

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

static void reset_if(struct mg_tcpip_if *ifp) {
  memset(ifp, 0, sizeof(*ifp));
  memcpy(ifp->mac, s_mac, sizeof(s_mac));
  ifp->l2type = MG_TCPIP_L2_ETH;
}

static size_t mk_eth(uint8_t *buf, const uint8_t *dst, uint16_t type,
                     const uint8_t *payload, size_t len) {
  struct eth *eth = (struct eth *) buf;
  memcpy(eth->dst, dst, sizeof(eth->dst));
  memcpy(eth->src, s_src, sizeof(eth->src));
  eth->type = mg_htons(type);
  memcpy(eth + 1, payload, len);
  return sizeof(*eth) + len;
}

static size_t mk_vlan(uint8_t *buf, const uint8_t *dst, uint16_t vlan,
                      uint16_t type, const uint8_t *payload, size_t len) {
  struct eth *eth = (struct eth *) buf;
  struct qtag *qtag = (struct qtag *) &eth->type;
  memcpy(eth->dst, dst, sizeof(eth->dst));
  memcpy(eth->src, s_src, sizeof(eth->src));
  qtag->tpid = mg_htons(0x8100);
  qtag->tci = mg_htons(vlan);
  MG_STORE_BE16(qtag + 1, type);
  memcpy((uint8_t *) (qtag + 1) + sizeof(uint16_t), payload, len);
  return sizeof(*eth) + sizeof(*qtag) + len;
}

static size_t add_fcs(uint8_t *buf, size_t len) {
  uint32_t crc = mg_crc32(0, (const char *) buf, len);
  memcpy(buf + len, &crc, sizeof(crc));
  return len + sizeof(crc);
}

static void test_plain_accept(void) {
  uint8_t frame[64], payload[] = {1, 2, 3, 4};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  mg_l2_init(&ifp);
  MG_DEBUG(("MTU: %u, frame size: %u", ifp.l2mtu, ifp.framesize));
  ASSERT(ifp.l2mtu != 0);
  ASSERT(ifp.framesize != 0);
  len = mk_eth(frame, s_mac, 0x0800, payload, sizeof(payload));
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == true);
  ASSERT(proto == MG_TCPIP_L2PROTO_IPV4);
  ASSERT(pay.len == sizeof(payload));
  ASSERT(memcmp(pay.buf, payload, sizeof(payload)) == 0);
}

static void test_plain_discard(void) {
  uint8_t frame[64], payload[] = {1, 2, 3, 4};
  char sentinel[] = "discard";
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay = mg_str_n(sentinel, sizeof(sentinel));
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  mg_l2_init(&ifp);
  len = mk_eth(frame, s_mac, 0x1234, payload, sizeof(payload));
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(proto == MG_TCPIP_L2PROTO_ARP);
}

static void test_mac_check_accept(void) {
  uint8_t frame[64], payload[] = {1, 2, 3, 4};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  ifp.enable_mac_check = true;
  mg_l2_init(&ifp);
  len = mk_eth(frame, s_mac, 0x0800, payload, sizeof(payload));
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == true);
  ASSERT(proto == MG_TCPIP_L2PROTO_IPV4);
  ASSERT(pay.len == sizeof(payload));
  ASSERT(memcmp(pay.buf, payload, sizeof(payload)) == 0);
}

static void test_mac_check_discard(void) {
  uint8_t frame[64], payload[] = {1, 2, 3, 4}, dst[6] = {1, 1, 1, 1, 1, 1};
  char sentinel[] = "discard";
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay = mg_str_n(sentinel, sizeof(sentinel));
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  ifp.enable_mac_check = true;
  mg_l2_init(&ifp);
  len = mk_eth(frame, dst, 0x0800, payload, sizeof(payload));
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(proto == MG_TCPIP_L2PROTO_ARP);
  ASSERT(pay.buf == sentinel);
  ASSERT(pay.len == sizeof(sentinel));
}

static void test_fcs_check_accept(void) {
  uint8_t frame[64], payload[] = {1, 2, 3, 4};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  ifp.enable_fcs_check = true;
  mg_l2_init(&ifp);
  len = add_fcs(frame, mk_eth(frame, s_mac, 0x0800, payload, sizeof(payload)));
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == true);
  ASSERT(proto == MG_TCPIP_L2PROTO_IPV4);
  ASSERT(pay.len == sizeof(payload));
  ASSERT(memcmp(pay.buf, payload, sizeof(payload)) == 0);
}

static void test_fcs_check_discard(void) {
  uint8_t frame[64], payload[] = {1, 2, 3, 4};
  char sentinel[] = "discard";
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay = mg_str_n(sentinel, sizeof(sentinel));
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  ifp.enable_fcs_check = true;
  mg_l2_init(&ifp);
  len = add_fcs(frame, mk_eth(frame, s_mac, 0x0800, payload, sizeof(payload)));
  frame[len - 1] ^= 1;
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(proto == MG_TCPIP_L2PROTO_ARP);
  ASSERT(pay.buf == sentinel);
  ASSERT(pay.len == sizeof(sentinel));
}

static void test_fcs_check_disabled(void) {
  uint8_t frame[64], payload[] = {1, 2, 3, 4};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  mg_l2_init(&ifp);
  len = add_fcs(frame, mk_eth(frame, s_mac, 0x0800, payload, sizeof(payload)));
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == true);
  ASSERT(proto == MG_TCPIP_L2PROTO_IPV4);
  ASSERT(pay.len == sizeof(payload) + sizeof(uint32_t));
  ASSERT(memcmp(pay.buf, payload, sizeof(payload)) == 0);
}

static void test_vlan_accept(void) {
  uint8_t frame[64], payload[] = {5, 6, 7, 8};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  ifp.l2data.eth.vlan_id = 123;
  mg_l2_init(&ifp);
  MG_DEBUG(("MTU: %u, frame size: %u", ifp.l2mtu, ifp.framesize));
  ASSERT(ifp.l2mtu != 0);
  ASSERT(ifp.framesize != 0);
  len = mk_vlan(frame, s_mac, 123, 0x86dd, payload, sizeof(payload));
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == true);
  ASSERT(proto == MG_TCPIP_L2PROTO_IPV6);
  ASSERT(pay.len == sizeof(payload));
  ASSERT(memcmp(pay.buf, payload, sizeof(payload)) == 0);
}

static void test_vlan_discard(void) {
  uint8_t frame[64], payload[] = {5, 6, 7, 8};
  char sentinel[] = "discard";
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay = mg_str_n(sentinel, sizeof(sentinel));
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  ifp.l2data.eth.vlan_id = 123;
  mg_l2_init(&ifp);
  len = mk_vlan(frame, s_mac, 124, 0x86dd, payload, sizeof(payload));
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(proto == MG_TCPIP_L2PROTO_ARP);
  ASSERT(pay.buf == sentinel);
  ASSERT(pay.len == sizeof(sentinel));
}

static void test_tagged_without_vlan_discard(void) {
  uint8_t frame[64], payload[] = {5, 6, 7, 8};
  struct mg_tcpip_if ifp;
  struct mg_str raw, pay;
  enum mg_l2proto proto = MG_TCPIP_L2PROTO_ARP;
  size_t len;
  reset_if(&ifp);
  mg_l2_init(&ifp);
  len = mk_vlan(frame, s_mac, 123, 0x86dd, payload, sizeof(payload));
  raw = mg_str_n((char *) frame, len);
  ASSERT(mg_l2_rx(&ifp, &proto, &pay, &raw) == false);
  ASSERT(proto == MG_TCPIP_L2PROTO_ARP);
}

#define DASHBOARD(x) \
  printf("HEALTH_DASHBOARD\t\"%s\": %s,\n", x, s_error ? "false" : "true")

int main(void) {
  s_error = false;
  test_plain_accept();
  test_plain_discard();
  DASHBOARD("eth");

  s_error = false;
  test_mac_check_accept();
  test_mac_check_discard();
  DASHBOARD("mac");

  s_error = false;
  test_fcs_check_accept();
  test_fcs_check_discard();
  test_fcs_check_disabled();
  DASHBOARD("fcs");

  s_error = false;
  test_vlan_accept();
  test_vlan_discard();
  test_tagged_without_vlan_discard();
  printf("HEALTH_DASHBOARD\t\"vlan\": %s\n", s_error ? "false" : "true");

#ifdef NO_ABORT
  if (s_abort != 0) return EXIT_FAILURE;
#endif

  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return EXIT_SUCCESS;
}
