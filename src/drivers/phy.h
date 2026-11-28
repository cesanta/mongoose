#pragma once

#include "net_builtin.h"

struct mg_phy {
  uint16_t (*read_reg)(uint8_t addr, uint8_t reg);
  void (*write_reg)(uint8_t addr, uint8_t reg, uint16_t value);
};

// PHY configuration settings, bitmask
enum {
  // Set if PHY LEDs are connected to ground
  MG_PHY_LEDS_ACTIVE_HIGH = (1 << 0),
  // Set on RMII when PHY clocks MAC. Otherwise, MAC clocks PHY
  MG_PHY_CLOCKS_MAC = (1 << 1),
  // Set on RGMII. Otherwise, we assume RMII
  MG_PHY_RGMII = (1 << 2)
};

enum { MG_PHY_SPEED_10M, MG_PHY_SPEED_100M, MG_PHY_SPEED_1000M };

void mg_phy_init(struct mg_phy *, uint8_t addr, uint8_t config);
bool mg_phy_up(struct mg_phy *, uint8_t addr, bool *full_duplex,
               uint8_t *speed);
