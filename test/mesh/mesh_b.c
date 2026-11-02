// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "l2_mesh.h"

#define JOIN_WINDOW_US 3000
#define JOIN_TURN_US 192
#define JOIN_JITTER_US 640
#define DATA_QUEUE_SIZE 4096
// Our frames: the radio header, with the radio addresses, then the mesh
// header, see MG_MESH_HDR_LEN, then the payload
#define HDR_LEN (MG_802154_HEADER_LEN + MG_MESH_HDR_LEN)
#define JOIN_UP_LEN (HDR_LEN + 6)  // JOIN_UP's payload: the node's MAC
#define ETH_FRAME_MAX (1500 + 14)
#define MAX_FRAGMENTS 14
#define MAX_MISSES 3  // Turns in a row without an answer: remove the member

enum { JOIN_DOWN = MG_MESH_JOIN_DOWN, JOIN_UP = MG_MESH_JOIN_UP,
       DATA_DOWN, DATA_UP };

struct star_slotted_data {
  uint8_t rx_buf[ETH_FRAME_MAX];       // Ethernet header and up to 1500 bytes
  size_t rx_len;                     // Bytes assembled in the current burst
  uint64_t rx_at;                    // End time of the last received fragment
  unsigned rx_next, rx_last;         // Expected index and final index
  struct mg_mesh_node *node;          // Owner of the TCP/IP interface
  struct mg_tcpip_if ethernet;        // TCP/IP side of the mesh
  struct mg_queue tx;                 // Ethernet frames waiting for a data turn
  char tx_buf[DATA_QUEUE_SIZE];       // Storage for the transmit queue
  uint32_t random;                    // PRNG state for join timing
  struct star_slotted_peer *members;  // Bridge membership list
  struct star_slotted_peer *next;     // Next member to get a data turn
  uint64_t join_window_at;            // Bridge join window start, us
  uint64_t join_send_at;              // Client JOIN deadline, us, or zero
  uint16_t coordinator;  // Coordinator radio address from the beacon
  struct star_slotted_peer *granted;  // Member granted the current turn
};

struct star_slotted_peer {
  struct star_slotted_peer *next;  // Next member in round-robin order
  uint16_t radio_address;          // Source address supplied in JOIN_UP
  struct mg_queue tx;              // LAN frames awaiting this member's turn
  char tx_buf[DATA_QUEUE_SIZE];
  uint8_t mac[6];  // Ethernet MAC supplied in JOIN_UP
  uint8_t ip[4];   // IPv4 source observed on a member uplink
  uint8_t misses;  // Turns in a row it did not answer
};

static size_t enqueue(struct mg_queue *q, const void *buf, size_t len) {
  char *dst;
  if (len < 14 || len > ETH_FRAME_MAX) return 0;
  if (mg_queue_book(q, &dst, len) < len) return 0;
  memcpy(dst, buf, len);
  mg_queue_add(q, len);
  return len;
}

static size_t ethernet_tx(const void *buf, size_t len,
                          struct mg_tcpip_if *ifp) {
  struct star_slotted_data *data = ifp->driver_data;
  return enqueue(&data->tx, buf, len);
}

static bool ethernet_link(struct mg_tcpip_if *ifp, bool s1) {
  struct star_slotted_data *data = ifp->driver_data;
  (void) s1;
  return data->node->joined;
}

static struct mg_tcpip_driver s_ethernet_driver = {NULL, ethernet_tx, NULL,
                                                   ethernet_link};

static uint32_t star_random(struct mg_mesh_node *node,
                            struct star_slotted_data *data) {
  uint32_t *random = &data->random;
  if (*random == 0) {
    size_t i;
    *random = (uint32_t) mg_now();
    for (i = 0; i < 6; i++) *random = *random * 33u + node->radio->mac[i];
    if (*random == 0) *random = 1;
  }
  *random ^= *random << 13;
  *random ^= *random >> 17;
  *random ^= *random << 5;
  return *random;
}

static const struct mg_radio *const s_radio = &mg_radio_802154;

// Read a frame's radio addresses to dst and src, see mg_radio::parse_hdr(), and
// return its type, from the mesh header. 0: not a frame of ours
static unsigned frame_parse(const uint8_t *frame, size_t len, uint16_t *dst,
                            uint16_t *src) {
  if (len < HDR_LEN || s_radio->parse_hdr(s_radio, frame, len, dst, src) != s_radio->header_len) {
    return 0;
  }
  return frame[s_radio->header_len];
}

static uint16_t join_field(const uint8_t *p) {
  return (uint16_t) ((unsigned) p[0] * 256 + p[1]);
}

// Write a frame's headers: the radio's, to dst from src, then ours: type,
// and burst: high nibble is the index, low nibble is the last index
static void frame_header(uint8_t *frame, uint16_t dst, uint16_t src,
                         uint8_t type, uint8_t burst) {
  size_t h = s_radio->header_len;
  s_radio->print_hdr(s_radio, frame, dst, src);
  frame[h] = type, frame[h + 1] = burst;
}

static bool control_send(struct mg_mesh_node *node, uint16_t dst,
                         uint8_t command) {
  struct mg_tcpip_if *ifp = node->radio;
  uint8_t frame[JOIN_UP_LEN] = {0};
  size_t len = HDR_LEN;
  frame_header(frame, dst, node->radio_address, command, 0);
  if (command == JOIN_UP) {
    struct star_slotted_data *data = node->data;
    memcpy(frame + HDR_LEN, data->ethernet.mac, 6);
    len = JOIN_UP_LEN;
  }
  return ifp->driver->tx(frame, len, ifp) == len;
}

static uint64_t frame_time(size_t len) {
  return ((uint64_t) (len + s_radio->overhead) * 8000000 +
          s_radio->bitrate - 1) / s_radio->bitrate;
}

static size_t fragment_capacity(void) {
  return s_radio->max_frame_size - HDR_LEN;
}

static uint64_t burst_time(size_t len) {
  size_t cap = fragment_capacity();
  size_t frames = len == 0 ? 1 : (len + cap - 1) / cap;
  return (frames - 1) * (frame_time(s_radio->max_frame_size) + s_radio->ifs_us) +
         frame_time(HDR_LEN + len - (frames - 1) * cap);
}

static uint64_t data_send(struct mg_mesh_node *node, uint16_t dst,
                          uint8_t command, struct mg_queue *q) {
  struct mg_tcpip_if *radio = node->radio;
  uint8_t frame[HDR_LEN + ETH_FRAME_MAX];
  char *buf;
  size_t len = mg_queue_next(q, &buf), cap = fragment_capacity(), offset = 0;
  size_t last = len == 0 ? 0 : (len - 1) / cap, index;
  uint64_t time = 0;
  for (index = 0; index <= last; index++) {
    size_t chunk = len - offset, wire_len;
    if (chunk > cap) chunk = cap;
    wire_len = HDR_LEN + chunk;
    frame_header(frame, dst, node->radio_address, command,
                 (uint8_t) (index << 4 | last));
    if (chunk != 0) memcpy(frame + HDR_LEN, buf + offset, chunk);
    if (radio->driver->tx(frame, wire_len, radio) != wire_len) return time;
    time += frame_time(wire_len) + (index == 0 ? 0 : s_radio->ifs_us);
    offset += chunk;
  }
  if (len != 0) mg_queue_del(q, len);
  return time;
}

// A missing or out-of-order fragment invalidates the burst.
static bool data_receive(struct star_slotted_data *data, const uint8_t *frame,
                         size_t len, uint64_t now) {
  unsigned burst = frame[HDR_LEN - 1], index = burst >> 4, last = burst & 15;
  size_t chunk = len - HDR_LEN, cap = fragment_capacity();
  if (index == 0) {
    data->rx_next = 0;
    data->rx_last = last;
    data->rx_len = 0;
  }
  if (last >= MAX_FRAGMENTS || index > last || index != data->rx_next ||
      last != data->rx_last || chunk > cap ||
      (index != 0 && now - data->rx_at >
                         frame_time(len) + s_radio->ifs_us + JOIN_TURN_US) ||
      (index < last && chunk != cap) || (last != 0 && chunk == 0) ||
      chunk > sizeof(data->rx_buf) - data->rx_len) {
    data->rx_next = 0;
    return false;
  }
  memcpy(data->rx_buf + data->rx_len, frame + HDR_LEN, chunk);
  data->rx_len += chunk;
  data->rx_at = now;
  if (index < last) {
    data->rx_next++;
    return false;
  }
  data->rx_next = 0;
  return data->rx_len == 0 || data->rx_len >= 14;
}

// Return chaddr for an Ethernet DHCP server reply, even when broadcast.
static const uint8_t *dhcp_client(const uint8_t *frame, size_t len) {
  size_t ip_len, ihl, udp_len;
  const uint8_t *udp, *dhcp;
  if (len < 34 || join_field(frame + 12) != 0x0800 || (frame[14] >> 4) != 4 ||
      frame[23] != 17 || (join_field(frame + 20) & 0x3fff) != 0)
    return NULL;
  ip_len = join_field(frame + 16);
  ihl = (size_t) (frame[14] & 15) * 4;
  if (ihl < 20 || ip_len > len - 14 || ip_len < ihl + 8 + 240) return NULL;
  udp = frame + 14 + ihl;
  udp_len = join_field(udp + 4);
  if (join_field(udp) != 67 || join_field(udp + 2) != 68 || udp_len < 8 + 240 ||
      udp_len > ip_len - ihl)
    return NULL;
  dhcp = udp + 8;
  if (dhcp[0] != 2 || dhcp[1] != 1 || dhcp[2] != 6 ||
      memcmp(dhcp + 236, "\x63\x82\x53\x63", 4) != 0)
    return NULL;
  return dhcp + 28;
}

static bool is_ipv4_arp(const uint8_t *frame, size_t len) {
  return len >= 42 &&
         memcmp(frame + 12, "\x08\x06\x00\x01\x08\x00\x06\x04", 8) == 0;
}

static void learn_peer_ip(struct star_slotted_peer *peer, const uint8_t *frame,
                          size_t len) {
  const uint8_t *ip = NULL;
  if (memcmp(frame + 6, peer->mac, sizeof(peer->mac)) != 0) return;
  if (is_ipv4_arp(frame, len) && memcmp(frame + 22, peer->mac, 6) == 0) {
    ip = frame + 28;
  } else if (len >= 34 && join_field(frame + 12) == 0x0800 &&
             (frame[14] >> 4) == 4) {
    ip = frame + 26;
  }
  if (ip != NULL && ip[0] != 0 && ip[0] < 224) memcpy(peer->ip, ip, 4);
}

static bool proxy_arp_reply(struct mg_mesh_node *bridge, const uint8_t *frame,
                            size_t len) {
  struct star_slotted_data *data = bridge->data;
  struct star_slotted_peer *peer;
  uint8_t reply[42];
  if (!is_ipv4_arp(frame, len) || join_field(frame + 20) != 1 || frame[38] == 0)
    return false;
  for (peer = data->members; peer != NULL; peer = peer->next) {
    if (memcmp(peer->ip, frame + 38, sizeof(peer->ip)) == 0) break;
  }
  if (peer == NULL) return false;
  memcpy(reply, frame + 6, 6);         // Ethernet destination
  memcpy(reply + 6, peer->mac, 6);     // Ethernet source
  memcpy(reply + 12, frame + 12, 8);   // Ethernet type and ARP format
  reply[20] = 0, reply[21] = 2;        // ARP reply
  memcpy(reply + 22, peer->mac, 6);    // Sender MAC
  memcpy(reply + 28, peer->ip, 4);     // Sender IP
  memcpy(reply + 32, frame + 22, 10);  // Target MAC and IP
  return bridge->lan->driver->tx(reply, sizeof(reply), bridge->lan) ==
         sizeof(reply);
}

static void bridge_queue_lan(struct mg_mesh_node *bridge) {
  struct star_slotted_data *data = bridge->data;
  struct mg_queue *rx = &bridge->lan->recv_queue;
  char *buf;
  size_t len;
  while ((len = mg_queue_next(rx, &buf)) > 0) {
    const uint8_t *frame = (const uint8_t *) buf;
    if (len >= 14 && len <= ETH_FRAME_MAX &&
        !(bridge->proxy_arp && proxy_arp_reply(bridge, frame, len))) {
      const uint8_t *client = dhcp_client(frame, len);
      const uint8_t *dst = client == NULL ? frame : client;
      bool multicast = client == NULL && (frame[0] & 1) != 0;
      struct star_slotted_peer *peer;
      for (peer = data->members; peer != NULL; peer = peer->next) {
        if (multicast || memcmp(dst, peer->mac, 6) == 0) {
          enqueue(&peer->tx, buf, len);
          if (!multicast) break;
        }
      }
    }
    mg_queue_del(rx, len);
  }
}

static void star_slotted_bridge_rx(uint64_t now, struct mg_mesh_node *b,
                                   const uint8_t *frame, size_t len) {
  struct star_slotted_data *data = b->data;
  struct star_slotted_peer *peer;
  uint16_t dst = 0, src = 0;
  unsigned command = frame_parse(frame, len, &dst, &src);
  if (command == DATA_UP && dst == b->radio_address &&
      data->granted != NULL && src == data->granted->radio_address &&
      now <= b->next_tick) {
    if (!data_receive(data, frame, len, now)) return;
    if (data->rx_len != 0) {
      if (b->proxy_arp) {
        learn_peer_ip(data->granted, data->rx_buf, data->rx_len);
      }
      b->lan->driver->tx(data->rx_buf, data->rx_len, b->lan);
    }
    data->granted->misses = 0;
    data->granted = NULL;
    b->next_tick = now;
    return;
  }
  if (len != JOIN_UP_LEN || command != JOIN_UP || dst != b->radio_address ||
      data->join_window_at == UINT64_MAX ||
      now >= data->join_window_at + JOIN_WINDOW_US) {
    return;
  }
  if (src == 0 || src == UINT16_MAX) return;
  for (peer = data->members; peer != NULL; peer = peer->next) {
    if (peer->radio_address == src) break;
  }
  if (peer == NULL) {
    peer = calloc(1, sizeof(*peer));
    if (peer == NULL) return;
    mg_queue_init(&peer->tx, peer->tx_buf, sizeof(peer->tx_buf));
    peer->radio_address = src;
    peer->next = data->members;
    data->members = peer;
  }
  if (memcmp(peer->mac, frame + HDR_LEN, sizeof(peer->mac)) != 0) {
    memset(peer->ip, 0, sizeof(peer->ip));
    memcpy(peer->mac, frame + HDR_LEN, sizeof(peer->mac));
  }
}

static void star_slotted_node_rx(struct mg_mesh_node *n, uint64_t now,
                                 uint8_t *frame, size_t len) {
  struct star_slotted_data *data = n->data;
  uint16_t dst = 0, src = 0;
  unsigned command = frame_parse(frame, len, &dst, &src);
  if (!n->joined && command == JOIN_DOWN && len == HDR_LEN &&
      (star_random(n, data) % 64) == 0) {
    // Spread contenders across windows as well as within the reply window.
    data->coordinator = src;
    data->join_send_at =
        now + JOIN_TURN_US + star_random(n, data) % JOIN_JITTER_US;
  } else if (command == DATA_DOWN &&
             data->coordinator != 0 && dst == n->radio_address &&
             src == data->coordinator) {
    if (!data_receive(data, frame, len, now)) return;
    n->joined = true;
    if (data->rx_len != 0) {
      mg_tcpip_qwrite(data->rx_buf, data->rx_len, &data->ethernet);
    }
    data_send(n, data->coordinator, DATA_UP, &data->tx);
  }
}

// Member peer did not answer its turn. After MAX_MISSES in a row, it is
// gone: remove it, and its queued downlink frames
static void member_missed(struct star_slotted_data *data,
                          struct star_slotted_peer *peer) {
  if (++peer->misses < MAX_MISSES) return;
  MG_INFO(("Member %M removed: no answer %d times", mg_print_mac, peer->mac,
           MAX_MISSES));
  if (data->next == peer) data->next = peer->next;
  LIST_DELETE(struct star_slotted_peer, &data->members, peer);
  free(peer);
}

static void star_slotted_bridge_io(uint64_t now, struct mg_mesh_node *b) {
  struct star_slotted_data *data = b->data;
  uint8_t frame[HDR_LEN + ETH_FRAME_MAX];
  size_t len;
  while ((len = b->radio->driver->rx(frame, sizeof(frame), b->radio)) > 0) {
    star_slotted_bridge_rx(now, b, frame, len);
  }
  bridge_queue_lan(b);
  if (now >= b->next_tick) {
    if (data->granted != NULL) member_missed(data, data->granted);
    data->granted = NULL;
    if (data->join_window_at != UINT64_MAX) {
      data->join_window_at = UINT64_MAX;
      data->next = data->members;
    }
    if (data->next != NULL) {
      struct star_slotted_peer *peer = data->next;
      uint64_t sent = data_send(b, peer->radio_address, DATA_DOWN, &peer->tx);
      if (sent != 0) {
        data->granted = peer;
        data->next = peer->next;
        data->rx_next = 0;
        b->next_tick = now + sent + burst_time(ETH_FRAME_MAX) +
                       2 * s_radio->ifs_us + JOIN_TURN_US;
      }
    } else if (control_send(b, UINT16_MAX, JOIN_DOWN)) {
      data->join_window_at = now;
      b->next_tick = now + JOIN_WINDOW_US;
    }
    if (b->next_tick <= now) b->next_tick = now + JOIN_TURN_US;
  }
}

static void star_slotted_node_io(struct mg_mesh_node *n, uint64_t now) {
  uint8_t frame[HDR_LEN + ETH_FRAME_MAX];
  size_t len;
  while ((len = n->radio->driver->rx(frame, sizeof(frame), n->radio)) > 0) {
    star_slotted_node_rx(n, now, frame, len);
  }
}

static void star_slotted_tick(struct mg_mesh_node *node, uint64_t now) {
  struct star_slotted_data *data = node->data;
  if (node->lan != NULL) {
    star_slotted_bridge_io(now, node);
    return;
  }
  star_slotted_node_io(node, now);
  if (data->join_send_at != 0 && data->join_send_at <= now) {
    if (now == data->join_send_at && !node->joined) {
      control_send(node, data->coordinator, JOIN_UP);
    }
    data->join_send_at = 0;
  }
  node->next_tick = data->join_send_at == 0 ? UINT64_MAX : data->join_send_at;
}

static bool star_slotted_init(struct mg_mesh_node *node, struct mg_mgr *mgr,
                              struct mg_tcpip_if *radio,
                              struct mg_tcpip_if *lan, uint16_t radio_address) {
  struct star_slotted_data *data = calloc(1, sizeof(*data));
  mg_mesh_node_init(node, radio, lan, radio_address);
  if (data == NULL) return false;
  node->ram_used += sizeof(*data);
  node->ram_peak = node->ram_used;
  data->join_window_at = UINT64_MAX;
  node->data = data;
  if (lan != NULL) {
    node->next_tick = 0;
  } else {
    data->node = node;
    data->ethernet.driver = &s_ethernet_driver;
    data->ethernet.driver_data = data;
    memcpy(data->ethernet.mac, radio->mac, sizeof(data->ethernet.mac));
    mg_queue_init(&data->tx, data->tx_buf, sizeof(data->tx_buf));
    mg_tcpip_init(mgr, &data->ethernet);
  }
  return true;
}

static void star_slotted_free(struct mg_mesh_node *node) {
  struct star_slotted_data *data = node->data;
  if (data == NULL) return;
  while (data->members != NULL) {
    struct star_slotted_peer *peer = data->members;
    data->members = peer->next;
    free(peer);
  }
  free(data);
  node->data = NULL;
}

const struct mg_mesh mg_mesh_b = {
    "B",
    star_slotted_init,
    star_slotted_tick,
    star_slotted_free,
    true,
    true,
    &mg_radio_802154,
};
