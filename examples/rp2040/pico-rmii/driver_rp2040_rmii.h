#pragma once

struct mip_driver_rp2040_rmii_data {
  uint8_t rx0;       // RX0, RX1, CRS_DV; consecutive GPIO pins
  uint8_t tx0;       // TX0, TX1, TX-EN; consecutive GPIO pins
  uint8_t mdio;      // MDIO, MDC; consecutive GPIO pins
  uint8_t phy_addr;  // PHY address
};

extern struct mip_driver mip_driver_rp2040_rmii;
