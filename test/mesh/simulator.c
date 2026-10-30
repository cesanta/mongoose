// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Radio mesh simulator. Bridges and nodes are stations at positions in an
// area. Time is virtual: see mg_millis(). On the LAN: a DHCP server, and a
// test station that sends HTTP requests to the nodes

#include "simulator.h"

#define STEP_MICROSECONDS 250  // Virtual time per main loop step, see README.md

static struct simulator s_sim;  // The simulator

// Mongoose's clock: virtual time. A minute of network traffic takes far less
// than a minute to simulate
uint64_t mg_millis(void) {
  return s_sim.now / 1000;
}

// Parse "X,Y,Z", or "X,Y" with Z 0. Return false if it is not that
static bool parse_pos(const char *s, struct pos *p) {
  int n = 0;  // Characters parsed
  p->x = p->y = p->z = 0;
  return (sscanf(s, "%lf,%lf,%lf%n", &p->x, &p->y, &p->z, &n) == 3 ||
          sscanf(s, "%lf,%lf%n", &p->x, &p->y, &n) == 2) &&
         s[n] == 0;
}

// Is position p inside the area of sim
static bool inside(const struct simulator *sim, const struct pos *p) {
  return p->x >= 0 && p->x <= sim->area.x && p->y >= 0 && p->y <= sim->area.y &&
         p->z >= 0 && p->z <= sim->area.z;
}

static void usage(const struct simulator *sim, const char *prog) {
  fprintf(stderr,
          "Usage: %s OPTIONS\n"
          "Positions: X,Y,Z in metres, or X,Y with Z 0\n"
          "  -area X,Y,Z    - area, a box, default: %g,%g,%g\n"
          "  -bridge X,Y,Z  - a bridge; repeat for more\n"
          "  -node X,Y,Z    - a radio node; repeat for more\n"
          "  -t SEC         - simulated time, seconds, default: %g\n"
          "  -w SEC         - mesh settle time before requests, default: %g\n",
          prog, sim->area.x, sim->area.y, sim->area.z,
          (double) sim->duration / 1e6, (double) sim->tester.wait / 1e6);
  exit(EXIT_FAILURE);
}

// The next radio MAC, to mac: 02:00:00:xx:xx:xx, numbered from 1
static void next_mac(struct simulator *sim, uint8_t *mac) {
  uint32_t n = ++sim->last_mac;
  mac[0] = 2, mac[1] = mac[2] = 0, mac[3] = (uint8_t) (n >> 16);
  mac[4] = (uint8_t) (n >> 8), mac[5] = (uint8_t) n;
}

// Add a bridge at position s. Return false if s is not a position
static bool add_bridge(struct simulator *sim, const char *s) {
  struct bridge *b = (struct bridge *) calloc(1, sizeof(*b));
  if (b == NULL || !parse_pos(s, &b->pos)) return free(b), false;
  next_mac(sim, b->rif.mac);
  LIST_ADD_TAIL(struct bridge, &sim->bridges, b);
  sim->num_bridges++;
  return true;
}

// Add a radio node at position s. Return false if s is not a position
static bool add_node(struct simulator *sim, const char *s) {
  struct radio_node *n = (struct radio_node *) calloc(1, sizeof(*n));
  if (n == NULL || !parse_pos(s, &n->pos)) return free(n), false;
  next_mac(sim, n->rif.mac);
  LIST_ADD_TAIL(struct radio_node, &sim->nodes, n);
  sim->num_nodes++;
  return true;
}

// Exit if position x is outside the area of sim
static void check_inside(const struct simulator *sim, const struct pos *x) {
  if (!inside(sim, x)) {
    fprintf(stderr, "%g,%g,%g is outside the area\n", x->x, x->y, x->z);
    exit(EXIT_FAILURE);
  }
}

// Check that all bridges and nodes are inside the area of sim
static void check_area(const struct simulator *sim) {
  struct bridge *b;
  struct radio_node *n;
  for (b = sim->bridges; b != NULL; b = b->next) check_inside(sim, &b->pos);
  for (n = sim->nodes; n != NULL; n = n->next) check_inside(sim, &n->pos);
}

#define LAN_QUEUE_SIZE (64 * 1024)  // Receive queue of a LAN interface

// Put frame buf, len bytes, into the receive queue of LAN interface to, if
// it is not the sender, from
static void lan_put(struct mg_tcpip_if *to, struct mg_tcpip_if *from,
                    const void *buf, size_t len) {
  if (to != from) mg_tcpip_qwrite((void *) buf, len, to);
}

// LAN driver, all LAN interfaces share it. The LAN is infinitely fast: tx
// puts the frame straight into the receive queues of the interfaces it is
// for: all, if broadcast; else the one with the destination MAC. Bridges get
// every frame: some are for their radio nodes. Each station takes frames
// from its queue when polled, see dhcp_poll(), bridges_poll(); Mongoose does
// it for the test station
static size_t lan_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  const uint8_t *dst = (const uint8_t *) buf;
  bool all = dst[0] & 1;  // Broadcast, multicast
  struct bridge *b;
  if (all || memcmp(dst, s_sim.dhcp.lif.mac, 6) == 0) {
    lan_put(&s_sim.dhcp.lif, ifp, buf, len);
  }
  if (all || memcmp(dst, s_sim.tester.lif.mac, 6) == 0) {
    lan_put(&s_sim.tester.lif, ifp, buf, len);
  }
  for (b = s_sim.bridges; b != NULL; b = b->next) {
    lan_put(&b->lif, ifp, buf, len);
  }
  return len;
}

static bool lan_init(struct mg_tcpip_if *ifp) {
  (void) ifp;
  return true;
}

static bool lan_link(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp, (void) s1;
  return true;
}

static struct mg_tcpip_driver s_lan_driver = {lan_init, lan_tx, NULL, lan_link};

// Internet checksum of len bytes
static uint16_t ip_csum(const uint8_t *buf, size_t len) {
  uint32_t sum = 0;
  size_t i;
  for (i = 0; i + 1 < len; i += 2) sum += (uint32_t) (buf[i] << 8 | buf[i + 1]);
  if (len & 1) sum += (uint32_t) (buf[len - 1] << 8);
  while (sum >> 16) sum = (sum & 0xffff) + (sum >> 16);
  return (uint16_t) ~sum;
}

// A node's address, from its MAC 02:00:00:X:Y:Z: 10.X+1.Y.Z, so it never
// clashes with the LAN stations' 10.0.0.x
static void node_ip(const uint8_t *mac, uint8_t *ip) {
  ip[0] = 10, ip[1] = (uint8_t) (mac[3] + 1), ip[2] = mac[4], ip[3] = mac[5];
}

// DHCP server, a frame from its queue: a DISCOVER gets an OFFER, a REQUEST
// an ACK. Replies are broadcast. The address comes from the client's MAC
static void dhcp_rx(struct simulator *sim, const uint8_t *buf, size_t len) {
  static const uint8_t opts[] = {53, 1, 0,  // Message type, set below
                                 1,  4, 255, 0, 0,  0,   // Mask
                                 3,  4, 10,  0, 0,  1,   // Router
                                 54, 4, 10,  0, 0,  1,   // Server
                                 51, 4, 0,   0, 14, 16,  // Lease, 3600 s
                                 255};
  uint8_t r[14 + 20 + 8 + 240 + sizeof(opts)], type = 0;
  const uint8_t *dh, *p, *end = buf + len;
  size_t ihl, rlen = sizeof(r);
  if ((buf[0] & 1) == 0) return;  // Bcast
  if (len < 34 || buf[12] != 8 || buf[13] != 0 || buf[23] != 17) return;  // UDP
  ihl = (size_t) (buf[14] & 15) * 4, dh = buf + 14 + ihl + 8;  // DHCP
  if (dh + 240 > end || buf[14 + ihl + 2] != 0 || buf[14 + ihl + 3] != 67) {
    return;
  }
  for (p = dh + 240; p + 2 <= end && p[0] != 255; p += p[0] ? p[1] + 2 : 1) {
    if (p[0] == 53 && p[1] == 1 && p + 3 <= end) type = p[2];
  }
  if (type != 1 && type != 3) return;  // DISCOVER, REQUEST
  memset(r, 0, sizeof(r));
  memset(r, 0xff, 6), memcpy(r + 6, sim->dhcp.lif.mac, 6),
      r[12] = 8;                                       // Ethernet
  r[14] = 0x45, r[16] = (uint8_t) ((rlen - 14) >> 8);  // IPv4
  r[17] = (uint8_t) (rlen - 14), r[22] = 64, r[23] = 17;
  r[26] = 10, r[29] = 1, memset(r + 30, 0xff, 4);  // 10.0.0.1 -> bcast
  r[24] = (uint8_t) (ip_csum(r + 14, 20) >> 8);
  r[25] = (uint8_t) ip_csum(r + 14, 20);
  r[35] = 67, r[37] = 68;  // UDP, csum 0: none
  r[38] = (uint8_t) ((rlen - 34) >> 8), r[39] = (uint8_t) (rlen - 34);
  r[42] = 2, r[43] = 1, r[44] = 6;                      // DHCP: reply, Ethernet
  memcpy(r + 46, dh + 4, 4);                            // xid
  memcpy(r + 52, dh + 10, 2);                           // flags
  node_ip(dh + 28, r + 58);                             // yiaddr
  r[62] = 10, r[65] = 1;                                // siaddr
  memcpy(r + 70, dh + 28, 16);                          // chaddr
  r[278] = 99, r[279] = 130, r[280] = 83, r[281] = 99;  // Magic
  memcpy(r + 282, opts, sizeof(opts));
  r[284] = type == 1 ? 2 : 5;  // OFFER, ACK
  sim->dhcp.lif.driver->tx(r, rlen, &sim->dhcp.lif);
}

// DHCP server's work: take the frames from its queue
static void dhcp_poll(struct simulator *sim) {
  char *buf;
  size_t len;
  while ((len = mg_queue_next(&sim->dhcp.lif.recv_queue, &buf)) > 0) {
    dhcp_rx(sim, (const uint8_t *) buf, len);
    mg_queue_del(&sim->dhcp.lif.recv_queue, len);
  }
}

// The request on the way finished: ok, or failed
static void tester_done(struct lan_tester *t, uint64_t now, bool ok) {
  double ms = (double) (now - t->start) / 1000;
  if (ok) {
    t->ok++, t->sum += ms;
    if (t->ok == 1 || ms < t->min) t->min = ms;
    if (ms > t->max) t->max = ms;
  } else {
    t->failed++;
  }
  if (t->c != NULL) t->c->is_closing = 1, t->c->fn_data = NULL;
  t->c = NULL;
}

// HTTP client: GET /, the reply ends the request
static void tester_fn(struct mg_connection *c, int ev, void *ev_data) {
  struct lan_tester *t = (struct lan_tester *) c->fn_data;
  if (t == NULL) return;  // Done with
  if (ev == MG_EV_CONNECT) {
    mg_printf(c, "GET / HTTP/1.0\r\n\r\n");
  } else if (ev == MG_EV_HTTP_MSG) {
    tester_done(t, s_sim.now, true);
  } else if (ev == MG_EV_ERROR || ev == MG_EV_CLOSE) {
    tester_done(t, s_sim.now, false);
  }
  (void) ev_data;
}

// Test station's work: Mongoose, then requests, one at a time, once the mesh
// had time to settle. A node without an IP address fails at once
static void tester_poll(struct simulator *sim) {
  struct radio_node *n;
  char url[64];
  mg_mgr_poll(&sim->tester.mgr, 0);
  if (sim->tester.c != NULL && sim->now - sim->tester.start >
                                   (uint64_t) sim->request_timeout_ms * 1000) {
    tester_done(&sim->tester, sim->now, false);
  }
  if (sim->now < sim->tester.wait || sim->tester.c != NULL ||
      sim->tester.next == NULL)
    return;
  n = sim->tester.next;
  sim->tester.next = n->next, sim->tester.start = sim->now;
  if (n->rif.ip == 0) {
    tester_done(&sim->tester, sim->now, false);
    return;
  }
  mg_snprintf(url, sizeof(url), "http://%M/", mg_print_ip4, &n->rif.ip);
  if ((sim->tester.c = mg_http_connect(&sim->tester.mgr, url, tester_fn,
                                       &sim->tester)) == NULL) {
    tester_done(&sim->tester, sim->now, false);
  }
}

// Set up sim: defaults, LAN stations
static void init(struct simulator *sim) {
  static const uint8_t dmac[6] = {2, 1, 0, 0, 0, 0},  // DHCP server MAC
      tmac[6] = {2, 2, 0, 0, 0, 0};                   // Test station MAC
  memset(sim, 0, sizeof(*sim));
  sim->area.x = sim->area.y = 100;
  sim->duration = 600 * 1000000ULL, sim->tester.wait = 60 * 1000000ULL;
  sim->request_timeout_ms = 10000;
  memcpy(sim->dhcp.lif.mac, dmac, 6), sim->dhcp.lif.driver = &s_lan_driver;
  mg_queue_init(&sim->dhcp.lif.recv_queue, (char *) calloc(1, LAN_QUEUE_SIZE),
                LAN_QUEUE_SIZE);
  memcpy(sim->tester.lif.mac, tmac, 6);
  sim->tester.lif.ip = MG_IPV4(10, 0, 0, 2),
  sim->tester.lif.mask = MG_IPV4(255, 0, 0, 0);
  sim->tester.lif.driver = &s_lan_driver,
  sim->tester.lif.enable_mac_check = true;
  sim->tester.lif.recv_queue.size = 128 * 1024;
}

// Radio driver, all radio interfaces share it. An ideal radio for now:
// whole Ethernet frames, no range, no loss, no airtime. It works like the
// LAN, see lan_tx(): a frame goes into the receive queues of all other
// radio interfaces if broadcast, else of the one with the destination MAC.
// Bridges get every frame: some are for the LAN. Who forwards what, and
// loops with several bridges, are for mg_mesh
static size_t radio_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  const uint8_t *dst = (const uint8_t *) buf;
  bool all = dst[0] & 1;  // Broadcast, multicast
  struct bridge *b;
  struct radio_node *n;
  for (b = s_sim.bridges; b != NULL; b = b->next) {
    lan_put(&b->rif, ifp, buf, len);
  }
  for (n = s_sim.nodes; n != NULL; n = n->next) {
    if (all || memcmp(dst, n->rif.mac, 6) == 0) {
      lan_put(&n->rif, ifp, buf, len);
    }
  }
  return len;
}

static bool radio_init(struct mg_tcpip_if *ifp) {
  (void) ifp;
  return true;
}

static bool radio_link(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp, (void) s1;
  return true;
}

static struct mg_tcpip_driver s_radio_driver = {radio_init, radio_tx, NULL,
                                                radio_link};

// A node's HTTP server: the test station requests GET /
static void node_http_ev_handler(struct mg_connection *c, int ev,
                                 void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "Hi from Mongoose, my MAC is %M\n", mg_print_mac,
                  c->fn_data);
  }
  (void) ev_data;
}

// Start each node's Mongoose: its radio interface, no IP address, so DHCP,
// and an HTTP server
static void nodes_init(struct simulator *sim) {
  struct radio_node *n;
  for (n = sim->nodes; n != NULL; n = n->next) {
    n->rif.driver = &s_radio_driver;
    n->rif.recv_queue.size = LAN_QUEUE_SIZE;
    mg_mgr_init(&n->mgr);
    mg_tcpip_init(&n->mgr, &n->rif);
    mg_http_listen(&n->mgr, "http://0.0.0.0:80", node_http_ev_handler,
                   n->rif.mac);
  }
}

// Start each bridge: its LAN and radio drivers. No Mongoose, no IP
static void bridges_init(struct simulator *sim) {
  struct bridge *b;
  for (b = sim->bridges; b != NULL; b = b->next) {
    b->lif.driver = &s_lan_driver, b->rif.driver = &s_radio_driver;
    mg_queue_init(&b->lif.recv_queue, (char *) calloc(1, LAN_QUEUE_SIZE),
                  LAN_QUEUE_SIZE);
    mg_queue_init(&b->rif.recv_queue, (char *) calloc(1, LAN_QUEUE_SIZE),
                  LAN_QUEUE_SIZE);
    b->lif.driver->init(&b->lif), b->rif.driver->init(&b->rif);
  }
}

// Move the frames waiting in the receive queue of interface from out of
// interface to
static void bridge_forward(struct mg_tcpip_if *from, struct mg_tcpip_if *to) {
  char *buf;
  size_t len;
  while ((len = mg_queue_next(&from->recv_queue, &buf)) > 0) {
    // MG_DEBUG(("Bridging ETH frame, size %zu", len));
    to->driver->tx(buf, len, to);
    mg_queue_del(&from->recv_queue, len);
  }
}

// Bridges' work: forward every frame, LAN to radio and radio to LAN. No
// filtering yet
static void bridges_poll(struct simulator *sim) {
  struct bridge *b;
  for (b = sim->bridges; b != NULL; b = b->next) {
    bridge_forward(&b->lif, &b->rif);
    bridge_forward(&b->rif, &b->lif);
  }
}

// Poll each node's Mongoose
static void nodes_poll(struct simulator *sim) {
  struct radio_node *n;
  for (n = sim->nodes; n != NULL; n = n->next) mg_mgr_poll(&n->mgr, 0);
}

int main(int argc, char *argv[]) {
  clock_t started;  // CPU time, to show how fast the simulation runs
  int i;

  init(&s_sim);
  for (i = 1; i < argc; i++) {
    if (i + 1 >= argc) {
      usage(&s_sim, argv[0]);
    } else if (strcmp(argv[i], "-area") == 0) {
      if (!parse_pos(argv[++i], &s_sim.area)) usage(&s_sim, argv[0]);
    } else if (strcmp(argv[i], "-bridge") == 0) {
      if (!add_bridge(&s_sim, argv[++i])) usage(&s_sim, argv[0]);
    } else if (strcmp(argv[i], "-node") == 0) {
      if (!add_node(&s_sim, argv[++i])) usage(&s_sim, argv[0]);
    } else if (strcmp(argv[i], "-t") == 0) {
      s_sim.duration = (uint64_t) (atof(argv[++i]) * 1e6);
    } else if (strcmp(argv[i], "-w") == 0) {
      s_sim.tester.wait = (uint64_t) (atof(argv[++i]) * 1e6);
    } else {
      usage(&s_sim, argv[0]);
    }
  }

  printf("Area %gx%gx%g m, radio %s, mesh %s, bridges: %d, nodes: %d\n",
         s_sim.area.x, s_sim.area.y, s_sim.area.z, mg_radio_802154.name,
         mg_mesh_star.name, s_sim.num_bridges, s_sim.num_nodes);
  check_area(&s_sim);

  // mg_log_set(MG_LL_INFO);

  mg_mgr_init(&s_sim.tester.mgr);
  mg_tcpip_init(&s_sim.tester.mgr, &s_sim.tester.lif);
  s_sim.tester.next = s_sim.nodes;
  bridges_init(&s_sim);
  nodes_init(&s_sim);

  // Main loop: STEP_MICROSECONDS of virtual time per step
  started = clock();
  while (s_sim.now < s_sim.duration) {
    dhcp_poll(&s_sim);
    tester_poll(&s_sim);
    bridges_poll(&s_sim);
    nodes_poll(&s_sim);
    s_sim.now += STEP_MICROSECONDS;
  }
  printf("Requests: %d ok, %d failed, of %d nodes", s_sim.tester.ok,
         s_sim.tester.failed, s_sim.num_nodes);
  if (s_sim.tester.ok > 0) {
    printf("; ms: min %.1f, mean %.1f, max %.1f", s_sim.tester.min,
           s_sim.tester.sum / s_sim.tester.ok, s_sim.tester.max);
  }
  printf("\nSimulated %g s in %.3f s\n", (double) s_sim.now / 1e6,
         (double) (clock() - started) / CLOCKS_PER_SEC);

  return 0;
}
