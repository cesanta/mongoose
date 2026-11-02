// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Ethernet over radio, through a mesh. Mongoose sees an Ethernet interface,
// its driver splits Ethernet frames into radio frames. Two parts plug in:
// - struct mg_radio: a radio standard, e.g. mg_radio_802154
// - struct mg_mesh: how the mesh is built and routed, e.g.
// mg_mesh_a

#pragma once

#include "mongoose.h"

// Monotonic time in microseconds.
uint64_t mg_micros(void);

// Mesh header, after the radio header: type, then burst: the fragment's
// index << 4 | the last fragment's index. Common to every radio
#define MG_MESH_HDR_LEN 2
enum { MG_MESH_JOIN_DOWN = 1, MG_MESH_JOIN_UP };

// A radio standard
struct mg_radio {
  const char *name;        // For logs
  size_t max_frame_size;   // Largest radio frame, bytes
  uint32_t bitrate;        // Bits per second on air
  size_t overhead;         // Bytes on air besides the frame: preamble, FCS
  uint32_t slot_us;        // Random backoff slot duration, us
  uint32_t cca_us;         // Channel sensing duration, us
  uint32_t turnaround_us;  // Idle check to transmission start, us
  uint32_t ifs_us;         // Inter-frame spacing in us: gap between transmits
  uint8_t backoff_min;     // Initial backoff exponent
  uint8_t backoff_max;     // Maximum backoff exponent
  bool csma;               // Use channel sensing and backoff before TX
  size_t header_len;       // Radio hardware header length
  // Write the radio header, with 0xffff as the broadcast address.
  void (*print_hdr)(const struct mg_radio *radio, uint8_t *buf, uint16_t dst,
                    uint16_t src);
  // Return the parsed header length, or zero for an invalid header.
  size_t (*parse_hdr)(const struct mg_radio *radio, const uint8_t *buf,
                      size_t len, uint16_t *dst, uint16_t *src);
};

#define MG_802154_HEADER_LEN 9   // MAC header, short addresses
#define MG_802154_PAN_ID 0xabcd  // Our network's PAN ID

extern const struct mg_radio mg_radio_802154;

struct mg_mesh_node {
  struct mg_tcpip_if *radio;  // Interface for raw radio frames
  struct mg_tcpip_if *lan;    // Bridge LAN interface, NULL for a client
  void *data;                 // Private state owned by the strategy
  size_t ram_used;            // Live mesh state bytes
  size_t ram_peak;            // Peak live mesh state bytes
  uint64_t next_tick;         // Next deadline in us, or UINT64_MAX
  uint16_t radio_address;     // Local address supplied by the radio
  bool joined;                // Client is a member of the mesh
  bool proxy_arp;             // Bridge may answer LAN ARP for members
};

struct mg_mesh {
  const char *name;
  bool (*init)(struct mg_mesh_node *node, struct mg_mgr *mgr,
               struct mg_tcpip_if *radio, struct mg_tcpip_if *lan,
               uint16_t radio_address);
  void (*tick)(struct mg_mesh_node *node, uint64_t now_microseconds);
  void (*free)(struct mg_mesh_node *node);
  bool single_bridge;            // Require exactly one bridge
  bool scheduled_tx;             // The mesh schedules transmission turns
  const struct mg_radio *radio;  // Radio configuration for this strategy
};

bool mg_mesh_node_init(struct mg_mesh_node *node, struct mg_tcpip_if *radio,
                       struct mg_tcpip_if *lan, uint16_t radio_address);

extern const struct mg_mesh mg_mesh_a;
extern const struct mg_mesh mg_mesh_b;
