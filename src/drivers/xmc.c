#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_XMC) && MG_ENABLE_DRIVER_XMC

struct ETH_GLOBAL_TypeDef {
  volatile uint32_t MAC_CONFIGURATION, MAC_FRAME_FILTER, HASH_TABLE_HIGH,
  HASH_TABLE_LOW, GMII_ADDRESS, GMII_DATA, FLOW_CONTROL, VLAN_TAG, VERSION,
  DEBUG, REMOTE_WAKE_UP_FRAME_FILTER, PMT_CONTROL_STATUS, RESERVED[2],
  INTERRUPT_STATUS, INTERRUPT_MASK, MAC_ADDRESS0_HIGH, MAC_ADDRESS0_LOW,
  MAC_ADDRESS1_HIGH, MAC_ADDRESS1_LOW, MAC_ADDRESS2_HIGH, MAC_ADDRESS2_LOW,
  MAC_ADDRESS3_HIGH, MAC_ADDRESS3_LOW, RESERVED1[40], MMC_CONTROL,
  MMC_RECEIVE_INTERRUPT, MMC_TRANSMIT_INTERRUPT, MMC_RECEIVE_INTERRUPT_MASK,
  MMC_TRANSMIT_INTERRUPT_MASK, TX_STATISTICS[26], RESERVED2,
  RX_STATISTICS_1[26], RESERVED3[6], MMC_IPC_RECEIVE_INTERRUPT_MASK,
  RESERVED4, MMC_IPC_RECEIVE_INTERRUPT, RESERVED5, RX_STATISTICS_2[30],
  RESERVED7[286], TIMESTAMP_CONTROL, SUB_SECOND_INCREMENT,
  SYSTEM_TIME_SECONDS, SYSTEM_TIME_NANOSECONDS,
  SYSTEM_TIME_SECONDS_UPDATE, SYSTEM_TIME_NANOSECONDS_UPDATE,
  TIMESTAMP_ADDEND, TARGET_TIME_SECONDS, TARGET_TIME_NANOSECONDS,
  SYSTEM_TIME_HIGHER_WORD_SECONDS, TIMESTAMP_STATUS,
  PPS_CONTROL, RESERVED8[564], BUS_MODE, TRANSMIT_POLL_DEMAND,
  RECEIVE_POLL_DEMAND, RECEIVE_DESCRIPTOR_LIST_ADDRESS,
  TRANSMIT_DESCRIPTOR_LIST_ADDRESS, STATUS, OPERATION_MODE,
  INTERRUPT_ENABLE, MISSED_FRAME_AND_BUFFER_OVERFLOW_COUNTER,
  RECEIVE_INTERRUPT_WATCHDOG_TIMER, RESERVED9, AHB_STATUS,
  RESERVED10[6], CURRENT_HOST_TRANSMIT_DESCRIPTOR,
  CURRENT_HOST_RECEIVE_DESCRIPTOR, CURRENT_HOST_TRANSMIT_BUFFER_ADDRESS,
  CURRENT_HOST_RECEIVE_BUFFER_ADDRESS, HW_FEATURE;
};

#undef ETH0
#define ETH0  ((struct ETH_GLOBAL_TypeDef*) 0x5000C000UL)

#define ETH_PKT_SIZE 1536 // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 4           // Descriptor size (words)

static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE];
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE];
static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];  // RX descriptors
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];  // TX descriptors
static uint8_t s_txno;                           // Current TX descriptor
static uint8_t s_rxno;                           // Current RX descriptor

static struct mg_tcpip_if *s_ifp;  // MIP interface
enum { MG_PHY_ADDR = 0, MG_PHYREG_BCR = 0, MG_PHYREG_BSR = 1 };

static uint16_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ETH0->GMII_ADDRESS = (ETH0->GMII_ADDRESS & 0x3c) |
                        ((uint32_t)addr << 11) |
                        ((uint32_t)reg << 6) | 1;
  while ((ETH0->GMII_ADDRESS & 1) != 0) (void) 0;
  return (uint16_t)(ETH0->GMII_DATA & 0xffff);
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  ETH0->GMII_DATA  = val;
  ETH0->GMII_ADDRESS = (ETH0->GMII_ADDRESS & 0x3c) |
                        ((uint32_t)addr << 11) |
                        ((uint32_t)reg << 6) | 3;
  while ((ETH0->GMII_ADDRESS & 1) != 0) (void) 0;
}

static uint32_t get_clock_rate(struct mg_tcpip_driver_xmc_data *d) {
  // assume ETH clock is 60MHz
  // then according to 13.2.8.1, we need to set value 3
  (void) d;
  return 3;
}

static bool mg_tcpip_driver_xmc_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_xmc_data *d =
      (struct mg_tcpip_driver_xmc_data *) ifp->driver_data;
  s_ifp = ifp;

  // reset MAC
  ETH0->BUS_MODE |= 1;
  while (ETH0->BUS_MODE & 1) (void) 0;

  // set clock rate
  ETH0->GMII_ADDRESS = get_clock_rate(d) << 2;

  // init phy
  eth_write_phy(MG_PHY_ADDR, MG_PHYREG_BCR, 0x8000); // reset
  uint8_t bcr = eth_read_phy(MG_PHY_ADDR, MG_PHYREG_BCR);
  while ((bcr & 0x8000) != 0) {
    bcr = eth_read_phy(MG_PHY_ADDR, MG_PHYREG_BCR);
  }

  // auto-negotiation, 100M, full-duplex
  eth_write_phy(MG_PHY_ADDR, MG_PHYREG_BCR, 0x3100);

  ETH0->MAC_CONFIGURATION = MG_BIT(13) /* DO */ | MG_BIT(11) /* DM */ | 
                            MG_BIT(14) /* FES */ | MG_BIT(24) /* TC */;

  // set the MAC address
  ETH0->MAC_ADDRESS0_HIGH = MG_U32(0, 0, ifp->mac[5], ifp->mac[4]);
  ETH0->MAC_ADDRESS0_LOW = 
        MG_U32(ifp->mac[3], ifp->mac[2], ifp->mac[1], ifp->mac[0]);

  // Configure the receive filter
  ETH0->MAC_FRAME_FILTER = MG_BIT(10) /* HPF */ | MG_BIT(2) /* HMC */;
  // Disable flow control
  ETH0->FLOW_CONTROL = 0;
  // Enable store and forward mode
  ETH0->OPERATION_MODE = MG_BIT(25) /* RSF */ | MG_BIT(21) /* TSF */;

  // Configure DMA bus mode
  ETH0->BUS_MODE = MG_BIT(25) /* AAL */ | MG_BIT(23) /* USP */ |
       (32 << 17 /* RPBL */) |  (32 << 8 /* PBL */);

  // init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = MG_BIT(31); // OWN descriptor
    //s_rxdesc[i][1] = MG_BIT(24) | ETH_PKT_SIZE; // chained descriptors
    s_rxdesc[i][1] = MG_BIT(14) | ETH_PKT_SIZE;
    s_rxdesc[i][2] = (uint32_t) s_rxbuf[i];
    if (i == ETH_DESC_CNT - 1) {
      s_rxdesc[i][3] = (uint32_t) &s_rxdesc[0][0];
    } else {
      s_rxdesc[i][3] = (uint32_t) &s_rxdesc[i + 1][0];
    }
  }
  ETH0->RECEIVE_DESCRIPTOR_LIST_ADDRESS = (uint32_t) &s_rxdesc[0][0];

  // init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i][0] = MG_BIT(30) | MG_BIT(20);
    s_txdesc[i][2] = (uint32_t) s_txbuf[i];
    if (i == ETH_DESC_CNT - 1) {
      s_txdesc[i][3] = (uint32_t) &s_txdesc[0][0];
    } else {
      s_txdesc[i][3] = (uint32_t) &s_txdesc[i + 1][0];
    }
  }
  ETH0->TRANSMIT_DESCRIPTOR_LIST_ADDRESS = (uint32_t) &s_txdesc[0][0];

  // Clear interrupts
  ETH0->STATUS = 0xFFFFFFFF;

  // Disable MAC interrupts
  ETH0->MMC_TRANSMIT_INTERRUPT_MASK = 0xFFFFFFFF;
  ETH0->MMC_RECEIVE_INTERRUPT_MASK = 0xFFFFFFFF;
  ETH0->MMC_IPC_RECEIVE_INTERRUPT_MASK = 0xFFFFFFFF;
  ETH0->INTERRUPT_MASK = MG_BIT(9) /* TSIM */ | MG_BIT(3) /* PMTIM */;

  //Enable interrupts
  ETH0->INTERRUPT_ENABLE = MG_BIT(16) /* NIE */ | MG_BIT(6) /* RIE */ |
                           MG_BIT(0) /* TIE */;

  // Enable MAC transmission and reception
  ETH0->MAC_CONFIGURATION |= MG_BIT(3) /* TE */ | MG_BIT(2) /* RE */;
  // Enable DMA transmission and reception
  ETH0->OPERATION_MODE |= MG_BIT(13) /* ST */ | MG_BIT(1) /* SR */;
  return true;
}

static size_t mg_tcpip_driver_xmc_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if ((s_txdesc[s_txno][0] & MG_BIT(31))) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors"));
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);
    s_txdesc[s_txno][1] = len;
    s_txdesc[s_txno][0] = MG_BIT(29) /* Last frame */ |
        MG_BIT(28) /* First frame */ | MG_BIT(30) /* Interrupt on send */ |
        MG_BIT(20) /* Chained descriptors */;
    s_txdesc[s_txno][0] |= MG_BIT(31);  // OWN bit: handle control to DMA
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }

  // Resume processing
  ETH0->STATUS = MG_BIT(2); // clear Transmit unavailable
  ETH0->TRANSMIT_POLL_DEMAND = 0;
  return len;
}

static bool mg_tcpip_driver_xmc_up(struct mg_tcpip_if *ifp) {
  uint16_t bsr = eth_read_phy(MG_PHY_ADDR, MG_PHYREG_BSR);
  bool up = bsr & MG_BIT(2) ? 1 : 0;
  (void) ifp;
  return up;
}

void ETH0_IRQHandler(void);
void ETH0_IRQHandler(void) {
  uint32_t irq_status = ETH0->STATUS;

  // check if a frame was received
  if (irq_status & MG_BIT(6)) {
    for (uint8_t i = 0; i < ETH_DESC_CNT; i++) {
      if ((s_rxdesc[s_rxno][0] & MG_BIT(31)) == 0) {
        size_t len = (s_rxdesc[s_rxno][0] & 0x3fff0000) >> 16;
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len, s_ifp);
        s_rxdesc[s_rxno][0] = MG_BIT(31);   // OWN bit: handle control to DMA
        // Resume processing
        ETH0->STATUS = MG_BIT(7) | // clear Receive unavailable
                       MG_BIT(6); // clear Receive interrupt
        ETH0->RECEIVE_POLL_DEMAND = 0;
        if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
      }
    }
    ETH0->STATUS = MG_BIT(6);
  }

  // clear Successful transmission interrupt
  if (irq_status & 1) {
    ETH0->STATUS = 1;
  }

  // clear normal interrupt
  if (irq_status & MG_BIT(16)) {
    ETH0->STATUS = MG_BIT(16);
  }
}

struct mg_tcpip_driver mg_tcpip_driver_xmc = {
    mg_tcpip_driver_xmc_init, mg_tcpip_driver_xmc_tx, NULL,
    mg_tcpip_driver_xmc_up};
#endif
