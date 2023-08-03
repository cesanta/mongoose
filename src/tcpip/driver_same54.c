#include "tcpip.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_SAME54) && \
    MG_ENABLE_DRIVER_SAME54

#include <sam.h>

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))
#define ETH_PKT_SIZE 1536  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 2           // Descriptor size (words)

static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE] __attribute__((aligned(8)));
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE] __attribute__((aligned(8)));
static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];  // RX descriptors
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];  // TX descriptors
static uint8_t s_txno;                           // Current TX descriptor
static uint8_t s_rxno;                           // Current RX descriptor

static struct mg_tcpip_if *s_ifp;  // MIP interface
enum { PHY_ADDR = 0, PHY_BCR = 0, PHY_BSR = 1 };

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

static bool mg_tcpip_driver_same54_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_same54_data *d =
      (struct mg_tcpip_driver_same54_data *) ifp->driver_data;
  s_ifp = ifp;

  MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_GMAC_Msk;
  MCLK_REGS->MCLK_AHBMASK |= MCLK_AHBMASK_GMAC_Msk;
  GMAC_REGS->GMAC_NCFGR = GMAC_NCFGR_CLK(d->mdc_cr);  // Set MDC divider
  GMAC_REGS->GMAC_NCR = 0;                            // Disable RX & TX
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_MPE_Msk;            // Enable MDC & MDIO

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i][0] = (uint32_t) s_txbuf[i];  // Point to data buffer
    s_txdesc[i][1] = BIT(31);                // OWN bit
  }
  s_txdesc[ETH_DESC_CNT - 1][1] |= BIT(30);  // Last tx descriptor - wrap

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = (uint32_t) s_rxbuf[i];  // Address of the data buffer
    s_rxdesc[i][1] = 0;                      // Clear status
  }
  s_rxdesc[ETH_DESC_CNT - 1][0] |= BIT(1);  // Last rx descriptor

  GMAC_REGS->GMAC_TBQB = (uint32_t) s_txdesc;  // about the descriptor addresses
  GMAC_REGS->GMAC_RBQB = (uint32_t) s_rxdesc;  // Let the controller know

  // Select RMII operation mode
  GMAC_REGS->GMAC_UR &= ~GMAC_UR_MII_Msk;

  // Configure the receive filter
  GMAC_REGS->GMAC_NCFGR |= GMAC_NCFGR_MAXFS_Msk | GMAC_NCFGR_MTIHEN_Msk |
                           GMAC_NCFGR_SPD_Msk | GMAC_NCFGR_FD_Msk;

  // Clear transmit status register
  GMAC_REGS->GMAC_TSR = GMAC_TSR_HRESP_Msk | GMAC_TSR_UND_Msk |
                        GMAC_TSR_TXCOMP_Msk | GMAC_TSR_TFC_Msk |
                        GMAC_TSR_TXGO_Msk | GMAC_TSR_RLE_Msk |
                        GMAC_TSR_COL_Msk | GMAC_TSR_UBR_Msk;

  // Clear receive status register
  GMAC_REGS->GMAC_RSR = GMAC_RSR_HNO_Msk | GMAC_RSR_RXOVR_Msk |
                        GMAC_RSR_REC_Msk | GMAC_RSR_BNA_Msk;

  // First disable all GMAC interrupts
  GMAC_REGS->GMAC_IDR = ~0U;

  // Only the desired ones are enabled
  GMAC_REGS->GMAC_IER = GMAC_IER_HRESP_Msk | GMAC_IER_ROVR_Msk |
                        GMAC_IER_TCOMP_Msk | GMAC_IER_TFC_Msk |
                        GMAC_IER_RLEX_Msk | GMAC_IER_TUR_Msk |
                        GMAC_IER_RXUBR_Msk | GMAC_IER_RCOMP_Msk;

  // Enable the GMAC to transmit and receive data
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
  // MG_INFO(("BSR: 0x%x", bsr));
  bool up = bsr & BIT(2) ? 1 : 0;
  (void) ifp;
  return up;
}

void GMAC_Handler(void);
void GMAC_Handler(void) {
  uint32_t isr = GMAC_REGS->GMAC_ISR;
  uint32_t rsr = GMAC_REGS->GMAC_RSR;
  uint32_t tsr = GMAC_REGS->GMAC_TSR;
  MG_INFO(("ISR: 0x%x, TSR: 0x%x, RSR: 0x%x", isr, tsr, rsr));
  if (isr & BIT(1)) {
    if (rsr & BIT(1)) {
      for (int i = 0; i < 10; i++) {
        if ((s_rxdesc[s_rxno][0] & BIT(0)) == 0) break;
        uint32_t len = s_rxdesc[s_rxno][1] & (BIT(14) - 1);
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len, s_ifp);
        s_rxdesc[s_rxno][0] &= ~BIT(0);  // Disown
        if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
      }
    }
  }

  if ((tsr & (GMAC_TSR_HRESP_Msk | GMAC_TSR_UND_Msk | GMAC_TSR_TXCOMP_Msk |
              GMAC_TSR_TFC_Msk | GMAC_TSR_TXGO_Msk | GMAC_TSR_RLE_Msk |
              GMAC_TSR_COL_Msk | GMAC_TSR_UBR_Msk)) != 0) {
    GMAC_REGS->GMAC_TSR = tsr;
    MG_INFO((" --> %#x %#x", s_txdesc[s_txno][1], tsr));
    if (!(s_txdesc[s_txno][1] & BIT(31))) s_txdesc[s_txno][1] |= BIT(31);
  }

  GMAC_REGS->GMAC_RSR = rsr;
  GMAC_REGS->GMAC_TSR = tsr;
}

struct mg_tcpip_driver mg_tcpip_driver_same54 = {
    mg_tcpip_driver_same54_init, mg_tcpip_driver_same54_tx, NULL,
    mg_tcpip_driver_same54_up};
#endif
