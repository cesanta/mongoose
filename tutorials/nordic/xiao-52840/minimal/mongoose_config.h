#pragma once
#include "nrf.h"
#define MG_ARCH MG_ARCH_ARMGCC
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_NRF52 1
#define MG_ENABLE_TCPIP_TCPRTX 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_TLS MG_TLS_BUILTIN
#define MQTT_CLIENT_ID "xiao-nrf52840"

#ifndef MQTT_SERVER_URL
#define MQTT_SERVER_URL "mqtt://broker.emqx.io:1883"
#endif

// Stable, locally administered MAC derived from the factory device ID.
#define MG_SET_MAC_ADDRESS(mac)                                    \
  do {                                                             \
    uint32_t id_[2] = {NRF_FICR->DEVICEID[0], NRF_FICR->DEVICEID[1]};   \
    uint8_t hash_[32];                                              \
    mg_sha256(hash_, (uint8_t *) id_, sizeof(id_));                   \
    memcpy(mac, hash_, 6);                                          \
    (mac)[0] = ((mac)[0] & 0xfc) | 2;                                \
  } while (0)
