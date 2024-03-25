#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RA) && MG_ENABLE_DRIVER_RA
struct ra_etherc {
  volatile uint32_t ECMR, RESERVED, RFLR, RESERVED1, ECSR, RESERVED2, ECSIPR,
      RESERVED3, PIR, RESERVED4, PSR, RESERVED5[5], RDMLR, RESERVED6[3], IPGR,
      APR, MPR, RESERVED7, RFCF, TPAUSER, TPAUSECR, BCFRR, RESERVED8[20], MAHR,
      RESERVED9, MALR, RESERVED10, TROCR, CDCR, LCCR, CNDCR, RESERVED11, CEFCR,
      FRECR, TSFRCR, TLFRCR, RFCR, MAFCR;
};

struct ra_edmac {
  volatile uint32_t EDMR, RESERVED, EDTRR, RESERVED1, EDRRR, RESERVED2, TDLAR,
      RESERVED3, RDLAR, RESERVED4, EESR, RESERVED5, EESIPR, RESERVED6, TRSCER,
      RESERVED7, RMFCR, RESERVED8, TFTR, RESERVED9, FDR, RESERVED10, RMCR,
      RESERVED11[2], TFUCR, RFOCR, IOSR, FCFTR, RESERVED12, RPADIR, TRIMD,
      RESERVED13[18], RBWAR, RDFAR, RESERVED14, TBRAR, TDFAR;
};

#undef ETHERC
#define ETHERC ((struct ra_etherc *) (uintptr_t) 0x40114100U)
#undef EDMAC
#define EDMAC ((struct ra_edmac *) (uintptr_t) 0x40114000U)
#undef RASYSC
#define RASYSC ((uint32_t *) (uintptr_t) 0x4001E000U)
#undef ICU_IELSR
#define ICU_IELSR ((uint32_t *) (uintptr_t) 0x40006300U)

#define ETH_PKT_SIZE 1536  // Max frame size, multiple of 32
#define ETH_DESC_CNT 4     // Descriptors count

// TODO(): handle these in a portable compiler-independent CMSIS-friendly way
#define MG_16BYTE_ALIGNED __attribute__((aligned((16U))))
#define MG_32BYTE_ALIGNED __attribute__((aligned((32U))))

// Descriptors: 16-byte aligned
// Buffers: 32-byte aligned (27.3.1)
static volatile uint32_t s_rxdesc[ETH_DESC_CNT][4] MG_16BYTE_ALIGNED;
static volatile uint32_t s_txdesc[ETH_DESC_CNT][4] MG_16BYTE_ALIGNED;
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_32BYTE_ALIGNED;
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE] MG_32BYTE_ALIGNED;
static struct mg_tcpip_if *s_ifp;  // MIP interface

// fastest is 3 cycles (SUB + BNE) on a 3-stage pipeline or equivalent
static inline void raspin(volatile uint32_t count) {
  while (count--) (void) 0;
}
// count to get the 200ns SMC semi-cycle period (2.5MHz) calling raspin():
// SYS_FREQUENCY * 200ns / 3 = SYS_FREQUENCY / 15000000
static uint32_t s_smispin;

// Bit-banged SMI
static void smi_preamble(void) {
  unsigned int i = 32;
  uint32_t pir = MG_BIT(1) | MG_BIT(2);  // write, mdio = 1, mdc = 0
  ETHERC->PIR = pir;
  while (i--) {
    pir &= ~MG_BIT(0);  // mdc = 0
    ETHERC->PIR = pir;
    raspin(s_smispin);
    pir |= MG_BIT(0);  // mdc = 1
    ETHERC->PIR = pir;
    raspin(s_smispin);
  }
}
static void smi_wr(uint16_t header, uint16_t data) {
  uint32_t word = (header << 16) | data;
  smi_preamble();
  unsigned int i = 32;
  while (i--) {
    uint32_t pir = MG_BIT(1) |
                   (word & 0x80000000 ? MG_BIT(2) : 0);  // write, mdc = 0, data
    ETHERC->PIR = pir;
    raspin(s_smispin);
    pir |= MG_BIT(0);  // mdc = 1
    ETHERC->PIR = pir;
    raspin(s_smispin);
    word <<= 1;
  }
}
static uint16_t smi_rd(uint16_t header) {
  smi_preamble();
  unsigned int i = 16;  // 2 LSb as turnaround
  uint32_t pir;
  while (i--) {
    pir = (i > 1 ? MG_BIT(1) : 0) |
          (header & 0x8000
               ? MG_BIT(2)
               : 0);  // mdc = 0, header, set read direction at turnaround
    ETHERC->PIR = pir;
    raspin(s_smispin);
    pir |= MG_BIT(0);  // mdc = 1
    ETHERC->PIR = pir;
    raspin(s_smispin);
    header <<= 1;
  }
  i = 16;
  uint16_t data = 0;
  while (i--) {
    data <<= 1;
    pir = 0;  // read, mdc = 0
    ETHERC->PIR = pir;
    raspin(s_smispin / 2);  // 1/4 clock period, 300ns max access time
    data |= (uint16_t)(ETHERC->PIR & MG_BIT(3) ? 1 : 0);  // read mdio
    raspin(s_smispin / 2);                    // 1/4 clock period
    pir |= MG_BIT(0);                         // mdc = 1
    ETHERC->PIR = pir;
    raspin(s_smispin);
  }
  return data;
}

static uint16_t raeth_read_phy(uint8_t addr, uint8_t reg) {
  return smi_rd((uint16_t)((1 << 14) | (2 << 12) | (addr << 7) | (reg << 2) | (2 << 0)));
}

static void raeth_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  smi_wr((uint16_t)((1 << 14) | (1 << 12) | (addr << 7) | (reg << 2) | (2 << 0)), val);
}

// MDC clock is generated manually; as per 802.3, it must not exceed 2.5MHz
static bool mg_tcpip_driver_ra_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_ra_data *d =
      (struct mg_tcpip_driver_ra_data *) ifp->driver_data;
  s_ifp = ifp;

  // Init SMI clock timing. If user told us the clock value, use it.
  // TODO(): Otherwise, guess
  s_smispin = d->clock / 15000000;

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = MG_BIT(31);             // RACT
    s_rxdesc[i][1] = ETH_PKT_SIZE << 16;     // RBL
    s_rxdesc[i][2] = (uint32_t) s_rxbuf[i];  // Point to data buffer
  }
  s_rxdesc[ETH_DESC_CNT - 1][0] |= MG_BIT(30);  // Wrap last descriptor

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    // TACT = 0
    s_txdesc[i][2] = (uint32_t) s_txbuf[i];
  }
  s_txdesc[ETH_DESC_CNT - 1][0] |= MG_BIT(30);  // Wrap last descriptor

  EDMAC->EDMR = MG_BIT(0);  // Software reset, wait 64 PCLKA clocks (27.2.1)
  uint32_t sckdivcr = RASYSC[8];  // get divisors from SCKDIVCR (8.2.2)
  uint32_t ick = 1 << ((sckdivcr >> 24) & 7);   // sys_clock div
  uint32_t pcka = 1 << ((sckdivcr >> 12) & 7);  // pclka div
  raspin((64U * pcka) / (3U * ick));
  EDMAC->EDMR = MG_BIT(6);  // Initialize, little-endian (27.2.1)

  MG_DEBUG(("PHY addr: %d, smispin: %d", d->phy_addr, s_smispin));
  struct mg_phy phy = {raeth_read_phy, raeth_write_phy};
  mg_phy_init(&phy, d->phy_addr, 0); // MAC clocks PHY

  // Select RMII mode,
  ETHERC->ECMR = MG_BIT(2) | MG_BIT(1);  // 100M, Full-duplex, CRC
  // ETHERC->ECMR |= MG_BIT(0);             // Receive all
  ETHERC->RFLR = 1518;  // Set max rx length

  EDMAC->RDLAR = (uint32_t) (uintptr_t) s_rxdesc;
  EDMAC->TDLAR = (uint32_t) (uintptr_t) s_txdesc;
  // MAC address filtering (bytes in reversed order)
  ETHERC->MAHR = (uint32_t) (ifp->mac[0] << 24U) |
                 ((uint32_t) ifp->mac[1] << 16U) |
                 ((uint32_t) ifp->mac[2] << 8U) | ifp->mac[3];
  ETHERC->MALR = ((uint32_t) ifp->mac[4] << 8U) | ifp->mac[5];

  EDMAC->TFTR = 0;                        // Store and forward (27.2.10)
  EDMAC->FDR = 0x070f;                    // (27.2.11)
  EDMAC->RMCR = MG_BIT(0);                // (27.2.12)
  ETHERC->ECMR |= MG_BIT(6) | MG_BIT(5);  // TE RE
  EDMAC->EESIPR = MG_BIT(18);             // Enable Rx IRQ
  EDMAC->EDRRR = MG_BIT(0);               // Receive Descriptors have changed
  EDMAC->EDTRR = MG_BIT(0);               // Transmit Descriptors have changed
  return true;
}

// Transmit frame
static size_t mg_tcpip_driver_ra_tx(const void *buf, size_t len,
                                    struct mg_tcpip_if *ifp) {
  static int s_txno;  // Current descriptor index
  if (len > sizeof(s_txbuf[ETH_DESC_CNT])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = (size_t) -1;  // fail
  } else if ((s_txdesc[s_txno][0] & MG_BIT(31))) {
    ifp->nerr++;
    MG_ERROR(("No descriptors available"));
    len = 0;  // retry later
  } else {
    memcpy(s_txbuf[s_txno], buf, len);            // Copy data
    s_txdesc[s_txno][1] = len << 16;              // Set data len
    s_txdesc[s_txno][0] |= MG_BIT(31) | 3 << 28;  // (27.3.1.1) mark valid
    EDMAC->EDTRR = MG_BIT(0);                     // Transmit request
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  return len;
}

static bool mg_tcpip_driver_ra_up(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_ra_data *d =
      (struct mg_tcpip_driver_ra_data *) ifp->driver_data;
  uint8_t speed = MG_PHY_SPEED_10M;
  bool up = false, full_duplex = false;
  struct mg_phy phy = {raeth_read_phy, raeth_write_phy};
  up = mg_phy_up(&phy, d->phy_addr, &full_duplex, &speed);
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    // tmp = reg with flags set to the most likely situation: 100M full-duplex
    // if(link is slow or half) set flags otherwise
    // reg = tmp
    uint32_t ecmr = ETHERC->ECMR | MG_BIT(2) | MG_BIT(1);  // 100M Full-duplex
    if (speed == MG_PHY_SPEED_10M) ecmr &= ~MG_BIT(2);     // 10M
    if (full_duplex == false) ecmr &= ~MG_BIT(1);          // Half-duplex
    ETHERC->ECMR = ecmr;  // IRQ handler does not fiddle with these registers
    MG_DEBUG(("Link is %uM %s-duplex", ecmr & MG_BIT(2) ? 100 : 10,
              ecmr & MG_BIT(1) ? "full" : "half"));
  }
  return up;
}

void EDMAC_IRQHandler(void);
static uint32_t s_rxno;
void EDMAC_IRQHandler(void) {
  struct mg_tcpip_driver_ra_data *d =
      (struct mg_tcpip_driver_ra_data *) s_ifp->driver_data;
  EDMAC->EESR = MG_BIT(18);            // Ack IRQ in EDMAC 1st
  ICU_IELSR[d->irqno] &= ~MG_BIT(16);  // Ack IRQ in ICU last
  // Frame received, loop
  for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
    uint32_t r = s_rxdesc[s_rxno][0];
    if (r & MG_BIT(31)) break;  // exit when done
    // skip partial/errored frames (27.3.1.2)
    if ((r & (MG_BIT(29) | MG_BIT(28)) && !(r & MG_BIT(27)))) {
      size_t len = s_rxdesc[s_rxno][1] & 0xffff;
      mg_tcpip_qwrite(s_rxbuf[s_rxno], len, s_ifp);  // CRC already stripped
    }
    s_rxdesc[s_rxno][0] |= MG_BIT(31);
    if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
  }
  EDMAC->EDRRR = MG_BIT(0);  // Receive Descriptors have changed
  // If b0 == 0, descriptors were exhausted and probably frames were dropped,
  // (27.2.9 RMFCR counts them)
}

struct mg_tcpip_driver mg_tcpip_driver_ra = {mg_tcpip_driver_ra_init,
                                             mg_tcpip_driver_ra_tx, NULL,
                                             mg_tcpip_driver_ra_up};

#endif
