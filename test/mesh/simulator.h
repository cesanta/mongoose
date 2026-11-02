// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#pragma once

#define MG_ENABLE_TCPIP 1

#include "l2_mesh.h"
#include "mongoose.h"

// A position in metres
struct pos {
  double x, y, z;  // Coordinates
};

struct simulator;

enum radio_kind {
  RADIO_ARP,
  RADIO_DHCP,
  RADIO_TCP_SYN,
  RADIO_TCP_DATA,
  RADIO_TCP_CONTROL,
  RADIO_MESH_CONTROL,
  RADIO_OTHER,
  RADIO_KINDS
};

// Private state of the simulated radio driver.
struct radio_port {
  struct simulator *sim;
  struct mg_queue rx;
  uint32_t random;   // Driver backoff PRNG
  uint16_t address;  // Preassigned radio address
  enum radio_kind burst_kind;  // Classification of the first fragment
  uint8_t burst_type, burst_next;  // Type and index byte expected next
  struct mg_mesh_node mesh;
};

// A bridge: forwards frames between the LAN and the radio, no IP stack
struct bridge {
  struct bridge *next;     // Next in simulator::bridges
  struct pos pos;          // Where it is
  struct mg_mgr mgr;       // Polls the raw radio driver
  struct mg_tcpip_if lif;  // LAN side: LAN driver, gets every LAN frame
  struct mg_tcpip_if rif;  // Radio side: radio driver, gets every frame
  struct radio_port radio;
  uint64_t tx_until;      // Latest transmission end, us
  uint64_t tx_ready_at;   // Inter-frame wait ends, us
  uint64_t rx_off_until;  // Latest backoff/sensing/turnaround/TX end, us
};

// A radio node: Mongoose on a radio interface
struct radio_node {
  struct radio_node *next;  // Next in simulator::nodes
  struct pos pos;           // Where it is
  struct mg_mgr mgr;        // Mongoose event manager
  struct mg_tcpip_if rif;   // Radio interface. rif.ip from DHCP, 0: none yet
  struct radio_port radio;
  uint64_t tx_until;      // Latest transmission end, us
  uint64_t tx_ready_at;   // Inter-frame wait ends, us
  uint64_t rx_off_until;  // Latest backoff/sensing/turnaround/TX end, us
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

enum { RADIO_QUEUED, RADIO_IFS, RADIO_BACKOFF, RADIO_CCA, RADIO_START, RADIO_END };

// One radio frame waiting on the simulated radio channel
struct radio_frame {
  struct radio_frame *next;
  struct mg_tcpip_if *from;
  uint64_t due;    // Next radio event, us
  uint64_t start;  // Transmission start, for receive overlap checks
  uint8_t state, backoff;
  bool collided;
  enum radio_kind kind;
  size_t len;
  uint8_t buf[1540];
};

enum radio_kind radio_kind(const uint8_t *f, size_t n);
uint64_t radio_airtime(size_t len);

// Simulator state
struct simulator {
  struct radio_frame *air_head;  // Pending and active frames, singly linked
  uint64_t air_busy_until;       // Last transmission end, for channel sensing
  unsigned long air_sent, air_collisions;
  unsigned long air_invisible;  // Receptions lost to range or random loss
  double radio_range;            // Maximum reception distance, metres
  double radio_loss;             // In-range reception loss probability, percent
  uint32_t loss_random;           // Reception-loss PRNG state
  unsigned long air_half_duplex[RADIO_KINDS];  // Receiver-frame TX losses
  unsigned long air_sent_by_kind[RADIO_KINDS];
  unsigned long air_collided_by_kind[RADIO_KINDS];
  size_t air_bytes;             // Queued payload bytes
  bool work_pending;            // Drain immediate work before advancing time
  struct pos area;              // A box: all positions inside
  struct bridge *bridges;       // Bridges, a list
  struct radio_node *nodes;     // Radio nodes, a list
  int num_bridges, num_nodes;   // How many
  uint64_t now;                 // Virtual time, us since the start
  uint64_t duration;            // Run this long, us
  bool proxy_arp;               // Enable bridge proxy ARP
  uint32_t last_mac;            // Low 3 bytes of the last MAC given
  int request_timeout_ms;       // A request that takes longer failed
  struct lan_dhcp_server dhcp;  // DHCP server on the LAN
  struct lan_tester tester;     // Test node on the LAN
};
