#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0

#include "mongoose.c"  // order is important, this one first

#include "driver_mock.c"

static int s_num_tests = 0;
static int s_sent_fragment = 0;
static int s_seg_sent = 0;

#ifdef NO_SLEEP_ABORT
#define ABORT() abort()
#else
#define ABORT()                       \
  sleep(2); /* 2s, GH print reason */ \
  abort();
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

static void test_csum(void) {
  uint8_t ip[20] = {0x45, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x28, 0x11,
                    0x94, 0xcf, 0x7f, 0x00, 0x00, 0x01, 0x7f, 0x00, 0x00, 0x01};
  ASSERT(ipcsum(ip, 20) == 0);
  // UDP and TCP checksum calc funcions use the same basic calls as ipcsum()
}

static void test_statechange(void) {
  char tx[1540];
  struct mg_tcpip_if iface;
  memset(&iface, 0, sizeof(iface));
  iface.ip = mg_htonl(0x01020304);
  iface.state = MG_TCPIP_STATE_READY;
  iface.tx.buf = tx, iface.tx.len = sizeof(tx);
  iface.driver = &mg_tcpip_driver_mock;
  onstatechange(&iface);
}

static void ph(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_POLL) ++(*(int *) c->fn_data);
  (void) c, (void) ev_data;
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  (void) c, (void) ev, (void) ev_data;
}

static void frag_recv_fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ERROR) {
    if (s_sent_fragment > 0) {
      ASSERT(s_sent_fragment == 1);
      ASSERT(strcmp((char *) ev_data, "Received fragmented packet") == 0);
      s_sent_fragment = 2;
    }
  }
  (void) c, (void) ev_data;
}

// mock send to a non-existent peer using the listener connection
static void frag_send_fn(struct mg_connection *c, int ev, void *ev_data) {
  static bool s_sent;
  static int s_seg_sizes[] = {416, 416, 368};  // based on len=1200 and MTU=500
  if (ev == MG_EV_POLL) {
    if (!s_sent) {
      struct connstate *s = (struct connstate *) (c + 1);
      s->dmss = 1500;      // mock set some destination MSS way larger
      c->send.len = 1200;  // setting TCP payload size
      s_sent = true;
    }
  } else if (ev == MG_EV_WRITE) {
    // Checking TCP segment sizes (ev_data points to the TCP payload length)
    ASSERT(*(int *) ev_data == s_seg_sizes[s_seg_sent++]);
  }
  (void) c, (void) ev_data;
}

static void test_poll(void) {
  int count = 0, i;
  struct mg_tcpip_if mif;
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  memset(&mif, 0, sizeof(mif));
  mif.driver = &mg_tcpip_driver_mock;
  mg_tcpip_init(&mgr, &mif);
  mg_http_listen(&mgr, "http://127.0.0.1:12346", ph, &count);
  for (i = 0; i < 10; i++) mg_mgr_poll(&mgr, 0);
  ASSERT(count == 10);
  mg_mgr_free(&mgr);
}

#define DRIVER_BUF_SIZE 1540

struct driver_data {
  char buf[DRIVER_BUF_SIZE];
  size_t len;
  bool tx_ready;  // data can be read from tx
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

static bool if_poll(struct mg_tcpip_if *ifp, bool s1) {
  return s1 && ifp->driver_data ? true : false;
}

static size_t if_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct driver_data *driver_data = (struct driver_data *) ifp->driver_data;
  if (driver_data->len == 0) return 0;
  if (len > driver_data->len) len = driver_data->len;
  memcpy(buf, driver_data->buf, len);
  driver_data->len = 0;  // cleaning up the buffer
  return len;
}

static bool received_response(struct driver_data *driver) {
  bool was_ready = driver->tx_ready;
  driver->tx_ready = false;
  return was_ready;
}

static void create_tcp_seg(struct eth *e, struct ip *ip, uint32_t seq,
                           uint32_t ack, uint8_t flags, uint16_t sport,
                           uint16_t dport, size_t payload_len, void *opts, unsigned int opts_len) {
  struct tcp t;
  memset(&t, 0, sizeof(struct tcp));
  t.flags = flags;
  t.seq = mg_htonl(seq);
  t.ack = mg_htonl(ack);
  t.sport = mg_htons(sport);
  t.dport = mg_htons(dport);
  t.off = (uint8_t) (sizeof(t) / 4 << 4) + (uint8_t) (opts_len / 4 << 4);
  memcpy(s_driver_data.buf, e, sizeof(*e));
  ip->len =
      mg_htons((uint16_t) (sizeof(*ip) + sizeof(struct tcp) + payload_len));
  memcpy(s_driver_data.buf + sizeof(*e), ip, sizeof(*ip));
  memcpy(s_driver_data.buf + sizeof(*e) + sizeof(*ip), &t, sizeof(t));
  if (opts != NULL && opts_len)
    memcpy(s_driver_data.buf + sizeof(*e) + sizeof(*ip) + sizeof(t), opts, opts_len);
  s_driver_data.len = sizeof(*e) + sizeof(*ip) + sizeof(t) + payload_len + opts_len;
}

static void create_tcp_simpleseg(struct eth *e, struct ip *ip, uint32_t seq,
                                 uint32_t ack, uint8_t flags,
                                 size_t payload_len) {
  // use sport=1 to ease seqno stuff, dport=80 due to init_tcp_tests() below
  create_tcp_seg(e, ip, seq, ack, flags, 1, 80, payload_len, NULL, 0);
}

static void init_tcp_tests(struct mg_mgr *mgr, struct eth *e, struct ip *ip,
                           struct mg_tcpip_driver *driver,
                           struct mg_tcpip_if *mif, mg_event_handler_t f) {
  mg_mgr_init(mgr);
  memset(mif, 0, sizeof(*mif));
  memset(&s_driver_data, 0, sizeof(struct driver_data));
  driver->init = NULL, driver->tx = if_tx, driver->poll = if_poll,
  driver->rx = if_rx;
  mif->driver = driver;
  mif->driver_data = &s_driver_data;
  mg_tcpip_init(mgr, mif);
  mg_http_listen(mgr, "http://0.0.0.0:80", f, NULL);
  mgr->conns->pfn = NULL;  // HTTP handler not needed
  mg_mgr_poll(mgr, 0);

  // setting the Ethernet header
  memset(e, 0, sizeof(*e));
  memcpy(e->dst, mif->mac, 6 * sizeof(uint8_t));
  e->type = mg_htons(0x800);

  // setting the IP header
  memset(ip, 0, sizeof(*ip));
  ip->ver = 4 << 4 | 5;
  ip->proto = 6;
}

static void init_tcp_handshake(struct eth *e, struct ip *ip,
                               struct mg_mgr *mgr) {
  struct tcp *t = (struct tcp *)(s_driver_data.buf + sizeof(*e) + sizeof(*ip));

  // SYN
  create_tcp_simpleseg(e, ip, 1000, 0, TH_SYN | TH_ACK, 0);
  mg_mgr_poll(mgr, 0); // make sure we clean former stuff in buffer

  // SYN-ACK
  while (!received_response(&s_driver_data)) mg_mgr_poll(mgr, 0);
  ASSERT((t->flags == (TH_SYN | TH_ACK)));
  ASSERT((t->ack == mg_htonl(1001)));

  // ACK
  create_tcp_simpleseg(e, ip, 1001, 2, TH_ACK, 0);
  mg_mgr_poll(mgr, 0);
}


static void test_tcp_basics(void) {
  struct mg_mgr mgr;
  struct eth e;
  struct ip ip;
  struct tcp *t = (struct tcp *) (s_driver_data.buf + sizeof(e) + sizeof(ip));
  // uint64_t start, now;
  // bool response_recv = true;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  init_tcp_tests(&mgr, &e, &ip, &driver, &mif, fn);

  // send SYN to a non-used port, expect RST + ACK
  create_tcp_seg(&e, &ip, 1234, 1, TH_SYN, 0, 69, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0); // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == (TH_RST | TH_ACK)));
  ASSERT((t->ack == mg_htonl(1235)));

  // send data to a non-used port, expect a RST + ACK
  create_tcp_seg(&e, &ip, 1234, 1, TH_PUSH, 0, 69, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == (TH_RST | TH_ACK)));
  ASSERT((t->ack == mg_htonl(1234)));

  

  init_tcp_handshake(&e, &ip, &mgr);   // starts with seq_no=1000, ackno=2

  // no MSS sent, so it must default to 536 (RFC-9293 3.7.1)
  ASSERT((((struct connstate *)(mgr.conns + 1))->dmss == 536));

  // segment with seq_no within window
  create_tcp_simpleseg(&e, &ip, 1010, 2, TH_PUSH, 2);
  mg_mgr_poll(&mgr, 0);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1001)));  // expecting 1001, dude

  // segment with seq_no way out of window
  create_tcp_simpleseg(&e, &ip, 1000000, 2, TH_PUSH, 2);
  mg_mgr_poll(&mgr, 0);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1001)));  // expecting 1001, dude

  // we currently don't validate checksum, no silently discarded segment test

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
}

// NOTE: a 1-byte payload could be an erroneous Keep-Alive, keep length > 1 in
// this operation, we're testing retransmissions and having len=1 won't work

static void test_tcp_retransmit(void) {
  struct mg_mgr mgr;
  struct eth e;
  struct ip ip;
  struct tcp *t = (struct tcp *) (s_driver_data.buf + sizeof(e) + sizeof(ip));
  uint64_t start, now;
  bool response_recv = true;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  init_tcp_tests(&mgr, &e, &ip, &driver, &mif, fn);

  init_tcp_handshake(&e, &ip, &mgr);   // starts with seq_no=1000, ackno=2

  // packet with seq_no = 1001
  create_tcp_simpleseg(&e, &ip, 1001, 2, TH_PUSH | TH_ACK, 2);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1003)));  // OK

  // resend packet with seq_no = 1001 (e.g.: MIP ACK lost)
  create_tcp_simpleseg(&e, &ip, 1001, 2, TH_PUSH | TH_ACK, 2);
  mg_mgr_poll(&mgr, 0);
  start = mg_millis();
  while (!received_response(&s_driver_data)) {
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
    // we wait enough time for a reply
    if (now > 2 * MIP_TCP_ACK_MS) {
      response_recv = false;
      break;
    }
  }
  ASSERT((!response_recv));  // replies should not be sent for duplicate packets

  // packet with seq_no = 1003 got lost/delayed, send seq_no = 1005
  create_tcp_simpleseg(&e, &ip, 1005, 2, TH_PUSH | TH_ACK, 2);
  mg_mgr_poll(&mgr, 0);
  start = mg_millis();
  while (!received_response(&s_driver_data)) {
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
    if (now > 2 * MIP_TCP_ACK_MS)
      ASSERT(0);  // response should have been received by now
  }
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1003)));  // dup ACK

  // retransmitting packet with seq_no = 1003
  create_tcp_simpleseg(&e, &ip, 1003, 2, TH_PUSH | TH_ACK, 2);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1005)));  // OK

  // packet with seq_no = 1005 got delayed, send FIN with seq_no = 1007
  create_tcp_simpleseg(&e, &ip, 1007, 2, TH_FIN, 0);
  mg_mgr_poll(&mgr, 0);
  start = mg_millis();
  while (!received_response(&s_driver_data)) {
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
    if (now > 2 * MIP_TCP_ACK_MS)
      ASSERT(0);  // response should have been received by now
  }
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1005)));  // dup ACK

  // retransmitting packet with seq_no = 1005
  create_tcp_simpleseg(&e, &ip, 1005, 2, TH_PUSH | TH_ACK, 2);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1007)));  // OK

  // retransmitting FIN packet with seq_no = 1007
  create_tcp_simpleseg(&e, &ip, 1007, 2, TH_FIN | TH_ACK, 0);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == (TH_FIN | TH_ACK)));  // check we respond with FIN ACK
  ASSERT((t->ack == mg_htonl(1008)));       // OK

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
}

static void test_frag_recv_path(void) {
  struct mg_mgr mgr;
  struct eth e;
  struct ip ip;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  init_tcp_tests(&mgr, &e, &ip, &driver, &mif, frag_recv_fn);

  init_tcp_handshake(&e, &ip, &mgr);   // starts with seq_no=1000, ackno=2

  // send fragmented TCP packet
  ip.frag |= IP_MORE_FRAGS_MSK;  // setting More Fragments bit to 1
  create_tcp_simpleseg(&e, &ip, 1001, 2, TH_PUSH | TH_ACK, 1000);
  s_sent_fragment = 1;           // "enable" fn
  mg_mgr_poll(&mgr, 0);          // call it (process fake frag IP)
  ASSERT(s_sent_fragment == 2);  // check it followed the right path

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
}

static void test_frag_send_path(void) {
  struct mg_mgr mgr;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;
  unsigned int i;

  mg_mgr_init(&mgr);
  memset(&mif, 0, sizeof(mif));
  memset(&s_driver_data, 0, sizeof(struct driver_data));
  driver.init = NULL, driver.tx = if_tx, driver.poll = if_poll,
  driver.rx = if_rx;
  mif.driver = &driver;
  mif.driver_data = &s_driver_data;
  mg_tcpip_init(&mgr, &mif);
  mif.mtu = 500;  // force ad hoc small MTU to fragment IP
  mg_http_listen(&mgr, "http://0.0.0.0:80", frag_send_fn, NULL);
  mgr.conns->pfn = NULL;
  for (i = 0; i < 10; i++) mg_mgr_poll(&mgr, 0);
  ASSERT(s_seg_sent == 3);
  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
}

static void test_fragmentation(void) {
  test_frag_recv_path();
  test_frag_send_path();
}


static void test_tcp_backlog(void) {
  struct mg_mgr mgr;
  struct eth e;
  struct ip ip;
  struct tcp *t = (struct tcp *) (s_driver_data.buf + sizeof(e) + sizeof(ip));
  struct ip *i = (struct ip *) (s_driver_data.buf + sizeof(e));
  uint64_t start, now;
  //bool response_recv = true;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;
  uint16_t opts[4 / 2];                            // Send MSS, RFC-9293 3.7.1
  struct mg_connection *c;
  unsigned int j;
#define LOGSZ (sizeof(c->data) / sizeof(struct mg_backlog))
  uint32_t seqnos[LOGSZ];

  init_tcp_tests(&mgr, &e, &ip, &driver, &mif, fn);

  // test expired connection attempts cleanup
  create_tcp_seg(&e, &ip, 1234, 0, TH_SYN, 1, 80, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0); // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == (TH_SYN | TH_ACK));
  // delay ACK so it is removed from the backlog
  start = mg_millis();
  do {
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
  } while (now < 2100);
  // Mongoose may have retransmitted SYN + ACK, so
  create_tcp_simpleseg(&e, &ip, 1235, 2, TH_ACK, 0);
  mg_mgr_poll(&mgr, 0); // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data) || i->proto != 6) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == (TH_RST | TH_ACK));


  opts[0] = mg_htons(0x0204); // RFC-9293 3.2
  // fill the backlog
  for (j = 0; j < LOGSZ; j++) {
    // assign one MSS for each connection
    opts[1] = mg_htons((uint16_t)(1010 + j));
    create_tcp_seg(&e, &ip, 100 + j, 0, TH_SYN, (uint16_t)(j + 1), 80, 0, opts, sizeof(opts));
    while (!received_response(&s_driver_data) || i->proto != 6)
      mg_mgr_poll(&mgr, 0);
    ASSERT(t->flags == (TH_SYN | TH_ACK));
    seqnos[j] = ntohl(t->seq);
    MG_VERBOSE(("SEQ: %p", seqnos[j]));
  }
  // check backlog is full and MSS are there
  c = mgr.conns;
  for (j = 0; j < LOGSZ; j++) {
    struct mg_backlog *b = (struct mg_backlog *)(c->data) + j;
    ASSERT(b->port != 0);
    MG_DEBUG(("SEQ: %p, MSS: %u", seqnos[j], (unsigned int)b->mss));
    ASSERT(b->mss == (1010 + j));
  }
  // one more attempt, it must fail
  opts[1] = mg_htons((uint16_t)(1010 + j));
  create_tcp_seg(&e, &ip, 100 + j, 0, TH_SYN, (uint16_t)(j + 1), 80, 0, opts, sizeof(opts));
  mg_mgr_poll(&mgr, 0);
  ASSERT(!received_response(&s_driver_data) || i->proto != 6);
  // a late response for this attempt would break what follows
  // establish all connections
  for (j = 0; j < LOGSZ; j++) {
    create_tcp_seg(&e, &ip, 100 + j + 1, seqnos[j] + 1, TH_ACK, (uint16_t)(j + 1), 80, 0, NULL, 0);
    mg_mgr_poll(&mgr, 0);
    ASSERT(!received_response(&s_driver_data) || i->proto != 6);
  }
  // check backlog is now empty
  c = mgr.conns; // last one is the listener
  for (; c->next != NULL; c = c->next);
  for (j = 0; j < LOGSZ; j++) {
    struct mg_backlog *b = (struct mg_backlog *)(c->data) + j;
    ASSERT(b->port == 0);
  }
  c = mgr.conns; // first one is more recent
  // check MSS is what we sent, everything's fine
  for (j = LOGSZ; j > 0 ; j--, c = c->next) {
    struct connstate *s = (struct connstate *)(c + 1);
    ASSERT(c != NULL);
    MG_DEBUG(("MSS: %u", (unsigned int) s->dmss));
    ASSERT(s->dmss == (1010 + j - 1));
  }
  ASSERT(c != NULL); // last one is the listener
  ASSERT(c->next == NULL);

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
}

static void test_tcp(void) {
  test_tcp_basics();
  test_tcp_backlog();
  test_tcp_retransmit();
}

int main(void) {
  test_csum();
  test_statechange();
  test_poll();
  test_tcp();
  test_fragmentation();
  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return 0;
}
