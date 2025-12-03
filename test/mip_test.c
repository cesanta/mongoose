#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0

#include "mongoose.c"  // order is important, this one first

#include "driver_mock.c"

static int s_num_tests = 0;
static bool s_error = false;
static int s_sent_fragment = 0;
static int s_seg_sent = 0;

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

struct ipp {
  struct ip *ip4;
  struct ip6 *ip6;
};

static void test_csum(void) {
  uint8_t ip[20] = {0x45, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x28, 0x11,
                    0x94, 0xcf, 0x7f, 0x00, 0x00, 0x01, 0x7f, 0x00, 0x00, 0x01};
  ASSERT(ipcsum(ip, 20) == 0);
  // UDP and TCP checksum calc funcions use the same basic calls as ipcsum()
}

static bool executed = false;

static void mif_fn(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  if (ev == MG_TCPIP_EV_ST_CHG) {
    ASSERT(*(uint8_t *) ev_data == MG_TCPIP_STATE_READY);
    executed = true;
  }
  (void) ifp;
}

static void test_statechange(void) {
  struct mg_tcpip_if iface;
  memset(&iface, 0, sizeof(iface));
  iface.ip = mg_htonl(0x01020304);
  iface.state = MG_TCPIP_STATE_READY;
  iface.driver = &mg_tcpip_driver_mock;
  iface.fn = mif_fn;
  onstatechange(&iface);
  ASSERT(executed == true);
  executed = false;
}
#if MG_ENABLE_IPV6
static void mif6_fn(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  if (ev == MG_TCPIP_EV_ST6_CHG) {
    ASSERT(*(uint8_t *) ev_data == MG_TCPIP_STATE_READY);
    executed = true;
  }
  (void) ifp;
}

static void test_state6change(void) {
  struct mg_tcpip_if iface;
  memset(&iface, 0, sizeof(iface));
  iface.ip6[0] = (uint64_t) mg_htonl(0x01020304);
  iface.ip6[1] = (uint64_t) mg_htonl(0x05060708);
  iface.state6 = MG_TCPIP_STATE_READY;
  iface.driver = &mg_tcpip_driver_mock;
  iface.fn = mif6_fn;
  onstate6change(&iface);
  ASSERT(executed == true);
  executed = false;
}
#endif

static void ph(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_POLL) ++(*(int *) c->fn_data);
  (void) c, (void) ev_data;
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  (void) c, (void) ev, (void) ev_data;
}

static void tcpclosure_fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ACCEPT) c->is_draining = 1;
  (void) c, (void) ev_data;
}

static void client_fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ERROR || ev == MG_EV_CONNECT) (*(int *) c->fn_data) = ev;
  (void) c, (void) ev_data;
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
  driver_data->tx_ready = false; 
  return len;
}

static bool received_response(struct driver_data *driver) {
  bool was_ready = driver->tx_ready;
  driver->tx_ready = false;
  return was_ready;
}

static void create_tcp_seg(struct eth *e, struct ipp *ipp, uint32_t seq,
                           uint32_t ack, uint8_t flags, uint16_t sport,
                           uint16_t dport, size_t payload_len, void *opts,
                           unsigned int opts_len) {
  struct tcp t;
  memset(&t, 0, sizeof(struct tcp));
  t.flags = flags;
  t.seq = mg_htonl(seq);
  t.ack = mg_htonl(ack);
  t.sport = mg_htons(sport);
  t.dport = mg_htons(dport);
  t.off = (uint8_t) ((sizeof(t) / 4) << 4) + (uint8_t) ((opts_len / 4) << 4);
  memcpy(s_driver_data.buf, e, sizeof(*e));
#if MG_ENABLE_IPV6
  if (ipp->ip6 != NULL) {
    struct ip6 *ip = ipp->ip6;
    ip->plen = mg_htons((uint16_t) (4 * (t.off >> 4) + payload_len));
    memcpy(s_driver_data.buf + sizeof(*e), ip, sizeof(*ip));
    memcpy(s_driver_data.buf + sizeof(*e) + sizeof(*ip), &t, sizeof(t));
    if (opts != NULL && opts_len)
      memcpy(s_driver_data.buf + sizeof(*e) + sizeof(*ip) + sizeof(t), opts,
             opts_len);
    s_driver_data.len =
        sizeof(*e) + sizeof(*ip) + sizeof(t) + payload_len + opts_len;
  } else
#endif
  {
    struct ip *ip = ipp->ip4;
    ip->len =
        mg_htons((uint16_t) (sizeof(*ip) + 4 * (t.off >> 4) + payload_len));
    memcpy(s_driver_data.buf + sizeof(*e), ip, sizeof(*ip));
    memcpy(s_driver_data.buf + sizeof(*e) + sizeof(*ip), &t, sizeof(t));
    if (opts != NULL && opts_len)
      memcpy(s_driver_data.buf + sizeof(*e) + sizeof(*ip) + sizeof(t), opts,
             opts_len);
    s_driver_data.len =
        sizeof(*e) + sizeof(*ip) + sizeof(t) + payload_len + opts_len;
  }
  if (s_driver_data.len < 64) s_driver_data.len = 64;  // add padding if needed
}

static void create_tcp_simpleseg(struct eth *e, struct ipp *ipp, uint32_t seq,
                                 uint32_t ack, uint8_t flags,
                                 size_t payload_len) {
  // use sport=1 to ease seqno stuff, dport=80 due to init_tcp_tests() below
  create_tcp_seg(e, ipp, seq, ack, flags, 1, 80, payload_len, NULL, 0);
}

static void init_tests(struct mg_mgr *mgr, struct eth *e, struct ipp *ipp,
                       struct mg_tcpip_driver *driver, struct mg_tcpip_if *mif,
                       uint8_t proto) {
  mg_mgr_init(mgr);
  memset(mif, 0, sizeof(*mif));
  memset(&s_driver_data, 0, sizeof(struct driver_data));
  driver->init = NULL, driver->tx = if_tx, driver->poll = if_poll,
  driver->rx = if_rx;
  mif->driver = driver;
  mif->driver_data = &s_driver_data;
#if MG_ENABLE_IPV6
  if (ipp->ip6 != NULL) {
    mif->ip6[0] = 1;
    mif->gw6[0] = 1;
    mif->gw6_ready = true;
    mif->state = MG_TCPIP_STATE_READY;  // so DHCP stops
    mif->state6 = MG_TCPIP_STATE_READY;  // so mg_send() works and RS stops
  } else
#endif
  {
    mif->ip = 1;
    mif->gw = 1;
    mif->gw_ready = true;
    mif->mask = 255; // use router, to avoid firing an ARP request
    mif->state = MG_TCPIP_STATE_READY;  // so mg_send() works and DHCP stops
  }
  mg_tcpip_init(mgr, mif);

  // setting the Ethernet header
  memset(e, 0, sizeof(*e));
  memcpy(e->dst, mif->mac, 6 * sizeof(uint8_t));
  e->type = mg_htons(ipp->ip4 != NULL ? 0x800 : 0x86dd);

  // setting the IP header
#if MG_ENABLE_IPV6
  if (ipp->ip6 != NULL) {
    struct ip6 *ip = ipp->ip6;
    memset(ip, 0, sizeof(*ip));
    ip->ver = 0x60;
    ip->next = proto;
    // must be outside of Mongoose network to avoid firing NS requests
    ip->src[0] = 2;
    ip->dst[0] = mif->ip6[0];
  } else
#endif
  {
    struct ip *ip = ipp->ip4;
    memset(ip, 0, sizeof(*ip));
    ip->ver = (4 << 4) | 5;
    ip->proto = proto;
    // must be outside of Mongoose network to avoid firing ARP requests
    ip->src = 2;
    ip->dst = mif->ip;
  }
}

static void init_tcp_tests(struct mg_mgr *mgr, struct eth *e, struct ipp *ipp,
                           struct mg_tcpip_driver *driver,
                           struct mg_tcpip_if *mif, mg_event_handler_t f) {
  init_tests(mgr, e, ipp, driver, mif, 6);  // 6 -> TCP
#if MG_ENABLE_IPV6
  if (ipp->ip6 != NULL) {
    mg_http_listen(mgr, "http://[::]:80", f, NULL);
  } else
#endif
  {
    mg_http_listen(mgr, "http://0.0.0.0:80", f, NULL);
  }
  mgr->conns->pfn = NULL;  // HTTP handler not needed
  mg_mgr_poll(mgr, 0);
}

static void init_tcp_handshake(struct eth *e, struct ipp *ipp,
                               struct mg_mgr *mgr) {
  struct tcp *t =
      (struct tcp *) (s_driver_data.buf + sizeof(*e) +
                      (ipp->ip4 ? sizeof(struct ip) : sizeof(struct ip6)));

  // SYN
  create_tcp_simpleseg(e, ipp, 1000, 0, TH_SYN, 0);
  MG_VERBOSE(("SYN     -->"));
  mg_mgr_poll(mgr, 0);  // make sure we clean former stuff in buffer

  // SYN-ACK
  while (!received_response(&s_driver_data)) mg_mgr_poll(mgr, 0);
  ASSERT((t->flags == (TH_SYN | TH_ACK)));
  ASSERT((t->ack == mg_htonl(1001)));
  MG_VERBOSE(("SYN+ACK <--"));

  // ACK
  create_tcp_simpleseg(e, ipp, 1001, 2, TH_ACK, 0);
  MG_VERBOSE(("ACK     -->"));
  mg_mgr_poll(mgr, 0);  // this may have data on return !
}

// DHCP discovery works as a 1 second timeout, we take advantage of it
// (something is received within 1s) and we mask it when doing longer waits
// (verify received data is TCP by checking IP's protocol field)
static void test_tcp_basics(bool ipv6) {
  struct mg_mgr mgr;
  struct eth e;
  struct ip ip;
  struct ip6 ip6;
  struct ipp ipp;
  struct tcp *t = (struct tcp *) (s_driver_data.buf + sizeof(e) + (!ipv6 ? sizeof(ip) : sizeof(ip6)));
  struct ip *i = (struct ip *) (s_driver_data.buf + sizeof(e));
  struct ip6 *i6 = (struct ip6 *) (s_driver_data.buf + sizeof(e));
  uint64_t start, now;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  ipp.ip4 = !ipv6 ? &ip : NULL;
  ipp.ip6 = ipv6 ? &ip6 : NULL;

  init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, fn);

  // https://datatracker.ietf.org/doc/html/rfc9293#section-3.5.2	Reset
  // Generation non-used port. Group 1 in RFC send SYN, expect RST + ACK
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_SYN, 1, 69, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == (TH_RST | TH_ACK));
  ASSERT(t->seq == mg_htonl(0));
  ASSERT(t->ack == mg_htonl(1235));
  if (ipv6) {
    ASSERT(i6->src[0] == 1 && i6->dst[0] == 2);
  } else {
    ASSERT(i->src == 1 && i->dst == 2);
  }
  
  // send SYN+ACK, expect RST
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_SYN | TH_ACK, 1, 69, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == TH_RST);
  ASSERT(t->seq == mg_htonl(4321));
  // send data, expect RST + ACK
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_PUSH, 1, 69, 2, NULL, 0);
  mg_mgr_poll(&mgr, 0);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == (TH_RST | TH_ACK));
  ASSERT(t->seq == mg_htonl(0));
  ASSERT(t->ack == mg_htonl(1236));

  // send ACK, expect RST
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_ACK, 1, 69, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == TH_RST);
  ASSERT(t->seq == mg_htonl(4321));

  // send FIN, expect RST + ACK
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_FIN, 1, 69, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == (TH_RST | TH_ACK));  // Linux answers RST only
  ASSERT(t->seq == mg_htonl(0));
  ASSERT(t->ack == mg_htonl(1235));

  // send FIN+ACK, expect RST
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_FIN | TH_ACK, 1, 69, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == TH_RST);
  ASSERT(t->seq == mg_htonl(4321));

  // listening, non-connected port. Group 2 in RFC
  // send data, expect no response
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_PUSH, 1, 80, 2, NULL, 0);
  mg_mgr_poll(&mgr, 0);
  ASSERT(!received_response(&s_driver_data));

  // send ACK, expect RST
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_ACK, 1, 80, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == TH_RST);
  ASSERT(t->seq == mg_htonl(4321));
  if (ipv6) {
    ASSERT(i6->src[0] == 1 && i6->dst[0] == 2);
  } else {
    ASSERT(i->src == 1 && i->dst == 2);
  }

  // send SYN+ACK, expect RST
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_SYN | TH_ACK, 1, 80, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == TH_RST);
  ASSERT(t->seq == mg_htonl(4321));

  // send FIN, expect no response
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_FIN, 1, 80, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);
  ASSERT(!received_response(&s_driver_data));

  // send FIN+ACK, expect RST
  create_tcp_seg(&e, &ipp, 1234, 4321, TH_FIN | TH_ACK, 1, 80, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == TH_RST);
  ASSERT(t->seq == mg_htonl(4321));

  // we currently don't validate checksum, no silently discarded segment test

  init_tcp_handshake(&e, &ipp, &mgr);  // starts with seq_no=1000, ackno=2

  // no MSS sent, so it must default to 536/1220 (RFC-9293 3.7.1)
  ASSERT(((struct connstate *) (mgr.conns + 1))->dmss == (ipv6 ? 1220 : 536));

  // segment with seq_no within window
  create_tcp_simpleseg(&e, &ipp, 1010, 2, TH_PUSH, 2);
  mg_mgr_poll(&mgr, 0);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1001)));  // expecting 1001, dude
  if (ipv6) {
    ASSERT(i6->src[0] == 1 && i6->dst[0] == 2);
  } else {
    ASSERT(i->src == 1 && i->dst == 2);
  }

  // segment with seq_no way out of window
  create_tcp_simpleseg(&e, &ipp, 1000000, 2, TH_PUSH, 2);
  mg_mgr_poll(&mgr, 0);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1001)));  // expecting 1001, dude

  // Initiate closure, send FIN (test client-initiated closure)
  // https://datatracker.ietf.org/doc/html/rfc9293#section-3.6
  // We are case 1, Mongoose is case 2
  create_tcp_simpleseg(&e, &ipp, 1001, 2, TH_FIN, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  // Mongoose does a fast reduced ("3-way instead of 4-way" closure)
  ASSERT((t->flags == (TH_FIN | TH_ACK)));  // Mongoose ACKs our FIN, sends FIN
  ASSERT((t->seq == mg_htonl(2)));
  ASSERT((t->ack == mg_htonl(1002)));
  // make sure it is still open
  ASSERT(mgr.conns->next !=
         NULL);  // more than one connection: the listener + us
  create_tcp_simpleseg(&e, &ipp, 1002, 3, TH_ACK, 0);  // ACK Mongoose FIN
  mg_mgr_poll(&mgr, 0);
  ASSERT(!received_response(&s_driver_data));
  // make sure it is closed
  ASSERT(mgr.conns->next == NULL);  // only one connection: the listener

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);

  // Test client-initiated closure timeout, do not ACK
  init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, fn);
  init_tcp_handshake(&e, &ipp, &mgr);  // starts with seq_no=1000, ackno=2
  create_tcp_simpleseg(&e, &ipp, 1001, 2, TH_FIN, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  // Mongoose does a fast reduced ("3-way instead of 4-way" closure)
  ASSERT((t->flags == (TH_FIN | TH_ACK)));  // Mongoose ACKs our FIN, sends FIN
  ASSERT((t->seq == mg_htonl(2)));
  ASSERT((t->ack == mg_htonl(1002)));
  // make sure it is still open
  ASSERT(mgr.conns->next !=
         NULL);           // more than one connection: the listener + us
  s_driver_data.len = 0;  // avoid Mongoose "receiving itself"
  start = mg_millis();
  now = 0;
  do {
    mg_mgr_poll(&mgr, 0);
    if (received_response(&s_driver_data) && (ipv6 ? i6->next : i->proto) == 6)
      break;  // check first
    now = mg_millis() - start;
  } while (now < (12 * MIP_TCP_FIN_MS) / 10);
  ASSERT(now > MIP_TCP_FIN_MS);
  // make sure it is closed
  ASSERT(mgr.conns->next == NULL);  // only one connection: the listener

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);

  // Test server-initiated closure, abbreviated 3-way: respond FIN+ACK
  // https://datatracker.ietf.org/doc/html/rfc9293#section-3.6
  // We are case 2, Mongoose is case 1
  init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, tcpclosure_fn);
  init_tcp_handshake(&e, &ipp, &mgr);  // starts with seq_no=1000, ackno=2
  // we should have already received the FIN due to the call above
  start = mg_millis();
  while (!received_response(&s_driver_data)) {
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
    if (now > 2 * MIP_TCP_ACK_MS)
      ASSERT(0);  // response should have been received by now
  }
  ASSERT((t->seq == mg_htonl(2)));
  ASSERT((t->ack == mg_htonl(1001)));
  ASSERT(t->flags == (TH_FIN | TH_ACK));  // Mongoose ACKs last data, sends FIN
  // send FIN + ACK
  create_tcp_simpleseg(&e, &ipp, 1001, 3, TH_FIN | TH_ACK,
                       0);  // ACK FIN, send FIN
  mg_mgr_poll(&mgr, 0);     // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));  // Mongoose ACKs our FIN
  ASSERT((t->seq == mg_htonl(3)));
  ASSERT((t->ack == mg_htonl(1002)));
  // make sure it is closed
  ASSERT(mgr.conns->next == NULL);  // only one connection: the listener

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);

  // Test server-initiated closure, long 4-way closure: respond ACK
  init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, tcpclosure_fn);
  init_tcp_handshake(&e, &ipp, &mgr);  // starts with seq_no=1000, ackno=2
  // we should have already received the FIN, tested in above tst
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->seq == mg_htonl(2)));
  ASSERT((t->ack == mg_htonl(1001)));
  ASSERT(t->flags == (TH_FIN | TH_ACK));  // Mongoose ACKs last data, sends FIN
  // ACK Mongoose FIN, do *not* send FIN yet
  create_tcp_simpleseg(&e, &ipp, 1001, 3, TH_ACK, 0);  // ACK FIN
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  start = mg_millis();
  now = 0;
  do {
    if (received_response(&s_driver_data)) break;  // check first
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
  } while (now < 2 * MIP_TCP_ACK_MS);  // keep timeout below 1s (DHCP discover)
  ASSERT(now >= 2 * MIP_TCP_ACK_MS);
  // make sure it is still open
  ASSERT(mgr.conns->next !=
         NULL);  // more than one connection: the listener + us
  create_tcp_simpleseg(&e, &ipp, 1001, 3, TH_FIN, 0);  // send FIN
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));  // Mongoose ACKs our FIN
  ASSERT((t->seq == mg_htonl(3)));
  ASSERT((t->ack == mg_htonl(1002)));
  // make sure it is closed
  ASSERT(mgr.conns->next == NULL);  // only one connection: the listener

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);

  // Test server-initiated closure, FIN retransmission: do not ACK FIN
  // Actual data retransmission is tested on another unit test
  init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, tcpclosure_fn);
  init_tcp_handshake(&e, &ipp, &mgr);  // starts with seq_no=1000, ackno=2
  // we should have already received the FIN, tested in some tst above
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->seq == mg_htonl(2)));
  ASSERT((t->ack == mg_htonl(1001)));
  ASSERT(t->flags == (TH_FIN | TH_ACK));  // Mongoose ACKs last data, sends FIN
  s_driver_data.len = 0;                  // avoid Mongoose "receiving itself"
  start = mg_millis();
  now = 0;
  do {
    if (received_response(&s_driver_data)) break;  // check first
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
  } while (now < 2 * MIP_TCP_ACK_MS);  // keep timeout below 1s (DHCP discover)
  //  ASSERT(now < 2 * MIP_TCP_ACK_MS); ******** WE FAIL THIS, Mongoose does not
  //  retransmit, FIN is not an additional element in the stream ASSERT((t->seq
  //  == mg_htonl(2))); ASSERT((t->ack == mg_htonl(1001))); ASSERT(t->flags ==
  //  (TH_FIN | TH_ACK)); // Mongoose retransmits FIN
  // send FIN + ACK
  create_tcp_simpleseg(&e, &ipp, 1001, 3, TH_FIN | TH_ACK,
                       0);  // ACK FIN, send FIN
  mg_mgr_poll(&mgr, 0);     // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));  // Mongoose ACKs our FIN
  ASSERT((t->seq == mg_htonl(3)));
  ASSERT((t->ack == mg_htonl(1002)));
  // make sure it is closed
  ASSERT(mgr.conns->next == NULL);  // only one connection: the listener

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);

  // Test simultaneous closure
  // https://datatracker.ietf.org/doc/html/rfc9293#section-3.6 case 3
  init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, tcpclosure_fn);
  init_tcp_handshake(&e, &ipp, &mgr);  // starts with seq_no=1000, ackno=2
  // we should have already received the FIN due to the call above
  start = mg_millis();
  while (!received_response(&s_driver_data)) {
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
    if (now > 2 * MIP_TCP_ACK_MS)
      ASSERT(0);  // response should have been received by now
  }
  ASSERT((t->seq == mg_htonl(2)));
  ASSERT((t->ack == mg_htonl(1001)));
  ASSERT(t->flags == (TH_FIN | TH_ACK));  // Mongoose ACKs last data, sends FIN
  // Also initiate closure, send FIN, do *not* ACK Mongoose FIN
  create_tcp_simpleseg(&e, &ipp, 1001, 2, TH_FIN, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));  // Mongoose ACKs our FIN
  ASSERT((t->seq == mg_htonl(3)));
  ASSERT((t->ack == mg_htonl(1002)));
  // make sure it is still open   ******** WE FAIL THIS, Mongoose closes
  // immediately, does not wait to retransmit its ACK nor to get the other end
  // ACK
  //  ASSERT(mgr.conns->next != NULL);  // more than one connection: the
  //  listener + us create_tcp_simpleseg(&e, &ipp, 1002, 3, TH_ACK, 0); // ACK
  //  FIN mg_mgr_poll(&mgr, 0);
  // make sure it is closed
  ASSERT(mgr.conns->next == NULL);  // only one connection: the listener

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);

  // Test responses to a connecting client
  // https://datatracker.ietf.org/doc/html/rfc9293#section-3.5
  // NOTE: Mongoose ignores any data until connection is actually established
  // NOTE: Mongoose does not support the concept of "simultaneous open",
  // Mongoose is either client or server
  {
    struct mg_connection *c;
    int event = 255;
    uint32_t ackno;
    // this creates a listener we won't use
    init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, tcpclosure_fn);

    // must be outside of our network to avoid firing ARP requests  
    if (ipv6) {
      c = mg_connect(&mgr, "tcp://[200::]:1234/", client_fn, &event);
    } else {
      c = mg_connect(&mgr, "tcp://2.0.0.0:1234/", client_fn, &event);
    }
    ASSERT(c != NULL);
    ASSERT(received_response(&s_driver_data));
    ASSERT((t->flags == TH_SYN));
    ASSERT(event == 255);
    if (ipv6) {
      ASSERT(i6->src[0] == 1 && i6->dst[0] == 2);
    } else {
      ASSERT(i->src == 1 && i->dst == 2);
    }

    // invalid SYN + ACK to connecting client (after SYN...), send ACK != seq
    ackno = mg_ntohl(t->seq) + 1000;
    //  create_tcp_seg(&e, &ipp, 4321, ackno, TH_SYN | TH_ACK, 1234,
    //  mg_ntohs(c->loc.port), 0, NULL, 0); mg_mgr_poll(&mgr, 0); 
    //  while(!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
    //  ASSERT((t->flags == (TH_RST | TH_ACK)));
    // ******** WE FAIL THIS, Mongoose does not validate the ACK number
    //  ASSERT((t->seq == mg_htonl(ackno)));
    //  ASSERT((t->ack == mg_htonl(4322)));

    // connect
    ackno = mg_ntohl(t->seq) + 1;
    create_tcp_seg(&e, &ipp, 4321, ackno, TH_SYN | TH_ACK, 1234,
                   mg_ntohs(c->loc.port), 0, NULL, 0);
    mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
    while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
    ASSERT(t->flags == TH_ACK);
    ASSERT(t->seq == mg_htonl(ackno));
    ASSERT((t->ack == mg_htonl(4322)));
    ASSERT(event == MG_EV_CONNECT);

    event = 255;
    s_driver_data.len = 0;
    mg_mgr_free(&mgr);

    // test connection failure, send RST+ACK
    // this creates a listener we won't use
    init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, tcpclosure_fn);
    if (ipv6) {
      c = mg_connect(&mgr, "tcp://[200::]:1234/", client_fn, &event);
    } else {
      c = mg_connect(&mgr, "tcp://2.0.0.0:1234/", client_fn, &event);
    }
    received_response(&s_driver_data);  // get the SYN
    ackno = mg_ntohl(t->seq) + 1;
    create_tcp_seg(&e, &ipp, 4321, ackno, TH_RST + TH_ACK, 1234,
                   mg_ntohs(c->loc.port), 0, NULL, 0);
    mg_mgr_poll(&mgr, 0);
    MG_DEBUG(("event: %d", event));
    ASSERT(event == MG_EV_ERROR);
    ASSERT(!received_response(&s_driver_data));
  }

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
}

// NOTE: a 1-byte payload could be an erroneous Keep-Alive, keep length > 1 in
// this operation, we're testing retransmissions and having len=1 won't work

static void test_tcp_retransmit(void) {
  struct mg_mgr mgr;
  struct eth e;
  struct ip ip;
  struct ipp ipp;
  struct tcp *t = (struct tcp *) (s_driver_data.buf + sizeof(e) + sizeof(ip));
  uint64_t start, now;
  bool response_recv = true;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  ipp.ip4 = &ip;
  ipp.ip6 = NULL;

  init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, fn);

  init_tcp_handshake(&e, &ipp, &mgr);  // starts with seq_no=1000, ackno=2

  // packet with seq_no = 1001
  create_tcp_simpleseg(&e, &ipp, 1001, 2, TH_PUSH | TH_ACK, 2);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1003)));  // OK

  // resend packet with seq_no = 1001 (e.g.: MIP ACK lost)
  create_tcp_simpleseg(&e, &ipp, 1001, 2, TH_PUSH | TH_ACK, 2);
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
  create_tcp_simpleseg(&e, &ipp, 1005, 2, TH_PUSH | TH_ACK, 2);
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
  create_tcp_simpleseg(&e, &ipp, 1003, 2, TH_PUSH | TH_ACK, 2);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1005)));  // OK

  // packet with seq_no = 1005 got delayed, send FIN with seq_no = 1007
  create_tcp_simpleseg(&e, &ipp, 1007, 2, TH_FIN, 0);
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
  create_tcp_simpleseg(&e, &ipp, 1005, 2, TH_PUSH | TH_ACK, 2);
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT((t->flags == TH_ACK));
  ASSERT((t->ack == mg_htonl(1007)));  // OK

  // retransmitting FIN packet with seq_no = 1007
  create_tcp_simpleseg(&e, &ipp, 1007, 2, TH_FIN | TH_ACK, 0);
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
  struct ipp ipp;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  ipp.ip4 = &ip;
  ipp.ip6 = NULL;

  init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, frag_recv_fn);

  init_tcp_handshake(&e, &ipp, &mgr);  // starts with seq_no=1000, ackno=2

  // send fragmented TCP packet
  ip.frag |= IP_MORE_FRAGS_MSK;  // setting More Fragments bit to 1
  create_tcp_simpleseg(&e, &ipp, 1001, 2, TH_PUSH | TH_ACK, 1000);
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
  struct ipp ipp;
  struct tcp *t = (struct tcp *) (s_driver_data.buf + sizeof(e) + sizeof(ip));
  struct ip *i = (struct ip *) (s_driver_data.buf + sizeof(e));
  uint64_t start, now;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;
  uint16_t opts[4 / 2];  // Send MSS, RFC-9293 3.7.1
  struct mg_connection *c;
  unsigned int j;
#define LOGSZ (sizeof(c->data) / sizeof(struct mg_backlog))
  uint32_t seqnos[LOGSZ];

  ipp.ip4 = &ip;
  ipp.ip6 = NULL;

  init_tcp_tests(&mgr, &e, &ipp, &driver, &mif, fn);

  // test expired connection attempts cleanup
  create_tcp_seg(&e, &ipp, 1234, 0, TH_SYN, 1, 80, 0, NULL, 0);
  mg_mgr_poll(&mgr, 0);  // make sure we clean former stuff in buffer
  while (!received_response(&s_driver_data)) mg_mgr_poll(&mgr, 0);
  ASSERT(t->flags == (TH_SYN | TH_ACK));
  // delay ACK so conn attempt is removed from the backlog
  s_driver_data.len = 0;  // avoid Mongoose "receiving itself"
  start = mg_millis();
  do {
    mg_mgr_poll(&mgr, 0);
    now = mg_millis() - start;
  } while (now < 2100);
  // check backlog is empty
  c = mgr.conns;
  ASSERT(c->next == NULL);
  for (j = 0; j < LOGSZ; j++) {
    struct mg_backlog *b = (struct mg_backlog *) (c->data) + j;
    ASSERT(b->port == 0);
  }
  // Mongoose may have retransmitted SYN + ACK, and DHCP sent discover
  received_response(&s_driver_data);  // make sure we clean buffer

  opts[0] = mg_htons(0x0204);  // RFC-9293 3.2
  // fill the backlog
  for (j = 0; j < LOGSZ; j++) {
    // assign one MSS for each connection
    opts[1] = mg_htons((uint16_t) (1010 + j));
    create_tcp_seg(&e, &ipp, 100 + j, 0, TH_SYN, (uint16_t) (j + 1), 80, 0,
                   opts, sizeof(opts));
    while (!received_response(&s_driver_data) || i->proto != 6)
      mg_mgr_poll(&mgr, 0);
    ASSERT(t->flags == (TH_SYN | TH_ACK));
    seqnos[j] = ntohl(t->seq);
    MG_VERBOSE(("SEQ: %p", seqnos[j]));
  }
  // check backlog is full and MSS are there
  c = mgr.conns;
  for (j = 0; j < LOGSZ; j++) {
    struct mg_backlog *b = (struct mg_backlog *) (c->data) + j;
    ASSERT(b->port != 0);
    MG_DEBUG(("SEQ: %p, MSS: %u", seqnos[j], (unsigned int) b->mss));
    ASSERT(b->mss == (1010 + j));
  }
  // one more attempt, it must fail
  opts[1] = mg_htons((uint16_t) (1010 + j));
  create_tcp_seg(&e, &ipp, 100 + j, 0, TH_SYN, (uint16_t) (j + 1), 80, 0, opts,
                 sizeof(opts));
  mg_mgr_poll(&mgr, 0);
  ASSERT(!received_response(&s_driver_data) || i->proto != 6);
  // a late response for this attempt would break what follows
  // establish all connections
  for (j = 0; j < LOGSZ; j++) {
    create_tcp_seg(&e, &ipp, 100 + j + 1, seqnos[j] + 1, TH_ACK,
                   (uint16_t) (j + 1), 80, 0, NULL, 0);
    mg_mgr_poll(&mgr, 0);
    ASSERT(!received_response(&s_driver_data) || i->proto != 6);
  }
  // check backlog is now empty
  c = mgr.conns;  // last one is the listener
  for (; c->next != NULL; c = c->next)
    ;
  for (j = 0; j < LOGSZ; j++) {
    struct mg_backlog *b = (struct mg_backlog *) (c->data) + j;
    ASSERT(b->port == 0);
  }
  c = mgr.conns;  // first one is more recent
  // check MSS is what we sent, everything's fine
  for (j = LOGSZ; j > 0; j--, c = c->next) {
    struct connstate *s = (struct connstate *) (c + 1);
    ASSERT(c != NULL);
    MG_DEBUG(("MSS: %u", (unsigned int) s->dmss));
    ASSERT(s->dmss == (1010 + j - 1));
  }
  ASSERT(c != NULL);  // last one is the listener
  ASSERT(c->next == NULL);

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
}

static void test_tcp(bool ipv6) {
  test_tcp_basics(ipv6);
  if(!ipv6) {
    test_tcp_backlog();
    test_tcp_retransmit();
  }
}

static void udp_fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_READ && c->recv.len == 2 && c->recv.buf[0] == 'p')
    mg_send(c, "P90", 3);
  (void) ev_data;
}

static void create_udp_dat(struct eth *e, struct ipp *ipp, uint16_t sport,
                           uint16_t dport, size_t payload_len) {
  struct udp u;
  memset(&u, 0, sizeof(struct udp));
  u.sport = mg_htons(sport);
  u.dport = mg_htons(dport);
  u.len = mg_htons((uint16_t) (sizeof(u) + payload_len));
  memcpy(s_driver_data.buf, e, sizeof(*e));
#if MG_ENABLE_IPV6
  if (ipp->ip6 != NULL) {
    struct ip6 *ip = ipp->ip6;
    ip->plen = mg_htons((uint16_t) (sizeof(u) + payload_len));
    memcpy(s_driver_data.buf + sizeof(*e), ip, sizeof(*ip));
    memcpy(s_driver_data.buf + sizeof(*e) + sizeof(*ip), &u, sizeof(u));
    *(s_driver_data.buf + sizeof(*e) + sizeof(*ip) + sizeof(u)) = 'p';
    s_driver_data.len = sizeof(*e) + sizeof(*ip) + sizeof(u) + payload_len;
  } else
#endif
  {
    struct ip *ip = ipp->ip4;
    ip->len = mg_htons((uint16_t) (sizeof(*ip) + sizeof(u) + payload_len));
    memcpy(s_driver_data.buf + sizeof(*e), ip, sizeof(*ip));
    memcpy(s_driver_data.buf + sizeof(*e) + sizeof(*ip), &u, sizeof(u));
    *(s_driver_data.buf + sizeof(*e) + sizeof(*ip) + sizeof(u)) = 'p';
    s_driver_data.len = sizeof(*e) + sizeof(*ip) + sizeof(u) + payload_len;
  }
  if (s_driver_data.len < 64) s_driver_data.len = 64;  // add padding if needed
}

static void init_udp_tests(struct mg_mgr *mgr, struct eth *e, struct ipp *ipp,
                           struct mg_tcpip_driver *driver,
                           struct mg_tcpip_if *mif, mg_event_handler_t f) {
  init_tests(mgr, e, ipp, driver, mif, 17);  // 17 -> UDP
#if MG_ENABLE_IPV6
  if (ipp->ip6 != NULL) {
    mif->state = MG_TCPIP_STATE_READY;  // so DHCP stops
    mif->state6 = MG_TCPIP_STATE_READY;  // so mg_send() works and RS stops
    mg_listen(mgr, "udp://[::]:888", f, NULL);
  } else
#endif
  {
    mif->state = MG_TCPIP_STATE_READY;  // so mg_send() works and DHCP stops
    mg_listen(mgr, "udp://0.0.0.0:888", f, NULL);
  }
  mg_mgr_poll(mgr, 0);
}

static void test_udp(bool ipv6) {
  struct mg_mgr mgr;
  struct eth e;
  struct ip ip;
  struct ip6 ip6;
  struct ipp ipp;
  struct udp *u = (struct udp *) (s_driver_data.buf + sizeof(e) + (!ipv6 ? sizeof(ip) : sizeof(ip6)));
  struct ip *i = (struct ip *) (s_driver_data.buf + sizeof(e));
  struct ip6 *i6 = (struct ip6 *) (s_driver_data.buf + sizeof(e));
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  ipp.ip4 = !ipv6 ? &ip : NULL;
  ipp.ip6 = ipv6 ? &ip6 : NULL;

  init_udp_tests(&mgr, &e, &ipp, &driver, &mif, udp_fn);
  received_response(&s_driver_data);
  s_driver_data.len = 0;

  // send data to a non-open port, expect no response (we don't send Destination
  // Unreachable)
  create_udp_dat(&e, &ipp, 1, 800, 2);
  mg_mgr_poll(&mgr, 0);
  ASSERT(!received_response(&s_driver_data));

  // send data to an open port, expect response
  create_udp_dat(&e, &ipp, 1, 888, 2);
  mg_mgr_poll(&mgr, 0);
  ASSERT(received_response(&s_driver_data));
  ASSERT(u->sport == mg_htons(888));
  ASSERT(u->len == mg_htons(sizeof(*u) + 3));
  ASSERT(*((char *) (u + 1)) == 'P');
  if (ipv6) {
    ASSERT(i6->src[0] == 1 && i6->dst[0] == 2);
  } else {
    ASSERT(i->src == 1 && i->dst == 2);
  }

  s_driver_data.len = 0;
  mg_mgr_free(&mgr);
}


#define DASHBOARD(x) \
  printf("HEALTH_DASHBOARD\t\"%s\": %s,\n", x, s_error ? "false" : "true");

int main(void) {
  s_error = false;
  test_csum();
  DASHBOARD("checksum");

  s_error = false;
  test_statechange();
  DASHBOARD("statechange");

  s_error = false;
  test_poll();
  DASHBOARD("poll");

  s_error = false;
  test_tcp(false);
  DASHBOARD("tcp");

  s_error = false;
  test_udp(false);
  DASHBOARD("udp");

#if MG_ENABLE_IPV6
  s_error = false;
  test_state6change();
  DASHBOARD("state6change");

  s_error = false;
  test_tcp(true);
  DASHBOARD("tcp_ipv6");
  
  s_error = false;
  test_udp(true);
  DASHBOARD("udp_ipv6");

#endif

  s_error = false;
  test_fragmentation();
  printf("HEALTH_DASHBOARD\t\"ipfrag\": %s\n", s_error ? "false" : "true");
  // last entry with no comma

#ifdef NO_ABORT
  if (s_abort != 0) return EXIT_FAILURE;
#endif

  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return EXIT_SUCCESS;
}
