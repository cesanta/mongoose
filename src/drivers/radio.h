// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Ethernet over radio: shared framing, scheduling and bridge forwarding.
// Hardware drivers pass their functions to mg_radio_init().

#pragma once

#include "net_builtin.h"

#define MG_RADIO_802154 1  // IEEE 802.15.4, 250 kbit/s
#define MG_RADIO_BLE 2     // Bluetooth LE 2 Mbit/s PHY, raw: no BLE link layer

#ifndef MG_RADIO
#define MG_RADIO MG_RADIO_802154
#endif

// The radio network: a channel, and an id that keeps other networks out.
// A bridge and its nodes must use the same values
#if MG_RADIO == MG_RADIO_BLE
#ifndef MG_RADIO_CHANNEL
#define MG_RADIO_CHANNEL 36  // Data channel 0-36. 36: 2478 MHz, clear of Wi-Fi
#endif
#ifndef MG_RADIO_NETWORK
#define MG_RADIO_NETWORK 0x71764129  // Access address
#endif
#else
#ifndef MG_RADIO_CHANNEL
#define MG_RADIO_CHANNEL 15  // Channel 11-26
#endif
#ifndef MG_RADIO_NETWORK
#define MG_RADIO_NETWORK 0xabcd  // PAN ID
#endif
#endif

// A turn's answer must arrive this soon after the turn's last frame ends on
// air, or the turn ends. A node that would answer later stays silent and keeps
// its frame for the next turn. Covers the node's poll latency, the radio
// turnaround and one full fragment's airtime, 4.3 ms on 802.15.4
#ifndef MG_RADIO_ANSWER_TIMEOUT_US
#define MG_RADIO_ANSWER_TIMEOUT_US 8000
#endif

// A radio driver aborts a transmission that has not finished this soon, and
// reports it done to mg_radio_poll()
#ifndef MG_RADIO_TX_TIMEOUT_US
#define MG_RADIO_TX_TIMEOUT_US 60000
#endif

// Membership timeout. The bridge removes a member that has not answered its
// turns for this long, counted from the first missed turn. A node leaves the
// mesh when it has had no turn for this long
#ifndef MG_RADIO_TIMEOUT_MS
#define MG_RADIO_TIMEOUT_MS 1000
#endif

#if MG_ENABLE_TCPIP

bool mg_radio_init(struct mg_tcpip_if *ifp,
                   bool (*hw_init)(struct mg_tcpip_if *, uint16_t addr),
                   bool (*hw_poll)(struct mg_tcpip_if *),
                   size_t (*hw_tx)(struct mg_tcpip_if *, const void *, size_t));
bool mg_radio_poll(struct mg_tcpip_if *, bool s1);  // Run it. true: joined
size_t mg_radio_tx(const void *, size_t, struct mg_tcpip_if *);  // Queue
size_t mg_radio_rx(void *, size_t, struct mg_tcpip_if *);  // Received frame

// Bridge LAN and radio Ethernet frames. Call after both interfaces initialize.
void mg_radio_bridge(struct mg_tcpip_if *, struct mg_tcpip_if *lan);

// Queue a received radio frame with its arrival time. Call it from the
// receive interrupt. Frames for other radios are skipped
void mg_radio_rx_queue(struct mg_tcpip_if *, const void *, size_t);

#endif
