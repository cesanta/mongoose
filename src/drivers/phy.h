#pragma once

#include "net_builtin.h"

struct mg_phy {
  uint16_t (*read_reg)(uint8_t addr, uint8_t reg);
  void (*write_reg)(uint8_t addr, uint8_t reg, uint16_t value);
};

// MG_TCPIP_PHY_CONF configuration settings, bitmask of the following:
#define MG_PHY_LEDS_ACTIVE_HIGH 1  // PHY LEDs are connected to ground
#define MG_PHY_CLOCKS_MAC 2        // PHY clocks MAC. Otherwise, MAC clocks PHY
#define MG_PHY_DISABLE_AUTONEG 4   // Disable autonegotiation

enum { MG_PHY_SPEED_10M, MG_PHY_SPEED_100M, MG_PHY_SPEED_1000M };

void mg_phy_init(struct mg_phy *, uint8_t addr, uint8_t config);
bool mg_phy_up(struct mg_phy *, uint8_t addr, bool *full_duplex,
               uint8_t *speed);
