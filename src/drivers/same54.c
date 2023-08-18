#include "tcpip.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_SAME54) && \
    MG_ENABLE_DRIVER_SAME54

#include <sam.h>

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))
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
enum { PHY_ADDR = 0, PHY_BCR = 0, PHY_BSR = 1 };

#define PHY_BCR_DUPLEX_MODE_Msk BIT(8)
#define PHY_BCR_SPEED_Msk BIT(13)
#define PHY_BSR_LINK_STATUS_Msk BIT(2)

static uint16_t eth_read_phy(uint8_t addr, uint8_t reg) {
  GMAC_REGS->GMAC_MAN = GMAC_MAN_CLTTO_Msk |
                        GMAC_MAN_OP(2) |  // Setting the read operation
                        GMAC_MAN_WTN(2) | GMAC_MAN_PHYA(addr) |  // PHY address
                        GMAC_MAN_REGA(reg);  // Setting the register
  while (!(GMAC_REGS->GMAC_NSR & GMAC_NSR_IDLE_Msk)) (void) 0;
  return GMAC_REGS->GMAC_MAN & GMAC_MAN_DATA_Msk;  // Getting the read value
}

#if 0
static void eth_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  GMAC_REGS->GMAC_MAN = GMAC_MAN_CLTTO_Msk | GMAC_MAN_OP(1) |   // Setting the write operation
                        GMAC_MAN_WTN(2) | GMAC_MAN_PHYA(addr) | // PHY address
                        GMAC_MAN_REGA(reg) | GMAC_MAN_DATA(val);  // Setting the register
  while (!(GMAC_REGS->GMAC_NSR & GMAC_NSR_IDLE_Msk)); // Waiting until the write op is complete
}
#endif

int get_clock_rate(struct mg_tcpip_driver_same54_data *d) {
  if (d && d->mdc_cr >= 0 && d->mdc_cr <= 5) {
    return d->mdc_cr;
  } else {
    // get MCLK from GCLK_GENERATOR 0
    uint32_t div = 512;
    uint32_t mclk;
    if (!(GCLK_REGS->GCLK_GENCTRL[0] & GCLK_GENCTRL_DIVSEL_Msk)) {
      div = ((GCLK_REGS->GCLK_GENCTRL[0] & 0x00FF0000) >> 16);
      if (div == 0) div = 1;
    }
    switch (GCLK_REGS->GCLK_GENCTRL[0] & GCLK_GENCTRL_SRC_Msk) {
      case GCLK_GENCTRL_SRC_XOSC0_Val:
        mclk = 32000000UL; /* 32MHz */
        break;
      case GCLK_GENCTRL_SRC_XOSC1_Val:
        mclk = 32000000UL; /* 32MHz */
        break;
      case GCLK_GENCTRL_SRC_OSCULP32K_Val:
        mclk = 32000UL;
        break;
      case GCLK_GENCTRL_SRC_XOSC32K_Val:
        mclk = 32000UL;
        break;
      case GCLK_GENCTRL_SRC_DFLL_Val:
        mclk = 48000000UL; /* 48MHz */
        break;
      case GCLK_GENCTRL_SRC_DPLL0_Val:
        mclk = 200000000UL; /* 200MHz */
        break;
      case GCLK_GENCTRL_SRC_DPLL1_Val:
        mclk = 200000000UL; /* 200MHz */
        break;
      default:
        mclk = 200000000UL; /* 200MHz */
    }

    mclk /= div;
    uint8_t crs[] = {0, 1, 2, 3, 4, 5};          // GMAC->NCFGR::CLK values
    uint8_t dividers[] = {8, 16, 32, 48, 64, 128};  // Respective CLK dividers
    for (int i = 0; i < 6; i++) {
      if (mclk / dividers[i] <= 2375000UL /* 2.5MHz - 5% */) {
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

  MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_GMAC_Msk;
  MCLK_REGS->MCLK_AHBMASK |= MCLK_AHBMASK_GMAC_Msk;
  GMAC_REGS->GMAC_NCFGR = GMAC_NCFGR_CLK(get_clock_rate(d));  // Set MDC divider
  GMAC_REGS->GMAC_NCR = 0;                            // Disable RX & TX
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_MPE_Msk;            // Enable MDC & MDIO

  for (int i = 0; i < ETH_DESC_CNT; i++) {   // Init TX descriptors
    s_txdesc[i][0] = (uint32_t) s_txbuf[i];  // Point to data buffer
    s_txdesc[i][1] = BIT(31);                // OWN bit
  }
  s_txdesc[ETH_DESC_CNT - 1][1] |= BIT(30);  // Last tx descriptor - wrap

  GMAC_REGS->GMAC_DCFGR = GMAC_DCFGR_DRBS(0x18);  // DMA recv buf 1536
  for (int i = 0; i < ETH_DESC_CNT; i++) {        // Init RX descriptors
    s_rxdesc[i][0] = (uint32_t) s_rxbuf[i];       // Address of the data buffer
    s_rxdesc[i][1] = 0;                           // Clear status
  }
  s_rxdesc[ETH_DESC_CNT - 1][0] |= BIT(1);  // Last rx descriptor - wrap

  GMAC_REGS->GMAC_TBQB = (uint32_t) s_txdesc;  // about the descriptor addresses
  GMAC_REGS->GMAC_RBQB = (uint32_t) s_rxdesc;  // Let the controller know

  GMAC_REGS->SA[0].GMAC_SAB =
      MG_U32(ifp->mac[3], ifp->mac[2], ifp->mac[1], ifp->mac[0]);
  GMAC_REGS->SA[0].GMAC_SAT = MG_U32(0, 0, ifp->mac[5], ifp->mac[4]);

  GMAC_REGS->GMAC_UR &= ~GMAC_UR_MII_Msk;  // Disable MII, use RMII
  GMAC_REGS->GMAC_NCFGR |= GMAC_NCFGR_MAXFS_Msk | GMAC_NCFGR_MTIHEN_Msk |
                           GMAC_NCFGR_EFRHD_Msk | GMAC_NCFGR_CAF_Msk;
  GMAC_REGS->GMAC_TSR = GMAC_TSR_HRESP_Msk | GMAC_TSR_UND_Msk |
                        GMAC_TSR_TXCOMP_Msk | GMAC_TSR_TFC_Msk |
                        GMAC_TSR_TXGO_Msk | GMAC_TSR_RLE_Msk |
                        GMAC_TSR_COL_Msk | GMAC_TSR_UBR_Msk;
  GMAC_REGS->GMAC_RSR = GMAC_RSR_HNO_Msk | GMAC_RSR_RXOVR_Msk |
                        GMAC_RSR_REC_Msk | GMAC_RSR_BNA_Msk;
  GMAC_REGS->GMAC_IDR = ~0U;  // Disable interrupts, then enable required
  GMAC_REGS->GMAC_IER = GMAC_IER_HRESP_Msk | GMAC_IER_ROVR_Msk |
                        GMAC_IER_TCOMP_Msk | GMAC_IER_TFC_Msk |
                        GMAC_IER_RLEX_Msk | GMAC_IER_TUR_Msk |
                        GMAC_IER_RXUBR_Msk | GMAC_IER_RCOMP_Msk;
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_TXEN_Msk | GMAC_NCR_RXEN_Msk;
  NVIC_EnableIRQ(GMAC_IRQn);

  return true;
}

static size_t mg_tcpip_driver_same54_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if ((s_txdesc[s_txno][1] & BIT(31)) == 0) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors"));
    len = 0;  // All descriptors are busy, fail
  } else {
    uint32_t status = len | BIT(15);  // Frame length, last chunk
    if (s_txno == ETH_DESC_CNT - 1) status |= BIT(30);  // wrap
    memcpy(s_txbuf[s_txno], buf, len);                  // Copy data
    s_txdesc[s_txno][1] = status;
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  __DSB();  // Ensure descriptors have been written
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_TSTART_Msk;  // Enable transmission
  return len;
}

static bool mg_tcpip_driver_same54_up(struct mg_tcpip_if *ifp) {
  uint16_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  bool up = bsr & PHY_BSR_LINK_STATUS_Msk ? 1 : 0;

  // If PHY is ready, update NCFGR accordingly
  if (ifp->state == MG_TCPIP_STATE_DOWN && up) {
    uint16_t bcr = eth_read_phy(PHY_ADDR, PHY_BCR);
    bool fd = bcr & PHY_BCR_DUPLEX_MODE_Msk ? 1 : 0;
    bool spd = bcr & PHY_BCR_SPEED_Msk ? 1 : 0;
    GMAC_REGS->GMAC_NCFGR |= GMAC_NCFGR_SPD(spd) | GMAC_NCFGR_FD(fd);
  }

  return up;
}

void GMAC_Handler(void);
void GMAC_Handler(void) {
  uint32_t isr = GMAC_REGS->GMAC_ISR;
  uint32_t rsr = GMAC_REGS->GMAC_RSR;
  uint32_t tsr = GMAC_REGS->GMAC_TSR;
  if (isr & GMAC_ISR_RCOMP_Msk) {
    if (rsr & GMAC_ISR_RCOMP_Msk) {
      for (uint8_t i = 0; i < ETH_DESC_CNT; i++) {
        if ((s_rxdesc[s_rxno][0] & BIT(0)) == 0) break;
        size_t len = s_rxdesc[s_rxno][1] & (BIT(13) - 1);
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len, s_ifp);
        s_rxdesc[s_rxno][0] &= ~BIT(0);  // Disown
        if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
      }
    }
  }

  if ((tsr & (GMAC_TSR_HRESP_Msk | GMAC_TSR_UND_Msk | GMAC_TSR_TXCOMP_Msk |
              GMAC_TSR_TFC_Msk | GMAC_TSR_TXGO_Msk | GMAC_TSR_RLE_Msk |
              GMAC_TSR_COL_Msk | GMAC_TSR_UBR_Msk)) != 0) {
    // MG_INFO((" --> %#x %#x", s_txdesc[s_txno][1], tsr));
    if (!(s_txdesc[s_txno][1] & BIT(31))) s_txdesc[s_txno][1] |= BIT(31);
  }

  GMAC_REGS->GMAC_RSR = rsr;
  GMAC_REGS->GMAC_TSR = tsr;
}

struct mg_tcpip_driver mg_tcpip_driver_same54 = {
    mg_tcpip_driver_same54_init, mg_tcpip_driver_same54_tx, NULL,
    mg_tcpip_driver_same54_up};
#endif
