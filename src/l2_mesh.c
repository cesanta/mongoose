// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "l2_mesh.h"
#include "log.h"
#include "printf.h"
#include "util.h"

#if MG_ENABLE_TCPIP

#define MGI_ETH_MAX 1514
#define MGI_TX_DST_LEN 2

// Mesh header, after the radio header: type, then burst: the fragment's
// index << 4 | the last fragment's index. Common to every radio
#define MG_MESH_HDR_LEN 2

#define MG_MESH_FRAME_HDR_LEN (MG_802154_HEADER_LEN + MG_MESH_HDR_LEN)

// MAC header, 9 bytes: frame control 0x8841, a data frame, PAN ID
// compression, short destination and source addresses; sequence number, 0,
// as nothing uses it yet; PAN ID; destination; source. Little endian, as
// the standard says
static void mgi_802154_print_hdr(const struct mg_radio *radio, uint8_t *buf,
                                 uint16_t dst, uint16_t src) {
  buf[0] = 0x41, buf[1] = 0x88, buf[2] = 0;
  buf[3] = (uint8_t) MG_802154_PAN_ID;
  buf[4] = (uint8_t) (MG_802154_PAN_ID >> 8);
  buf[5] = (uint8_t) dst, buf[6] = (uint8_t) (dst >> 8);
  buf[7] = (uint8_t) src, buf[8] = (uint8_t) (src >> 8);
  (void) radio;
}

static size_t mgi_802154_parse_hdr(const struct mg_radio *radio,
                                   const uint8_t *buf, size_t len,
                                   uint16_t *dst, uint16_t *src) {
  (void) radio;
  if (len < MG_802154_HEADER_LEN || buf[0] != 0x41 || buf[1] != 0x88 ||
      buf[3] != (uint8_t) MG_802154_PAN_ID ||
      buf[4] != (uint8_t) (MG_802154_PAN_ID >> 8)) {
    return 0;
  }
  *dst = (uint16_t) (buf[5] | buf[6] << 8);
  *src = (uint16_t) (buf[7] | buf[8] << 8);
  return MG_802154_HEADER_LEN;
}

const struct mg_radio mg_radio_802154 = {
    "802.15.4",            // name
    125,                   // max_frame_size: 127 minus FCS
    250000,                // bitrate
    8,                     // overhead
    320,                   // slot_us
    128,                   // cca_us
    100,                   // turnaround_us
    640,                   // ifs_us
    3,                     // backoff_min
    5,                     // backoff_max
    true,                  // csma
    MG_802154_HEADER_LEN,  // header_len
    mgi_802154_print_hdr,  // print_hdr
    mgi_802154_parse_hdr   // parse_hdr
};

static uint64_t mgi_mesh_b_airtime(size_t len) {
  const struct mg_radio *radio = &mg_radio_802154;
  return ((uint64_t) (len + radio->overhead) * 8000000 + radio->bitrate - 1) /
         radio->bitrate;
}

// Each queue record holds a destination and a complete burst.
static size_t mgi_mesh_b_queue(struct mg_mesh_node *node, struct mg_queue *q,
                               const void *buf, size_t len, uint16_t dst,
                               uint8_t type) {
  const struct mg_radio *radio = &mg_radio_802154;
  const uint8_t *eth = (const uint8_t *) buf;
  size_t hdr = MG_MESH_FRAME_HDR_LEN, cap = radio->max_frame_size - hdr;
  size_t frames, size, i;
  char *record;
  uint8_t *out;
  if (len < 14 || len > MGI_ETH_MAX) return 0;
  frames = (len + cap - 1) / cap;
  size = MGI_TX_DST_LEN + len + frames * (1 + hdr);
  if (mg_queue_next(q, NULL) == 0) mg_queue_init(q, q->buf, q->size);
  if (mg_queue_book(q, &record, size) < size) return 0;
  out = (uint8_t *) record;
  *out++ = (uint8_t) (dst >> 8);
  *out++ = (uint8_t) dst;
  for (i = 0; i < frames; i++) {
    size_t chunk = len - i * cap;
    if (chunk > cap) chunk = cap;
    *out++ = (uint8_t) (hdr + chunk);
    radio->print_hdr(radio, out, dst, node->radio_address);
    out[radio->header_len] = type;
    out[radio->header_len + 1] = (uint8_t) (i << 4 | (frames - 1));
    memcpy(out + hdr, eth + i * cap, chunk);
    out += hdr + chunk;
  }
  mg_queue_add(q, size);
  return len;
}

// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#define MGI_JOIN_WINDOW_US 3000
#define MGI_JOIN_TURN_US 192
#define MGI_JOIN_JITTER_US 640
#define MGI_DATA_QUEUE_SIZE 2048
#define MGI_TX_RETRY_US 32
// Our frames: the radio header, with the radio addresses, then the mesh
// header MG_MESH_HDR_LEN, then the payload
#define MGI_JOIN_UP_LEN \
  (MG_MESH_FRAME_HDR_LEN + 6)  // MGI_JOIN_UP's payload: MAC addr
#define MGI_MAX_FRAGMENTS 14

enum { MGI_JOIN_DOWN = 1, MGI_JOIN_UP, MGI_DATA_DOWN, MGI_DATA_UP };

enum { MGI_TURN_IDLE, MGI_TURN_JOIN, MGI_TURN_DATA };

struct mgi_mesh_b_data {
  uint8_t rx_buf[MGI_ETH_MAX];  // Ethernet frame being reassembled
  uint8_t rx_expected;          // Expected index | last index, 0xff if idle
  struct mg_mesh_node *node;    // Owner of the TCP/IP interface
  struct mg_queue tx;           // One record per burst of RF fragments
  size_t tx_offset;             // Next queued fragment; zero for an empty frame
  uint64_t tx_at;               // Next fragment or JOIN transmission time
  uint64_t deadline;            // End of the current JOIN or DATA turn
  struct mgi_mesh_b_peer *fanout;    // Next broadcast recipient
  char tx_buf[MGI_DATA_QUEUE_SIZE];  // Transmit queue storage
  uint32_t random;                   // PRNG state for join timing
  struct mgi_mesh_b_peer *members;   // Bridge membership list
  struct mgi_mesh_b_peer *current;   // Member whose turn is active or next
  int8_t missed_rounds;  // -1 after a grant; otherwise completed rounds missed
  uint16_t coordinator;  // Coordinator radio address
  uint8_t phase;         // MGI_TURN_IDLE, MGI_TURN_JOIN, or MGI_TURN_DATA
};

struct mgi_mesh_b_peer {
  struct mgi_mesh_b_peer *next;  // Next member in round-robin order
  uint16_t radio_address;        // Source address supplied in MGI_JOIN_UP
  uint8_t mac[6];                // Ethernet MAC supplied in MGI_JOIN_UP
  uint8_t misses;                // Turns in a row it did not answer
};

static uint32_t mgi_mesh_b_random(struct mg_mesh_node *node,
                                  struct mgi_mesh_b_data *data) {
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

static const struct mg_radio *const s_mesh_default_radio = &mg_radio_802154;

// Read a frame's radio addresses to dst and src, see mg_radio::parse_hdr(), and
// return its type, from the mesh header. 0: not a frame of ours
static unsigned mgi_mesh_b_frame_parse(const uint8_t *frame, size_t len,
                                       uint16_t *dst, uint16_t *src) {
  if (len < MG_MESH_FRAME_HDR_LEN ||
      s_mesh_default_radio->parse_hdr(s_mesh_default_radio, frame, len, dst,
                                      src) !=
          s_mesh_default_radio->header_len) {
    return 0;
  }
  return frame[s_mesh_default_radio->header_len];
}

// Write a frame's headers: the radio's, to dst from src, then ours: type,
// and burst: high nibble is the index, low nibble is the last index
static void mgi_mesh_b_frame_header(uint8_t *frame, uint16_t dst, uint16_t src,
                                    uint8_t type, uint8_t burst) {
  size_t h = s_mesh_default_radio->header_len;
  s_mesh_default_radio->print_hdr(s_mesh_default_radio, frame, dst, src);
  frame[h] = type, frame[h + 1] = burst;
}

static bool mgi_mesh_b_control_send(struct mg_mesh_node *node, uint16_t dst,
                                    uint8_t command) {
  struct mg_tcpip_if *ifp = node->radio;
  uint8_t frame[MGI_JOIN_UP_LEN] = {0};
  size_t len = MG_MESH_FRAME_HDR_LEN;
  mgi_mesh_b_frame_header(frame, dst, node->radio_address, command, 0);
  if (command == MGI_JOIN_UP) {
    memcpy(frame + MG_MESH_FRAME_HDR_LEN, node->radio->mac, 6);
    len = MGI_JOIN_UP_LEN;
  }
  return node->now >= node->tx_at && node->tx(ifp, frame, len) == len;
}

static size_t mgi_mesh_b_fragment_capacity(void) {
  return s_mesh_default_radio->max_frame_size - MG_MESH_FRAME_HDR_LEN;
}

static uint64_t mgi_mesh_b_burst_time(size_t len) {
  size_t cap = mgi_mesh_b_fragment_capacity();
  size_t frames = len == 0 ? 1 : (len + cap - 1) / cap;
  return (frames - 1) *
             (mgi_mesh_b_airtime(s_mesh_default_radio->max_frame_size) +
              s_mesh_default_radio->ifs_us) +
         mgi_mesh_b_airtime(MG_MESH_FRAME_HDR_LEN + len - (frames - 1) * cap);
}

static void mgi_mesh_b_tx_finish(struct mgi_mesh_b_data *data) {
  if (data->tx_offset != 0) {
    if (data->fanout != NULL) data->fanout = data->fanout->next;
    if (data->fanout == NULL) {
      size_t len = mg_queue_next(&data->tx, NULL);
      mg_queue_del(&data->tx, len);
    }
  }
  data->tx_offset = 0;
  data->tx_at = (uint64_t) -1;
}

static void mgi_mesh_b_tx_step(struct mgi_mesh_b_data *data, uint64_t now) {
  struct mg_mesh_node *node = data->node;
  struct mg_tcpip_if *radio = node->radio;
  uint8_t empty[MG_MESH_FRAME_HDR_LEN], *frame = empty;
  size_t len = MG_MESH_FRAME_HDR_LEN, record_len = 0;
  uint16_t dst;
  uint8_t command;
  if (now < data->tx_at) return;
  if (now >= data->deadline) {
    mgi_mesh_b_tx_finish(data);
    return;
  }
  dst = node->lan == NULL ? data->coordinator : data->current->radio_address;
  command = node->lan == NULL ? MGI_DATA_UP : MGI_DATA_DOWN;
  if (data->tx_offset != 0) {
    char *record;
    uint8_t *part;
    record_len = mg_queue_next(&data->tx, &record);
    part = (uint8_t *) record + data->tx_offset;
    len = part[0];
    frame = part + 1;
    s_mesh_default_radio->print_hdr(s_mesh_default_radio, frame, dst,
                                    node->radio_address);
  } else {
    mgi_mesh_b_frame_header(frame, dst, node->radio_address, command, 0);
  }
  if (now < data->node->tx_at || data->node->tx(radio, frame, len) != len) {
    data->tx_at = now + MGI_TX_RETRY_US;
    return;
  }
  if (data->tx_offset == 0 || (data->tx_offset += len + 1) == record_len) {
    mgi_mesh_b_tx_finish(data);
  } else {
    data->tx_at = now + mgi_mesh_b_airtime(len) + s_mesh_default_radio->ifs_us;
  }
}

static void mgi_mesh_b_tx_begin(struct mgi_mesh_b_data *data, bool burst,
                                uint64_t now, uint64_t deadline) {
  data->phase = MGI_TURN_DATA;
  data->tx_offset = burst ? MGI_TX_DST_LEN : 0;
  data->tx_at = now;
  data->deadline = deadline;
}

// Return the assembled length, zero for an empty frame, or -1 if incomplete
// or invalid. Full non-final fragments make the write offset derivable.
static int mgi_mesh_b_data_receive(struct mgi_mesh_b_data *data,
                                   const uint8_t *frame, size_t len,
                                   uint64_t now) {
  unsigned burst = frame[MG_MESH_FRAME_HDR_LEN - 1], index = burst >> 4,
           last = burst & 15;
  size_t chunk = len - MG_MESH_FRAME_HDR_LEN,
         cap = mgi_mesh_b_fragment_capacity();
  size_t offset = index * cap;
  if (index == 0) {
    data->rx_expected = (uint8_t) burst;
    if (data->node->lan == NULL)
      data->deadline = now + mgi_mesh_b_burst_time(MGI_ETH_MAX) +
                       2 * s_mesh_default_radio->ifs_us + MGI_JOIN_TURN_US;
  }
  if (last >= MGI_MAX_FRAGMENTS || index > last || burst != data->rx_expected ||
      chunk > cap || now > data->deadline || (index < last && chunk != cap) ||
      (last != 0 && chunk == 0) || offset + chunk > sizeof(data->rx_buf)) {
    data->rx_expected = 0xff;
    return -1;
  }
  memcpy(data->rx_buf + offset, frame + MG_MESH_FRAME_HDR_LEN, chunk);
  if (index < last) {
    data->rx_expected = (uint8_t) (burst + 0x10);
    return -1;
  }
  data->rx_expected = 0xff;
  return offset + chunk == 0 || offset + chunk >= 14 ? (int) (offset + chunk)
                                                     : -1;
}

// Bridge scheduling and Ethernet forwarding.

#define MGI_MAX_MISSES 3  // Turns in a row without an answer: remove the member

static uint16_t mgi_mesh_b_join_field(const uint8_t *p) {
  return (uint16_t) ((unsigned) p[0] * 256 + p[1]);
}

// Return chaddr for an Ethernet DHCP server reply, even when broadcast.
static const uint8_t *mgi_mesh_b_dhcp_client(const uint8_t *frame, size_t len) {
  size_t ip_len, ihl, udp_len;
  const uint8_t *udp, *dhcp;
  if (len < 34 || mgi_mesh_b_join_field(frame + 12) != 0x0800 ||
      (frame[14] >> 4) != 4 || frame[23] != 17 ||
      (mgi_mesh_b_join_field(frame + 20) & 0x3fff) != 0)
    return NULL;
  ip_len = mgi_mesh_b_join_field(frame + 16);
  ihl = (size_t) (frame[14] & 15) * 4;
  if (ihl < 20 || ip_len > len - 14 || ip_len < ihl + 8 + 240) return NULL;
  udp = frame + 14 + ihl;
  udp_len = mgi_mesh_b_join_field(udp + 4);
  if (mgi_mesh_b_join_field(udp) != 67 ||
      mgi_mesh_b_join_field(udp + 2) != 68 || udp_len < 8 + 240 ||
      udp_len > ip_len - ihl)
    return NULL;
  dhcp = udp + 8;
  if (dhcp[0] != 2 || dhcp[1] != 1 || dhcp[2] != 6 ||
      memcmp(dhcp + 236, "\x63\x82\x53\x63", 4) != 0)
    return NULL;
  return dhcp + 28;
}

static size_t mgi_mesh_b_enqueue(struct mg_mesh_node *node, const void *buf,
                                 size_t len) {
  struct mgi_mesh_b_data *data = (struct mgi_mesh_b_data *) node->data;
  const uint8_t *frame = (const uint8_t *) buf;
  if (node->lan == NULL)
    return mgi_mesh_b_queue(node, &data->tx, buf, len, data->coordinator,
                            MGI_DATA_UP);
  if (len >= 14 && len <= MGI_ETH_MAX) {
    const uint8_t *client = mgi_mesh_b_dhcp_client(frame, len);
    const uint8_t *dst = client == NULL ? frame : client;
    bool multicast = client == NULL && (frame[0] & 1) != 0;
    struct mgi_mesh_b_peer *peer;
    for (peer = data->members; peer != NULL; peer = peer->next) {
      if (multicast || memcmp(dst, peer->mac, 6) == 0) {
        uint16_t addr = multicast ? 0xffffu : peer->radio_address;
        return mgi_mesh_b_queue(node, &data->tx, buf, len, addr, MGI_DATA_DOWN);
      }
    }
  }
  return 0;
}

static void mgi_mesh_b_bridge_queue_lan(struct mg_mesh_node *bridge) {
  struct mg_queue *rx = &bridge->lan->recv_queue;
  char *buf;
  size_t len;
  while ((len = mg_queue_next(rx, &buf)) > 0) {
    bridge->strategy->tx(bridge, buf, len);
    mg_queue_del(rx, len);
  }
}

static void mgi_mesh_b_bridge_rx(uint64_t now, struct mg_mesh_node *b,
                                 const uint8_t *frame, size_t len) {
  struct mgi_mesh_b_data *data = (struct mgi_mesh_b_data *) b->data;
  struct mgi_mesh_b_peer *peer;
  uint16_t dst = 0, src = 0;
  unsigned command = mgi_mesh_b_frame_parse(frame, len, &dst, &src);
  if (command == MGI_DATA_UP && dst == b->radio_address &&
      data->phase == MGI_TURN_DATA && data->current != NULL &&
      src == data->current->radio_address && now <= data->deadline) {
    int received = mgi_mesh_b_data_receive(data, frame, len, now);
    if (received < 0) return;
    if (received != 0) {
      b->lan->driver->tx(data->rx_buf, (size_t) received, b->lan);
    }
    data->current->misses = 0;
    data->current = data->current->next;
    data->phase = MGI_TURN_IDLE;
    data->deadline = now;
    return;
  }
  if (len != MGI_JOIN_UP_LEN || command != MGI_JOIN_UP ||
      dst != b->radio_address || data->phase != MGI_TURN_JOIN ||
      now >= data->deadline) {
    return;
  }
  if (src == 0 || src == 0xffffu) return;
  for (peer = data->members; peer != NULL; peer = peer->next) {
    if (peer->radio_address == src) break;
  }
  if (peer == NULL) {
    peer = (struct mgi_mesh_b_peer *) calloc(1, sizeof(*peer));
    if (peer == NULL) return;
    peer->radio_address = src;
    peer->next = data->members;
    data->members = peer;
  }
  memcpy(peer->mac, frame + MG_MESH_FRAME_HDR_LEN, sizeof(peer->mac));
}

// Member peer did not answer its turn. After MGI_MAX_MISSES in a row, it is
// gone: remove it, and its queued downlink frames
static void mgi_mesh_b_member_missed(struct mgi_mesh_b_data *data,
                                     struct mgi_mesh_b_peer *peer) {
  if (++peer->misses < MGI_MAX_MISSES) return;
  MG_INFO(("Member %M removed: no answer %d times", mg_print_mac, peer->mac,
           MGI_MAX_MISSES));
  if (data->fanout == peer) {
    data->fanout = peer->next;
    if (data->fanout == NULL) {
      size_t len = mg_queue_next(&data->tx, NULL);
      mg_queue_del(&data->tx, len);
    }
  }
  LIST_DELETE(struct mgi_mesh_b_peer, &data->members, peer);
  free(peer);
}

// A broadcast occupies one record and is sent once to each member in list
// order.
static size_t mgi_mesh_b_bridge_burst(struct mgi_mesh_b_data *data,
                                      struct mgi_mesh_b_peer *peer) {
  char *record;
  size_t len;
  while ((len = mg_queue_next(&data->tx, &record)) != 0) {
    uint16_t dst = mgi_mesh_b_join_field((const uint8_t *) record);
    struct mgi_mesh_b_peer *member;
    if (dst == 0xffffu) {
      if (data->fanout == NULL) data->fanout = data->members;
      return data->fanout == peer ? len : 0;
    }
    if (dst == peer->radio_address) return len;
    for (member = data->members; member != NULL; member = member->next) {
      if (member->radio_address == dst) return 0;
    }
    mg_queue_del(&data->tx, len);  // Its destination has left.
  }
  return 0;
}

static uint64_t mgi_mesh_b_queued_burst_time(struct mgi_mesh_b_data *data,
                                             size_t len) {
  char *record;
  size_t frames;
  if (len == 0) return mgi_mesh_b_burst_time(0);
  mg_queue_next(&data->tx, &record);
  // Fragment zero carries the final index. Count overhead and IFS per frame.
  frames =
      ((uint8_t) record[MGI_TX_DST_LEN + MG_MESH_FRAME_HDR_LEN] & 15u) + 1u;
  return mgi_mesh_b_airtime(len - MGI_TX_DST_LEN - frames +
                            (frames - 1) * s_mesh_default_radio->overhead) +
         (frames - 1) * s_mesh_default_radio->ifs_us;
}

static void mgi_mesh_b_bridge_io(uint64_t now, struct mg_mesh_node *b) {
  struct mgi_mesh_b_data *data = (struct mgi_mesh_b_data *) b->data;
  mgi_mesh_b_bridge_queue_lan(b);
  if (now >= data->deadline) {
    if (data->tx_at != (uint64_t) -1) mgi_mesh_b_tx_finish(data);
    if (data->phase == MGI_TURN_DATA && data->current != NULL) {
      struct mgi_mesh_b_peer *peer = data->current;
      data->current = peer->next;
      mgi_mesh_b_member_missed(data, peer);
    } else if (data->phase == MGI_TURN_JOIN) {
      data->current = data->members;
    }
    data->phase = MGI_TURN_IDLE;
    if (data->current != NULL) {
      size_t burst = mgi_mesh_b_bridge_burst(data, data->current);
      uint64_t down = mgi_mesh_b_queued_burst_time(data, burst);
      data->rx_expected = 0xff;
      mgi_mesh_b_tx_begin(data, burst != 0, now,
                          now + down + mgi_mesh_b_burst_time(MGI_ETH_MAX) +
                              2 * s_mesh_default_radio->ifs_us +
                              MGI_JOIN_TURN_US);
    } else if (mgi_mesh_b_control_send(b, 0xffffu, MGI_JOIN_DOWN)) {
      data->phase = MGI_TURN_JOIN;
      data->deadline = now + MGI_JOIN_WINDOW_US;
    }
    if (data->deadline <= now) data->deadline = now + MGI_JOIN_TURN_US;
  }
}

static void mgi_mesh_b_bridge_free(struct mgi_mesh_b_data *data) {
  while (data->members != NULL) {
    struct mgi_mesh_b_peer *peer = data->members;
    data->members = peer->next;
    free(peer);
  }
}

static void mgi_mesh_b_node_rx(struct mg_mesh_node *n, uint64_t now,
                               uint8_t *frame, size_t len) {
  struct mgi_mesh_b_data *data = (struct mgi_mesh_b_data *) n->data;
  uint16_t dst = 0, src = 0;
  unsigned command = mgi_mesh_b_frame_parse(frame, len, &dst, &src);
  if (command == MGI_JOIN_DOWN && len == MG_MESH_FRAME_HDR_LEN &&
      dst == 0xffffu) {
    if (n->joined && src == data->coordinator &&
        ++data->missed_rounds >= MGI_MAX_MISSES) {
      n->joined = false;
      data->phase = MGI_TURN_IDLE;
      data->tx_at = (uint64_t) -1;
      data->tx_offset = 0;
      data->rx_expected = 0xff;
    }
    if (!n->joined && (mgi_mesh_b_random(n, data) % 64) == 0) {
      // Spread contenders across windows as well as within the reply window.
      data->coordinator = src;
      data->phase = MGI_TURN_JOIN;
      data->tx_at = now + MGI_JOIN_TURN_US +
                    mgi_mesh_b_random(n, data) % MGI_JOIN_JITTER_US;
    }
  } else if (command == MGI_DATA_DOWN && data->coordinator != 0 &&
             dst == n->radio_address && src == data->coordinator) {
    int received = mgi_mesh_b_data_receive(data, frame, len, now);
    if (received < 0) return;
    n->joined = true;
    data->missed_rounds =
        -1;  // This round had a grant; do not count it as missed.
    if (received != 0) {
      struct mg_tcpip_if *ifp = n->radio;
      if (n->rx_len != 0 || (size_t) received > ifp->recv_queue.size) {
        ifp->ndrop++;
      } else {
        memcpy(ifp->recv_queue.buf, data->rx_buf, (size_t) received);
        n->rx_len = (size_t) received;
      }
    }
    if (data->tx_at != (uint64_t) -1) mgi_mesh_b_tx_finish(data);
    mgi_mesh_b_tx_begin(data, mg_queue_next(&data->tx, NULL) != 0, now,
                        now + mgi_mesh_b_burst_time(MGI_ETH_MAX) +
                            2 * s_mesh_default_radio->ifs_us +
                            MGI_JOIN_TURN_US);
  }
}

static void mgi_mesh_b_input(struct mg_mesh_node *node, void *buf, size_t len) {
  uint64_t ready = node->now + node->strategy->radio->turnaround_us;
  if (node->tx_at < ready) node->tx_at = ready;
  node->next_tick = node->now;
  if (node->lan != NULL)
    mgi_mesh_b_bridge_rx(node->now, node, (const uint8_t *) buf, len);
  else
    mgi_mesh_b_node_rx(node, node->now, (uint8_t *) buf, len);
}

static void mgi_mesh_b_tick(struct mg_mesh_node *node, uint64_t now) {
  struct mgi_mesh_b_data *data = (struct mgi_mesh_b_data *) node->data;
  node->now = now;
  if (node->lan != NULL) {
    mgi_mesh_b_bridge_io(now, node);
    mgi_mesh_b_tx_step(data, now);
    node->next_tick = data->deadline;
    if (data->tx_at < node->next_tick) node->next_tick = data->tx_at;
    return;
  }
  if (data->phase == MGI_TURN_JOIN) {
    if (data->tx_at <= now) {
      if (now - data->tx_at <= MGI_JOIN_TURN_US && !node->joined)
        mgi_mesh_b_control_send(node, data->coordinator, MGI_JOIN_UP);
      data->tx_at = (uint64_t) -1;
      data->phase = MGI_TURN_IDLE;
    }
  } else {
    mgi_mesh_b_tx_step(data, now);
  }
  node->next_tick = data->tx_at;
}

static bool mgi_mesh_b_init(struct mg_mesh_node *node,
                            struct mg_tcpip_if *radio, struct mg_tcpip_if *lan,
                            uint16_t radio_address) {
  struct mgi_mesh_b_data *data =
      (struct mgi_mesh_b_data *) calloc(1, sizeof(*data));
  memset(node, 0, sizeof(*node));
  node->radio = radio;
  node->lan = lan;
  node->radio_address = radio_address;
  node->next_tick = (uint64_t) -1;
  node->ram_used = node->ram_peak = sizeof(*node);
  if (data == NULL) return false;
  node->ram_used += sizeof(*data);
  node->ram_peak = node->ram_used;
  data->rx_expected = 0xff;
  node->data = data;
  data->node = node;
  data->tx_at = (uint64_t) -1;
  mg_queue_init(&data->tx, data->tx_buf, sizeof(data->tx_buf));
  if (lan != NULL) node->next_tick = 0;
  return true;
}

static void mgi_mesh_b_free(struct mg_mesh_node *node) {
  struct mgi_mesh_b_data *data = (struct mgi_mesh_b_data *) node->data;
  if (data == NULL) return;
  mgi_mesh_b_bridge_free(data);
  free(data);
  node->data = NULL;
}

const struct mg_mesh mg_mesh_default = {
    "default",
    mgi_mesh_b_init,
    mgi_mesh_b_tick,
    mgi_mesh_b_free,
    mgi_mesh_b_enqueue,
    mgi_mesh_b_input,
    true,
    true,
    &mg_radio_802154,
};

#endif
