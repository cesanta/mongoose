// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "radio.h"
#include "log.h"
#include "printf.h"
#include "util.h"

#if MG_ENABLE_TCPIP

#define MGI_ETH_MAX 1514
#define MGI_TX_DST_LEN 2

// The radio, chosen by MG_RADIO. A frame starts with the radio header, with
// the radio addresses, then the mesh header: type, then burst: the
// fragment's index << 4 | the last fragment's index. Then the payload
#if MG_RADIO == MG_RADIO_BLE
#define MGI_RADIO_MTU 251           // Largest frame: BLE 5 data length
#define MGI_RADIO_BITRATE 2000000   // Bits per second on air
#define MGI_RADIO_OVERHEAD 11       // Preamble, access address, length, CRC
#define MGI_RADIO_TURNAROUND_US 40  // Receive to transmit: nRF52840 fast ramp
#define MGI_RADIO_IFS_US 300        // Gap between transmissions
#define MGI_RADIO_HDR_LEN 4         // Destination, source

// Write the radio header. Little endian
static void mgi_radio_write_hdr(uint8_t *buf, uint16_t dst, uint16_t src) {
  buf[0] = (uint8_t) dst, buf[1] = (uint8_t) (dst >> 8);
  buf[2] = (uint8_t) src, buf[3] = (uint8_t) (src >> 8);
}

// Read the radio header. The access address already keeps other networks out
static bool mgi_radio_parse_hdr(const uint8_t *buf, size_t len, uint16_t *dst,
                                uint16_t *src) {
  if (len < MGI_RADIO_HDR_LEN) return false;
  *dst = (uint16_t) (buf[0] | buf[1] << 8);
  *src = (uint16_t) (buf[2] | buf[3] << 8);
  return true;
}
#else
#define MGI_RADIO_MTU 125            // 127 minus FCS
#define MGI_RADIO_BITRATE 250000     // Bits per second on air
#define MGI_RADIO_OVERHEAD 8         // Preamble, SFD, PHR, FCS
#define MGI_RADIO_TURNAROUND_US 100  // Receive to transmit
#define MGI_RADIO_IFS_US 640         // Gap between transmissions
#define MGI_RADIO_HDR_LEN 9          // MAC header, short addresses

// MAC header: frame control 0x8841, a data frame, PAN ID compression, short
// destination and source addresses; sequence number, 0, as nothing uses it
// yet; PAN ID; destination; source. Little endian, as the standard says
static void mgi_radio_write_hdr(uint8_t *buf, uint16_t dst, uint16_t src) {
  buf[0] = 0x41, buf[1] = 0x88, buf[2] = 0;
  buf[3] = (uint8_t) MG_RADIO_NETWORK;
  buf[4] = (uint8_t) (MG_RADIO_NETWORK >> 8);
  buf[5] = (uint8_t) dst, buf[6] = (uint8_t) (dst >> 8);
  buf[7] = (uint8_t) src, buf[8] = (uint8_t) (src >> 8);
}

// Read the MAC header. Other networks' frames fail the check
static bool mgi_radio_parse_hdr(const uint8_t *buf, size_t len, uint16_t *dst,
                                uint16_t *src) {
  if (len < MGI_RADIO_HDR_LEN || buf[0] != 0x41 || buf[1] != 0x88 ||
      buf[3] != (uint8_t) MG_RADIO_NETWORK ||
      buf[4] != (uint8_t) (MG_RADIO_NETWORK >> 8)) {
    return false;
  }
  *dst = (uint16_t) (buf[5] | buf[6] << 8);
  *src = (uint16_t) (buf[7] | buf[8] << 8);
  return true;
}
#endif

#define MGI_FRAME_HDR_LEN (MGI_RADIO_HDR_LEN + 2)  // Radio and mesh headers
#define MGI_FRAG_CAP (MGI_RADIO_MTU - MGI_FRAME_HDR_LEN)  // Payload per frame
#define MGI_TX_QUEUE_SIZE 2048  // Ethernet frames to send, as bursts
#define MGI_RX_STAMP_LEN sizeof(uint64_t)
#define MGI_RX_QUEUE_SIZE (16 * (MGI_RX_STAMP_LEN + MGI_RADIO_MTU + 8))

// A radio interface's state, its driver_data, allocated by mg_radio_init()
struct mg_radio_node {
  bool (*hw_poll)(struct mg_tcpip_if *);  // Hardware: service it
  size_t (*hw_tx)(struct mg_tcpip_if *, const void *, size_t);  // Start TX
  struct mg_tcpip_if *ifp;         // Radio interface
  struct mg_tcpip_if *lan;         // Bridge: LAN interface. NULL: a node
  struct mg_queue frames;          // Received radio frames, with arrival times
  struct mg_queue tx;              // Ethernet frames to send, one burst each
  uint64_t now;                    // Current poll time, us
  uint64_t tx_at;                  // Earliest next radio TX, us
  uint64_t burst_at;               // Next fragment TX, us
  uint64_t deadline;               // End of the current turn, us
  uint64_t answer_by;             // Node: first answer frame must end by then
  uint64_t heard_at;               // Node: last frame from the bridge, us
  size_t rx_len;                   // Ethernet frame waiting for the stack
  size_t tx_offset;                // Next fragment of the queued burst
  struct mgi_radio_peer *members;  // Bridge: members, round-robin order
  struct mgi_radio_peer *current;  // Bridge: member whose turn is active/next
  struct mgi_radio_peer *fanout;   // Bridge: next broadcast recipient
  uint32_t random;                 // PRNG state, for join timing
  uint16_t radio_address;          // Our radio address, made from the MAC
  uint16_t coordinator;            // Node: the bridge's radio address
  uint8_t phase;                   // Turn: idle, join or data
  uint8_t rx_expected;  // Next fragment: index << 4 | last index. 0xff: idle
  bool joined;          // Node: member of the mesh
  bool granted;         // Node: had a turn this round
  bool rejoin;          // Node: just left, answer the next join window
  uint8_t missed;       // Node: rounds in a row without a turn
  uint8_t rx_buf[MGI_ETH_MAX];         // Ethernet frame being reassembled
  char tx_buf[MGI_TX_QUEUE_SIZE];      // Storage of the tx queue
  char frames_buf[MGI_RX_QUEUE_SIZE];  // Storage of the frames queue
};

// Time on air of a len byte frame, us
static uint64_t mgi_radio_airtime(size_t len) {
  return ((uint64_t) (len + MGI_RADIO_OVERHEAD) * 8000000 + MGI_RADIO_BITRATE -
          1) /
         MGI_RADIO_BITRATE;
}

// Each queue record holds a destination and a complete burst.
static size_t mgi_radio_queue(struct mg_radio_node *node, struct mg_queue *q,
                              const void *buf, size_t len, uint16_t dst,
                              uint8_t type) {
  const uint8_t *eth = (const uint8_t *) buf;
  size_t hdr = MGI_FRAME_HDR_LEN, cap = MGI_RADIO_MTU - hdr;
  size_t frames, size, i;
  char *record;
  uint8_t *out;
  if (len < 14 || len > MGI_ETH_MAX) return 0;
  frames = (len + cap - 1) / cap;
  size = MGI_TX_DST_LEN + len + frames * (1 + hdr);
  if (mg_queue_next(q, NULL) == 0) mg_queue_init(q, q->buf, q->size);
  if (mg_queue_book(q, &record, size) < size) {
    MG_ERROR(("TX queue full: need %u, head %u, tail %u, size %u",
              (unsigned) size, (unsigned) q->head, (unsigned) q->tail,
              (unsigned) q->size));
    return 0;
  }
  out = (uint8_t *) record;
  *out++ = (uint8_t) (dst >> 8);
  *out++ = (uint8_t) dst;
  for (i = 0; i < frames; i++) {
    size_t chunk = len - i * cap;
    if (chunk > cap) chunk = cap;
    *out++ = (uint8_t) (hdr + chunk);
    mgi_radio_write_hdr(out, dst, node->radio_address);
    out[MGI_RADIO_HDR_LEN] = type;
    out[MGI_RADIO_HDR_LEN + 1] = (uint8_t) (i << 4 | (frames - 1));
    memcpy(out + hdr, eth + i * cap, chunk);
    out += hdr + chunk;
  }
  mg_queue_add(q, size);
  return len;
}

#define MGI_JOIN_WINDOW_US 3000
#define MGI_JOIN_TURN_US 192
#define MGI_JOIN_JITTER_US 640
#define MGI_TX_RETRY_US 32
// Node: finish the first answer frame this much before the bridge stops
// waiting
#define MGI_ANSWER_MARGIN_US 500
#define MGI_JOIN_UP_LEN (MGI_FRAME_HDR_LEN + 6)  // Payload: the MAC address
#define MGI_MAX_FRAGMENTS 14
// Node: rounds without a turn that prove the bridge no longer lists us
#define MGI_MAX_MISSED_ROUNDS 3

enum { MGI_JOIN_DOWN = 1, MGI_JOIN_UP, MGI_DATA_DOWN, MGI_DATA_UP };

enum { MGI_TURN_IDLE, MGI_TURN_JOIN, MGI_TURN_DATA };

struct mgi_radio_peer {
  struct mgi_radio_peer *next;  // Next member in round-robin order
  uint16_t radio_address;       // Source address supplied in MGI_JOIN_UP
  uint8_t mac[6];               // Ethernet MAC supplied in MGI_JOIN_UP
  uint64_t expire_at;  // First missed turn + MG_RADIO_TIMEOUT_MS, 0: answering
};

static uint32_t mgi_radio_random(struct mg_radio_node *node) {
  uint32_t *random = &node->random;
  if (*random == 0) {
    size_t i;
    *random = (uint32_t) mg_now();
    for (i = 0; i < 6; i++) *random = *random * 33u + node->ifp->mac[i];
    if (*random == 0) *random = 1;
  }
  *random ^= *random << 13;
  *random ^= *random >> 17;
  *random ^= *random << 5;
  return *random;
}

// Read a frame's radio addresses to dst and src, and return its type, from
// the mesh header. 0: not a frame of ours
static unsigned mgi_radio_frame_parse(const uint8_t *frame, size_t len,
                                      uint16_t *dst, uint16_t *src) {
  if (len < MGI_FRAME_HDR_LEN || !mgi_radio_parse_hdr(frame, len, dst, src)) {
    return 0;
  }
  return frame[MGI_RADIO_HDR_LEN];
}

// Write a frame's headers: the radio's, to dst from src, then ours: type,
// and burst: high nibble is the index, low nibble is the last index
static void mgi_radio_frame_header(uint8_t *frame, uint16_t dst, uint16_t src,
                                   uint8_t type, uint8_t burst) {
  mgi_radio_write_hdr(frame, dst, src);
  frame[MGI_RADIO_HDR_LEN] = type, frame[MGI_RADIO_HDR_LEN + 1] = burst;
}

// Hand a radio frame to the hardware. The interface's fn sees it first
static bool mgi_radio_send(struct mg_radio_node *node, const void *frame,
                           size_t len) {
  struct mg_str s = mg_str_n((const char *) frame, len);
  mg_tcpip_call(node->ifp, MG_TCPIP_EV_PHY_TX, &s);
  return node->hw_tx(node->ifp, frame, len) == len;
}

static bool mgi_radio_control_send(struct mg_radio_node *node, uint16_t dst,
                                   uint8_t command) {
  uint8_t frame[MGI_JOIN_UP_LEN] = {0};
  size_t len = MGI_FRAME_HDR_LEN;
  mgi_radio_frame_header(frame, dst, node->radio_address, command, 0);
  if (command == MGI_JOIN_UP) {
    memcpy(frame + MGI_FRAME_HDR_LEN, node->ifp->mac, 6);
    len = MGI_JOIN_UP_LEN;
  }
  return node->now >= node->tx_at && mgi_radio_send(node, frame, len);
}

// Time on air of a burst carrying len bytes, gaps between frames included
static uint64_t mgi_radio_burst_time(size_t len) {
  size_t frames = len == 0 ? 1 : (len + MGI_FRAG_CAP - 1) / MGI_FRAG_CAP;
  return (frames - 1) * (mgi_radio_airtime(MGI_RADIO_MTU) + MGI_RADIO_IFS_US) +
         mgi_radio_airtime(MGI_FRAME_HDR_LEN + len -
                           (frames - 1) * MGI_FRAG_CAP);
}

// End the current turn. A queued burst is used up only when done: all its
// frames went out. Otherwise it stays queued, and a later turn sends it again
static void mgi_radio_tx_finish(struct mg_radio_node *node, bool done) {
  if (node->tx_offset != 0 && done) {
    if (node->fanout != NULL) node->fanout = node->fanout->next;
    if (node->fanout == NULL) {
      size_t len = mg_queue_next(&node->tx, NULL);
      mg_queue_del(&node->tx, len);
    }
  }
  node->tx_offset = 0;
  node->burst_at = (uint64_t) -1;
}

static void mgi_radio_tx_step(struct mg_radio_node *node, uint64_t now) {
  uint8_t empty[MGI_FRAME_HDR_LEN], *frame = empty;
  size_t len = MGI_FRAME_HDR_LEN, record_len = 0;
  uint16_t dst;
  uint8_t command;
  if (now < node->burst_at) return;
  if (now >= node->deadline) {
    mgi_radio_tx_finish(node, false);
    return;
  }
  dst = node->lan == NULL ? node->coordinator : node->current->radio_address;
  command = node->lan == NULL ? MGI_DATA_UP : MGI_DATA_DOWN;
  if (node->tx_offset != 0) {
    char *record;
    uint8_t *part;
    record_len = mg_queue_next(&node->tx, &record);
    part = (uint8_t *) record + node->tx_offset;
    len = part[0];
    frame = part + 1;
    mgi_radio_write_hdr(frame, dst, node->radio_address);
  } else {
    mgi_radio_frame_header(frame, dst, node->radio_address, command, 0);
  }
  // A node's first answer frame: if it cannot end on air before the bridge
  // stops waiting, stay silent. The queued frame waits for the next turn
  if (node->lan == NULL &&
      (node->tx_offset == 0 || node->tx_offset == MGI_TX_DST_LEN) &&
      now + mgi_radio_airtime(len) > node->answer_by) {
    node->tx_offset = 0;
    node->burst_at = (uint64_t) -1;
    return;
  }
  if (now < node->tx_at || !mgi_radio_send(node, frame, len)) {
    node->burst_at = now + MGI_TX_RETRY_US;
    return;
  }
  if (node->tx_offset == 0 || (node->tx_offset += len + 1) == record_len) {
    // Bridge: the turn's last frame is out, wait for the answer
    if (node->lan != NULL)
      node->deadline =
          now + mgi_radio_airtime(len) + MG_RADIO_ANSWER_TIMEOUT_US;
    mgi_radio_tx_finish(node, true);
  } else {
    node->burst_at = now + mgi_radio_airtime(len) + MGI_RADIO_IFS_US;
  }
}

static void mgi_radio_tx_begin(struct mg_radio_node *node, bool burst,
                               uint64_t now, uint64_t deadline) {
  node->phase = MGI_TURN_DATA;
  node->answer_by = (uint64_t) -1;  // Node sets it after a grant
  node->tx_offset = burst ? MGI_TX_DST_LEN : 0;
  node->burst_at = now;
  node->deadline = deadline;
}

// Return the assembled length, zero for an empty frame, or -1 if incomplete
// or invalid. Full non-final fragments make the write offset derivable.
static int mgi_radio_data_receive(struct mg_radio_node *node,
                                  const uint8_t *frame, size_t len,
                                  uint64_t now) {
  unsigned burst = frame[MGI_FRAME_HDR_LEN - 1], index = burst >> 4,
           last = burst & 15;
  size_t chunk = len - MGI_FRAME_HDR_LEN, cap = MGI_FRAG_CAP;
  size_t offset = index * cap;
  if (index == 0) {
    node->rx_expected = (uint8_t) burst;
    if (node->lan == NULL)
      node->deadline = now + mgi_radio_burst_time(MGI_ETH_MAX) +
                       2 * MGI_RADIO_IFS_US + MGI_JOIN_TURN_US;
  }
  if (last >= MGI_MAX_FRAGMENTS || index > last || burst != node->rx_expected ||
      chunk > cap || now > node->deadline || (index < last && chunk != cap) ||
      (last != 0 && chunk == 0) || offset + chunk > sizeof(node->rx_buf)) {
    MG_ERROR(
        ("%s: fragment %u/%u dropped, expected %u/%u, %u bytes, %lld us "
         "past deadline",
         node->lan == NULL ? "Node" : "Bridge", index, last,
         (unsigned) node->rx_expected >> 4, node->rx_expected & 15U,
         (unsigned) chunk, (long long) (now - node->deadline)));
    node->rx_expected = 0xff;
    return -1;
  }
  memcpy(node->rx_buf + offset, frame + MGI_FRAME_HDR_LEN, chunk);
  if (index < last) {
    node->rx_expected = (uint8_t) (burst + 0x10);
    // Bridge: an answer is under way, wait for its next fragment
    if (node->lan != NULL)
      node->deadline = now + mgi_radio_airtime(MGI_RADIO_MTU) +
                       MGI_RADIO_IFS_US + MG_RADIO_ANSWER_TIMEOUT_US;
    return -1;
  }
  node->rx_expected = 0xff;
  return offset + chunk == 0 || offset + chunk >= 14 ? (int) (offset + chunk)
                                                     : -1;
}

// Bridge scheduling and Ethernet forwarding.

static uint16_t mgi_radio_join_field(const uint8_t *p) {
  return (uint16_t) ((unsigned) p[0] * 256 + p[1]);
}

// Return chaddr for an Ethernet DHCP server reply, even when broadcast.
static const uint8_t *mgi_radio_dhcp_client(const uint8_t *frame, size_t len) {
  size_t ip_len, ihl, udp_len;
  const uint8_t *udp, *dhcp;
  if (len < 34 || mgi_radio_join_field(frame + 12) != 0x0800 ||
      (frame[14] >> 4) != 4 || frame[23] != 17 ||
      (mgi_radio_join_field(frame + 20) & 0x3fff) != 0)
    return NULL;
  ip_len = mgi_radio_join_field(frame + 16);
  ihl = (size_t) (frame[14] & 15) * 4;
  if (ihl < 20 || ip_len > len - 14 || ip_len < ihl + 8 + 240) return NULL;
  udp = frame + 14 + ihl;
  udp_len = mgi_radio_join_field(udp + 4);
  if (mgi_radio_join_field(udp) != 67 || mgi_radio_join_field(udp + 2) != 68 ||
      udp_len < 8 + 240 || udp_len > ip_len - ihl)
    return NULL;
  dhcp = udp + 8;
  if (dhcp[0] != 2 || dhcp[1] != 1 || dhcp[2] != 6 ||
      memcmp(dhcp + 236, "\x63\x82\x53\x63", 4) != 0)
    return NULL;
  return dhcp + 28;
}

static size_t mgi_radio_enqueue(struct mg_radio_node *node, const void *buf,
                                size_t len) {
  const uint8_t *frame = (const uint8_t *) buf;
  if (node->lan == NULL)
    return mgi_radio_queue(node, &node->tx, buf, len, node->coordinator,
                           MGI_DATA_UP);
  if (len >= 14 && len <= MGI_ETH_MAX) {
    const uint8_t *client = mgi_radio_dhcp_client(frame, len);
    const uint8_t *dst = client == NULL ? frame : client;
    bool multicast = client == NULL && (frame[0] & 1) != 0;
    struct mgi_radio_peer *peer;
    for (peer = node->members; peer != NULL; peer = peer->next) {
      if (multicast || memcmp(dst, peer->mac, 6) == 0) {
        uint16_t addr = multicast ? 0xffffu : peer->radio_address;
        return mgi_radio_queue(node, &node->tx, buf, len, addr, MGI_DATA_DOWN);
      }
    }
  }
  return 0;
}

static void mgi_radio_bridge_queue_lan(struct mg_radio_node *bridge) {
  struct mg_queue *rx = &bridge->lan->recv_queue;
  char *buf;
  size_t len;
  while ((len = mg_queue_next(rx, &buf)) > 0) {
    bridge->ifp->driver->tx(buf, len, bridge->ifp);
    mg_queue_del(rx, len);
  }
}

static void mgi_radio_bridge_rx(uint64_t now, struct mg_radio_node *b,
                                const uint8_t *frame, size_t len) {
  struct mgi_radio_peer *peer;
  uint16_t dst = 0, src = 0;
  unsigned command = mgi_radio_frame_parse(frame, len, &dst, &src);
  if (command == MGI_DATA_UP && dst == b->radio_address &&
      b->phase == MGI_TURN_DATA && b->current != NULL &&
      src == b->current->radio_address && now <= b->deadline) {
    int received = mgi_radio_data_receive(b, frame, len, now);
    if (received < 0) return;
    if (received != 0) {
      b->lan->driver->tx(b->rx_buf, (size_t) received, b->lan);
    }
    b->current->expire_at = 0;
    b->current = b->current->next;
    b->phase = MGI_TURN_IDLE;
    b->deadline = now;
    return;
  }
  if (command == MGI_DATA_UP && dst == b->radio_address) {
    MG_ERROR(
        ("Bridge: fragment %u/%u from %04x outside its turn: phase %u, "
         "turn of %04x, %lld us past deadline",
         frame[MGI_FRAME_HDR_LEN - 1] >> 4U, frame[MGI_FRAME_HDR_LEN - 1] & 15U,
         src, b->phase, b->current == NULL ? 0 : b->current->radio_address,
         (long long) (now - b->deadline)));
    return;
  }
  if (len != MGI_JOIN_UP_LEN || command != MGI_JOIN_UP ||
      dst != b->radio_address || b->phase != MGI_TURN_JOIN ||
      now >= b->deadline) {
    return;
  }
  if (src == 0 || src == 0xffffu) return;
  for (peer = b->members; peer != NULL; peer = peer->next) {
    if (peer->radio_address == src) break;
  }
  if (peer == NULL) {
    peer = (struct mgi_radio_peer *) mg_calloc(1, sizeof(*peer));
    if (peer == NULL) return;
    peer->radio_address = src;
    peer->next = b->members;
    b->members = peer;
    MG_DEBUG(("Member %M joined, radio address %04x", mg_print_mac,
              frame + MGI_FRAME_HDR_LEN, src));
  }
  memcpy(peer->mac, frame + MGI_FRAME_HDR_LEN, sizeof(peer->mac));
  peer->expire_at = 0;
}

// Member peer did not answer its turn. The first miss starts the timeout. If
// it is still silent when that ends, it is gone: remove it, and its queued
// downlink frames
static void mgi_radio_member_missed(struct mg_radio_node *node,
                                    struct mgi_radio_peer *peer, uint64_t now) {
  if (peer->expire_at == 0) peer->expire_at = now + MG_RADIO_TIMEOUT_MS * 1000;
  if (now < peer->expire_at) return;
  MG_DEBUG(("Member %M removed: no answer for %d ms", mg_print_mac, peer->mac,
            MG_RADIO_TIMEOUT_MS));
  if (node->fanout == peer) {
    node->fanout = peer->next;
    if (node->fanout == NULL) {
      size_t len = mg_queue_next(&node->tx, NULL);
      mg_queue_del(&node->tx, len);
    }
  }
  LIST_DELETE(struct mgi_radio_peer, &node->members, peer);
  mg_free(peer);
}

// A broadcast occupies one record and is sent once to each member in list
// order.
static size_t mgi_radio_bridge_burst(struct mg_radio_node *node,
                                     struct mgi_radio_peer *peer) {
  char *record;
  size_t len;
  while ((len = mg_queue_next(&node->tx, &record)) != 0) {
    uint16_t dst = mgi_radio_join_field((const uint8_t *) record);
    struct mgi_radio_peer *member;
    if (dst == 0xffffu) {
      if (node->fanout == NULL) node->fanout = node->members;
      return node->fanout == peer ? len : 0;
    }
    if (dst == peer->radio_address) return len;
    for (member = node->members; member != NULL; member = member->next) {
      if (member->radio_address == dst) return 0;
    }
    mg_queue_del(&node->tx, len);  // Its destination has left.
  }
  return 0;
}

static uint64_t mgi_radio_queued_burst_time(struct mg_radio_node *node,
                                            size_t len) {
  char *record;
  size_t frames;
  if (len == 0) return mgi_radio_burst_time(0);
  mg_queue_next(&node->tx, &record);
  // Fragment zero carries the final index. Count overhead and IFS per frame.
  frames = ((uint8_t) record[MGI_TX_DST_LEN + MGI_FRAME_HDR_LEN] & 15u) + 1u;
  return mgi_radio_airtime(len - MGI_TX_DST_LEN - frames +
                           (frames - 1) * MGI_RADIO_OVERHEAD) +
         (frames - 1) * MGI_RADIO_IFS_US;
}

static void mgi_radio_bridge_io(uint64_t now, struct mg_radio_node *b) {
  mgi_radio_bridge_queue_lan(b);
  if (now >= b->deadline) {
    if (b->burst_at != (uint64_t) -1) mgi_radio_tx_finish(b, false);
    if (b->phase == MGI_TURN_DATA && b->current != NULL) {
      struct mgi_radio_peer *peer = b->current;
      b->current = peer->next;
      mgi_radio_member_missed(b, peer, now);
    } else if (b->phase == MGI_TURN_JOIN) {
      b->current = b->members;
    }
    b->phase = MGI_TURN_IDLE;
    if (b->current != NULL) {
      size_t burst = mgi_radio_bridge_burst(b, b->current);
      uint64_t down = mgi_radio_queued_burst_time(b, burst);
      b->rx_expected = 0xff;
      down += mgi_radio_burst_time(MGI_ETH_MAX);
      mgi_radio_tx_begin(b, burst != 0, now,
                         now + down + 2 * MGI_RADIO_IFS_US + MGI_JOIN_TURN_US);
    } else if (mgi_radio_control_send(b, 0xffffu, MGI_JOIN_DOWN)) {
      b->phase = MGI_TURN_JOIN;
      b->deadline = now + MGI_JOIN_WINDOW_US;
    }
    if (b->deadline <= now) b->deadline = now + MGI_JOIN_TURN_US;
  }
}

// Node: leave the mesh, and answer the next join window. Queued frames are
// dropped, so a node that lost its bridge does not keep a full queue that
// refuses every new frame. The stack sees the link go down on its next 1s
// tick, from mg_radio_poll()
static void mgi_radio_node_leave(struct mg_radio_node *n) {
  n->joined = false;
  n->granted = false;
  n->rejoin = true;
  n->missed = 0;
  n->phase = MGI_TURN_IDLE;
  n->burst_at = (uint64_t) -1;
  n->tx_offset = 0;
  n->rx_expected = 0xff;
  mg_queue_init(&n->tx, n->tx_buf, sizeof(n->tx_buf));
}

static void mgi_radio_node_rx(struct mg_radio_node *n, uint64_t now,
                              uint8_t *frame, size_t len) {
  uint16_t dst = 0, src = 0;
  unsigned command = mgi_radio_frame_parse(frame, len, &dst, &src);
  if (command != 0 && n->coordinator != 0 && src == n->coordinator) {
    n->heard_at = now;
  }
  if (command == MGI_JOIN_DOWN && len == MGI_FRAME_HDR_LEN && dst == 0xffffu) {
    // The bridge gives each member a turn every round, before its JOIN_DOWN.
    // Rounds in a row without a turn mean it does not list us, e.g. it was
    // restarted: leave at once, and answer this join window
    if (n->joined && src == n->coordinator && n->granted) {
      n->granted = false;
      n->missed = 0;
    } else if (n->joined && src == n->coordinator &&
               ++n->missed >= MGI_MAX_MISSED_ROUNDS) {
      MG_INFO(("Left the mesh: no turn in %d rounds", MGI_MAX_MISSED_ROUNDS));
      mgi_radio_node_leave(n);
    }
    // Spread contenders across windows as well as within the reply window.
    // A node that just left answers the first window it sees
    if (!n->joined && (n->rejoin || (mgi_radio_random(n) % 64) == 0)) {
      n->rejoin = false;
      n->coordinator = src;
      n->phase = MGI_TURN_JOIN;
      n->burst_at =
          now + MGI_JOIN_TURN_US + mgi_radio_random(n) % MGI_JOIN_JITTER_US;
    }
  } else if (command == MGI_DATA_DOWN && n->coordinator != 0 &&
             dst == n->radio_address && src == n->coordinator) {
    int received = mgi_radio_data_receive(n, frame, len, now);
    if (received < 0) return;
    if (!n->joined) MG_INFO(("Joined the mesh, bridge address %04x", src));
    n->joined = true;
    n->granted = true;
    n->missed = 0;
    if (received != 0) {
      struct mg_tcpip_if *ifp = n->ifp;
      if (n->rx_len != 0 || (size_t) received > ifp->recv_queue.size) {
        ifp->ndrop++;
      } else {
        memcpy(ifp->recv_queue.buf, n->rx_buf, (size_t) received);
        n->rx_len = (size_t) received;
      }
    }
    // A grant that waited in the receive queue while the node was busy, e.g.
    // computing TLS keys, is too old to answer. Leave the turn state alone:
    // queued frames go out on the first grant that can still be answered
    if (n->now + MGI_ANSWER_MARGIN_US >= now + MG_RADIO_ANSWER_TIMEOUT_US) {
      return;
    }
    if (n->burst_at != (uint64_t) -1) mgi_radio_tx_finish(n, false);
    mgi_radio_tx_begin(n, mg_queue_next(&n->tx, NULL) != 0, now,
                       now + mgi_radio_burst_time(MGI_ETH_MAX) +
                           2 * MGI_RADIO_IFS_US + MGI_JOIN_TURN_US);
    n->answer_by = now + MG_RADIO_ANSWER_TIMEOUT_US - MGI_ANSWER_MARGIN_US;
  }
}

// Handle a received radio frame. Its timing counts from its arrival, us
static void mgi_radio_input(struct mg_radio_node *node, uint64_t arrived,
                            void *buf, size_t len) {
  uint64_t ready = node->now + MGI_RADIO_TURNAROUND_US;
  if (node->tx_at < ready) node->tx_at = ready;
  if (node->lan != NULL) {
    mgi_radio_bridge_rx(arrived, node, (const uint8_t *) buf, len);
  } else {
    mgi_radio_node_rx(node, arrived, (uint8_t *) buf, len);
  }
}

static void mgi_radio_tick(struct mg_radio_node *node, uint64_t now) {
  node->now = now;
  if (node->lan != NULL) {
    mgi_radio_bridge_io(now, node);
    mgi_radio_tx_step(node, now);
    return;
  }
  // No JOIN_DOWN comes from a silent bridge, so the turn timeout never fires
  if (node->joined &&
      now > node->heard_at + (uint64_t) MG_RADIO_TIMEOUT_MS * 1000) {
    MG_INFO(("Left the mesh: bridge silent for %d ms", MG_RADIO_TIMEOUT_MS));
    mgi_radio_node_leave(node);
  }
  if (node->phase == MGI_TURN_JOIN) {
    if (node->burst_at <= now) {
      if (now - node->burst_at <= MGI_JOIN_TURN_US && !node->joined)
        mgi_radio_control_send(node, node->coordinator, MGI_JOIN_UP);
      node->burst_at = (uint64_t) -1;
      node->phase = MGI_TURN_IDLE;
    }
  } else {
    mgi_radio_tx_step(node, now);
  }
}

bool mg_radio_init(struct mg_tcpip_if *ifp,
                   bool (*hw_init)(struct mg_tcpip_if *, uint16_t addr),
                   bool (*hw_poll)(struct mg_tcpip_if *),
                   size_t (*hw_tx)(struct mg_tcpip_if *, const void *,
                                   size_t)) {
  struct mg_radio_node *node =
      (struct mg_radio_node *) mg_calloc(1, sizeof(*node));
  if (node == NULL) return false;
  node->hw_poll = hw_poll, node->hw_tx = hw_tx;
  node->ifp = ifp;
  node->radio_address =
      (uint16_t) (2 + ((unsigned) ifp->mac[4] * 256 + ifp->mac[5]) % 65532u);
  node->burst_at = (uint64_t) -1;
  node->rx_expected = 0xff;
  mg_queue_init(&node->tx, node->tx_buf, sizeof(node->tx_buf));
  mg_queue_init(&node->frames, node->frames_buf, sizeof(node->frames_buf));
  ifp->driver_data = node;
  if (hw_init(ifp, node->radio_address)) return true;
  ifp->driver_data = NULL;
  mg_free(node);
  return false;
}

void mg_radio_bridge(struct mg_tcpip_if *ifp, struct mg_tcpip_if *lan) {
  struct mg_radio_node *node = (struct mg_radio_node *) ifp->driver_data;
  node->lan = lan;
}

size_t mg_radio_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct mg_radio_node *node = (struct mg_radio_node *) ifp->driver_data;
  size_t n = mgi_radio_enqueue(node, buf, len);
  if (n != len) {  // Mongoose ignores a failed send: count and report it
    ifp->nerr++;
    MG_ERROR(("Mesh refused a %u byte frame", (unsigned) len));
  }
  return n;
}

size_t mg_radio_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct mg_radio_node *node = (struct mg_radio_node *) ifp->driver_data;
  size_t n = node->rx_len;
  if (n > len) return 0;
  if (n != 0 && buf != ifp->recv_queue.buf) memcpy(buf, ifp->recv_queue.buf, n);
  node->rx_len = 0;
  return n;
}

void mg_radio_rx_queue(struct mg_tcpip_if *ifp, const void *frame, size_t len) {
  struct mg_radio_node *node = (struct mg_radio_node *) ifp->driver_data;
  uint64_t arrived = mg_micros();
  size_t need = MGI_RX_STAMP_LEN + len;
  uint16_t dst, src;
  char *p;
  if (len > MGI_RADIO_MTU ||
      !mgi_radio_parse_hdr((const uint8_t *) frame, len, &dst, &src) ||
      (dst != 0xffffu && dst != node->radio_address)) {
    return;
  }
  if (mg_queue_book(&node->frames, &p, need) < need) {
    ifp->ndrop++;
    return;
  }
  memcpy(p, &arrived, MGI_RX_STAMP_LEN);
  memcpy(p + MGI_RX_STAMP_LEN, frame, len);
  mg_queue_add(&node->frames, need);
}

// Process queued radio frames, oldest first, each with its arrival time.
// Stop while the node holds an Ethernet frame the TCP/IP stack has not taken
// yet
bool mg_radio_poll(struct mg_tcpip_if *ifp, bool s1) {
  struct mg_radio_node *node = (struct mg_radio_node *) ifp->driver_data;
  bool tx_done = node->hw_poll(ifp);
  uint64_t now = mg_micros();
  char *record;
  size_t len;
  (void) s1;
  if (tx_done) node->tx_at = now + MGI_RADIO_IFS_US;
  node->now = now;
  while (node->rx_len == 0 &&
         (len = mg_queue_next(&node->frames, &record)) != 0) {
    if (len > MGI_RX_STAMP_LEN) {
      struct mg_str s =
          mg_str_n(record + MGI_RX_STAMP_LEN, len - MGI_RX_STAMP_LEN);
      uint64_t arrived;
      memcpy(&arrived, record, MGI_RX_STAMP_LEN);
      mg_tcpip_call(ifp, MG_TCPIP_EV_PHY_RX, &s);
      mgi_radio_input(node, arrived, s.buf, s.len);
    }
    mg_queue_del(&node->frames, len);
  }
  mgi_radio_tick(node, now);
  return node->joined;
}

#endif
