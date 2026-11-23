// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#pragma once

#define MG_ENABLE_TCPIP 1

#include "mongoose.h"

// The simulated radio: 802.15.4, as radio.c sees it with MG_RADIO_802154
#define SIM_RADIO_MTU 125            // Largest frame, without the FCS
#define SIM_RADIO_BITRATE 250000     // Bits per second on air
#define SIM_RADIO_OVERHEAD 8         // Preamble, SFD, PHR, FCS
#define SIM_RADIO_TURNAROUND_US 100  // Receive to transmit
#define SIM_RADIO_IFS_US 640         // Gap between transmissions
#define SIM_RADIO_HDR_LEN 9          // MAC header, short addresses

// Read the addresses from an 802.15.4 MAC header
static inline bool sim_parse_hdr(const uint8_t *buf, size_t len, uint16_t *dst,
                                 uint16_t *src) {
  if (len < SIM_RADIO_HDR_LEN || buf[0] != 0x41 || buf[1] != 0x88 ||
      buf[3] != (uint8_t) MG_RADIO_NETWORK ||
      buf[4] != (uint8_t) (MG_RADIO_NETWORK >> 8)) {
    return false;
  }
  *dst = (uint16_t) (buf[5] | buf[6] << 8);
  *src = (uint16_t) (buf[7] | buf[8] << 8);
  return true;
}

// A position in metres
struct pos {
  double x, y, z;  // Coordinates
};

struct simulator;

// A simulated radio: its interface, and the simulated hardware's state
struct radio_port {
  struct mg_tcpip_if ifp;   // First: driver functions cast ifp to the port
  struct simulator *sim;    // Where it is simulated
  struct mg_tcpip_if *lan;  // Bridge LAN interface, NULL for a node
  const struct pos *pos;    // Location of the radio
  uint64_t tx_until;        // Latest transmission end, us
  uint64_t tx_ready_at;     // Inter-frame wait ends, us
  uint64_t rx_off_until;    // Latest TX end, us
  bool tx_done;             // Completion consumed by the generic driver
};

// A bridge: forwards frames between the LAN and the radio, no IP stack
struct bridge {
  struct bridge *next;      // Next in simulator::bridges
  struct pos pos;           // Where it is
  struct mg_mgr mgr;        // Polls the raw radio driver
  struct mg_tcpip_if lif;   // LAN side: LAN driver, gets every LAN frame
  struct radio_port radio;  // Radio side: radio driver, gets every frame
};

// A radio node: Mongoose on a radio interface
struct radio_node {
  struct radio_node *next;  // Next in simulator::nodes
  struct pos pos;           // Where it is
  struct mg_mgr mgr;        // Mongoose event manager
  struct radio_port radio;  // Radio interface. radio.ifp.ip from DHCP
};

struct simulator;

// LAN node: a DHCP server, 10.0.0.1. Gives each client an address made
// from its MAC, see node_ip()
struct lan_dhcp_server {
  struct mg_tcpip_if lif;  // Its LAN interface, no Mongoose on it
};

// LAN node: the test node, 10.0.0.2, a Mongoose instance. Waits for
// the mesh to settle, then cycles GET / through nodes until 5 s before the end
struct lan_tester {
  struct mg_mgr mgr;        // Its Mongoose
  struct mg_tcpip_if lif;   // Its LAN interface
  uint64_t wait;            // Mesh settle time: start requests then, us
  struct radio_node *next;  // Node to request next, wraps at the end
  struct mg_connection *c;  // Request on the way, NULL: none
  uint64_t start;           // When it started, us
  int ok, failed;           // Requests done, by result
  double min, max, sum;     // Times of successful requests, ms
};

enum { RADIO_START, RADIO_END };

// One radio frame waiting on the simulated radio channel
struct radio_frame {
  struct radio_frame *next;
  struct mg_tcpip_if *from;
  uint64_t due;    // Next radio event, us
  uint64_t start;  // Transmission start, for receive overlap checks
  uint8_t state;
  bool collided;
  size_t len;
  uint8_t buf[1540];
};

uint64_t radio_airtime(size_t len);

// Simulator state
struct simulator {
  struct radio_frame *air_head;  // Pending and active frames, singly linked
  uint64_t air_busy_until;       // Last transmission end, for channel sensing
  unsigned long air_sent, air_collisions;
  unsigned long
      air_lost;          // Receptions lost to range, random loss or TX overlap
  double radio_range;    // Maximum reception distance, metres
  double radio_loss;     // In-range reception loss probability, percent
  uint32_t loss_random;  // Reception-loss PRNG state
  bool work_pending;     // Drain immediate work before advancing time
  struct pos area;       // A box: all positions inside
  struct bridge *bridges;       // Bridges, a list
  struct radio_node *nodes;     // Radio nodes, a list
  int num_bridges, num_nodes;   // How many
  uint64_t now;                 // Virtual time, us since the start
  uint64_t duration;            // Run this long, us
  uint32_t last_mac;            // Low 3 bytes of the last MAC given
  int request_timeout_ms;       // A request that takes longer failed
  struct lan_dhcp_server dhcp;  // DHCP server on the LAN
  struct lan_tester tester;     // Test node on the LAN
};
