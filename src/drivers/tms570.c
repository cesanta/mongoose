#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_TMS570) && MG_ENABLE_DRIVER_TMS570
struct tms570_emac_ctrl {
  volatile uint32_t REVID, SOFTRESET, RESERVED1[1], INTCONTROL, C0RXTHRESHEN,
  C0RXEN, C0TXEN, C0MISCEN, RESERVED2[8],
  C0RXTHRESHSTAT, C0RXSTAT, C0TXSTAT, C0MISCSTAT,
  RESERVED3[8],
  C0RXIMAX, C0TXIMAX;
};
struct tms570_emac {
  volatile uint32_t TXREVID, TXCONTROL, TXTEARDOWN, RESERVED1[1], RXREVID,
  RXCONTROL, RXTEARDOWN, RESERVED2[25], TXINTSTATRAW,TXINTSTATMASKED,
  TXINTMASKSET, TXINTMASKCLEAR, MACINVECTOR, MACEOIVECTOR, RESERVED8[2], RXINTSTATRAW,
  RXINTSTATMASKED, RXINTMASKSET, RXINTMASKCLEAR, MACINTSTATRAW, MACINTSTATMASKED,
  MACINTMASKSET, MACINTMASKCLEAR, RESERVED3[16], RXMBPENABLE, RXUNICASTSET,
  RXUNICASTCLEAR, RXMAXLEN, RXBUFFEROFFSET, RXFILTERLOWTHRESH, RESERVED9[2], RXFLOWTHRESH[8],
  RXFREEBUFFER[8], MACCONTROL, MACSTATUS, EMCONTROL, FIFOCONTROL, MACCONFIG,
  SOFTRESET, RESERVED4[22], MACSRCADDRLO, MACSRCADDRHI, MACHASH1, MACHASH2,
  BOFFTEST, TPACETEST, RXPAUSE, TXPAUSE, RESERVED5[4], RXGOODFRAMES, RXBCASTFRAMES,
  RXMCASTFRAMES, RXPAUSEFRAMES, RXCRCERRORS, RXALIGNCODEERRORS, RXOVERSIZED,
  RXJABBER, RXUNDERSIZED, RXFRAGMENTS, RXFILTERED, RXQOSFILTERED, RXOCTETS,
  TXGOODFRAMES, TXBCASTFRAMES, TXMCASTFRAMES, TXPAUSEFRAMES, TXDEFERRED,
  TXCOLLISION, TXSINGLECOLL, TXMULTICOLL, TXEXCESSIVECOLL, TXLATECOLL,
  TXUNDERRUN, TXCARRIERSENSE, TXOCTETS, FRAME64, FRAME65T127, FRAME128T255,
  FRAME256T511, FRAME512T1023, FRAME1024TUP, NETOCTETS, RXSOFOVERRUNS,
  RXMOFOVERRUNS, RXDMAOVERRUNS, RESERVED6[156], MACADDRLO, MACADDRHI,
  MACINDEX, RESERVED7[61], TXHDP[8], RXHDP[8], TXCP[8], RXCP[8];
};
struct tms570_mdio {
  volatile uint32_t REVID, CONTROL, ALIVE, LINK, LINKINTRAW, LINKINTMASKED,
  RESERVED1[2], USERINTRAW, USERINTMASKED, USERINTMASKSET, USERINTMASKCLEAR,
  RESERVED2[20], USERACCESS0, USERPHYSEL0, USERACCESS1, USERPHYSEL1;
};
#define SWAP32(x) ( (((x) & 0x000000FF) << 24) | \
                              (((x) & 0x0000FF00) << 8)  | \
                              (((x) & 0x00FF0000) >> 8)  | \
                              (((x) & 0xFF000000) >> 24) )
#undef EMAC
#undef EMAC_CTRL
#undef MDIO
#define EMAC ((struct tms570_emac *) (uintptr_t) 0xFCF78000)
#define EMAC_CTRL ((struct tms570_emac_ctrl *) (uintptr_t) 0xFCF78800)
#define MDIO ((struct tms570_mdio *) (uintptr_t) 0xFCF78900)
#define ETH_PKT_SIZE 1540  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 4           // Descriptor size (words)
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS] 
  __attribute__((section(".ETH_CPPI"), aligned(4)));      // TX descriptors
static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS] 
  __attribute__((section(".ETH_CPPI"), aligned(4)));      // RX descriptors
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE] 
  __attribute__((aligned(4)));  // RX ethernet buffers
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE] 
  __attribute__((aligned(4)));  // TX ethernet buffers
static struct mg_tcpip_if *s_ifp;                    // MIP interface
static uint16_t emac_read_phy(uint8_t addr, uint8_t reg) {
  while(MDIO->USERACCESS0 & MG_BIT(31)) (void) 0;
  MDIO->USERACCESS0 = MG_BIT(31) | ((reg & 0x1f) << 21) |
                      ((addr & 0x1f) << 16);
  while(MDIO->USERACCESS0 & MG_BIT(31)) (void) 0;
  return MDIO->USERACCESS0 & 0xffff;
}
static void emac_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  while(MDIO->USERACCESS0 & MG_BIT(31)) (void) 0;
  MDIO->USERACCESS0 = MG_BIT(31) | MG_BIT(30) | ((reg & 0x1f) << 21) |
                      ((addr & 0x1f) << 16) | (val & 0xffff);
  while(MDIO->USERACCESS0 & MG_BIT(31)) (void) 0;
}
static bool mg_tcpip_driver_tms570_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_tms570_data *d =
      (struct mg_tcpip_driver_tms570_data *) ifp->driver_data;
  s_ifp = ifp;
  EMAC_CTRL->SOFTRESET = MG_BIT(0); // Reset the EMAC Control Module
  while(EMAC_CTRL->SOFTRESET & MG_BIT(0)) (void) 0; // wait
  EMAC->SOFTRESET = MG_BIT(0); // Reset the EMAC Module
  while(EMAC->SOFTRESET & MG_BIT(0)) (void) 0;
  EMAC->MACCONTROL = 0;
  EMAC->RXCONTROL = 0;
  EMAC->TXCONTROL = 0;
  // Initialize all the header descriptor pointer registers
  uint32_t i;
  for(i =  0; i < ETH_DESC_CNT; i++) {
    EMAC->RXHDP[i] = 0;
    EMAC->TXHDP[i] = 0;
    EMAC->RXCP[i] = 0;
    EMAC->TXCP[i] = 0;
    ///EMAC->RXFREEBUFFER[i] = 0xff;
  }
  // Clear the interrupt enable for all the channels
  EMAC->TXINTMASKCLEAR = 0xff;
  EMAC->RXINTMASKCLEAR = 0xff;
  EMAC->MACHASH1 = 0;
  EMAC->MACHASH2 = 0;
  EMAC->RXBUFFEROFFSET = 0;
  EMAC->RXUNICASTCLEAR = 0xff;
  EMAC->RXUNICASTSET = 0;
  EMAC->RXMBPENABLE = 0;
  // init MDIO
  // MDIO_CLK frequency = VCLK3/(CLKDIV + 1). (MDIO must be between 1.0 - 2.5Mhz)
  uint32_t clkdiv = 75; // VCLK is configured to 75Mhz
  // CLKDIV, ENABLE, PREAMBLE, FAULTENB
  MDIO->CONTROL = (clkdiv - 1) | MG_BIT(30) | MG_BIT(20) | MG_BIT(18);
  volatile int delay = 0xfff;
  while (delay-- != 0) (void) 0;
  struct mg_phy phy = {emac_read_phy, emac_write_phy};
  mg_phy_init(&phy, d->phy_addr, MG_PHY_CLOCKS_MAC);
  uint32_t channel;
  for (channel = 0; channel < 8; channel++) {
    EMAC->MACINDEX = channel;
    EMAC->MACADDRHI = ifp->mac[0] | (ifp->mac[1] << 8) | (ifp->mac[2] << 16) |
                       (ifp->mac[3] << 24);
    EMAC->MACADDRLO = ifp->mac[4] | (ifp->mac[5] << 8) | MG_BIT(20) |
                      MG_BIT(19) | (channel << 16);
  }
  EMAC->RXUNICASTSET = 1; // accept unicast frames;

  EMAC->RXMBPENABLE |= MG_BIT(30) | MG_BIT(13); // CRC, broadcast

  // Initialize the descriptors
  for (i = 0; i < ETH_DESC_CNT; i++) {
    if (i < ETH_DESC_CNT - 1) {
      s_txdesc[i][0] = 0;
      s_rxdesc[i][0] = SWAP32(((uint32_t) &s_rxdesc[i + 1][0]));
    }
    s_txdesc[i][1] = SWAP32(((uint32_t) s_txbuf[i]));
    s_rxdesc[i][1] = SWAP32(((uint32_t) s_rxbuf[i]));
    s_txdesc[i][2] = 0;
    s_rxdesc[i][2] = SWAP32(ETH_PKT_SIZE);
    s_txdesc[i][3] = 0;
    s_rxdesc[i][3] = SWAP32(MG_BIT(29)); // OWN
  }
  s_txdesc[ETH_DESC_CNT - 1][0] = 0;
  s_rxdesc[ETH_DESC_CNT - 1][0] = 0;
  
  EMAC->MACCONTROL = MG_BIT(5) | MG_BIT(0); // Enable MII, Full-duplex
  //EMAC->TXINTMASKSET = 1; // Enable TX interrupt
  EMAC->RXINTMASKSET = 1; // Enable RX interrupt
  //EMAC_CTRL->C0TXEN = 1; // TX completion interrupt
  EMAC_CTRL->C0RXEN = 1; // RX completion interrupt
  EMAC->TXCONTROL = 1; // TXEN
  EMAC->RXCONTROL = 1; // RXEN
  EMAC->RXHDP[0] = (uint32_t) &s_rxdesc[0][0];
  return true;
}
static uint32_t s_txno;
static size_t mg_tcpip_driver_tms570_tx(const void *buf, size_t len,
                                      struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // fail
  } else if ((s_txdesc[s_txno][3] & SWAP32(MG_BIT(29)))) {
    ifp->nerr++;
    MG_ERROR(("No descriptors available"));
    len = 0;  // fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);     // Copy data
    if (len < 128) len = 128;
    s_txdesc[s_txno][2] = SWAP32((uint32_t) len);  // Set data len
    s_txdesc[s_txno][3] =
        SWAP32(MG_BIT(31) | MG_BIT(30) | MG_BIT(29) | len);  // SOP, EOP, OWN, length
    
    while(EMAC->TXHDP[0] != 0) (void) 0;
    EMAC->TXHDP[0] = (uint32_t) &s_txdesc[s_txno][0];
    if(++s_txno == ETH_DESC_CNT) {
      s_txno = 0;
    }
  }
  return len;
  (void) ifp;
}

static void mg_tcpip_driver_tms570_update_hash_table(struct mg_tcpip_if *ifp) {
  // TODO(): read database, rebuild hash table
  // Setting Hash Index for 01:00:5e:00:00:fb (multicast)
  // using TMS570 XOR method (32.5.37).
  // computed hash is 55, which means bit 23 (55 - 32) in
  // HASH2 register must be set
  EMAC->MACHASH2 = MG_BIT(23);
  EMAC->RXMBPENABLE = MG_BIT(5); // enable hash filtering
  (void) ifp;
}

static bool mg_tcpip_driver_tms570_poll(struct mg_tcpip_if *ifp, bool s1) {
  if (ifp->update_mac_hash_table) {
    mg_tcpip_driver_tms570_update_hash_table(ifp);
    ifp->update_mac_hash_table = false;
  }
  if (!s1) return false;
  struct mg_tcpip_driver_tms570_data *d =
      (struct mg_tcpip_driver_tms570_data *) ifp->driver_data;
  uint8_t speed = MG_PHY_SPEED_10M;
  bool up = false, full_duplex = false;
  struct mg_phy phy = {emac_read_phy, emac_write_phy};
  if (!s1) return false;
  up = mg_phy_up(&phy, d->phy_addr, &full_duplex, &speed);
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {
    // link state just went up
    MG_DEBUG(("Link is %uM %s-duplex", speed == MG_PHY_SPEED_10M ? 10 : 100,
              full_duplex ? "full" : "half"));
  }
  return up;
}

#pragma CODE_STATE(EMAC_TX_IRQHandler, 32)
#pragma INTERRUPT(EMAC_TX_IRQHandler, IRQ)
void EMAC_TX_IRQHandler(void) {
  uint32_t status = EMAC_CTRL->C0TXSTAT;
  if (status & 1) { // interrupt caused on channel 0
    while(s_txdesc[s_txno][3] & SWAP32(MG_BIT(29))) (void) 0;
    EMAC->TXCP[0] = (uint32_t) &s_txdesc[s_txno][0];
  }
  //Write the DMA end of interrupt vector
  EMAC->MACEOIVECTOR = 2;
}
static uint32_t s_rxno;
#pragma CODE_STATE(EMAC_RX_IRQHandler, 32)
#pragma INTERRUPT(EMAC_RX_IRQHandler, IRQ)
void EMAC_RX_IRQHandler(void) {
  uint32_t status = EMAC_CTRL->C0RXSTAT;
  if (status & 1) { // Frame received, loop
    uint32_t i;
    //MG_INFO(("RX interrupt"));
    for (i = 0; i < 10; i++) {   // read as they arrive but not forever
      if (s_rxdesc[s_rxno][3] & SWAP32(MG_BIT(29))) break;
      uint32_t len = SWAP32(s_rxdesc[s_rxno][3]) & 0xffff;
      //MG_INFO(("recv len: %d", len));
      //mg_hexdump(s_rxbuf[s_rxno], len);
      mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
      uint32_t flags = s_rxdesc[s_rxno][3];
      s_rxdesc[s_rxno][3] = SWAP32(MG_BIT(29));
      s_rxdesc[s_rxno][2] = SWAP32(ETH_PKT_SIZE);
      EMAC->RXCP[0] = (uint32_t) &s_rxdesc[s_rxno][0];
      if (flags & SWAP32(MG_BIT(28))) {
        //MG_INFO(("EOQ detected"));
        EMAC->RXHDP[0] = (uint32_t) &s_rxdesc[0][0];
      }
      if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
    }
  }
  //Write the DMA end of interrupt vector
  EMAC->MACEOIVECTOR = 1;
}
struct mg_tcpip_driver mg_tcpip_driver_tms570 = {mg_tcpip_driver_tms570_init,
                                               mg_tcpip_driver_tms570_tx, NULL,
                                               mg_tcpip_driver_tms570_poll};
#endif

