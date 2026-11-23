#pragma once

#include <esp_mac.h>

// See https://mongoose.ws/docs/getting-started/build-options/
#define MG_ARCH MG_ARCH_ESP32
#define MG_TLS MG_TLS_BUILTIN
#define MQTT_CLIENT_ID "esp32c6"  // For tutorials/mqtt/mqtt-client

#define MG_ENABLE_TCPIP 1                // Built-in TCP/IP stack, not lwIP
#define MG_ENABLE_DRIVER_ESP32 1  // ESP32-C6 802.15.4 radio
#define MG_RADIO MG_RADIO_802154  // The driver supports 802.15.4 only
#define MG_ENABLE_TCPIP_TCPRTX 1         // Link can be lossy, enable retransmit

// Stable MAC, so the DHCP address stays the same across reboots: the factory
// MAC, locally administered
#define MG_SET_MAC_ADDRESS(mac)      \
  do {                               \
    esp_read_mac(mac, ESP_MAC_BASE); \
    (mac)[0] |= 2;                   \
  } while (0)

// mdash.net device management service support
#define MG_ENABLE_MDASH 0
#define MG_MDASH_KEY "..."  // Set this - mDash device token
#define MG_FIRMWARE_VERSION "esp32c6.1.0.0"
