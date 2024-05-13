#include "phy.h"

enum {                      // ID1  ID2
  MG_PHY_KSZ8x = 0x22,      // 0022 1561 - KSZ8081RNB
  MG_PHY_DP83x = 0x2000,    // 2000 a140 - TI DP83825I
  MG_PHY_DP83867 = 0xa231,  // 2000 a231 - TI DP83867I
  MG_PHY_LAN87x = 0x7,      // 0007 c0fx - LAN8720
  MG_PHY_RTL8201 = 0x1C     // 001c c816 - RTL8201
};

enum {
  MG_PHY_REG_BCR = 0,
  MG_PHY_REG_BSR = 1,
  MG_PHY_REG_ID1 = 2,
  MG_PHY_REG_ID2 = 3,
  MG_PHY_DP83x_REG_PHYSTS = 16,
  MG_PHY_DP83867_REG_PHYSTS = 17,
  MG_PHY_DP83x_REG_RCSR = 23,
  MG_PHY_DP83x_REG_LEDCR = 24,
  MG_PHY_KSZ8x_REG_PC1R = 30,
  MG_PHY_KSZ8x_REG_PC2R = 31,
  MG_PHY_LAN87x_REG_SCSR = 31,
  MG_PHY_RTL8201_REG_RMSR = 16,  // in page 7
  MG_PHY_RTL8201_REG_PAGESEL = 31
};

static const char *mg_phy_id_to_str(uint16_t id1, uint16_t id2) {
  switch (id1) {
    case MG_PHY_DP83x:
      switch (id2) {
        case MG_PHY_DP83867:
          return "DP83867";
        default:
          return "DP83x";
      }
    case MG_PHY_KSZ8x:
      return "KSZ8x";
    case MG_PHY_LAN87x:
      return "LAN87x";
    case MG_PHY_RTL8201:
      return "RTL8201";
    default:
      return "unknown";
  }
  (void) id2;
}

void mg_phy_init(struct mg_phy *phy, uint8_t phy_addr, uint8_t config) {
  uint16_t id1, id2;
  phy->write_reg(phy_addr, MG_PHY_REG_BCR, MG_BIT(15));  // Reset PHY
  while (phy->read_reg(phy_addr, MG_PHY_REG_BCR) & MG_BIT(15)) (void) 0;
  // MG_PHY_REG_BCR[12]: Autonegotiation is default unless hw says otherwise

  id1 = phy->read_reg(phy_addr, MG_PHY_REG_ID1);
  id2 = phy->read_reg(phy_addr, MG_PHY_REG_ID2);
  MG_INFO(("PHY ID: %#04x %#04x (%s)", id1, id2, mg_phy_id_to_str(id1, id2)));

  if (id1 == MG_PHY_DP83x && id2 == MG_PHY_DP83867) {
    phy->write_reg(phy_addr, 0x0d, 0x1f);    // write 0x10d to IO_MUX_CFG (0x0170)
    phy->write_reg(phy_addr, 0x0e, 0x170);
    phy->write_reg(phy_addr, 0x0d, 0x401f);
    phy->write_reg(phy_addr, 0x0e, 0x10d);
  }

  if (config & MG_PHY_CLOCKS_MAC) {
    // Use PHY crystal oscillator (preserve defaults)
    // nothing to do
  } else {  // MAC clocks PHY, PHY has no xtal
    // Enable 50 MHz external ref clock at XI (preserve defaults)
    if (id1 == MG_PHY_DP83x && id2 != MG_PHY_DP83867) {
      phy->write_reg(phy_addr, MG_PHY_DP83x_REG_RCSR, MG_BIT(7) | MG_BIT(0));
    } else if (id1 == MG_PHY_KSZ8x) {
      phy->write_reg(phy_addr, MG_PHY_KSZ8x_REG_PC2R,
                     MG_BIT(15) | MG_BIT(8) | MG_BIT(7));
    } else if (id1 == MG_PHY_LAN87x) {
      // nothing to do
    } else if (id1 == MG_PHY_RTL8201) {
      // assume PHY has been hardware strapped properly
#if 0
      phy->write_reg(phy_addr, MG_PHY_RTL8201_REG_PAGESEL, 7);  // Select page 7
      phy->write_reg(phy_addr, MG_PHY_RTL8201_REG_RMSR, 0x1ffa);
      phy->write_reg(phy_addr, MG_PHY_RTL8201_REG_PAGESEL, 0);  // Select page 0
#endif
    }
  }

  if (config & MG_PHY_LEDS_ACTIVE_HIGH && id1 == MG_PHY_DP83x) {
    phy->write_reg(phy_addr, MG_PHY_DP83x_REG_LEDCR,
                   MG_BIT(9) | MG_BIT(7));  // LED status, active high
  }  // Other PHYs do not support this feature
}

bool mg_phy_up(struct mg_phy *phy, uint8_t phy_addr, bool *full_duplex,
               uint8_t *speed) {
  bool up = false;
  uint16_t bsr = phy->read_reg(phy_addr, MG_PHY_REG_BSR);
  if ((bsr & MG_BIT(5)) && !(bsr & MG_BIT(2)))  // some PHYs latch down events
    bsr = phy->read_reg(phy_addr, MG_PHY_REG_BSR);  // read again
  up = bsr & MG_BIT(2);
  if (up && full_duplex != NULL && speed != NULL) {
    uint16_t id1 = phy->read_reg(phy_addr, MG_PHY_REG_ID1);
    if (id1 == MG_PHY_DP83x) {
      uint16_t id2 = phy->read_reg(phy_addr, MG_PHY_REG_ID2);
      if (id2 == MG_PHY_DP83867) {
        uint16_t physts = phy->read_reg(phy_addr, MG_PHY_DP83867_REG_PHYSTS);
        *full_duplex = physts & MG_BIT(13);
        *speed = (physts & MG_BIT(15))   ? MG_PHY_SPEED_1000M
                 : (physts & MG_BIT(14)) ? MG_PHY_SPEED_100M
                                         : MG_PHY_SPEED_10M;
      } else {
        uint16_t physts = phy->read_reg(phy_addr, MG_PHY_DP83x_REG_PHYSTS);
        *full_duplex = physts & MG_BIT(2);
        *speed = (physts & MG_BIT(1)) ? MG_PHY_SPEED_10M : MG_PHY_SPEED_100M;
      }
    } else if (id1 == MG_PHY_KSZ8x) {
      uint16_t pc1r = phy->read_reg(phy_addr, MG_PHY_KSZ8x_REG_PC1R);
      *full_duplex = pc1r & MG_BIT(2);
      *speed = (pc1r & 3) == 1 ? MG_PHY_SPEED_10M : MG_PHY_SPEED_100M;
    } else if (id1 == MG_PHY_LAN87x) {
      uint16_t scsr = phy->read_reg(phy_addr, MG_PHY_LAN87x_REG_SCSR);
      *full_duplex = scsr & MG_BIT(4);
      *speed = (scsr & MG_BIT(3)) ? MG_PHY_SPEED_100M : MG_PHY_SPEED_10M;
    } else if (id1 == MG_PHY_RTL8201) {
      uint16_t bcr = phy->read_reg(phy_addr, MG_PHY_REG_BCR);
      *full_duplex = bcr & MG_BIT(8);
      *speed = (bcr & MG_BIT(13)) ? MG_PHY_SPEED_100M : MG_PHY_SPEED_10M;
    }
  }
  return up;
}
