#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_XMC7) && MG_ENABLE_DRIVER_XMC7

struct ETH_Type {
  volatile uint32_t CTL, STATUS, RESERVED[1022], NETWORK_CONTROL,
      NETWORK_CONFIG, NETWORK_STATUS, USER_IO_REGISTER, DMA_CONFIG,
      TRANSMIT_STATUS, RECEIVE_Q_PTR, TRANSMIT_Q_PTR, RECEIVE_STATUS,
      INT_STATUS, INT_ENABLE, INT_DISABLE, INT_MASK, PHY_MANAGEMENT, PAUSE_TIME,
      TX_PAUSE_QUANTUM, PBUF_TXCUTTHRU, PBUF_RXCUTTHRU, JUMBO_MAX_LENGTH,
      EXTERNAL_FIFO_INTERFACE, RESERVED1, AXI_MAX_PIPELINE, RSC_CONTROL,
      INT_MODERATION, SYS_WAKE_TIME, RESERVED2[7], HASH_BOTTOM, HASH_TOP,
      SPEC_ADD1_BOTTOM, SPEC_ADD1_TOP, SPEC_ADD2_BOTTOM, SPEC_ADD2_TOP,
      SPEC_ADD3_BOTTOM, SPEC_ADD3_TOP, SPEC_ADD4_BOTTOM, SPEC_ADD4_TOP,
      SPEC_TYPE1, SPEC_TYPE2, SPEC_TYPE3, SPEC_TYPE4, WOL_REGISTER,
      STRETCH_RATIO, STACKED_VLAN, TX_PFC_PAUSE, MASK_ADD1_BOTTOM,
      MASK_ADD1_TOP, DMA_ADDR_OR_MASK, RX_PTP_UNICAST, TX_PTP_UNICAST,
      TSU_NSEC_CMP, TSU_SEC_CMP, TSU_MSB_SEC_CMP, TSU_PTP_TX_MSB_SEC,
      TSU_PTP_RX_MSB_SEC, TSU_PEER_TX_MSB_SEC, TSU_PEER_RX_MSB_SEC,
      DPRAM_FILL_DBG, REVISION_REG, OCTETS_TXED_BOTTOM, OCTETS_TXED_TOP,
      FRAMES_TXED_OK, BROADCAST_TXED, MULTICAST_TXED, PAUSE_FRAMES_TXED,
      FRAMES_TXED_64, FRAMES_TXED_65, FRAMES_TXED_128, FRAMES_TXED_256,
      FRAMES_TXED_512, FRAMES_TXED_1024, FRAMES_TXED_1519, TX_UNDERRUNS,
      SINGLE_COLLISIONS, MULTIPLE_COLLISIONS, EXCESSIVE_COLLISIONS,
      LATE_COLLISIONS, DEFERRED_FRAMES, CRS_ERRORS, OCTETS_RXED_BOTTOM,
      OCTETS_RXED_TOP, FRAMES_RXED_OK, BROADCAST_RXED, MULTICAST_RXED,
      PAUSE_FRAMES_RXED, FRAMES_RXED_64, FRAMES_RXED_65, FRAMES_RXED_128,
      FRAMES_RXED_256, FRAMES_RXED_512, FRAMES_RXED_1024, FRAMES_RXED_1519,
      UNDERSIZE_FRAMES, EXCESSIVE_RX_LENGTH, RX_JABBERS, FCS_ERRORS,
      RX_LENGTH_ERRORS, RX_SYMBOL_ERRORS, ALIGNMENT_ERRORS, RX_RESOURCE_ERRORS,
      RX_OVERRUNS, RX_IP_CK_ERRORS, RX_TCP_CK_ERRORS, RX_UDP_CK_ERRORS,
      AUTO_FLUSHED_PKTS, RESERVED3, TSU_TIMER_INCR_SUB_NSEC, TSU_TIMER_MSB_SEC,
      TSU_STROBE_MSB_SEC, TSU_STROBE_SEC, TSU_STROBE_NSEC, TSU_TIMER_SEC,
      TSU_TIMER_NSEC, TSU_TIMER_ADJUST, TSU_TIMER_INCR, TSU_PTP_TX_SEC,
      TSU_PTP_TX_NSEC, TSU_PTP_RX_SEC, TSU_PTP_RX_NSEC, TSU_PEER_TX_SEC,
      TSU_PEER_TX_NSEC, TSU_PEER_RX_SEC, TSU_PEER_RX_NSEC, PCS_CONTROL,
      PCS_STATUS, RESERVED4[2], PCS_AN_ADV, PCS_AN_LP_BASE, PCS_AN_EXP,
      PCS_AN_NP_TX, PCS_AN_LP_NP, RESERVED5[6], PCS_AN_EXT_STATUS, RESERVED6[8],
      TX_PAUSE_QUANTUM1, TX_PAUSE_QUANTUM2, TX_PAUSE_QUANTUM3, RESERVED7,
      RX_LPI, RX_LPI_TIME, TX_LPI, TX_LPI_TIME, DESIGNCFG_DEBUG1,
      DESIGNCFG_DEBUG2, DESIGNCFG_DEBUG3, DESIGNCFG_DEBUG4, DESIGNCFG_DEBUG5,
      DESIGNCFG_DEBUG6, DESIGNCFG_DEBUG7, DESIGNCFG_DEBUG8, DESIGNCFG_DEBUG9,
      DESIGNCFG_DEBUG10, RESERVED8[22], SPEC_ADD5_BOTTOM, SPEC_ADD5_TOP,
      RESERVED9[60], SPEC_ADD36_BOTTOM, SPEC_ADD36_TOP, INT_Q1_STATUS,
      INT_Q2_STATUS, INT_Q3_STATUS, RESERVED10[11], INT_Q15_STATUS, RESERVED11,
      TRANSMIT_Q1_PTR, TRANSMIT_Q2_PTR, TRANSMIT_Q3_PTR, RESERVED12[11],
      TRANSMIT_Q15_PTR, RESERVED13, RECEIVE_Q1_PTR, RECEIVE_Q2_PTR,
      RECEIVE_Q3_PTR, RESERVED14[3], RECEIVE_Q7_PTR, RESERVED15,
      DMA_RXBUF_SIZE_Q1, DMA_RXBUF_SIZE_Q2, DMA_RXBUF_SIZE_Q3, RESERVED16[3],
      DMA_RXBUF_SIZE_Q7, CBS_CONTROL, CBS_IDLESLOPE_Q_A, CBS_IDLESLOPE_Q_B,
      UPPER_TX_Q_BASE_ADDR, TX_BD_CONTROL, RX_BD_CONTROL, UPPER_RX_Q_BASE_ADDR,
      RESERVED17[2], HIDDEN_REG0, HIDDEN_REG1, HIDDEN_REG2, HIDDEN_REG3,
      RESERVED18[2], HIDDEN_REG4, HIDDEN_REG5;
};

#define ETH0 ((struct ETH_Type *) 0x40490000)

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
enum { MG_PHY_ADDR = 0, MG_PHYREG_BCR = 0, MG_PHYREG_BSR = 1 };

static uint16_t eth_read_phy(uint8_t addr, uint8_t reg) {
  // WRITE1, READ OPERATION, PHY, REG, WRITE10
  ETH0->PHY_MANAGEMENT = MG_BIT(30) | MG_BIT(29) | ((addr & 0xf) << 24) |
                         ((reg & 0x1f) << 18) | MG_BIT(17);
  while ((ETH0->NETWORK_STATUS & MG_BIT(2)) == 0) (void) 0;
  return ETH0->PHY_MANAGEMENT & 0xffff;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  ETH0->PHY_MANAGEMENT = MG_BIT(30) | MG_BIT(28) | ((addr & 0xf) << 24) |
                         ((reg & 0x1f) << 18) | MG_BIT(17) | val;
  while ((ETH0->NETWORK_STATUS & MG_BIT(2)) == 0) (void) 0;
}

static uint32_t get_clock_rate(struct mg_tcpip_driver_xmc7_data *d) {
  // see ETH0 -> NETWORK_CONFIG register
  (void) d;
  return 3;
}

static bool mg_tcpip_driver_xmc7_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_xmc7_data *d =
      (struct mg_tcpip_driver_xmc7_data *) ifp->driver_data;
  s_ifp = ifp;

  // enable controller, set RGMII mode
  ETH0->CTL = MG_BIT(31) | 2;

  uint32_t cr = get_clock_rate(d);
  // set NSP change, ignore RX FCS, data bus width, clock rate
  // frame length 1536, full duplex, speed
  ETH0->NETWORK_CONFIG = MG_BIT(29) | MG_BIT(26) | MG_BIT(21) |
                         ((cr & 7) << 18) | MG_BIT(8) | MG_BIT(4) |
                         MG_BIT(1) | MG_BIT(0);

  // config DMA settings: Force TX burst, Discard on Error, set RX buffer size
  // to 1536, TX_PBUF_SIZE, RX_PBUF_SIZE, AMBA_BURST_LENGTH
  ETH0->DMA_CONFIG =
      MG_BIT(26) | MG_BIT(24) | (0x18 << 16) | MG_BIT(10) | (3 << 8) | 4;

  // initialize descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = (uint32_t) s_rxbuf[i];
    if (i == ETH_DESC_CNT - 1) {
      s_rxdesc[i][0] |= MG_BIT(1);  // mark last descriptor
    }

    s_txdesc[i][0] = (uint32_t) s_txbuf[i];
    s_txdesc[i][1] = MG_BIT(31);  // OWN descriptor
    if (i == ETH_DESC_CNT - 1) {
      s_txdesc[i][1] |= MG_BIT(30);  // mark last descriptor
    }
  }
  ETH0->RECEIVE_Q_PTR = (uint32_t) s_rxdesc;
  ETH0->TRANSMIT_Q_PTR = (uint32_t) s_txdesc;

  // disable other queues
  ETH0->TRANSMIT_Q2_PTR = 1;
  ETH0->TRANSMIT_Q1_PTR = 1;
  ETH0->RECEIVE_Q2_PTR = 1;
  ETH0->RECEIVE_Q1_PTR = 1;

  // enable interrupts (TX and RX complete)
  ETH0->INT_ENABLE = MG_BIT(7) | MG_BIT(1);

  // set MAC address
  ETH0->SPEC_ADD1_BOTTOM =
      ifp->mac[3] << 24 | ifp->mac[2] << 16 | ifp->mac[1] << 8 | ifp->mac[0];
  ETH0->SPEC_ADD1_TOP = ifp->mac[5] << 8 | ifp->mac[4];

  // enable MDIO, TX, RX
  ETH0->NETWORK_CONTROL = MG_BIT(4) | MG_BIT(3) | MG_BIT(2);

  // start transmission
  ETH0->NETWORK_CONTROL |= MG_BIT(9);

  // init phy
  struct mg_phy phy = {eth_read_phy, eth_write_phy};
  mg_phy_init(&phy, d->phy_addr, MG_PHY_CLOCKS_MAC);

  (void) d;
  return true;
}

static size_t mg_tcpip_driver_xmc7_tx(const void *buf, size_t len,
                                      struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if (((s_txdesc[s_txno][1] & MG_BIT(31)) == 0)) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors"));
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);
    s_txdesc[s_txno][1] = (s_txno == ETH_DESC_CNT - 1 ? MG_BIT(30) : 0) |
                          MG_BIT(15) | len;  // Last buffer and length

    ETH0->NETWORK_CONTROL |= MG_BIT(9);  // enable transmission
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }

  MG_DSB();
  ETH0->TRANSMIT_STATUS = ETH0->TRANSMIT_STATUS;
  ETH0->NETWORK_CONTROL |= MG_BIT(9);  // enable transmission

  return len;
}

static bool mg_tcpip_driver_xmc7_up(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_xmc7_data *d =
      (struct mg_tcpip_driver_xmc7_data *) ifp->driver_data;
  uint8_t speed = MG_PHY_SPEED_10M;
  bool up = false, full_duplex = false;
  struct mg_phy phy = {eth_read_phy, eth_write_phy};
  up = mg_phy_up(&phy, d->phy_addr, &full_duplex, &speed);
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    if (speed == MG_PHY_SPEED_1000M) {
		  ETH0->NETWORK_CONFIG |= MG_BIT(10);
	  }
    MG_DEBUG(("Link is %uM %s-duplex",
              speed == MG_PHY_SPEED_10M ? 10 : 
              (speed == MG_PHY_SPEED_100M ? 100 : 1000),
              full_duplex ? "full" : "half"));
  }
  (void) d;
  return up;
}

void ETH_IRQHandler(void) {
  uint32_t irq_status = ETH0->INT_STATUS;
  if (irq_status & MG_BIT(1)) {
    for (uint8_t i = 0; i < ETH_DESC_CNT; i++) {
      if (s_rxdesc[s_rxno][0] & MG_BIT(0)) {
        size_t len = s_rxdesc[s_rxno][1] & (MG_BIT(13) - 1);
        //MG_INFO(("Receive complete: %ld bytes", len));
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len, s_ifp);
        s_rxdesc[s_rxno][0] &= ~MG_BIT(0);  // OWN bit: handle control to DMA
        if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
      }
    }
  }

  ETH0->INT_STATUS = irq_status;
}

struct mg_tcpip_driver mg_tcpip_driver_xmc7 = {mg_tcpip_driver_xmc7_init,
                                               mg_tcpip_driver_xmc7_tx, NULL,
                                               mg_tcpip_driver_xmc7_up};
#endif
