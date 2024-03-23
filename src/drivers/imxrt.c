#include "net_builtin.h"

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
#if defined(MG_DRIVER_IMXRT_RT11) && MG_DRIVER_IMXRT_RT11
#define ENET ((struct imxrt_enet *) (uintptr_t) 0x40424000U)
#define ETH_DESC_CNT 5     // Descriptors count
#else
#define ENET ((struct imxrt_enet *) (uintptr_t) 0x402D8000U)
#define ETH_DESC_CNT 4     // Descriptors count
#endif

#define ETH_PKT_SIZE 1536  // Max frame size, 64-bit aligned

struct enet_desc {
  uint16_t length;   // Data length
  uint16_t control;  // Control and status
  uint32_t *buffer;  // Data ptr
};

// TODO(): handle these in a portable compiler-independent CMSIS-friendly way
#define MG_64BYTE_ALIGNED __attribute__((aligned((64U))))

// Descriptors: in non-cached area (TODO(scaprile)), (37.5.1.22.2 37.5.1.23.2)
// Buffers: 64-byte aligned (37.3.14)
static volatile struct enet_desc s_rxdesc[ETH_DESC_CNT] MG_64BYTE_ALIGNED;
static volatile struct enet_desc s_txdesc[ETH_DESC_CNT] MG_64BYTE_ALIGNED;
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_64BYTE_ALIGNED;
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_64BYTE_ALIGNED;
static struct mg_tcpip_if *s_ifp;  // MIP interface

static uint16_t enet_read_phy(uint8_t addr, uint8_t reg) {
  ENET->EIR |= MG_BIT(23);  // MII interrupt clear
  ENET->MMFR = (1 << 30) | (2 << 28) | (addr << 23) | (reg << 18) | (2 << 16);
  while ((ENET->EIR & MG_BIT(23)) == 0) (void) 0;
  return ENET->MMFR & 0xffff;
}

static void enet_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  ENET->EIR |= MG_BIT(23);  // MII interrupt clear
  ENET->MMFR =
      (1 << 30) | (1 << 28) | (addr << 23) | (reg << 18) | (2 << 16) | val;
  while ((ENET->EIR & MG_BIT(23)) == 0) (void) 0;
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
  struct mg_phy phy = {enet_read_phy, enet_write_phy};
  mg_phy_init(&phy, d->phy_addr, MG_PHY_LEDS_ACTIVE_HIGH); // MAC clocks PHY  
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
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = (size_t) -1;  // fail
  } else if ((s_txdesc[s_txno].control & MG_BIT(15))) {
    ifp->nerr++;
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
  uint8_t speed = MG_PHY_SPEED_10M;
  bool up = false, full_duplex = false;
  struct mg_phy phy = {enet_read_phy, enet_write_phy};
  up = mg_phy_up(&phy, d->phy_addr, &full_duplex, &speed);
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    // tmp = reg with flags set to the most likely situation: 100M full-duplex
    // if(link is slow or half) set flags otherwise
    // reg = tmp
    uint32_t tcr = ENET->TCR | MG_BIT(2);             // Full-duplex
    uint32_t rcr = ENET->RCR & ~MG_BIT(9);            // 100M
    if (speed == MG_PHY_SPEED_10M) rcr |= MG_BIT(9);  // 10M
    if (full_duplex == false) tcr &= ~MG_BIT(2);      // Half-duplex
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
