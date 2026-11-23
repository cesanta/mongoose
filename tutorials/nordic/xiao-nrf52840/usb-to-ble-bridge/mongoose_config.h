#pragma once

#include "nrf.h"

#define MG_ARCH MG_ARCH_ARMGCC
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0  // main.c sets up both interfaces
#define MG_ENABLE_CUSTOM_MICROS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_DRIVER_NRF52 1      // Radio: the mesh side
#define MG_ENABLE_DRIVER_USB_NRF52 1  // nRF52840 USB controller: the LAN side
#define MG_ENABLE_USB_ETHERNET 1      // USB NCM Ethernet interface
#define MG_ENABLE_USB_SERIAL 1        // USB serial log console

// #define MG_RADIO MG_RADIO_BLE

// Stable, locally administered MAC derived from the factory device ID
#define MG_SET_MAC_ADDRESS(mac)                                       \
  do {                                                                \
    uint32_t id_[2] = {NRF_FICR->DEVICEID[0], NRF_FICR->DEVICEID[1]}; \
    uint8_t hash_[32];                                                \
    mg_sha256(hash_, (uint8_t *) id_, sizeof(id_));                   \
    memcpy(mac, hash_, 6);                                            \
    (mac)[0] = ((mac)[0] & 0xfc) | 2;                                 \
  } while (0)
