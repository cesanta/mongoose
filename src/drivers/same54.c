#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_SAME54) && \
    MG_ENABLE_DRIVER_SAME54

struct GMAC_REGS_ {
  uint32_t GMAC_NCR, GMAC_NCFGR, GMAC_NSR, GMAC_UR, GMAC_DCFGR, GMAC_TSR,
      GMAC_RBQB, GMAC_TBQB, GMAC_RSR, GMAC_ISR, GMAC_IER, GMAC_IDR, GMAC_IMR,
      GMAC_MAN, GMAC_RPQ, GMAC_TPQ, GMAC_TPSF, GMAC_RPSF, GMAC_RJFML,
      Reserved1[13], GMAC_HRB, GMAC_HRT, GMAC_SAB0, GMAC_SAT0, GMAC_SAB1,
      GMAC_SAT1, GMAC_SAB2, GMAC_SAT2, GMAC_SAB3, GMAC_SAT3, GMAC_TIDM[4],
      GMAC_WOL, GMAC_IPGS, GMAC_SVLAN, GMAC_TPFCP, GMAC_SAMB1, GMAC_SAMT1,
      Reserved2[3], GMAC_NSC, GMAC_SCL, GMAC_SCH, GMAC_EFTSH, GMAC_EFRSH,
      GMAC_PEFTSH, GMAC_PEFRSH, Reserved3[2], GMAC_OTLO, GMAC_OTHI, GMAC_FT,
      GMAC_BCFT, GMAC_MFT, GMAC_PFT, GMAC_BFT64, GMAC_TBFT127, GMAC_TBFT255,
      GMAC_TBFT511, GMAC_TBFT1023, GMAC_TBFT1518, GMAC_GTBFT1518, GMAC_TUR,
      GMAC_SCF, GMAC_MCF, GMAC_EC, GMAC_LC, GMAC_DTF, GMAC_CSE, GMAC_ORLO,
      GMAC_ORHI, GMAC_FR, GMAC_BCFR, GMAC_MFR, GMAC_PFR, GMAC_BFR64,
      GMAC_TBFR127, GMAC_TBFR255, GMAC_TBFR511, GMAC_TBFR1023, GMAC_TBFR1518,
      GMAC_TMXBFR, GMAC_UFR, GMAC_OFR, GMAC_JR, GMAC_FCSE, GMAC_LFFE, GMAC_RSE,
      GMAC_AE, GMAC_RRE, GMAC_ROE, GMAC_IHCE, GMAC_TCE, GMAC_UCE, Reserved4[2],
      GMAC_TISUBN, GMAC_TSH, Reserved5, GMAC_TSSSL, GMAC_TSSN, GMAC_TSL,
      GMAC_TN, GMAC_TA, GMAC_TI, GMAC_EFTSL, GMAC_EFTN, GMAC_EFRSL, GMAC_EFRN,
      GMAC_PEFTSL, GMAC_PEFTN, GMAC_PEFRSL, GMAC_PEFRN, Reserved6[28],
      GMAC_RLPITR, GMAC_RLPITI, GMAC_TLPITR, GMAC_TLPITI;
};

struct GCLK_REGS_ {
  uint32_t GCLK_CTRLA_RESERVED, GCLK_SYNCBUSY, Reserved2[6], GCLK_GENCTRL[12],
      Reserved3[12], GCLK_PCHCTRL[48];
};

#define GMAC_REGS ((struct GMAC_REGS_ *) 0x42000800)
#define GCLK_REGS ((struct GCLK_REGS_ *) 0x40001c00)

#define ETH_PKT_SIZE 1536  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 2           // Descriptor size (words)

static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE];
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE];
static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];  // RX descriptors
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];  // TX descriptors
static uint8_t s_txno;                           // Current TX descriptor
static uint8_t s_rxno;                           // Current RX descriptor

static struct mg_tcpip_if *s_ifp;  // MIP interface

static uint16_t eth_read_phy(uint8_t addr, uint8_t reg) {
  GMAC_REGS->GMAC_MAN = MG_BIT(30) |  // Clause 22
                        MG_BIT(29) |  // Setting the read operation
                        MG_BIT(17) | ((addr & 0x1f) << 23) |  // PHY address
                        ((reg & 0x1f) << 18);  // Setting the register
  while (!(GMAC_REGS->GMAC_NSR & MG_BIT(2))) (void) 0;
  return GMAC_REGS->GMAC_MAN & 0xffff;  // Getting the read value
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  GMAC_REGS->GMAC_MAN = MG_BIT(30) |
                        MG_BIT(28) |  // Setting the write operation
                        MG_BIT(17) | ((addr & 0x1f) << 23) |  // PHY address
                        ((reg & 0x1f) << 18) | val;  // Setting the register
  while (!(GMAC_REGS->GMAC_NSR & MG_BIT(2)))
    ;  // Waiting until the write op is complete
}

uint32_t get_clock_rate(struct mg_tcpip_driver_same54_data *d) {
  if (d && d->mdc_cr >= 0 && d->mdc_cr <= 5) {
    return d->mdc_cr;
  } else {
    // get MCLK from GCLK_GENERATOR 0
    uint32_t div = 512;
    uint32_t mclk;
    if (!(GCLK_REGS->GCLK_GENCTRL[0] & MG_BIT(12))) {
      div = ((GCLK_REGS->GCLK_GENCTRL[0] & 0x00FF0000) >> 16);
      if (div == 0) div = 1;
    }
    switch (GCLK_REGS->GCLK_GENCTRL[0] & 0xF) {
      case 0:               // GCLK_GENCTRL_SRC_XOSC0_Val
        mclk = 32000000UL;  // 32MHz
        break;
      case 1:               // GCLK_GENCTRL_SRC_XOSC1_Val
        mclk = 32000000UL;  // 32MHz
        break;
      case 4:            // GCLK_GENCTRL_SRC_OSCULP32K_Val
        mclk = 32000UL;  // 32Khz
        break;
      case 5:            // GCLK_GENCTRL_SRC_XOSC32K_Val
        mclk = 32000UL;  // 32Khz
        break;
      case 6:               // GCLK_GENCTRL_SRC_DFLL_Val
        mclk = 48000000UL;  // 48MHz
        break;
      case 7:                // GCLK_GENCTRL_SRC_DPLL0_Val:
        mclk = 200000000UL;  // 200MHz
        break;
      case 8:                // GCLK_GENCTRL_SRC_DPLL1_Val
        mclk = 200000000UL;  // 200MHz
        break;
      default:
        mclk = 200000000UL;  // 200MHz
    }

    mclk /= div;
    uint8_t crs[] = {0, 1, 2, 3, 4, 5};            // GMAC->NCFGR::CLK values
    uint8_t dividers[] = {8, 16, 32, 48, 64, 96};  // Respective CLK dividers
    for (int i = 0; i < 6; i++) {
      if (mclk / dividers[i] <= 2375000UL) {  // 2.5MHz - 5%
        return crs[i];
      }
    }

    return 5;
  }
}

static bool mg_tcpip_driver_same54_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_same54_data *d =
      (struct mg_tcpip_driver_same54_data *) ifp->driver_data;
  s_ifp = ifp;

  GMAC_REGS->GMAC_NCFGR = get_clock_rate(d) << 18;  // Set MDC divider
  GMAC_REGS->GMAC_NCR = 0;                          // Disable RX & TX
  GMAC_REGS->GMAC_NCR |= MG_BIT(4);                 // Enable MDC & MDIO

  struct mg_phy phy = {eth_read_phy, eth_write_phy};
  mg_phy_init(&phy, d->phy_addr, 0);

  for (int i = 0; i < ETH_DESC_CNT; i++) {   // Init TX descriptors
    s_txdesc[i][0] = (uint32_t) s_txbuf[i];  // Point to data buffer
    s_txdesc[i][1] = MG_BIT(31);             // OWN bit
  }
  s_txdesc[ETH_DESC_CNT - 1][1] |= MG_BIT(30);  // Last tx descriptor - wrap

  GMAC_REGS->GMAC_DCFGR = (0x18 << 16) |     // DMA recv buf 1536
                          (3 << 8) |         // RXBMS
                          MG_BIT(10);        // See #2487
  for (int i = 0; i < ETH_DESC_CNT; i++) {   // Init RX descriptors
    s_rxdesc[i][0] = (uint32_t) s_rxbuf[i];  // Address of the data buffer
    s_rxdesc[i][1] = 0;                      // Clear status
  }
  s_rxdesc[ETH_DESC_CNT - 1][0] |= MG_BIT(1);  // Last rx descriptor - wrap

  GMAC_REGS->GMAC_TBQB = (uint32_t) s_txdesc;  // about the descriptor addresses
  GMAC_REGS->GMAC_RBQB = (uint32_t) s_rxdesc;  // Let the controller know

  GMAC_REGS->GMAC_SAB0 =
      MG_U32(ifp->mac[3], ifp->mac[2], ifp->mac[1], ifp->mac[0]);
  GMAC_REGS->GMAC_SAT0 = MG_U32(0, 0, ifp->mac[5], ifp->mac[4]);

  GMAC_REGS->GMAC_UR &= ~MG_BIT(0);                 // Disable MII, use RMII
  GMAC_REGS->GMAC_NCFGR |= MG_BIT(8) | MG_BIT(6) |  // MAXFX, MTIHEN
                           MG_BIT(25) | MG_BIT(4);  // EFRHD, CAF
  GMAC_REGS->GMAC_TSR = 0x17f;                      // all transmit statuses
  GMAC_REGS->GMAC_RSR = 0xf;                        // all recv statuses
  GMAC_REGS->GMAC_IDR = ~0U;  // Disable interrupts, then enable required
  GMAC_REGS->GMAC_IER = MG_BIT(11) | MG_BIT(10) |  // HRESP, ROVR
                        MG_BIT(7) | MG_BIT(6) |    // TCOMP, TFC
                        MG_BIT(5) | MG_BIT(4) |    // RLEX, TUR
                        MG_BIT(2) | MG_BIT(1);     // RXUBR, RCOMP
  GMAC_REGS->GMAC_NCR |= MG_BIT(3) | MG_BIT(2);    // TXEN, RXEN

  return true;
}

static size_t mg_tcpip_driver_same54_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if ((s_txdesc[s_txno][1] & MG_BIT(31)) == 0) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors"));
    len = 0;  // All descriptors are busy, fail
  } else {
    uint32_t status = len | MG_BIT(15);  // Frame length, last chunk
    if (s_txno == ETH_DESC_CNT - 1) status |= MG_BIT(30);  // wrap
    memcpy(s_txbuf[s_txno], buf, len);                     // Copy data
    s_txdesc[s_txno][1] = status;
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  MG_DSB();                          // Ensure descriptors have been written
  GMAC_REGS->GMAC_NCR |= MG_BIT(9);  // Enable transmission
  return len;
}

static void mg_tcpip_driver_same54_update_hash_table(struct mg_tcpip_if *ifp) {
  // TODO(): read database, rebuild hash table
  // Setting Hash Index for 01:00:5e:00:00:fb (multicast)
  // 24.6.9 Hash addressing
  // computed hash is 55, which means bit 23 (55 - 32) in
  // HRT register must be set
  GMAC_REGS->GMAC_HRT = MG_BIT(23);
  GMAC_REGS->GMAC_NCFGR |= MG_BIT(6);  // enable multicast hash filtering
  (void) ifp;
}

static bool mg_tcpip_driver_same54_poll(struct mg_tcpip_if *ifp, bool s1) {
  if (ifp->update_mac_hash_table) {
    mg_tcpip_driver_same54_update_hash_table(ifp);
    ifp->update_mac_hash_table = false;
  }

  bool up = false;
  if (s1) {
    uint8_t speed = MG_PHY_SPEED_10M;
    bool full_duplex = false;
    struct mg_phy phy = {eth_read_phy, eth_write_phy};
    up = mg_phy_up(&phy, 0, &full_duplex, &speed);

    // If PHY is ready, update NCFGR accordingly
    if (ifp->state == MG_TCPIP_STATE_DOWN && up) {
      GMAC_REGS->GMAC_NCFGR =
          (GMAC_REGS->GMAC_NCFGR & ~(MG_BIT(0) | MG_BIT(1))) | (speed & 1) |
          (full_duplex << 1);
    }
  }
  return up;
}

void GMAC_Handler(void);
void GMAC_Handler(void) {
  uint32_t isr = GMAC_REGS->GMAC_ISR;
  uint32_t rsr = GMAC_REGS->GMAC_RSR;
  uint32_t tsr = GMAC_REGS->GMAC_TSR;
  if (isr & MG_BIT(1)) {
    if (rsr & MG_BIT(1)) {
      for (uint8_t i = 0; i < ETH_DESC_CNT; i++) {
        if ((s_rxdesc[s_rxno][0] & MG_BIT(0)) == 0) break;
        size_t len = s_rxdesc[s_rxno][1] & (MG_BIT(13) - 1);
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len, s_ifp);
        s_rxdesc[s_rxno][0] &= ~MG_BIT(0);  // Disown
        if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
      }
    }
  }

  if (tsr != 0) {
    // MG_INFO((" --> %#x %#x", s_txdesc[s_txno][1], tsr));
    if (!(s_txdesc[s_txno][1] & MG_BIT(31))) s_txdesc[s_txno][1] |= MG_BIT(31);
  }

  GMAC_REGS->GMAC_RSR = rsr;
  GMAC_REGS->GMAC_TSR = tsr;
}

struct mg_tcpip_driver mg_tcpip_driver_same54 = {
    mg_tcpip_driver_same54_init, mg_tcpip_driver_same54_tx, NULL,
    mg_tcpip_driver_same54_poll};
#endif
