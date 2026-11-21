// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_ESP32_802154) && \
    MG_ENABLE_DRIVER_ESP32_802154

// ESP32-C6 IEEE 802.15.4 radio, as an Ethernet interface through the mesh.
// Needs MG_ARCH_ESP32 and CONFIG_IEEE802154_ENABLED: it uses ESP-IDF's
// esp_ieee802154 driver, and defines its esp_ieee802154_receive_done(),
// esp_ieee802154_transmit_done() and esp_ieee802154_transmit_failed()
// callbacks. See tutorials/esp32/minimal-802.15.4
struct mg_tcpip_driver_esp32_802154_data {
  struct mg_mesh_node *mesh;       // Driver-owned runtime state
  const struct mg_mesh *strategy;  // NULL selects mg_mesh_default
};

extern struct mg_tcpip_driver mg_tcpip_driver_esp32_802154;

#define MG_TCPIP_DRIVER_ESP32_802154_INIT(manager)                      \
  do {                                                                  \
    static struct mg_tcpip_driver_esp32_802154_data driver_data_;       \
    static struct mg_tcpip_if mif_;                                     \
    MG_SET_MAC_ADDRESS(mif_.mac);                                       \
    mif_.ip = MG_TCPIP_IP;                                              \
    mif_.mask = MG_TCPIP_MASK;                                          \
    mif_.gw = MG_TCPIP_GW;                                              \
    mif_.mgr = (manager);                                               \
    mif_.driver = &mg_tcpip_driver_esp32_802154;                        \
    mif_.driver_data = &driver_data_;                                   \
    mg_tcpip_init((manager), &mif_);                                    \
    MG_INFO(("Driver: esp32_802154, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(x) MG_TCPIP_DRIVER_ESP32_802154_INIT(x)
#endif

#endif
