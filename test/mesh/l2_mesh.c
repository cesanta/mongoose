// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "l2_mesh.h"

// Forward queued frames without learning or proxy ARP.
bool mg_mesh_node_init(struct mg_mesh_node *node, struct mg_tcpip_if *radio,
                       struct mg_tcpip_if *lan, uint16_t radio_address) {
  memset(node, 0, sizeof(*node));
  node->radio = radio;
  node->lan = lan;
  node->radio_address = radio_address;
  node->next_tick = UINT64_MAX;
  node->ram_used = node->ram_peak = sizeof(*node);
  return true;
}

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
