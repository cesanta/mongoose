#pragma once

#include "arch.h"
#include "net.h"

struct mip_driver {
  void *data;                                       // Driver-specific data
  void (*init)(void *data);                         // Initialise driver
  size_t (*tx)(const void *, size_t, void *data);   // Transmit frame
  size_t (*rx)(void *buf, size_t len, void *data);  // Receive frame (polling)
  bool (*status)(void *data);                       // Up/down status
  // Set receive callback for interrupt-driven drivers
  void (*rxcb)(void (*fn)(void *buf, size_t len, void *rxdata), void *rxdata);
};

struct mip_ipcfg {
  uint8_t mac[6];         // MAC address. Must not be 0
  uint32_t ip, mask, gw;  // IP, netmask, GW. If IP is 0, DHCP is used
};

void mip_init(struct mg_mgr *, struct mip_ipcfg *, struct mip_driver *);

extern struct mip_driver mip_driver_stm32;
