#include "tcpip.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_STM32) && MG_ENABLE_DRIVER_STM32
struct stm32_eth {
  volatile uint32_t MACCR, MACFFR, MACHTHR, MACHTLR, MACMIIAR, MACMIIDR, MACFCR,
      MACVLANTR, RESERVED0[2], MACRWUFFR, MACPMTCSR, RESERVED1, MACDBGR, MACSR,
      MACIMR, MACA0HR, MACA0LR, MACA1HR, MACA1LR, MACA2HR, MACA2LR, MACA3HR,
      MACA3LR, RESERVED2[40], MMCCR, MMCRIR, MMCTIR, MMCRIMR, MMCTIMR,
      RESERVED3[14], MMCTGFSCCR, MMCTGFMSCCR, RESERVED4[5], MMCTGFCR,
      RESERVED5[10], MMCRFCECR, MMCRFAECR, RESERVED6[10], MMCRGUFCR,
      RESERVED7[334], PTPTSCR, PTPSSIR, PTPTSHR, PTPTSLR, PTPTSHUR, PTPTSLUR,
      PTPTSAR, PTPTTHR, PTPTTLR, RESERVED8, PTPTSSR, PTPPPSCR, RESERVED9[564],
      DMABMR, DMATPDR, DMARPDR, DMARDLAR, DMATDLAR, DMASR, DMAOMR, DMAIER,
      DMAMFBOCR, DMARSWTR, RESERVED10[8], DMACHTDR, DMACHRDR, DMACHTBAR,
      DMACHRBAR;
};
#undef ETH
#define ETH ((struct stm32_eth *) (uintptr_t) 0x40028000)

#undef DSB
#if defined(__CC_ARM)
#define DSB() __dsb(0xF)
#elif defined(__ARMCC_VERSION)
#define DSB() __builtin_arm_dsb(0xF)
#elif defined(__GNUC__) && defined(__arm__) && defined(__thumb__)
#define DSB() asm("DSB 0xF")
#elif defined(__ICCARM__)
#define DSB() __iar_builtin_DSB()
#else
#define DSB()
#endif

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))
#define ETH_PKT_SIZE 1540  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 4           // Descriptor size (words)

static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];      // RX descriptors
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];      // TX descriptors
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE];  // RX ethernet buffers
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE];  // TX ethernet buffers
static uint8_t s_txno;                               // Current TX descriptor
static uint8_t s_rxno;                               // Current RX descriptor

static struct mg_tcpip_if *s_ifp;  // MIP interface
enum { PHY_ADDR = 0, PHY_BCR = 0, PHY_BSR = 1, PHY_CSCR = 31 };

static uint32_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ETH->MACMIIAR &= (7 << 2);
  ETH->MACMIIAR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6);
  ETH->MACMIIAR |= BIT(0);
  while (ETH->MACMIIAR & BIT(0)) (void) 0;
  return ETH->MACMIIDR;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  ETH->MACMIIDR = val;
  ETH->MACMIIAR &= (7 << 2);
  ETH->MACMIIAR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6) | BIT(1);
  ETH->MACMIIAR |= BIT(0);
  while (ETH->MACMIIAR & BIT(0)) (void) 0;
}

static uint32_t get_hclk(void) {
  struct rcc {
    volatile uint32_t CR, PLLCFGR, CFGR;
  } *rcc = (struct rcc *) 0x40023800;
  uint32_t clk = 0, hsi = 16000000 /* 16 MHz */, hse = 8000000 /* 8MHz */;

  if (rcc->CFGR & (1 << 2)) {
    clk = hse;
  } else if (rcc->CFGR & (1 << 3)) {
    uint32_t vco, m, n, p;
    m = (rcc->PLLCFGR & (0x3f << 0)) >> 0;
    n = (rcc->PLLCFGR & (0x1ff << 6)) >> 6;
    p = (((rcc->PLLCFGR & (3 << 16)) >> 16) + 1) * 2;
    clk = (rcc->PLLCFGR & (1 << 22)) ? hse : hsi;
    vco = (uint32_t) ((uint64_t) clk * n / m);
    clk = vco / p;
  } else {
    clk = hsi;
  }
  uint32_t hpre = (rcc->CFGR & (15 << 4)) >> 4;
  if (hpre < 8) return clk;

  uint8_t ahbptab[8] = {1, 2, 3, 4, 6, 7, 8, 9};  // log2(div)
  return ((uint32_t) clk) >> ahbptab[hpre - 8];
}

//  Guess CR from HCLK. MDC clock is generated from HCLK (AHB); as per 802.3,
//  it must not exceed 2.5MHz As the AHB clock can be (and usually is) derived
//  from the HSI (internal RC), and it can go above specs, the datasheets
//  specify a range of frequencies and activate one of a series of dividers to
//  keep the MDC clock safely below 2.5MHz. We guess a divider setting based on
//  HCLK with a +5% drift. If the user uses a different clock from our
//  defaults, needs to set the macros on top Valid for STM32F74xxx/75xxx
//  (38.8.1) and STM32F42xxx/43xxx (33.8.1) (both 4.5% worst case drift)
static int guess_mdc_cr(void) {
  uint8_t crs[] = {2, 3, 0, 1, 4, 5};          // ETH->MACMIIAR::CR values
  uint8_t div[] = {16, 26, 42, 62, 102, 124};  // Respective HCLK dividers
  uint32_t hclk = get_hclk();                  // Guess system HCLK
  int result = -1;                             // Invalid CR value
  if (hclk < 25000000) {
    MG_ERROR(("HCLK too low"));
  } else {
    for (int i = 0; i < 6; i++) {
      if (hclk / div[i] <= 2375000UL /* 2.5MHz - 5% */) {
        result = crs[i];
        break;
      }
    }
    if (result < 0) MG_ERROR(("HCLK too high"));
  }
  MG_DEBUG(("HCLK: %u, CR: %d", hclk, result));
  return result;
}

static bool mg_tcpip_driver_stm32_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_stm32_data *d =
      (struct mg_tcpip_driver_stm32_data *) ifp->driver_data;
  s_ifp = ifp;

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = BIT(31);                            // Own
    s_rxdesc[i][1] = sizeof(s_rxbuf[i]) | BIT(14);       // 2nd address chained
    s_rxdesc[i][2] = (uint32_t) (uintptr_t) s_rxbuf[i];  // Point to data buffer
    s_rxdesc[i][3] =
        (uint32_t) (uintptr_t) s_rxdesc[(i + 1) % ETH_DESC_CNT];  // Chain
  }

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i][2] = (uint32_t) (uintptr_t) s_txbuf[i];  // Buf pointer
    s_txdesc[i][3] =
        (uint32_t) (uintptr_t) s_txdesc[(i + 1) % ETH_DESC_CNT];  // Chain
  }

  ETH->DMABMR |= BIT(0);                         // Software reset
  while ((ETH->DMABMR & BIT(0)) != 0) (void) 0;  // Wait until done

  // Set MDC clock divider. If user told us the value, use it. Otherwise, guess
  int cr = (d == NULL || d->mdc_cr < 0) ? guess_mdc_cr() : d->mdc_cr;
  ETH->MACMIIAR = ((uint32_t) cr & 7) << 2;

  // NOTE(cpq): we do not use extended descriptor bit 7, and do not use
  // hardware checksum. Therefore, descriptor size is 4, not 8
  // ETH->DMABMR = BIT(13) | BIT(16) | BIT(22) | BIT(23) | BIT(25);
  ETH->MACIMR = BIT(3) | BIT(9);  // Mask timestamp & PMT IT
  ETH->MACFCR = BIT(7);           // Disable zero quarta pause
  // ETH->MACFFR = BIT(31);                            // Receive all
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(15));           // Reset PHY
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(12));           // Set autonegotiation
  ETH->DMARDLAR = (uint32_t) (uintptr_t) s_rxdesc;     // RX descriptors
  ETH->DMATDLAR = (uint32_t) (uintptr_t) s_txdesc;     // RX descriptors
  ETH->DMAIER = BIT(6) | BIT(16);                      // RIE, NISE
  ETH->MACCR = BIT(2) | BIT(3) | BIT(11) | BIT(14);    // RE, TE, Duplex, Fast
  ETH->DMAOMR = BIT(1) | BIT(13) | BIT(21) | BIT(25);  // SR, ST, TSF, RSF

  // MAC address filtering
  ETH->MACA0HR = ((uint32_t) ifp->mac[5] << 8U) | ifp->mac[4];
  ETH->MACA0LR = (uint32_t) (ifp->mac[3] << 24) |
                 ((uint32_t) ifp->mac[2] << 16) |
                 ((uint32_t) ifp->mac[1] << 8) | ifp->mac[0];
  return true;
}

static size_t mg_tcpip_driver_stm32_tx(const void *buf, size_t len,
                                       struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if ((s_txdesc[s_txno][0] & BIT(31))) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors"));
    // printf("D0 %lx SR %lx\n", (long) s_txdesc[0][0], (long) ETH->DMASR);
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);     // Copy data
    s_txdesc[s_txno][1] = (uint32_t) len;  // Set data len
    s_txdesc[s_txno][0] = BIT(20) | BIT(28) | BIT(29);  // Chain,FS,LS
    s_txdesc[s_txno][0] |= BIT(31);  // Set OWN bit - let DMA take over
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  DSB();                         // ensure descriptors have been written
  ETH->DMASR = BIT(2) | BIT(5);  // Clear any prior TBUS/TUS
  ETH->DMATPDR = 0;              // and resume
  return len;
}

static bool mg_tcpip_driver_stm32_up(struct mg_tcpip_if *ifp) {
  uint32_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  bool up = bsr & BIT(2) ? 1 : 0;
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    uint32_t scsr = eth_read_phy(PHY_ADDR, PHY_CSCR);
    uint32_t maccr = ETH->MACCR | BIT(14) | BIT(11);  // 100M, Full-duplex
    if ((scsr & BIT(3)) == 0) maccr &= ~BIT(14);      // 10M
    if ((scsr & BIT(4)) == 0) maccr &= ~BIT(11);      // Half-duplex
    ETH->MACCR = maccr;  // IRQ handler does not fiddle with this register
    MG_DEBUG(("Link is %uM %s-duplex", maccr & BIT(14) ? 100 : 10,
              maccr & BIT(11) ? "full" : "half"));
  }
  return up;
}

void ETH_IRQHandler(void);
void ETH_IRQHandler(void) {
  if (ETH->DMASR & BIT(6)) {             // Frame received, loop
    ETH->DMASR = BIT(16) | BIT(6);       // Clear flag
    for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
      if (s_rxdesc[s_rxno][0] & BIT(31)) break;  // exit when done
      if (((s_rxdesc[s_rxno][0] & (BIT(8) | BIT(9))) == (BIT(8) | BIT(9))) &&
          !(s_rxdesc[s_rxno][0] & BIT(15))) {  // skip partial/errored frames
        uint32_t len = ((s_rxdesc[s_rxno][0] >> 16) & (BIT(14) - 1));
        //  printf("%lx %lu %lx %.8lx\n", s_rxno, len, s_rxdesc[s_rxno][0],
        //  ETH->DMASR);
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
      }
      s_rxdesc[s_rxno][0] = BIT(31);
      if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
    }
  }
  ETH->DMASR = BIT(7);  // Clear possible RBUS while processing
  ETH->DMARPDR = 0;     // and resume RX
}

struct mg_tcpip_driver mg_tcpip_driver_stm32 = {mg_tcpip_driver_stm32_init,
                                                mg_tcpip_driver_stm32_tx, NULL,
                                                mg_tcpip_driver_stm32_up};
#endif
