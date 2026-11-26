#pragma once
#include "sdkconfig.h"
#define MG_ARCH MG_ARCH_ESP32
#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0
#define MG_ENABLE_DRIVER_ESP32 1
#define MG_RADIO MG_RADIO_802154  // The driver supports 802.15.4 only
#define MG_TLS MG_TLS_NONE
#define MG_RADIO_CHANNEL CONFIG_BRIDGE_RADIO_CHANNEL
