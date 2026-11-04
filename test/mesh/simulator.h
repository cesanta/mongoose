// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#pragma once

#define MG_ENABLE_TCPIP 1

#include "l2_802154.h"
#include "l2_mesh.h"
#include "mongoose.h"
#include "star.h"

// A position in metres
struct pos {
  double x, y, z;  // Coordinates
};

// A bridge: forwards frames between the LAN and the radio, no IP stack
struct bridge {
  struct bridge *next;     // Next in simulator::bridges
  struct pos pos;          // Where it is
  struct mg_tcpip_if lif;  // LAN side: LAN driver, gets every LAN frame
  struct mg_tcpip_if rif;  // Radio side: radio driver, gets every frame
};

// A radio node: Mongoose on a radio interface
struct radio_node {
  struct radio_node *next;  // Next in simulator::nodes
  struct pos pos;           // Where it is
  struct mg_mgr mgr;        // Mongoose event manager
  struct mg_tcpip_if rif;   // Radio interface. rif.ip from DHCP, 0: none yet
};

struct simulator;

// LAN station: a DHCP server, 10.0.0.1. Gives each client an address made
// from its MAC, see node_ip()
struct lan_dhcp_server {
  struct mg_tcpip_if lif;  // Its LAN interface, no Mongoose on it
};

// LAN station: the test station, 10.0.0.2, a Mongoose instance. Waits for
// the mesh to settle, then sends GET / to each node
struct lan_tester {
  struct mg_mgr mgr;        // Its Mongoose
  struct mg_tcpip_if lif;   // Its LAN interface
  uint64_t wait;            // Mesh settle time: start requests then, us
  struct radio_node *next;  // Node to request next, NULL: all done
  struct mg_connection *c;  // Request on the way, NULL: none
  uint64_t start;           // When it started, us
  int ok, failed;           // Requests done, by result
  double min, max, sum;     // Times of successful requests, ms
};

// Simulator state
struct simulator {
  struct pos area;              // A box: all positions inside
  struct bridge *bridges;       // Bridges, a list
  struct radio_node *nodes;     // Radio nodes, a list
  int num_bridges, num_nodes;   // How many
  uint64_t now;                 // Virtual time, us since the start
  uint64_t duration;            // Run this long, us
  uint32_t last_mac;            // Low 3 bytes of the last MAC given
  int request_timeout_ms;       // A request that takes longer failed
  struct lan_dhcp_server dhcp;  // DHCP server on the LAN
  struct lan_tester tester;     // Test station on the LAN
};
