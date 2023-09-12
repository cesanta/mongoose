#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_LINES 1
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_PACKED_FS 0

#include "mongoose.c"
#include "driver_mock.c"

static int s_num_tests = 0;
static bool s_sent_fragment = 0;
static int s_seg_sent = 0;

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      abort();                                                  \
    }                                                           \
  } while (0)

static void test_statechange(void) {
  char tx[1540];
  struct mg_tcpip_if iface;
  memset(&iface, 0, sizeof(iface));
  iface.ip = mg_htonl(0x01020304);
  iface.state = MG_TCPIP_STATE_READY;
  iface.tx.ptr = tx, iface.tx.len = sizeof(tx);
  iface.driver = &mg_tcpip_driver_mock;
  onstatechange(&iface);
}

static void ph(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_POLL) ++(*(int *) fn_data);
  (void) c, (void) ev_data;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  (void) c, (void) ev, (void) ev_data, (void) fn_data;
}

static void frag_recv_fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_ERROR) {
    if (s_sent_fragment) {
      ASSERT(strcmp((char*) ev_data, "Received fragmented packet") == 0);
    }
  }
  (void) c, (void) ev_data, (void) fn_data;
}

static void frag_send_fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  static bool s_sent;
  static int s_seg_sizes[] = {416, 416, 368};
  if (ev == MG_EV_POLL) {
    if (!s_sent) {
      c->send.len = 1200; // setting TCP payload size
      s_sent = true;
    }
  } else if (ev == MG_EV_WRITE) {
    // Checking TCP segment sizes (ev_data points to the TCP payload length)
    ASSERT(*(int *) ev_data == s_seg_sizes[s_seg_sent++]);
  }
  (void) c, (void) ev_data, (void) fn_data;
}

static void test_poll(void) {
  int count = 0, i;
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  struct mg_tcpip_if mif;
  memset(&mif, 0, sizeof(mif));
  mif.driver = &mg_tcpip_driver_mock;
  mg_tcpip_init(&mgr, &mif);
  mg_http_listen(&mgr, "http://127.0.0.1:12346", ph, &count);
  for (i = 0; i < 10; i++) mg_mgr_poll(&mgr, 0);
  ASSERT(count == 10);
  mg_tcpip_free(&mif);
  mg_mgr_free(&mgr);
}

#define DRIVER_BUF_SIZE 1540

struct driver_data {
  char buf[DRIVER_BUF_SIZE];
  size_t len;
  bool tx_ready; // data can be read from tx
};

static struct driver_data s_driver_data;

static size_t if_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct driver_data *driver_data = (struct driver_data *) ifp->driver_data;
  if (len > DRIVER_BUF_SIZE) len = DRIVER_BUF_SIZE;
  driver_data->len = len;
  memcpy(driver_data->buf, buf, len);
  driver_data->tx_ready = true;
  return len;
}

static bool if_up(struct mg_tcpip_if *ifp) {
  return ifp->driver_data ? true : false;
}

static size_t if_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct driver_data *driver_data = (struct driver_data *) ifp->driver_data;
  if (!driver_data->len) return 0;
  if (len > driver_data->len) len = driver_data->len;
  memcpy(buf, driver_data->buf, len);
  driver_data->len = 0; // cleaning up the buffer
  return len;
}

static bool received_response(struct driver_data *driver) {
  bool was_ready = driver->tx_ready;
  driver->tx_ready = false;
  return was_ready;
}

static void create_tcp_pkt(struct eth *e, struct ip *ip, uint32_t seq,
                           uint32_t ack, uint8_t flags, size_t payload_len) {
  struct tcp t;
  memset(&t, 0, sizeof(struct tcp));
  t.flags = flags;
  t.seq = mg_htonl(seq);
  t.ack = mg_htonl(ack);
  t.off = 5 << 4;
  memcpy(s_driver_data.buf, e, sizeof(*e));
  memcpy(s_driver_data.buf + sizeof(*e), ip, sizeof(*ip));
  memcpy(s_driver_data.buf + sizeof(*e) + sizeof(*ip), &t, sizeof(struct tcp));
  s_driver_data.len = sizeof(*e) + sizeof(*ip) 
                      + sizeof(struct tcp) + payload_len;
}

static void init_tcp_handshake(struct eth *e, struct ip *ip, struct tcp *tcp,
                               struct mg_mgr *mgr) {
  // SYN
  create_tcp_pkt(e, ip, 1000, 0, TH_SYN | TH_ACK, 0);
  mg_mgr_poll(mgr, 0);

  // SYN-ACK
  while(!received_response(&s_driver_data)) mg_mgr_poll(mgr, 0);
  tcp = (struct tcp *) (s_driver_data.buf + sizeof(struct eth) +
                      sizeof(struct ip));
  ASSERT((tcp->flags == (TH_SYN | TH_ACK)));
  ASSERT((tcp->ack == mg_htonl(1001)));

  // ACK
  create_tcp_pkt(e, ip, 1001, 1, TH_ACK, 0);
  mg_mgr_poll(mgr, 0);
}

static void test_retransmit(void) {
  struct mg_mgr mgr;
  struct eth e;
  struct ip ip;
  struct tcp *t = NULL;
  uint64_t start, now;
  bool response_recv = true;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  mg_mgr_init(&mgr);
  memset(&mif, 0, sizeof(mif));
  memset(&s_driver_data, 0, sizeof(struct driver_data));
  driver.init = NULL, driver.tx = if_tx, driver.up = if_up, driver.rx = if_rx;
  mif.driver = &driver;
  mif.driver_data = &s_driver_data;
  mg_tcpip_init(&mgr, &mif);
  mg_http_listen(&mgr, "http://0.0.0.0:0", fn, NULL);
  mgr.conns->pfn = NULL; // HTTP handler not needed
  mg_mgr_poll(&mgr, 0);

  // setting the Ethernet header
  memset(&e, 0, sizeof(e));
  memcpy(e.dst, mif.mac, 6 * sizeof(uint8_t));
  e.type = mg_htons(0x800);

  // setting the IP header
  memset(&ip, 0, sizeof(ip));
  ip.ver = 4 << 4, ip.proto = 6;
  ip.len = mg_htons(sizeof(ip) + sizeof(struct tcp));

  init_tcp_handshake(&e, &ip, t, &mgr);

  // packet with seq_no = 1001
  ip.len =
      mg_htons(sizeof(struct ip) + sizeof(struct tcp) + /* TCP Payload */ 1);
  create_tcp_pkt(&e, &ip, 1001, 1, TH_PUSH | TH_ACK, 1);
  mg_mgr_poll(&mgr, 0);
  while(!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  t = (struct tcp *) (s_driver_data.buf + sizeof(struct eth) +
                      sizeof(struct ip));
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1002))); // OK

  // resend packet with seq_no = 1001 (e.g.: MIP ACK lost)
  create_tcp_pkt(&e, &ip, 1001, 1, TH_PUSH | TH_ACK, 1);
  mg_mgr_poll(&mgr, 0);
  start = mg_millis();
  while(!received_response(&s_driver_data)) {
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
    // we wait enough time for a reply
    if (now > 2 * MIP_TCP_ACK_MS) {
      response_recv = false;
      break;
    }
  }
  ASSERT((!response_recv)); // replies should not be sent for duplicate packets

  // packet with seq_no = 1002 got lost/delayed, send seq_no = 1003
  create_tcp_pkt(&e, &ip, 1003, 1, TH_PUSH | TH_ACK, 1);
  mg_mgr_poll(&mgr, 0);
  start = mg_millis();
  while(!received_response(&s_driver_data)) {
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
    if (now > 2 * MIP_TCP_ACK_MS)
      ASSERT(0); // response should have been received by now
  }
  t = (struct tcp *) (s_driver_data.buf + sizeof(struct eth) +
                      sizeof(struct ip));
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1002))); // dup ACK

  // retransmitting packet with seq_no = 1002
  create_tcp_pkt(&e, &ip, 1002, 1, TH_PUSH | TH_ACK, 1);
  mg_mgr_poll(&mgr, 0);
  while(!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  t = (struct tcp *) (s_driver_data.buf + sizeof(struct eth) +
                      sizeof(struct ip));
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1003))); // OK

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
  mg_tcpip_free(&mif);
}

static void test_frag_recv_path(void) {
  struct mg_mgr mgr;
  struct eth e;
  struct ip ip;
  struct tcp *t = NULL;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  mg_mgr_init(&mgr);
  memset(&mif, 0, sizeof(mif));
  memset(&s_driver_data, 0, sizeof(struct driver_data));
  driver.init = NULL, driver.tx = if_tx, driver.up = if_up, driver.rx = if_rx;
  mif.driver = &driver;
  mif.driver_data = &s_driver_data;
  mg_tcpip_init(&mgr, &mif);
  mg_http_listen(&mgr, "http://0.0.0.0:0", frag_recv_fn, NULL);
  mgr.conns->pfn = NULL;
  mg_mgr_poll(&mgr, 0);

  // setting the Ethernet header
  memset(&e, 0, sizeof(e));
  memcpy(e.dst, mif.mac, 6 * sizeof(uint8_t));
  e.type = mg_htons(0x800);

  // setting the IP header
  memset(&ip, 0, sizeof(ip));
  ip.ver = 0x45, ip.proto = 6;
  ip.len = mg_htons(sizeof(ip) + sizeof(struct tcp));

  init_tcp_handshake(&e, &ip, t, &mgr);

  // send fragmented TCP packet
  ip.len =
      mg_htons(sizeof(struct ip) + sizeof(struct tcp) + 1000);
  ip.frag |= IP_MORE_FRAGS_MSK; // setting More Fragments bit to 1
  create_tcp_pkt(&e, &ip, 1001, 1, TH_PUSH | TH_ACK, 1000);
  s_sent_fragment = true;
  mg_mgr_poll(&mgr, 0);

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
  mg_tcpip_free(&mif);
}

static void test_frag_send_path(void) {
  struct mg_mgr mgr;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  mg_mgr_init(&mgr);
  memset(&mif, 0, sizeof(mif));
  memset(&s_driver_data, 0, sizeof(struct driver_data));
  driver.init = NULL, driver.tx = if_tx, driver.up = if_up, driver.rx = if_rx;
  mif.driver = &driver;
  mif.driver_data = &s_driver_data;
  mg_tcpip_init(&mgr, &mif);
  mif.mtu = 500;
  mg_http_listen(&mgr, "http://0.0.0.0:0", frag_send_fn, NULL);
  mgr.conns->pfn = NULL;
  for (int i = 0; i < 10; i++)
    mg_mgr_poll(&mgr, 0);
  ASSERT(s_seg_sent == 3);
  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
  mg_tcpip_free(&mif);
}

static void test_fragmentation(void) {
  test_frag_recv_path();
  test_frag_send_path();
}

int main(void) {
  test_statechange();
  test_poll();
  test_retransmit();
  test_fragmentation();
  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return 0;
}
