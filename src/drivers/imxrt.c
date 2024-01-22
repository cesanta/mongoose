#include "tcpip.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_IMXRT) && MG_ENABLE_DRIVER_IMXRT
struct imxrt_enet {
  volatile uint32_t RESERVED0, EIR, EIMR, RESERVED1, RDAR, TDAR, RESERVED2[3],
      ECR, RESERVED3[6], MMFR, MSCR, RESERVED4[7], MIBC, RESERVED5[7], RCR,
      RESERVED6[15], TCR, RESERVED7[7], PALR, PAUR, OPD, TXIC0, TXIC1, TXIC2,
      RESERVED8, RXIC0, RXIC1, RXIC2, RESERVED9[3], IAUR, IALR, GAUR, GALR,
      RESERVED10[7], TFWR, RESERVED11[14], RDSR, TDSR, MRBR[2], RSFL, RSEM,
      RAEM, RAFL, TSEM, TAEM, TAFL, TIPG, FTRL, RESERVED12[3], TACC, RACC,
      RESERVED13[15], RMON_T_PACKETS, RMON_T_BC_PKT, RMON_T_MC_PKT,
      RMON_T_CRC_ALIGN, RMON_T_UNDERSIZE, RMON_T_OVERSIZE, RMON_T_FRAG,
      RMON_T_JAB, RMON_T_COL, RMON_T_P64, RMON_T_P65TO127, RMON_T_P128TO255,
      RMON_T_P256TO511, RMON_T_P512TO1023, RMON_T_P1024TO2048, RMON_T_GTE2048,
      RMON_T_OCTETS, IEEE_T_DROP, IEEE_T_FRAME_OK, IEEE_T_1COL, IEEE_T_MCOL,
      IEEE_T_DEF, IEEE_T_LCOL, IEEE_T_EXCOL, IEEE_T_MACERR, IEEE_T_CSERR,
      IEEE_T_SQE, IEEE_T_FDXFC, IEEE_T_OCTETS_OK, RESERVED14[3], RMON_R_PACKETS,
      RMON_R_BC_PKT, RMON_R_MC_PKT, RMON_R_CRC_ALIGN, RMON_R_UNDERSIZE,
      RMON_R_OVERSIZE, RMON_R_FRAG, RMON_R_JAB, RESERVED15, RMON_R_P64,
      RMON_R_P65TO127, RMON_R_P128TO255, RMON_R_P256TO511, RMON_R_P512TO1023,
      RMON_R_P1024TO2047, RMON_R_GTE2048, RMON_R_OCTETS, IEEE_R_DROP,
      IEEE_R_FRAME_OK, IEEE_R_CRC, IEEE_R_ALIGN, IEEE_R_MACERR, IEEE_R_FDXFC,
      IEEE_R_OCTETS_OK, RESERVED16[71], ATCR, ATVR, ATOFF, ATPER, ATCOR, ATINC,
      ATSTMP, RESERVED17[122], TGSR, TCSR0, TCCR0, TCSR1, TCCR1, TCSR2, TCCR2,
      TCSR3;
};

#undef ENET
#define ENET ((struct imxrt_enet *) (uintptr_t) 0x402D8000U)

#define ETH_PKT_SIZE 1536  // Max frame size, 64-bit aligned
#define ETH_DESC_CNT 4     // Descriptors count

struct enet_desc {
  uint16_t length;   // Data length
  uint16_t control;  // Control and status
  uint32_t *buffer;  // Data ptr
};

// TODO(): handle these in a portable compiler-independent CMSIS-friendly way
#define MG_64BIT_ALIGNED __attribute__((aligned((64U))))

// Descriptors: in non-cached area (TODO(scaprile)), 64-bit aligned
// Buffers: 64-bit aligned
static volatile struct enet_desc s_rxdesc[ETH_DESC_CNT] MG_64BIT_ALIGNED;
static volatile struct enet_desc s_txdesc[ETH_DESC_CNT] MG_64BIT_ALIGNED;
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_64BIT_ALIGNED;
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_64BIT_ALIGNED;
static struct mg_tcpip_if *s_ifp;  // MIP interface

enum {
  MG_PHYREG_BCR = 0,
  MG_PHYREG_BSR = 1,
  MG_PHYREG_ID1 = 2,
  MG_PHYREG_ID2 = 3
};

static uint16_t enet_phy_read(uint8_t addr, uint8_t reg) {
  ENET->EIR |= MG_BIT(23);  // MII interrupt clear
  ENET->MMFR = (1 << 30) | (2 << 28) | (addr << 23) | (reg << 18) | (2 << 16);
  while ((ENET->EIR & MG_BIT(23)) == 0) (void) 0;
  return ENET->MMFR & 0xffff;
}

static void enet_phy_write(uint8_t addr, uint8_t reg, uint16_t val) {
  ENET->EIR |= MG_BIT(23);  // MII interrupt clear
  ENET->MMFR =
      (1 << 30) | (1 << 28) | (addr << 23) | (reg << 18) | (2 << 16) | val;
  while ((ENET->EIR & MG_BIT(23)) == 0) (void) 0;
}

static uint32_t enet_phy_id(uint8_t addr) {
  uint16_t phy_id1 = enet_phy_read(addr, MG_PHYREG_ID1);
  uint16_t phy_id2 = enet_phy_read(addr, MG_PHYREG_ID2);
  return (uint32_t) phy_id1 << 16 | phy_id2;
}

//  MDC clock is generated from IPS Bus clock (ipg_clk); as per 802.3,
//  it must not exceed 2.5MHz
// The PHY receives the PLL6-generated 50MHz clock
static bool mg_tcpip_driver_imxrt_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_imxrt_data *d =
      (struct mg_tcpip_driver_imxrt_data *) ifp->driver_data;
  s_ifp = ifp;

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i].control = MG_BIT(15);              // Own (E)
    s_rxdesc[i].buffer = (uint32_t *) s_rxbuf[i];  // Point to data buffer
  }
  s_rxdesc[ETH_DESC_CNT - 1].control |= MG_BIT(13);  // Wrap last descriptor

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    // s_txdesc[i].control = MG_BIT(10);  // Own (TC)
    s_txdesc[i].buffer = (uint32_t *) s_txbuf[i];
  }
  s_txdesc[ETH_DESC_CNT - 1].control |= MG_BIT(13);  // Wrap last descriptor

  ENET->ECR = MG_BIT(0);                     // Software reset, disable
  while ((ENET->ECR & MG_BIT(0))) (void) 0;  // Wait until done

  // Set MDC clock divider. If user told us the value, use it.
  // TODO(): Otherwise, guess (currently assuming max freq)
  int cr = (d == NULL || d->mdc_cr < 0) ? 24 : d->mdc_cr;
  ENET->MSCR = (1 << 8) | ((cr & 0x3f) << 1);  // HOLDTIME 2 clks

  enet_phy_write(d->phy_addr, MG_PHYREG_BCR, MG_BIT(15));  // Reset PHY
  enet_phy_write(d->phy_addr, MG_PHYREG_BCR,
                 MG_BIT(12));  // Set autonegotiation

  // PHY: Enable 50 MHz external ref clock at XI (preserve defaults)
  uint32_t id = enet_phy_id(d->phy_addr);
  MG_INFO(("PHY ID: %#04x %#04x", (uint16_t) (id >> 16), (uint16_t) id));
  // 2000 a140 - TI DP83825I
  // 0007 c0fx - LAN8720
  // 0022 1561 - KSZ8081RNB

  if ((id & 0xffff0000) == 0x220000) {  // KSZ8081RNB, like EVK-RTxxxx boards
    enet_phy_write(d->phy_addr, 31,
                   MG_BIT(15) | MG_BIT(8) | MG_BIT(7));  // PC2R
  } else if ((id & 0xffff0000) == 0x20000000) {  // DP83825I, like Teensy4.1
    enet_phy_write(d->phy_addr, 23, 0x81);       // 50MHz clock input
    enet_phy_write(d->phy_addr, 24, 0x280);      // LED status, active high
  } else {                                       // Default to LAN8720
    MG_INFO(("Defaulting to LAN8720 PHY..."));   // TODO()
  }

  // Select RMII mode, 100M, keep CRC, set max rx length, disable loop
  ENET->RCR = (1518 << 16) | MG_BIT(8) | MG_BIT(2);
  // ENET->RCR |= MG_BIT(3);     // Receive all
  ENET->TCR = MG_BIT(2);  // Full-duplex
  ENET->RDSR = (uint32_t) (uintptr_t) s_rxdesc;
  ENET->TDSR = (uint32_t) (uintptr_t) s_txdesc;
  ENET->MRBR[0] = ETH_PKT_SIZE;  // Same size for RX/TX buffers
  // MAC address filtering (bytes in reversed order)
  ENET->PAUR = ((uint32_t) ifp->mac[4] << 24U) | (uint32_t) ifp->mac[5] << 16U;
  ENET->PALR = (uint32_t) (ifp->mac[0] << 24U) |
               ((uint32_t) ifp->mac[1] << 16U) |
               ((uint32_t) ifp->mac[2] << 8U) | ifp->mac[3];
  ENET->ECR = MG_BIT(8) | MG_BIT(1);  // Little-endian CPU, Enable
  ENET->EIMR = MG_BIT(25);            // Set interrupt mask
  ENET->RDAR = MG_BIT(24);            // Receive Descriptors have changed
  ENET->TDAR = MG_BIT(24);            // Transmit Descriptors have changed
  // ENET->OPD = 0x10014;
  return true;
}

// Transmit frame
static size_t mg_tcpip_driver_imxrt_tx(const void *buf, size_t len,
                                       struct mg_tcpip_if *ifp) {
  static int s_txno;  // Current descriptor index
  if (len > sizeof(s_txbuf[ETH_DESC_CNT])) {
    ifp->nerr++;
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = (size_t) -1;  // fail
  } else if ((s_txdesc[s_txno].control & MG_BIT(15))) {
    MG_ERROR(("No descriptors available"));
    len = 0;  // retry later
  } else {
    memcpy(s_txbuf[s_txno], buf, len);         // Copy data
    s_txdesc[s_txno].length = (uint16_t) len;  // Set data len
    // Table 37-34, R, L, TC (Ready, last, transmit CRC after frame
    s_txdesc[s_txno].control |=
        (uint16_t) (MG_BIT(15) | MG_BIT(11) | MG_BIT(10));
    ENET->TDAR = MG_BIT(24);  // Descriptor ring updated
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  (void) ifp;
  return len;
}

static bool mg_tcpip_driver_imxrt_up(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_imxrt_data *d =
      (struct mg_tcpip_driver_imxrt_data *) ifp->driver_data;
  uint32_t bsr = enet_phy_read(d->phy_addr, MG_PHYREG_BSR);
  bool up = bsr & MG_BIT(2) ? 1 : 0;
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    // tmp = reg with flags set to the most likely situation: 100M full-duplex
    // if(link is slow or half) set flags otherwise
    // reg = tmp
    uint32_t tcr = ENET->TCR | MG_BIT(2);   // Full-duplex
    uint32_t rcr = ENET->RCR & ~MG_BIT(9);  // 100M
    uint32_t phy_id = enet_phy_id(d->phy_addr);
    if ((phy_id & 0xffff0000) == 0x220000) {             // KSZ8081RNB
      uint16_t pc1r = enet_phy_read(d->phy_addr, 30);    // Read PC1R
      if ((pc1r & 3) == 1) rcr |= MG_BIT(9);             // 10M
      if ((pc1r & MG_BIT(2)) == 0) tcr &= ~MG_BIT(2);    // Half-duplex
    } else if ((phy_id & 0xffff0000) == 0x20000000) {    // DP83825I
      uint16_t physts = enet_phy_read(d->phy_addr, 16);  // Read PHYSTS
      if (physts & MG_BIT(1)) rcr |= MG_BIT(9);          // 10M
      if ((physts & MG_BIT(2)) == 0) tcr &= ~MG_BIT(2);  // Half-duplex
    } else {                                             // Default to LAN8720
      uint16_t scsr = enet_phy_read(d->phy_addr, 31);    // Read CSCR
      if ((scsr & MG_BIT(3)) == 0) rcr |= MG_BIT(9);     // 10M
      if ((scsr & MG_BIT(4)) == 0) tcr &= ~MG_BIT(2);    // Half-duplex
    }
    ENET->TCR = tcr;  // IRQ handler does not fiddle with these registers
    ENET->RCR = rcr;
    MG_DEBUG(("Link is %uM %s-duplex", rcr & MG_BIT(9) ? 10 : 100,
              tcr & MG_BIT(2) ? "full" : "half"));
  }
  return up;
}

void ENET_IRQHandler(void);
static uint32_t s_rxno;
void ENET_IRQHandler(void) {
  ENET->EIR = MG_BIT(25);  // Ack IRQ
  // Frame received, loop
  for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
    uint32_t r = s_rxdesc[s_rxno].control;
    if (r & MG_BIT(15)) break;  // exit when done
    // skip partial/errored frames (Table 37-32)
    if ((r & MG_BIT(11)) &&
        !(r & (MG_BIT(5) | MG_BIT(4) | MG_BIT(2) | MG_BIT(1) | MG_BIT(0)))) {
      size_t len = s_rxdesc[s_rxno].length;
      mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
    }
    s_rxdesc[s_rxno].control |= MG_BIT(15);
    if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
  }
  ENET->RDAR = MG_BIT(24);  // Receive Descriptors have changed
  // If b24 == 0, descriptors were exhausted and probably frames were dropped
}

struct mg_tcpip_driver mg_tcpip_driver_imxrt = {mg_tcpip_driver_imxrt_init,
                                                mg_tcpip_driver_imxrt_tx, NULL,
                                                mg_tcpip_driver_imxrt_up};

#endif
