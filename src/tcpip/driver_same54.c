#include "tcpip.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_SAME54) && MG_ENABLE_DRIVER_SAME54

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))
#define ETH_PKT_SIZE 1536  // Max frame size
#define GMAC_DESC_CNT 4     // Descriptors count
#define GMAC_DS 2           // Descriptor size (words)

static uint8_t s_rxbuf[GMAC_DESC_CNT][ETH_PKT_SIZE];  // RX ethernet buffers
static uint8_t s_txbuf[GMAC_DESC_CNT][ETH_PKT_SIZE];  // TX ethernet buffers
static uint32_t s_rxdesc[GMAC_DESC_CNT][GMAC_DS];      // RX descriptors
static uint32_t s_txdesc[GMAC_DESC_CNT][GMAC_DS];      // TX descriptors
static uint8_t s_txno;                               // Current TX descriptor
static uint8_t s_rxno;                               // Current RX descriptor

static struct mg_tcpip_if *s_ifp;  // MIP interface
enum { PHY_ADDR = 0, PHY_BCR = 0, PHY_BSR = 1};

static uint16_t eth_read_phy(uint8_t addr, uint8_t reg) {
  GMAC_REGS->GMAC_MAN = GMAC_MAN_CLTTO_Msk | GMAC_MAN_OP(2) |   // Setting the read operation
                        GMAC_MAN_WTN(2) | GMAC_MAN_PHYA(addr) | // PHY address
                        GMAC_MAN_REGA(reg);   // Setting the register
  while (!(GMAC_REGS->GMAC_NSR & GMAC_NSR_IDLE_Msk)); // Waiting until the read op is complete
  return GMAC_REGS->GMAC_MAN & GMAC_MAN_DATA_Msk;   // Getting the read value
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  GMAC_REGS->GMAC_MAN = GMAC_MAN_CLTTO_Msk | GMAC_MAN_OP(1) |   // Setting the write operation
                        GMAC_MAN_WTN(2) | GMAC_MAN_PHYA(addr) | // PHY address
                        GMAC_MAN_REGA(reg) | GMAC_MAN_DATA(val);  // Setting the register
  while (!(GMAC_REGS->GMAC_NSR & GMAC_NSR_IDLE_Msk)); // Waiting until the write op is complete
}

static bool mg_tcpip_driver_same54_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_same54_data *d =
      (struct mg_tcpip_driver_same54_data *) ifp->driver_data;
  s_ifp = ifp;

  // enabling GMAC bus clocks
  MCLK_REGS->MCLK_AHBMASK |= MCLK_AHBMASK_GMAC(1);
  MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_GMAC(1);
   
  GMAC_REGS->GMAC_NCR &= ~GMAC_NCR_RXEN_Msk; // Disable receive circuit
  GMAC_REGS->GMAC_NCR &= ~GMAC_NCR_TXEN_Msk; // Disable transmit circuit

  GMAC_REGS->GMAC_DCFGR = GMAC_DCFGR_DRBS(0x18);
  GMAC_REGS->GMAC_DCFGR |= GMAC_DCFGR_TXPBMS(1);
  GMAC_REGS->GMAC_DCFGR |= GMAC_DCFGR_RXBMS(2);
  GMAC_REGS->GMAC_DCFGR |= GMAC_DCFGR_FBLDO(4);

  // Init RX descriptors
  for (int i = 0; i < GMAC_DESC_CNT; i++) {
    s_rxdesc[i][0] = ((uint32_t) s_rxbuf[i]) & 0xfffffffc;  // Point to data buffer (bits [31:2])
    s_rxdesc[i][1] = 0;
  }
  s_rxdesc[GMAC_DESC_CNT - 1][0] |= BIT(1); // Marking last rx descriptor

  // Init TX descriptors
  for (int i = 0; i < GMAC_DESC_CNT; i++) {
    s_txdesc[i][0] = (uint32_t) s_txbuf[i];  // Point to data buffer
    s_txdesc[i][1] = BIT(31); // Setting the OWN bit
  }
  s_txdesc[GMAC_DESC_CNT - 1][1] |= BIT(30); // Marking last tx descriptor

  // let the controller know about the descriptor addresses
  GMAC_REGS->GMAC_RBQB = (uint32_t) s_rxdesc;
  GMAC_REGS->GMAC_TBQB = (uint32_t) s_txdesc;

  // GPIO pin configuration
  MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_PORT_Msk; // //enable bus clock for PORT
  uint16_t pins[] = {PIN('A', 12), PIN('A', 13), PIN('A', 14), PIN('A', 15),
                     PIN('A', 17), PIN('A', 18), PIN('A', 19),
                     PIN('C', 11), PIN('C', 12),  PIN('C', 20)};

  uint32_t pin_fns[] = {
        MUX_PA12L_GMAC_GRX1, MUX_PA13L_GMAC_GRX0, MUX_PA14L_GMAC_GTXCK,
        MUX_PA15L_GMAC_GRXER, MUX_PA17L_GMAC_GTXEN, MUX_PA18L_GMAC_GTX0,
        MUX_PA19L_GMAC_GTX1, MUX_PC11L_GMAC_GMDC, MUX_PC12L_GMAC_GMDIO,
        MUX_PC20L_GMAC_GRXDV
  };

  for (uint32_t i = 0; i < sizeof(pins) / sizeof(uint16_t); i++) {
    uint8_t group = (uint8_t) (pins[i] >> 8);
    uint8_t pin_no = (uint8_t) pins[i];
    PORT_REGS->GROUP[group].PORT_PINCFG[pin_no] |= PORT_PINCFG_PMUXEN_Msk;
    if (pin_no % 2)
      PORT_REGS->GROUP[group].PORT_PMUX[pin_no / 2] |= (uint8_t) PORT_PMUX_PMUXO(pin_fns[i]);
    else
      PORT_REGS->GROUP[group].PORT_PMUX[pin_no / 2] |= (uint8_t) PORT_PMUX_PMUXE(pin_fns[i]);
  }

  PORT_REGS->GROUP[0].PORT_PINCFG[17] |= PORT_PINCFG_DRVSTR_Msk;
  PORT_REGS->GROUP[0].PORT_PINCFG[18] |= PORT_PINCFG_DRVSTR_Msk;
  PORT_REGS->GROUP[0].PORT_PINCFG[19] |= PORT_PINCFG_DRVSTR_Msk;

  GMAC_REGS->GMAC_UR &= ~GMAC_UR_MII_Msk; // RMII operation mode

  // Resetting the PHY
  /*uint64_t start = mg_millis(), now = start;
  PORT_REGS->GROUP[2].PORT_DIRSET = PORT_PC21;
  PORT_REGS->GROUP[2].PORT_OUTCLR = PORT_PC21;
  for (; now - start < 10000; now = mg_millis());
  PORT_REGS->GROUP[2].PORT_OUTSET = PORT_PC21;
  for (start = mg_millis(), now = start; now - start < 10000; now = mg_millis());*/

  // Clock advanced configuration
#if 0
  uint8_t mdc_clk_div = 5;

  // get MCLK from GCLK_GENERATOR 0
  uint32_t div = 512;
  uint32_t mclk;
  if (!(GCLK_REGS->GCLK_GENCTRL[0] & GCLK_GENCTRL_DIVSEL_Msk))
    div = ((GCLK_REGS->GCLK_GENCTRL[0] & 0x00FF0000) >> 16);
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
      mdc_clk_div = crs[i];
      break;
    }
  }

#endif

  GMAC_REGS->GMAC_NCFGR = GMAC_NCFGR_CLK(5); // set the MDC clock
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_MPE_Msk; //enable management port

  // configure MAC
  GMAC_REGS->SA[0].GMAC_SAT = ((uint32_t) ifp->mac[5] << 8U) | ifp->mac[4];
  GMAC_REGS->SA[0].GMAC_SAB = (uint32_t) (ifp->mac[3] << 24) |
                 ((uint32_t) ifp->mac[2] << 16) |
                 ((uint32_t) ifp->mac[1] << 8) | ifp->mac[0];

  //Configure the receive filter
  GMAC_REGS->GMAC_NCFGR |= GMAC_NCFGR_MAXFS_Msk;

  //GMAC_REGS->GMAC_IER = GMAC_IER_RCOMP(1); // Enable the receive interrupt
  GMAC_REGS->GMAC_IER = 0x3ffcfcff; // Enabling all interrupts
  NVIC_EnableIRQ(GMAC_IRQn);

  GMAC_REGS->GMAC_NCR |= GMAC_NCR_TXEN_Msk;  // Enable transmit circuit
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_RXEN_Msk ; // Enable receive circuit

  return true;
}

static size_t mg_tcpip_driver_same54_tx(const void *buf, size_t len,
                                       struct mg_tcpip_if *ifp) {
  //MG_INFO(("DMAC_CTRL: 0x%x", DMAC_REGS->DMAC_CTRL));
  MG_INFO(("GMAC_TSR: 0x%x", GMAC_REGS->GMAC_TSR));
  GMAC_REGS->GMAC_TSR |= 0x21;
  uint32_t rsr = GMAC_REGS->GMAC_RSR;
  MG_INFO(("GMAC_RSR: 0x%x", rsr));
  GMAC_REGS->GMAC_RSR = rsr;
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if (!(s_txdesc[s_txno][1] & BIT(31))) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors"));
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);     // Copy data
    if (++s_txno >= GMAC_DESC_CNT) {
      s_txdesc[GMAC_DESC_CNT - 1][1] = (len & (BIT(14) - 1)) | BIT(15)| BIT(30);
      s_txno = 0;
      MG_INFO(("s_tx_no: %d, tx_desc.status: 0x%x", GMAC_DESC_CNT - 1, s_txdesc[GMAC_DESC_CNT - 1][1]));
    } else {
      s_txdesc[s_txno - 1][1] = (len & (BIT(14) - 1)) | BIT(15);
      MG_INFO(("s_tx_no: %d, tx_desc.status: 0x%x", s_txno - 1, s_txdesc[s_txno - 1][1]));
    }
  }
  __DSB();  // Ensure descriptors have been written
  GMAC_REGS->GMAC_NCR |= GMAC_NCR_TSTART_Msk; // Enable transmission
  return len;
}

static bool mg_tcpip_driver_same54_up(struct mg_tcpip_if *ifp) {
  uint16_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  //MG_INFO(("BSR: 0x%x", bsr));
  bool up = bsr & BIT(2) ? 1 : 0;
  (void) ifp;
  return up;
}

void GMAC_Handler(void);
void GMAC_Handler(void) {
  MG_INFO(("Entering GMAC IRQ HANDLER"));
  uint32_t isr = GMAC_REGS->GMAC_ISR;
  MG_INFO(("isr: 0x%x", isr));
  if (/*GMAC_REGS->GMAC_ISR*/ isr & GMAC_ISR_RCOMP_Msk) {
    int frame_start = -1;
    int frame_end = -1;

    // find the start of the frame
    for (int i = s_rxno; i < GMAC_DESC_CNT + s_rxno; i++) {
      int desc_index = i % GMAC_DESC_CNT;
      if (!(s_rxdesc[desc_index][0] & 1)) break;
      if (s_rxdesc[desc_index][1] & BIT(14))
        frame_start = desc_index;
    }

    // if frame start not found, then clear the own bit
    if (frame_start == -1) {
      for (int i = s_rxno; i < GMAC_DESC_CNT + s_rxno; i++) {
        int desc_index = i % GMAC_DESC_CNT;
        if (!(s_rxdesc[desc_index][0] & 1)) {
          break;
        } else {
          s_rxdesc[desc_index][0] &= ~(BIT(0));
        }
      }
      return;
    }

    // find the end of the frame
    for (int i = frame_start; i < GMAC_DESC_CNT + frame_start; i++) {
      int desc_index = i % GMAC_DESC_CNT;
      if (s_rxdesc[desc_index][1] & BIT(15))
        frame_end = desc_index;
    }

    // clear all used buffers until you reach start of frame
    for (int i = 0; i < GMAC_DESC_CNT; i++) {
      int desc_index = (i + s_rxno) % GMAC_DESC_CNT;
      if (s_rxno == frame_start) break;
      if (++s_rxno >= GMAC_DESC_CNT) s_rxno = 0;
      s_rxdesc[desc_index][0] &= ~(BIT(0)); // Clearing the OWN bit
    }

    // from start of frame to end of frame you copy into queue
    size_t offset = 0, len;
    for (int i = 0; i < GMAC_DESC_CNT; i++) {
      int desc_index = (i + s_rxno) % GMAC_DESC_CNT;
      if (desc_index == frame_start)
        offset = (GMAC_REGS->GMAC_NCFGR & GMAC_NCFGR_RXBUFO_Msk) >> GMAC_NCFGR_RXBUFO_Pos;
      else
        offset = 0;
      len = s_rxdesc[s_rxno][1] & (BIT(13) - 1);
      mg_tcpip_qwrite(s_rxbuf[s_rxno] + offset, len, s_ifp);
      s_rxdesc[desc_index][0] &= ~(BIT(0)); // Clearing the OWN bit
      if (++s_rxno >= GMAC_DESC_CNT) s_rxno = 0;
      if (desc_index == frame_end) break;
    }
  }
}

struct mg_tcpip_driver mg_tcpip_driver_same54 = {mg_tcpip_driver_same54_init,
                                                mg_tcpip_driver_same54_tx, NULL,
                                                mg_tcpip_driver_same54_up};
#endif
