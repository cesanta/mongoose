#include "tcpip.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RT1020) && \
    MG_ENABLE_DRIVER_RT1020
struct rt1020_enet {
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
#define ENET ((struct rt1020_enet *) (uintptr_t) 0x402D8000u)

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))

// Max frame size, every buffer must be 64-bit aligned (1536 = 0x600)
#define ETH_PKT_SIZE 1536
#define ETH_DESC_CNT 4  // Descriptors count

typedef struct {
  uint16_t length;   // Data length
  uint16_t control;  // Control and status
  uint32_t *buffer;  // Data ptr
} enet_bd_t;

// TODO(): handle these in a portable compiler-independent CMSIS-friendly way
// Descriptors: in non-cached area (TODO(scaprile)), 64-bit aligned
enet_bd_t s_rxdesc[ETH_DESC_CNT] __attribute__((aligned((64U))));
enet_bd_t s_txdesc[ETH_DESC_CNT] __attribute__((aligned((64U))));
// Buffers: 64-bit aligned
uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE] __attribute__((aligned((64U))));
uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE] __attribute__((aligned((64U))));

static struct mg_tcpip_if *s_ifp;  // MIP interface

enum { PHY_ADDR = 2, PHY_BCR = 0, PHY_BSR = 1, PHY_PC1R = 30, PHY_PC2R = 31 };

static uint32_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ENET->EIR |= BIT(23);  // MII interrupt clear
  ENET->MMFR = (1 << 30) | (2 << 28) | (addr << 23) | (reg << 18) | (2 << 16);
  while ((ENET->EIR & BIT(23)) == 0) (void) 0;
  return ENET->MMFR & 0xffff;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  ENET->EIR |= BIT(23);  // MII interrupt clear
  ENET->MMFR =
      (1 << 30) | (1 << 28) | (addr << 23) | (reg << 18) | (2 << 16) | val;
  while ((ENET->EIR & BIT(23)) == 0) (void) 0;
}

//  MDC clock is generated from IPS Bus clock (ipg_clk); as per 802.3,
//  it must not exceed 2.5MHz
// The PHY receives the PLL6-generated 50MHz clock
static bool mg_tcpip_driver_rt1020_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_rt1020_data *d =
      (struct mg_tcpip_driver_rt1020_data *) ifp->driver_data;
  s_ifp = ifp;

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i].control = BIT(15);                 // Own (E)
    s_rxdesc[i].buffer = (uint32_t *) s_rxbuf[i];  // Point to data buffer
  }
  s_rxdesc[ETH_DESC_CNT - 1].control |= BIT(13);  // Wrap last descriptor

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i].control = BIT(10);  // Own (TC)
    s_txdesc[i].buffer = (uint32_t *) s_txbuf[i];
  }
  s_txdesc[ETH_DESC_CNT - 1].control |= BIT(13);  // Wrap last descriptor

  ENET->ECR = BIT(0);                     // Software reset, disable
  while ((ENET->ECR & BIT(0))) (void) 0;  // Wait until done

  // Set MDC clock divider. If user told us the value, use it.
  // TODO(): Otherwise, guess (currently assuming max freq)
  int cr = (d == NULL || d->mdc_cr < 0) ? 24 : d->mdc_cr;
  ENET->MSCR = (1 << 8) | ((cr & 0x3f) << 1);  // HOLDTIME 2 clks

  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(15));  // Reset PHY
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(12));  // Set autonegotiation
  // PHY: Enable 50 MHz external ref clock at XI (preserve defaults)
  eth_write_phy(PHY_ADDR, PHY_PC2R, BIT(15) | BIT(8) | BIT(7));
  // Select RMII mode, 100M, keep CRC, set max rx length, disable loop
  ENET->RCR = (1518 << 16) | BIT(8) | BIT(2);
  // ENET->RCR |= BIT(3);     // Receive all
  ENET->TCR = BIT(2);  // Full-duplex
  ENET->RDSR = (uint32_t) (uintptr_t) s_rxdesc;
  ENET->TDSR = (uint32_t) (uintptr_t) s_txdesc;
  ENET->MRBR[0] = ETH_PKT_SIZE;  // Same size for RX/TX buffers
  // MAC address filtering (bytes in reversed order)
  ENET->PAUR = ((uint32_t) ifp->mac[4] << 24U) | (uint32_t) ifp->mac[5] << 16U;
  ENET->PALR = (uint32_t) (ifp->mac[0] << 24U) |
               ((uint32_t) ifp->mac[1] << 16U) |
               ((uint32_t) ifp->mac[2] << 8U) | ifp->mac[3];
  ENET->ECR = BIT(8) | BIT(1);  // Little-endian CPU, Enable
  ENET->EIMR = BIT(25);         // Set interrupt mask
  ENET->RDAR = BIT(24);         // Receive Descriptors have changed
  return true;
}

// Transmit frame
static uint32_t s_txno;

static size_t mg_tcpip_driver_rt1020_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[ETH_DESC_CNT])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // fail
  } else if ((s_txdesc[s_txno].control & BIT(15))) {
    ifp->nerr++;
    MG_ERROR(("No descriptors available"));
    len = 0;  // fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);         // Copy data
    s_txdesc[s_txno].length = (uint16_t) len;  // Set data len
    // Table 37-34, R, L, TC (Ready, last, transmit CRC after frame
    s_txdesc[s_txno].control |= (uint16_t) (BIT(15) | BIT(11) | BIT(10));
    ENET->TDAR = BIT(24);  // Descriptor ring updated
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  (void) ifp;
  return len;
}

static bool mg_tcpip_driver_rt1020_up(struct mg_tcpip_if *ifp) {
  uint32_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  bool up = bsr & BIT(2) ? 1 : 0;
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    uint32_t pc1r = eth_read_phy(PHY_ADDR, PHY_PC1R);
    uint32_t tcr = ENET->TCR |= BIT(2);        // Full-duplex
    uint32_t rcr = ENET->RCR &= ~BIT(9);       // 100M
    if ((pc1r & 3) == 1) rcr |= BIT(9);        // 10M
    if ((pc1r & BIT(2)) == 0) tcr &= ~BIT(2);  // Half-duplex
    ENET->TCR = tcr;  // IRQ handler does not fiddle with these registers
    ENET->RCR = rcr;
    MG_DEBUG(("Link is %uM %s-duplex", rcr & BIT(9) ? 10 : 100,
              tcr & BIT(2) ? "full" : "half"));
  }
  return up;
}

void ENET_IRQHandler(void);
static uint32_t s_rxno;
void ENET_IRQHandler(void) {
  ENET->EIR = BIT(25);  // Ack IRQ
  // Frame received, loop
  for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
    if (s_rxdesc[s_rxno].control & BIT(15)) break;  // exit when done
    // skip partial/errored frames (Table 37-32)
    if ((s_rxdesc[s_rxno].control & BIT(11)) &&
        !(s_rxdesc[s_rxno].control &
          (BIT(5) | BIT(4) | BIT(2) | BIT(1) | BIT(0)))) {
      uint32_t len = (s_rxdesc[s_rxno].length);
      mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
    }
    s_rxdesc[s_rxno].control |= BIT(15);
    if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
  }
  ENET->RDAR = BIT(24);  // Receive Descriptors have changed
  // If b24 == 0, descriptors were exhausted and probably frames were dropped
}

struct mg_tcpip_driver mg_tcpip_driver_rt1020 = {
    mg_tcpip_driver_rt1020_init, mg_tcpip_driver_rt1020_tx, NULL,
    mg_tcpip_driver_rt1020_up};

#endif
