// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Radio mesh simulator. Bridges and radio nodes have positions in an area. Time
// is virtual: see mg_millis(). On the LAN: a DHCP server, and a test node that
// sends HTTP requests to the nodes

#include "simulator.h"

#define STEP_MICROSECONDS 1000  // Periodic stack timer polling only

static const struct mg_mesh *s_mesh = &mg_mesh_default;
static struct simulator s_sim;  // The simulator

// Mongoose's clock: virtual time. A minute of network traffic takes far less
// than a minute to simulate
uint64_t mg_micros(void) {
  return s_sim.now;
}

uint64_t mg_millis(void) {
  return mg_micros() / 1000;
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
          "  -w SEC         - mesh settle time before requests, default: %g\n"
          "  -range METRES  - radio range, default: %g\n"
          "  -loss PERCENT  - random reception loss, default: %g\n"
          "  -m A|default   - mesh strategy, default: default\n",
          prog, sim->area.x, sim->area.y, sim->area.z,
          (double) (int64_t) sim->duration / 1e6,
          (double) (int64_t) sim->tester.wait / 1e6, sim->radio_range,
          sim->radio_loss);
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
  if (sim->last_mac >= 0xffffu - 1u) return free(b), false;
  next_mac(sim, b->rif.mac);
  b->radio.address = (uint16_t) sim->last_mac;
  LIST_ADD_TAIL(struct bridge, &sim->bridges, b);
  sim->num_bridges++;
  return true;
}

// Add a radio node at position s. Return false if s is not a position
static bool add_node(struct simulator *sim, const char *s) {
  struct radio_node *n = (struct radio_node *) calloc(1, sizeof(*n));
  if (n == NULL || !parse_pos(s, &n->pos)) return free(n), false;
  if (sim->last_mac >= 0xffffu - 1u) return free(n), false;
  next_mac(sim, n->rif.mac);
  n->radio.address = (uint16_t) sim->last_mac;
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
  if (to != from) {
    mg_tcpip_qwrite((void *) buf, len, to);
    s_sim.work_pending = true;
  }
}

// LAN driver, all LAN interfaces share it. The LAN is infinitely fast: tx
// puts the frame straight into the receive queues of the interfaces it is
// for: all, if broadcast; else the one with the destination MAC. Bridges get
// every frame: some are for their radio nodes. Each node takes frames
// from its queue when polled, see dhcp_poll() and the mesh bridge callback;
// Mongoose does it for the test node
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
// clashes with the LAN nodes' 10.0.0.x
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

// The DHCP node also owns the advertised gateway IP, 10.0.0.1.
static bool dhcp_arp(struct simulator *sim, const uint8_t *f, size_t n) {
  static const uint8_t header[] = {0, 1, 8, 0, 6, 4, 0, 1};
  static const uint8_t ip[] = {10, 0, 0, 1};
  uint8_t reply[60];
  if (n < 42 || f[12] != 8 || f[13] != 6 ||
      memcmp(f + 14, header, sizeof(header)) != 0 ||
      memcmp(f + 38, ip, sizeof(ip)) != 0)
    return false;
  memset(reply, 0, sizeof(reply));
  memcpy(reply, f + 6, 6);
  memcpy(reply + 6, sim->dhcp.lif.mac, 6);
  memcpy(reply + 12, f + 12, 8);
  reply[21] = 2;
  memcpy(reply + 22, sim->dhcp.lif.mac, 6);
  memcpy(reply + 28, ip, sizeof(ip));
  memcpy(reply + 32, f + 22, 10);
  sim->dhcp.lif.driver->tx(reply, sizeof(reply), &sim->dhcp.lif);
  return true;
}

// DHCP server's work: take the frames from its queue
static void dhcp_poll(struct simulator *sim) {
  char *buf;
  size_t len;
  while ((len = mg_queue_next(&sim->dhcp.lif.recv_queue, &buf)) > 0) {
    if (!dhcp_arp(sim, (const uint8_t *) buf, len)) {
      dhcp_rx(sim, (const uint8_t *) buf, len);
    }
    mg_queue_del(&sim->dhcp.lif.recv_queue, len);
  }
}

// The request on the way finished: ok, or failed
static void tester_done(struct lan_tester *t, uint64_t now, bool ok) {
  double ms = (double) (int64_t) (now - t->start) / 1000;
  if (ok) {
    t->ok++, t->sum += ms;
    if (t->ok == 1 || ms < t->min) t->min = ms;
    if (ms > t->max) t->max = ms;
  } else {
    t->failed++;
  }
  if (t->c != NULL) t->c->is_closing = 1, t->c->fn_data = NULL;
  t->c = NULL;
  s_sim.work_pending = true;
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

// Test node's work: Mongoose, then requests, one at a time, once the mesh
// had time to settle. Cycle through nodes until five seconds before the end.
static void tester_poll(struct simulator *sim) {
  struct radio_node *n;
  char url[64];
  mg_mgr_poll(&sim->tester.mgr, 0);
  if (sim->tester.c != NULL && sim->now - sim->tester.start >
                                   (uint64_t) sim->request_timeout_ms * 1000) {
    tester_done(&sim->tester, sim->now, false);
  }
  if (sim->now < sim->tester.wait || sim->tester.c != NULL ||
      sim->tester.next == NULL || sim->duration <= 5000000U ||
      sim->now >= sim->duration - 5000000U)
    return;
  n = sim->tester.next;
  sim->tester.next = n->next == NULL ? sim->nodes : n->next;
  if (n->mgr.ifp == NULL) return;
  sim->tester.start = sim->now;
  if (n->mgr.ifp->ip == 0) {
    // Wait for the next poll rather than endlessly cycling at this time.
    sim->tester.failed++;
    return;
  }
  mg_snprintf(url, sizeof(url), "http://%M/", mg_print_ip4, &n->mgr.ifp->ip);
  if ((sim->tester.c = mg_http_connect(&sim->tester.mgr, url, tester_fn,
                                       &sim->tester)) == NULL) {
    tester_done(&sim->tester, sim->now, false);
  }
}

// Set up sim: defaults, LAN nodes
static void init(struct simulator *sim) {
  static const uint8_t dmac[6] = {2, 1, 0, 0, 0, 0},  // DHCP server MAC
      tmac[6] = {2, 2, 0, 0, 0, 0};                   // Test node MAC
  memset(sim, 0, sizeof(*sim));
  sim->area.x = sim->area.y = 100;
  sim->radio_range = 185;
  sim->loss_random = 1;
  sim->duration = 600 * 1000000U, sim->tester.wait = 60 * 1000000U;
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

// The driver queues each submitted frame for transmission over the air.
// All nodes hear one channel. CCA and turnaround are separate events:
// two idle checks can succeed before either node starts transmitting.
// Inter-frame spacing is per sender, not a reservation of the shared air.
uint64_t radio_airtime(size_t len) {
  const struct mg_radio *radio = s_mesh->radio;
  return ((uint64_t) (len + radio->overhead) * 8000000 + radio->bitrate - 1) /
         radio->bitrate;
}

static void radio_schedule(struct simulator *sim, struct radio_frame *f,
                           bool busy) {
  struct radio_port *port = (struct radio_port *) f->from->driver_data;
  const struct mg_radio *radio = s_mesh->radio;
  bool checked = f->state == RADIO_CCA;
  if (s_mesh->scheduled_tx || !radio->csma || (checked && !busy)) {
    f->state = RADIO_START;
    f->due = sim->now + (checked ? radio->turnaround_us : 0);
    return;
  }
  if (!checked) {
    f->backoff = radio->backoff_min;
  } else if (f->backoff < radio->backoff_max) {
    f->backoff++;
  }
  if (port->random == 0) {
    size_t i;
    port->random = (uint32_t) mg_now();
    for (i = 0; i < 6; i++) {
      port->random = port->random * 33u + f->from->mac[i];
    }
    if (port->random == 0) port->random = 1;
  }
  port->random ^= port->random << 13;
  port->random ^= port->random >> 17;
  port->random ^= port->random << 5;
  f->state = RADIO_BACKOFF;
  f->due = sim->now + (port->random % (1U << f->backoff)) * radio->slot_us;
  if (f->due > sim->now) port->rx_off_until = f->due;
}

static size_t radio_send(struct mg_tcpip_if *ifp, const void *buf, size_t len) {
  struct radio_port *port = (struct radio_port *) ifp->driver_data;
  struct simulator *sim = port->sim;
  struct radio_frame *f, **p = &sim->air_head;
  const struct mg_radio *radio = s_mesh->radio;
  if (len < radio->header_len || len > sizeof(f->buf) ||
      len > radio->max_frame_size || port->tx_ready_at > sim->now ||
      radio->bitrate == 0)
    return 0;
  while (*p != NULL) {
    if ((*p)->from == ifp) return 0;
    p = &(*p)->next;
  }
  if ((f = (struct radio_frame *) calloc(1, sizeof(*f))) == NULL) return 0;
  f->from = ifp, f->len = len;
  memcpy(f->buf, buf, len);
  *p = f;
  sim->work_pending = true;
  radio_schedule(sim, f, false);
  return len;
}

static void radio_receive(struct radio_port *port,
                          const struct radio_frame *f) {
  char *buf;
  if (mg_queue_book(&port->rx, &buf, f->len) >= f->len) {
    memcpy(buf, f->buf, f->len);
    mg_queue_add(&port->rx, f->len);
    port->sim->work_pending = true;
  }
}

// Test range and sample independent interference loss for this reception.
static bool visible(struct simulator *sim, const struct pos *a,
                    const struct pos *b) {
  double dx = a->x - b->x, dy = a->y - b->y, dz = a->z - b->z;
  double range = sim->radio_range;
  if (dx * dx + dy * dy + dz * dz > range * range) return false;
  if (sim->radio_loss == 0) return true;
  sim->loss_random ^= sim->loss_random << 13;
  sim->loss_random ^= sim->loss_random >> 17;
  sim->loss_random ^= sim->loss_random << 5;
  return (double) sim->loss_random * (100.0 / 4294967296.0) >= sim->radio_loss;
}

static void radio_deliver_to(struct radio_port *port,
                             const struct radio_port *from,
                             const struct radio_frame *f, uint16_t dst) {
  bool all = dst == 0xffffu;
  if (port == from || (!all && port->address != dst)) return;
  if (!visible(port->sim, from->pos, port->pos)) {
    port->sim->air_lost++;
  } else if (port->tx_until > f->start) {
    if (!all) port->sim->air_lost++;
  } else if (!f->collided && port->rx_off_until <= f->start) {
    radio_receive(port, f);
  }
}

static void radio_deliver(struct simulator *sim, const struct radio_frame *f) {
  const struct mg_radio *radio = s_mesh->radio;
  const struct radio_port *from = f->from->driver_data;
  uint16_t dst, src;
  struct bridge *b;
  struct radio_node *n;
  if (radio->parse_hdr(radio, f->buf, f->len, &dst, &src) == 0) return;
  for (b = sim->bridges; b != NULL; b = b->next)
    radio_deliver_to(&b->radio, from, f, dst);
  for (n = sim->nodes; n != NULL; n = n->next)
    radio_deliver_to(&n->radio, from, f, dst);
}

// Process ends, then all channel checks, then starts at this timestamp.
// Checks therefore see the same air state regardless of list order.
static void radio_poll(struct simulator *sim) {
  struct radio_frame **p = &sim->air_head, *f, *g;
  while ((f = *p) != NULL) {
    if (f->state != RADIO_END || f->due > sim->now) {
      p = &f->next;
      continue;
    }
    if (f->collided) {
      sim->air_collisions++;
    }
    radio_deliver(sim, f);
    *p = f->next;
    free(f);
  }
  for (f = sim->air_head; f != NULL; f = f->next) {
    if (f->state == RADIO_BACKOFF && f->due <= sim->now) {
      f->state = RADIO_CCA;
      f->due = sim->now + s_mesh->radio->cca_us;
      ((struct radio_port *) f->from->driver_data)->rx_off_until = f->due;
    }
  }
  for (f = sim->air_head; f != NULL; f = f->next) {
    if (f->state != RADIO_CCA || f->due > sim->now) continue;
    radio_schedule(sim, f,
                   sim->air_busy_until > sim->now - s_mesh->radio->cca_us);
    if (f->state == RADIO_START) {
      ((struct radio_port *) f->from->driver_data)->rx_off_until = f->due;
    }
  }
  for (f = sim->air_head; f != NULL; f = f->next) {
    struct radio_port *port = f->from->driver_data;
    if (f->state != RADIO_START || f->due > sim->now) continue;
    f->state = RADIO_END;
    f->start = sim->now;
    f->due = sim->now + radio_airtime(f->len);
    port->rx_off_until = port->tx_until = f->due;
    port->tx_ready_at = f->due + s_mesh->radio->ifs_us;
    for (g = sim->air_head; g != NULL; g = g->next) {
      if (g != f && g->state == RADIO_END && g->due > sim->now) {
        f->collided = g->collided = true;
      }
    }
    if (f->due > sim->air_busy_until) sim->air_busy_until = f->due;
    sim->air_sent++;
  }
}

// Radio events keep microsecond precision between periodic timer polls.
static uint64_t radio_next(const struct simulator *sim, uint64_t next) {
  const struct radio_frame *f;
  for (f = sim->air_head; f != NULL; f = f->next) {
    if (f->due < next) next = f->due;
  }
  return next;
}

static size_t radio_receive_frame(void *buf, size_t cap, struct mg_tcpip_if *ifp) {
  struct radio_port *port = (struct radio_port *) ifp->driver_data;
  char *src;
  size_t len;
  while ((len = mg_queue_next(&port->rx, &src)) > 0) {
    if (len <= cap) memcpy(buf, src, len);
    mg_queue_del(&port->rx, len);
    if (len <= cap) {
      if (mg_queue_next(&port->rx, &src) > 0) port->sim->work_pending = true;
      return len;
    }
  }
  return 0;
}

static size_t radio_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct radio_port *port = ifp->driver_data;
  return port->mesh->strategy->tx(port->mesh, buf, len);
}

static size_t radio_rx(void *buf, size_t cap, struct mg_tcpip_if *ifp) {
  struct radio_port *port = ifp->driver_data;
  size_t len = port->mesh->rx_len;
  if (len > cap) return 0;
  if (len != 0 && buf != ifp->recv_queue.buf)
    memcpy(buf, ifp->recv_queue.buf, len);
  port->mesh->rx_len = 0;
  return len;
}

static bool radio_driver_poll(struct mg_tcpip_if *ifp, bool s1) {
  struct radio_port *port = ifp->driver_data;
  uint8_t frame[128];
  size_t len;
  (void) s1;
  port->mesh->now = mg_micros();
  while (port->mesh->rx_len == 0 &&
         (len = radio_receive_frame(frame, sizeof(frame), ifp)) != 0)
    port->mesh->strategy->rx(port->mesh, frame, len);
  s_mesh->tick(port->mesh, mg_micros());
  return port->mesh->joined;
}

static bool radio_init(struct mg_tcpip_if *ifp) {
  struct radio_port *port = ifp->driver_data;
  char *buf = malloc(LAN_QUEUE_SIZE);
  if (buf == NULL) return false;
  mg_queue_init(&port->rx, buf, LAN_QUEUE_SIZE);
  port->mesh = calloc(1, sizeof(*port->mesh));
  if (port->mesh == NULL ||
      !s_mesh->init(port->mesh, ifp, port->lan, port->address)) {
    free(port->mesh);
    port->mesh = NULL;
    free(buf);
    port->rx.buf = NULL;
    return false;
  }
  port->mesh->strategy = s_mesh;
  port->mesh->tx = radio_send;
  return true;
}

static struct mg_tcpip_driver s_radio_driver = {
    radio_init, radio_tx, radio_rx, radio_driver_poll};

#define MG_TCPIP_DRIVER_SIM_RADIO_INIT(manager, interface) \
  do { \
    struct mg_tcpip_if *mif_ = (interface); \
    mif_->mgr = (manager); \
    mg_tcpip_init((manager), mif_); \
    MG_INFO(("Driver: sim_radio, MAC: %M", mg_print_mac, mif_->mac)); \
  } while (0)

#define MG_TCPIP_DRIVER_SIM_LAN_INIT(manager, interface) \
  do { \
    struct mg_tcpip_if *mif_ = (interface); \
    mg_tcpip_init((manager), mif_); \
    MG_INFO(("Driver: sim_lan, MAC: %M", mg_print_mac, mif_->mac)); \
  } while (0)

// A node's HTTP server: the test node requests GET /
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
    n->rif.driver_data = &n->radio;
    n->radio.sim = sim;
    n->radio.pos = &n->pos;
    mg_mgr_init(&n->mgr);
    MG_TCPIP_DRIVER_SIM_RADIO_INIT(&n->mgr, &n->rif);
    if (n->mgr.ifp != NULL) {
      mg_http_listen(&n->mgr, "http://0.0.0.0:80", node_http_ev_handler,
                     n->mgr.ifp->mac);
    }
  }
}

// Start each bridge: its LAN and radio drivers, with no IP stack.
static void bridges_init(struct simulator *sim) {
  struct bridge *b;
  for (b = sim->bridges; b != NULL; b = b->next) {
    b->lif.driver = &s_lan_driver, b->rif.driver = &s_radio_driver;
    b->rif.driver_data = &b->radio;
    b->radio.sim = sim;
    b->radio.pos = &b->pos;
    mg_mgr_init(&b->mgr);
    b->radio.lan = &b->lif;
    MG_TCPIP_DRIVER_SIM_RADIO_INIT(&b->mgr, &b->rif);
    mg_queue_init(&b->lif.recv_queue, (char *) calloc(1, LAN_QUEUE_SIZE),
                  LAN_QUEUE_SIZE);
    b->lif.driver->init(&b->lif);
  }
}

int main(int argc, char *argv[]) {
  uint64_t poll_at = 0, mesh_at = (uint64_t) -1;
  char *pending;
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
    } else if (strcmp(argv[i], "-range") == 0 ||
               strcmp(argv[i], "-loss") == 0) {
      bool loss = strcmp(argv[i], "-loss") == 0;
      const char *arg = argv[++i];
      char *end;
      double value = strtod(arg, &end);
      if (end == arg || *end != '\0' ||
          !(value >= 0 && value <= (loss ? 100 : 1e100)))
        usage(&s_sim, argv[0]);
      if (loss)
        s_sim.radio_loss = value;
      else
        s_sim.radio_range = value;
    } else if (strcmp(argv[i], "-m") == 0) {
      const char *name = argv[++i];
      if (strcmp(name, mg_mesh_a.name) == 0)
        s_mesh = &mg_mesh_a;
      else if (strcmp(name, mg_mesh_default.name) == 0)
        s_mesh = &mg_mesh_default;
      else
        usage(&s_sim, argv[0]);
    } else {
      usage(&s_sim, argv[0]);
    }
  }

  printf("Area %gx%gx%g m, radio %s, mesh %s, bridges: %d, nodes: %d\n",
         s_sim.area.x, s_sim.area.y, s_sim.area.z, s_mesh->radio->name,
         s_mesh->name, s_sim.num_bridges, s_sim.num_nodes);
  check_area(&s_sim);
  if (s_mesh->single_bridge && s_sim.num_bridges != 1) {
    fprintf(stderr, "%s requires exactly one bridge\n", s_mesh->name);
    return EXIT_FAILURE;
  }

  // mg_log_set(MG_LL_INFO);

  mg_mgr_init(&s_sim.tester.mgr);
  MG_TCPIP_DRIVER_SIM_LAN_INIT(&s_sim.tester.mgr, &s_sim.tester.lif);
  s_sim.tester.next = s_sim.nodes;
  bridges_init(&s_sim);
  nodes_init(&s_sim);

  // Drain packet processing at the current time. Only idle timer polling
  // waits for STEP_MICROSECONDS; radio deadlines remain exact.
  started = clock();
  while (s_sim.now < s_sim.duration) {
    radio_poll(&s_sim);
    if (s_sim.now == poll_at || s_sim.now == mesh_at || s_sim.work_pending) {
      do {
        s_sim.work_pending = false;
        dhcp_poll(&s_sim);
        tester_poll(&s_sim);
        {
          struct bridge *b;
          struct radio_node *n;
          mesh_at = (uint64_t) -1;
          for (b = s_sim.bridges; b != NULL; b = b->next) {
            mg_mgr_poll(&b->mgr, 0);
            if (b->radio.mesh->next_tick < mesh_at)
              mesh_at = b->radio.mesh->next_tick;
          }
          for (n = s_sim.nodes; n != NULL; n = n->next) {
            mg_mgr_poll(&n->mgr, 0);
            if (n->radio.mesh->next_tick < mesh_at)
              mesh_at = n->radio.mesh->next_tick;
          }
        }
        if (mg_queue_next(&s_sim.tester.lif.recv_queue, &pending) > 0) {
          s_sim.work_pending = true;
        }
      } while (s_sim.work_pending);
      if (s_sim.now == poll_at) poll_at += STEP_MICROSECONDS;
    }
    s_sim.now = radio_next(&s_sim, mesh_at < poll_at ? mesh_at : poll_at);
    if (s_sim.now > s_sim.duration) s_sim.now = s_sim.duration;
  }
  printf("Requests: %d ok, %d failed, of %d nodes", s_sim.tester.ok,
         s_sim.tester.failed, s_sim.num_nodes);
  if (s_sim.tester.ok > 0) {
    printf("; ms: min %.1f, mean %.1f, max %.1f", s_sim.tester.min,
           s_sim.tester.sum / s_sim.tester.ok, s_sim.tester.max);
  }
  printf("\nSimulated %g s in %.3f s", (double) (int64_t) s_sim.now / 1e6,
         (double) (clock() - started) / CLOCKS_PER_SEC);
  if (s_sim.nodes != NULL) {
    const struct radio_node *n;
    size_t min = (size_t) -1, max = 0;
    double total = 0;
    for (n = s_sim.nodes; n != NULL; n = n->next) {
      size_t peak = n->radio.mesh->ram_peak;
      if (peak < min) min = peak;
      if (peak > max) max = peak;
      total += (double) peak;
    }
    printf("; peak mesh RAM per node (bytes): min %zu, max %zu, avg %.1f", min,
           max, total / s_sim.num_nodes);
  }
  putchar('\n');

  while (s_sim.air_head != NULL) {
    struct radio_frame *f = s_sim.air_head;
    s_sim.air_head = f->next;
    free(f);
  }
  printf("Radio: %lu sent, %lu collided, %lu lost\n",
         s_sim.air_sent, s_sim.air_collisions, s_sim.air_lost);
  {
    struct radio_node *n;
    int count = 0, joined = 0;
    for (n = s_sim.nodes; n != NULL; n = n->next) {
      if (n->mgr.ifp != NULL && n->mgr.ifp->ip != 0) count++;
      if (n->radio.mesh->joined) joined++;
    }
    printf("Nodes with IP: %d of %d, joined: %d of %d\n", count,
           s_sim.num_nodes, joined, s_sim.num_nodes);
  }
  {
    struct bridge *b;
    struct radio_node *n;
    mg_log_set(MG_LL_NONE);
    for (b = s_sim.bridges; b != NULL; b = b->next) {
      mg_mgr_free(&b->mgr);
      s_mesh->free(b->radio.mesh);
      free(b->radio.mesh);
      free(b->radio.rx.buf);
    }
    for (n = s_sim.nodes; n != NULL; n = n->next) {
      mg_mgr_free(&n->mgr);
      s_mesh->free(n->radio.mesh);
      free(n->radio.mesh);
      free(n->radio.rx.buf);
    }
  }
  return 0;
}
