#include "mip.h"

#if MG_ENABLE_MIP && defined(__arm__)

// define to your own clock if using external clocking
#if !defined(MG_STM32_CLK_HSE)
#define MG_STM32_CLK_HSE 8000000UL
#endif

// define to your chip internal clock if different
#if !defined(MG_STM32_CLK_HSI)
#define MG_STM32_CLK_HSI 16000000UL
#endif

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
#define ETH ((struct stm32_eth *) (uintptr_t) 0x40028000)

#define BIT(x) ((uint32_t) 1 << (x))
#define ETH_PKT_SIZE 1540  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 4           // Descriptor size (words)

static uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];      // RX descriptors
static uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];      // TX descriptors
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE];  // RX ethernet buffers
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE];  // TX ethernet buffers
static void (*s_rx)(void *, size_t, void *);         // Recv callback
static void *s_rxdata;                               // Recv callback data
enum { PHY_ADDR = 0, PHY_BCR = 0, PHY_BSR = 1 };     // PHY constants

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

static uint32_t hclk_get(void);
static uint8_t cr_guess(uint32_t hclk);

static uint32_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ETH->MACMIIAR &= (7 << 2);
  ETH->MACMIIAR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6);
  ETH->MACMIIAR |= BIT(0);
  while (ETH->MACMIIAR & BIT(0)) spin(1);
  return ETH->MACMIIDR;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  ETH->MACMIIDR = val;
  ETH->MACMIIAR &= (7 << 2);
  ETH->MACMIIAR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6) | BIT(1);
  ETH->MACMIIAR |= BIT(0);
  while (ETH->MACMIIAR & BIT(0)) spin(1);
}

static bool mip_driver_stm32_init(uint8_t *mac, void *userdata) {
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

  ETH->DMABMR |= BIT(0);                        // Software reset
  while ((ETH->DMABMR & BIT(0)) != 0) spin(1);  // Wait until done
  // NOTE(cpq): we do not use extended descriptor bit 7, and do not use
  // hardware checksum. Therefore, descriptor size is 4, not 8
  // ETH->DMABMR = BIT(13) | BIT(16) | BIT(22) | BIT(23) | BIT(25);
  ETH->MACIMR = BIT(3) | BIT(9);                    // Mask timestamp & PMT IT
  ETH->MACMIIAR = cr_guess(hclk_get()) << 2;        // MDC clock
  ETH->MACFCR = BIT(7);                             // Disable zero quarta pause
  ETH->MACFFR = BIT(31);                            // Receive all
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(15));        // Reset PHY
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(12));        // Set autonegotiation
  ETH->DMARDLAR = (uint32_t) (uintptr_t) s_rxdesc;  // RX descriptors
  ETH->DMATDLAR = (uint32_t) (uintptr_t) s_txdesc;  // RX descriptors
  ETH->DMAIER = BIT(6) | BIT(16);                   // RIE, NISE
  ETH->MACCR = BIT(2) | BIT(3) | BIT(11) | BIT(14);    // RE, TE, Duplex, Fast
  ETH->DMAOMR = BIT(1) | BIT(13) | BIT(21) | BIT(25);  // SR, ST, TSF, RSF

  // MAC address filtering
  ETH->MACA0HR = ((uint32_t) mac[5] << 8U) | mac[4];
  ETH->MACA0LR = (uint32_t) (mac[3] << 24) | ((uint32_t) mac[2] << 16) |
                 ((uint32_t) mac[1] << 8) | mac[0];
  (void) userdata;
  return true;
}

static void mip_driver_stm32_setrx(void (*rx)(void *, size_t, void *),
                                   void *rxdata) {
  s_rx = rx;
  s_rxdata = rxdata;
}

static uint32_t s_txno;
static size_t mip_driver_stm32_tx(const void *buf, size_t len, void *userdata) {
  if (len > sizeof(s_txbuf[s_txno])) {
    printf("%s: frame too big, %ld\n", __func__, (long) len);
    len = 0;  // Frame is too big
  } else if ((s_txdesc[s_txno][0] & BIT(31))) {
    printf("%s: no free descr\n", __func__);
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);     // Copy data
    s_txdesc[s_txno][1] = (uint32_t) len;  // Set data len
    s_txdesc[s_txno][0] = BIT(20) | BIT(28) | BIT(29) | BIT(30);  // Chain,FS,LS
    s_txdesc[s_txno][0] |= BIT(31);  // Set OWN bit - let DMA take over
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  uint32_t sr = ETH->DMASR;
  if (sr & BIT(2)) ETH->DMASR = BIT(2), ETH->DMATPDR = 0;  // Resume
  if (sr & BIT(5)) ETH->DMASR = BIT(5), ETH->DMATPDR = 0;  // if busy
  if (len == 0) printf("E: D0 %lx SR %lx\n", (long) s_txdesc[0][0], (long) sr);
  return len;
  (void) userdata;
}

static bool mip_driver_stm32_up(void *userdata) {
  uint32_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  (void) userdata;
  return bsr & BIT(2) ? 1 : 0;
}

void ETH_IRQHandler(void);
void ETH_IRQHandler(void) {
  qp_mark(QP_IRQTRIGGERED, 0);
  volatile uint32_t sr = ETH->DMASR;
  if (sr & BIT(6)) {  // Frame received, loop
    for (uint32_t i = 0; i < ETH_DESC_CNT; i++) {
      if (s_rxdesc[i][0] & BIT(31)) continue;
      uint32_t len = ((s_rxdesc[i][0] >> 16) & (BIT(14) - 1));
      //    printf("%lx %lu %lx %lx\n", i, len, s_rxdesc[i][0], sr);
      if (s_rx != NULL) s_rx(s_rxbuf[i], len > 4 ? len - 4 : len, s_rxdata);
      s_rxdesc[i][0] = BIT(31);
    }
  }
  if (sr & BIT(7)) ETH->DMARPDR = 0;     // Resume RX
  ETH->DMASR = sr & ~(BIT(2) | BIT(7));  // Clear status
}

struct mip_driver mip_driver_stm32 = {.init = mip_driver_stm32_init,
                                      .tx = mip_driver_stm32_tx,
                                      .setrx = mip_driver_stm32_setrx,
                                      .up = mip_driver_stm32_up};

// Calculate HCLK from clock settings,
// valid for STM32F74xxx/75xxx (5.3) and STM32F42xxx/43xxx (6.3)
static const uint8_t ahbptab[8] = {1, 2, 3, 4, 6, 7, 8, 9};  // log2(div)
struct rcc {
  volatile uint32_t CR, PLLCFGR, CFGR;
};
#define RCC ((struct rcc *) 0x40023800)

static uint32_t hclk_get(void) {
  uint32_t clk = 0;
  if (RCC->CFGR & (1 << 2)) {
    clk = MG_STM32_CLK_HSE;
  } else if (RCC->CFGR & (1 << 3)) {
    uint32_t vco, m, n, p;
    m = (RCC->PLLCFGR & (0x3FUL << 0)) >> 0;
    n = (RCC->PLLCFGR & (0x1FFUL << 6)) >> 6;
    p = (((RCC->PLLCFGR & (0x03UL << 16)) >> 16) + 1) * 2;
    if (RCC->PLLCFGR & (1UL << 22))
      clk = MG_STM32_CLK_HSE;
    else
      clk = MG_STM32_CLK_HSI;
    vco = (uint32_t) ((uint64_t) (((uint32_t) clk * (uint32_t) n)) /
                      ((uint32_t) m));
    clk = vco / p;
  } else {
    clk = MG_STM32_CLK_HSI;
  }
  int hpre = (RCC->CFGR & (0x0F << 4)) >> 4;
  if (hpre < 8) return clk;
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
#define CRDTAB_LEN 6
static const uint8_t crdtab[CRDTAB_LEN][2] = {
    // [{setting, div ratio},...]
    {2, 16}, {3, 26}, {0, 42}, {1, 62}, {4, 102}, {5, 124},
};

static uint8_t cr_guess(uint32_t hclk) {
  MG_DEBUG(("HCLK: %u", hclk));
  if (hclk < 25000000) {
    MG_ERROR(("HCLK too low"));
    return CRDTAB_LEN;
  }
  for (int i = 0; i < CRDTAB_LEN; i++)
    if (hclk / crdtab[i][1] <= 2375000UL) return crdtab[i][0];  // 2.5MHz - 5%
  MG_ERROR(("HCLK too high"));
  return CRDTAB_LEN;
}

#endif  // MG_ENABLE_MIP
