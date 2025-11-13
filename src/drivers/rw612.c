#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RW612) && MG_ENABLE_DRIVER_RW612

struct ENET_Type {
  volatile uint32_t RESERVED_0[1], EIR, EIMR, RESERVED_1[1], RDAR, TDAR,
      RESERVED_2[3], ECR, RESERVED_3[6], MMFR, MSCR, RESERVED_4[7], MIBC,
      RESERVED_5[7], RCR, RESERVED_6[15], TCR, RESERVED_7[7], PALR, PAUR, OPD,
      TXIC[1], RESERVED_8[3], RXIC[1], RESERVED_9[5], IAUR, IALR, GAUR, GALR,
      RESERVED_10[7], TFWR, RESERVED_11[14], RDSR, TDSR, MRBR, RESERVED_12[1],
      RSFL, RSEM, RAEM, RAFL, TSEM, TAEM, TAFL, TIPG, FTRL, RESERVED_13[3],
      TACC, RACC, RESERVED_14[15], RMON_T_PACKETS, RMON_T_BC_PKT, RMON_T_MC_PKT,
      RMON_T_CRC_ALIGN, RMON_T_UNDERSIZE, RMON_T_OVERSIZE, RMON_T_FRAG,
      RMON_T_JAB, RMON_T_COL, RMON_T_P64, RMON_T_P65TO127, RMON_T_P128TO255,
      RMON_T_P256TO511, RMON_T_P512TO1023, RMON_T_P1024TO2047, RMON_T_P_GTE2048,
      RMON_T_OCTETS, IEEE_T_DROP, IEEE_T_FRAME_OK, IEEE_T_1COL, IEEE_T_MCOL,
      IEEE_T_DEF, IEEE_T_LCOL, IEEE_T_EXCOL, IEEE_T_MACERR, IEEE_T_CSERR,
      IEEE_T_SQE, IEEE_T_FDXFC, IEEE_T_OCTETS_OK, RESERVED_15[3],
      RMON_R_PACKETS, RMON_R_BC_PKT, RMON_R_MC_PKT, RMON_R_CRC_ALIGN,
      RMON_R_UNDERSIZE, RMON_R_OVERSIZE, RMON_R_FRAG, RMON_R_JAB,
      RESERVED_16[1], RMON_R_P64, RMON_R_P65TO127, RMON_R_P128TO255,
      RMON_R_P256TO511, RMON_R_P512TO1023, RMON_R_P1024TO2047, RMON_R_P_GTE2048,
      RMON_R_OCTETS, IEEE_R_DROP, IEEE_R_FRAME_OK, IEEE_R_CRC, IEEE_R_ALIGN,
      IEEE_R_MACERR, IEEE_R_FDXFC, IEEE_R_OCTETS_OK, RESERVED_17[71], ATCR,
      ATVR, ATOFF, ATPER, ATCOR, ATINC, ATSTMP, RESERVED_18[122], TGSR,
      CHANNEL_TCSR[4], CHANNEL_TCCR[4];
};

#undef ENET
#define ENET ((struct ENET_Type *) 0x40138000)

#define ETH_PKT_SIZE 1536  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 2           // Descriptor size (words)

static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_ETH_RAM MG_8BYTE_ALIGNED;
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_ETH_RAM MG_8BYTE_ALIGNED;
static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS] MG_ETH_RAM MG_8BYTE_ALIGNED;
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS] MG_ETH_RAM MG_8BYTE_ALIGNED;
static uint8_t s_txno;  // Current TX descriptor
static uint8_t s_rxno;  // Current RX descriptor

static struct mg_tcpip_if *s_ifp;  // MIP interface

static uint16_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ENET->MMFR = MG_BIT(30) |  // Start of frame delimiter
               MG_BIT(29) |  // Opcode
               ((addr & 0x1f) << 23) | ((reg & 0x1f) << 18) | MG_BIT(17);
  while ((ENET->EIR & MG_BIT(23)) == 0) (void) 0;
  ENET->EIR |= MG_BIT(23);
  return ENET->MMFR & 0xffff;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  ENET->MMFR = MG_BIT(30) |  // Start of frame delimiter
               MG_BIT(28) |  // Opcode
               ((addr & 0x1f) << 23) | ((reg & 0x1f) << 18) | MG_BIT(17) | val;
  while ((ENET->EIR & MG_BIT(23)) == 0) (void) 0;
  ENET->EIR |= MG_BIT(23);
}

static bool mg_tcpip_driver_rw612_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_rw612_data *d =
      (struct mg_tcpip_driver_rw612_data *) ifp->driver_data;
  s_ifp = ifp;
  ENET->MSCR = ((d->mdc_cr & 0x3f) << 1) | ((d->mdc_holdtime & 7) << 8);
  struct mg_phy phy = {eth_read_phy, eth_write_phy};
  mg_phy_init(&phy, d->phy_addr, 0);
  ENET->ECR |= MG_BIT(0);  // reset ETH

  // initialize descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][1] = (uint32_t) s_rxbuf[i];
    s_rxdesc[i][0] = MG_BIT(31);  // OWN
    if (i == ETH_DESC_CNT - 1) {
      s_rxdesc[i][0] |= MG_BIT(29);  // mark last descriptor
    }
    s_txdesc[i][1] = (uint32_t) s_txbuf[i];
    if (i == ETH_DESC_CNT - 1) {
      s_txdesc[i][0] |= MG_BIT(29);  // mark last descriptor
    }
  }

  ENET->RCR = (ENET->RCR & (0xffff << 16)) | MG_BIT(14) | MG_BIT(8) | MG_BIT(2);
  ENET->TCR = MG_BIT(2);  // full duplex
  ENET->TDSR = (uint32_t) &s_txdesc[0][0];
  ENET->RDSR = (uint32_t) &s_rxdesc[0][0];
  ENET->MRBR = ETH_PKT_SIZE;
  ENET->PALR =
      ifp->mac[0] << 24 | ifp->mac[1] << 16 | ifp->mac[2] << 8 | ifp->mac[3];
  ENET->PAUR |= (ifp->mac[4] << 24 | ifp->mac[5] << 16);
  ENET->IALR = 0;
  ENET->IAUR = 0;
  ENET->GALR = 0;
  ENET->GAUR = 0;
  ENET->MSCR = ((d->mdc_cr & 0x3f) << 1) | ((d->mdc_holdtime & 7) << 8);
  ENET->EIMR = MG_BIT(25);             // Enable RX interrupt
  ENET->ECR |= MG_BIT(8) | MG_BIT(1);  // DBSWP, Enable
  ENET->RDAR = 0;                      // activate RX descriptors ring
  return true;
}

static size_t mg_tcpip_driver_rw612_tx(const void *buf, size_t len,
                                       struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if (((s_txdesc[s_txno][0] & MG_BIT(31)) != 0)) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors"));
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);
    s_txdesc[s_txno][0] = len | MG_BIT(27) | MG_BIT(26);  // last buffer, crc
    if (s_txno == ETH_DESC_CNT - 1) {
      s_txdesc[s_txno][0] |= MG_BIT(29);  // wrap
    }
    s_txdesc[s_txno][0] |= MG_BIT(31);  // release ownership
    MG_DSB();
    ENET->TDAR = 0;
    // MG_INFO(("s_txdesc[%d][0]: 0x%x", s_txno, s_txdesc[s_txno][0]));
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }

  return len;
}


static void mg_tcpip_driver_rw612_update_hash_table(struct mg_tcpip_if *ifp) {
  // TODO(): read database, rebuild hash table
  ENET->GAUR = MG_BIT(1); // see imxrt, it reduces to this for mDNS
  (void) ifp;
}

static bool mg_tcpip_driver_rw612_poll(struct mg_tcpip_if *ifp, bool s1) {
  if (ifp->update_mac_hash_table) {
    mg_tcpip_driver_rw612_update_hash_table(ifp);
    ifp->update_mac_hash_table = false;
  }
  if (!s1) return false;
  struct mg_tcpip_driver_rw612_data *d =
      (struct mg_tcpip_driver_rw612_data *) ifp->driver_data;
  uint8_t speed = MG_PHY_SPEED_10M;
  bool up = false, full_duplex = false;
  struct mg_phy phy = {eth_read_phy, eth_write_phy};
  up = mg_phy_up(&phy, d->phy_addr, &full_duplex, &speed);
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    // tmp = reg with flags set to the most likely situation: 100M full-duplex
    // if(link is slow or half) set flags otherwise
    // reg = tmp
    if (speed == MG_PHY_SPEED_100M && (ENET->RCR & MG_BIT(9))) {
      ENET->RCR &= ~MG_BIT(9);
    } else if (speed == MG_PHY_SPEED_10M && (ENET->RCR & MG_BIT(9)) == 0) {
      ENET->RCR |= MG_BIT(9);
    }
    if (full_duplex && (ENET->TCR & MG_BIT(2)) == 0) {
      ENET->ECR &= ~MG_BIT(1);
      ENET->TCR |= MG_BIT(2);
      ENET->ECR |= MG_BIT(1);
    } else if (!full_duplex && (ENET->TCR & MG_BIT(2))) {
      ENET->ECR &= ~MG_BIT(1);
      ENET->TCR &= ~MG_BIT(2);
      ENET->ECR |= MG_BIT(1);
    }
    MG_INFO(("Link is %uM %s-duplex",
             speed == MG_PHY_SPEED_10M
                 ? 10
                 : (speed == MG_PHY_SPEED_100M ? 100 : 1000),
             full_duplex ? "full" : "half"));
  }
  return up;
}

void ENET_IRQHandler(void) {
  if (ENET->EIR & MG_BIT(25)) {
    ENET->EIR = MG_BIT(25);              // Ack RX
    for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
      if ((s_rxdesc[s_rxno][0] & MG_BIT(31)) != 0) break;  // exit when done
      // skip partial/errored frames
      if ((s_rxdesc[s_rxno][0] & MG_BIT(27)) &&
          !(s_rxdesc[s_rxno][0] &
            (MG_BIT(21) | MG_BIT(20) | MG_BIT(18) | MG_BIT(17) | MG_BIT(16)))) {
        size_t len = s_rxdesc[s_rxno][0] & 0xffff;
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len, s_ifp);
        s_rxdesc[s_rxno][0] |= MG_BIT(31);  // OWN bit: handle control to DMA
        MG_DSB();
        ENET->RDAR = 0;
        if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
      }
    }
  }
}

struct mg_tcpip_driver mg_tcpip_driver_rw612 = {mg_tcpip_driver_rw612_init,
                                                mg_tcpip_driver_rw612_tx, NULL,
                                                mg_tcpip_driver_rw612_poll};
#endif
