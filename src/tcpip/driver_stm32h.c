#include "tcpip.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_STM32H) && \
    MG_ENABLE_DRIVER_STM32H
struct stm32h_eth {
  volatile uint32_t MACCR, MACECR, MACPFR, MACWTR, MACHT0R, MACHT1R,
      RESERVED1[14], MACVTR, RESERVED2, MACVHTR, RESERVED3, MACVIR, MACIVIR,
      RESERVED4[2], MACTFCR, RESERVED5[7], MACRFCR, RESERVED6[7], MACISR,
      MACIER, MACRXTXSR, RESERVED7, MACPCSR, MACRWKPFR, RESERVED8[2], MACLCSR,
      MACLTCR, MACLETR, MAC1USTCR, RESERVED9[12], MACVR, MACDR, RESERVED10,
      MACHWF0R, MACHWF1R, MACHWF2R, RESERVED11[54], MACMDIOAR, MACMDIODR,
      RESERVED12[2], MACARPAR, RESERVED13[59], MACA0HR, MACA0LR, MACA1HR,
      MACA1LR, MACA2HR, MACA2LR, MACA3HR, MACA3LR, RESERVED14[248], MMCCR,
      MMCRIR, MMCTIR, MMCRIMR, MMCTIMR, RESERVED15[14], MMCTSCGPR, MMCTMCGPR,
      RESERVED16[5], MMCTPCGR, RESERVED17[10], MMCRCRCEPR, MMCRAEPR,
      RESERVED18[10], MMCRUPGR, RESERVED19[9], MMCTLPIMSTR, MMCTLPITCR,
      MMCRLPIMSTR, MMCRLPITCR, RESERVED20[65], MACL3L4C0R, MACL4A0R,
      RESERVED21[2], MACL3A0R0R, MACL3A1R0R, MACL3A2R0R, MACL3A3R0R,
      RESERVED22[4], MACL3L4C1R, MACL4A1R, RESERVED23[2], MACL3A0R1R,
      MACL3A1R1R, MACL3A2R1R, MACL3A3R1R, RESERVED24[108], MACTSCR, MACSSIR,
      MACSTSR, MACSTNR, MACSTSUR, MACSTNUR, MACTSAR, RESERVED25, MACTSSR,
      RESERVED26[3], MACTTSSNR, MACTTSSSR, RESERVED27[2], MACACR, RESERVED28,
      MACATSNR, MACATSSR, MACTSIACR, MACTSEACR, MACTSICNR, MACTSECNR,
      RESERVED29[4], MACPPSCR, RESERVED30[3], MACPPSTTSR, MACPPSTTNR, MACPPSIR,
      MACPPSWR, RESERVED31[12], MACPOCR, MACSPI0R, MACSPI1R, MACSPI2R, MACLMIR,
      RESERVED32[11], MTLOMR, RESERVED33[7], MTLISR, RESERVED34[55], MTLTQOMR,
      MTLTQUR, MTLTQDR, RESERVED35[8], MTLQICSR, MTLRQOMR, MTLRQMPOCR, MTLRQDR,
      RESERVED36[177], DMAMR, DMASBMR, DMAISR, DMADSR, RESERVED37[60], DMACCR,
      DMACTCR, DMACRCR, RESERVED38[2], DMACTDLAR, RESERVED39, DMACRDLAR,
      DMACTDTPR, RESERVED40, DMACRDTPR, DMACTDRLR, DMACRDRLR, DMACIER,
      DMACRIWTR, DMACSFCSR, RESERVED41, DMACCATDR, RESERVED42, DMACCARDR,
      RESERVED43, DMACCATBR, RESERVED44, DMACCARBR, DMACSR, RESERVED45[2],
      DMACMFCR;
};
#undef ETH
#define ETH \
  ((struct stm32h_eth *) (uintptr_t) (0x40000000UL + 0x00020000UL + 0x8000UL))

#undef BIT
#define BIT(x) ((uint32_t) 1 << (x))
#define ETH_PKT_SIZE 1540  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 4           // Descriptor size (words)

static volatile uint32_t s_rxdesc[ETH_DESC_CNT][ETH_DS];  // RX descriptors
static volatile uint32_t s_txdesc[ETH_DESC_CNT][ETH_DS];  // TX descriptors
static uint8_t s_rxbuf[ETH_DESC_CNT][ETH_PKT_SIZE];       // RX ethernet buffers
static uint8_t s_txbuf[ETH_DESC_CNT][ETH_PKT_SIZE];       // TX ethernet buffers
static struct mg_tcpip_if *s_ifp;                         // MIP interface
enum {
  PHY_ADDR = 0,
  PHY_BCR = 0,
  PHY_BSR = 1,
  PHY_CSCR = 31
};  // PHY constants

static uint32_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ETH->MACMDIOAR &= (0xF << 8);
  ETH->MACMDIOAR |= ((uint32_t) addr << 21) | ((uint32_t) reg << 16) | 3 << 2;
  ETH->MACMDIOAR |= BIT(0);
  while (ETH->MACMDIOAR & BIT(0)) (void) 0;
  return ETH->MACMDIODR;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  ETH->MACMDIODR = val;
  ETH->MACMDIOAR &= (0xF << 8);
  ETH->MACMDIOAR |= ((uint32_t) addr << 21) | ((uint32_t) reg << 16) | 1 << 2;
  ETH->MACMDIOAR |= BIT(0);
  while (ETH->MACMDIOAR & BIT(0)) (void) 0;
}

static uint32_t get_hclk(void) {
  struct rcc {
    volatile uint32_t CR, HSICFGR, CRRCR, CSICFGR, CFGR, RESERVED1, D1CFGR,
        D2CFGR, D3CFGR, RESERVED2, PLLCKSELR, PLLCFGR, PLL1DIVR, PLL1FRACR,
        PLL2DIVR, PLL2FRACR, PLL3DIVR, PLL3FRACR, RESERVED3, D1CCIPR, D2CCIP1R,
        D2CCIP2R, D3CCIPR, RESERVED4, CIER, CIFR, CICR, RESERVED5, BDCR, CSR,
        RESERVED6, AHB3RSTR, AHB1RSTR, AHB2RSTR, AHB4RSTR, APB3RSTR, APB1LRSTR,
        APB1HRSTR, APB2RSTR, APB4RSTR, GCR, RESERVED8, D3AMR, RESERVED11[9],
        RSR, AHB3ENR, AHB1ENR, AHB2ENR, AHB4ENR, APB3ENR, APB1LENR, APB1HENR,
        APB2ENR, APB4ENR, RESERVED12, AHB3LPENR, AHB1LPENR, AHB2LPENR,
        AHB4LPENR, APB3LPENR, APB1LLPENR, APB1HLPENR, APB2LPENR, APB4LPENR,
        RESERVED13[4];
  } *rcc = ((struct rcc *) (0x40000000 + 0x18020000 + 0x4400));
  uint32_t clk = 0, hsi = 64000000 /* 64 MHz */, hse = 8000000 /* 8MHz */,
           csi = 4000000 /* 4MHz */;
  unsigned int sel = (rcc->CFGR & (7 << 3)) >> 3;

  if (sel == 1) {
    clk = csi;
  } else if (sel == 2) {
    clk = hse;
  } else if (sel == 3) {
    uint32_t vco, m, n, p;
    unsigned int src = (rcc->PLLCKSELR & (3 << 0)) >> 0;
    m = ((rcc->PLLCKSELR & (0x3F << 4)) >> 4);
    n = ((rcc->PLL1DIVR & (0x1FF << 0)) >> 0) + 1 +
        ((rcc->PLLCFGR & BIT(0)) ? 1 : 0);  // round-up in fractional mode
    p = ((rcc->PLL1DIVR & (0x7F << 9)) >> 9) + 1;
    if (src == 1) {
      clk = csi;
    } else if (src == 2) {
      clk = hse;
    } else {
      clk = hsi;
      clk >>= ((rcc->CR & 3) >> 3);
    }
    vco = (uint32_t) ((uint64_t) clk * n / m);
    clk = vco / p;
  } else {
    clk = hsi;
    clk >>= ((rcc->CR & 3) >> 3);
  }
  const uint8_t cptab[12] = {1, 2, 3, 4, 6, 7, 8, 9};  // log2(div)
  uint32_t d1cpre = (rcc->D1CFGR & (0x0F << 8)) >> 8;
  if (d1cpre >= 8) clk >>= cptab[d1cpre - 8];
  MG_DEBUG(("D1 CLK: %u", clk));
  uint32_t hpre = (rcc->D1CFGR & (0x0F << 0)) >> 0;
  if (hpre < 8) return clk;
  return ((uint32_t) clk) >> cptab[hpre - 8];
}

//  Guess CR from AHB1 clock. MDC clock is generated from the ETH peripheral
//  clock (AHB1); as per 802.3, it must not exceed 2. As the AHB clock can
//  be derived from HSI or CSI (internal RC) clocks, and those can go above
//  specs, the datasheets specify a range of frequencies and activate one of a
//  series of dividers to keep the MDC clock safely below 2.5MHz. We guess a
//  divider setting based on HCLK with some drift. If the user uses a different
//  clock from our defaults, needs to set the macros on top. Valid for
//  STM32H74xxx/75xxx (58.11.4)(4.5% worst case drift)(CSI clock has a 7.5 %
//  worst case drift @ max temp)
static int guess_mdc_cr(void) {
  const uint8_t crs[] = {2, 3, 0, 1, 4, 5};  // ETH->MACMDIOAR::CR values
  const uint8_t div[] = {16, 26, 42, 62, 102, 124};  // Respective HCLK dividers
  uint32_t hclk = get_hclk();                        // Guess system HCLK
  int result = -1;                                   // Invalid CR value
  for (int i = 0; i < 6; i++) {
    if (hclk / div[i] <= 2375000UL /* 2.5MHz - 5% */) {
      result = crs[i];
      break;
    }
  }
  if (result < 0) MG_ERROR(("HCLK too high"));
  MG_DEBUG(("HCLK: %u, CR: %d", hclk, result));
  return result;
}

static bool mg_tcpip_driver_stm32h_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_stm32h_data *d =
      (struct mg_tcpip_driver_stm32h_data *) ifp->driver_data;
  s_ifp = ifp;

  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = (uint32_t) (uintptr_t) s_rxbuf[i];  // Point to data buffer
    s_rxdesc[i][3] = BIT(31) | BIT(30) | BIT(24);        // OWN, IOC, BUF1V
  }

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i][0] = (uint32_t) (uintptr_t) s_txbuf[i];  // Buf pointer
  }

  ETH->DMAMR |= BIT(0);                         // Software reset
  while ((ETH->DMAMR & BIT(0)) != 0) (void) 0;  // Wait until done

  // Set MDC clock divider. If user told us the value, use it. Otherwise, guess
  int cr = (d == NULL || d->mdc_cr < 0) ? guess_mdc_cr() : d->mdc_cr;
  ETH->MACMDIOAR = ((uint32_t) cr & 0xF) << 8;

  // NOTE(scaprile): We do not use timing facilities so the DMA engine does not
  // re-write buffer address
  ETH->DMAMR = 0 << 16;     // use interrupt mode 0 (58.8.1) (reset value)
  ETH->DMASBMR |= BIT(12);  // AAL NOTE(scaprile): is this actually needed
  ETH->MACIER = 0;        // Do not enable additional irq sources (reset value)
  ETH->MACTFCR = BIT(7);  // Disable zero-quanta pause
  // ETH->MACPFR = BIT(31);  // Receive all
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(15));  // Reset PHY
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(12));  // Set autonegotiation
  ETH->DMACRDLAR =
      (uint32_t) (uintptr_t) s_rxdesc;  // RX descriptors start address
  ETH->DMACRDRLR = ETH_DESC_CNT - 1;    // ring length
  ETH->DMACRDTPR =
      (uint32_t) (uintptr_t) &s_rxdesc[ETH_DESC_CNT -
                                       1];  // last valid descriptor address
  ETH->DMACTDLAR =
      (uint32_t) (uintptr_t) s_txdesc;  // TX descriptors start address
  ETH->DMACTDRLR = ETH_DESC_CNT - 1;    // ring length
  ETH->DMACTDTPR =
      (uint32_t) (uintptr_t) s_txdesc;  // first available descriptor address
  ETH->DMACCR = 0;  // DSL = 0 (contiguous descriptor table) (reset value)
  ETH->DMACIER = BIT(6) | BIT(15);  // RIE, NIE
  ETH->MACCR = BIT(0) | BIT(1) | BIT(13) | BIT(14) |
               BIT(15);     // RE, TE, Duplex, Fast, Reserved
  ETH->MTLTQOMR |= BIT(1);  // TSF
  ETH->MTLRQOMR |= BIT(5);  // RSF
  ETH->DMACTCR |= BIT(0);   // ST
  ETH->DMACRCR |= BIT(0);   // SR

  // MAC address filtering
  ETH->MACA0HR = ((uint32_t) ifp->mac[5] << 8U) | ifp->mac[4];
  ETH->MACA0LR = (uint32_t) (ifp->mac[3] << 24) |
                 ((uint32_t) ifp->mac[2] << 16) |
                 ((uint32_t) ifp->mac[1] << 8) | ifp->mac[0];
  return true;
}

static uint32_t s_txno;
static size_t mg_tcpip_driver_stm32h_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // Frame is too big
  } else if ((s_txdesc[s_txno][3] & BIT(31))) {
    MG_ERROR(("No free descriptors: %u %08X %08X %08X", s_txno,
              s_txdesc[s_txno][3], ETH->DMACSR, ETH->DMACTCR));
    for (int i = 0; i < ETH_DESC_CNT; i++) MG_ERROR(("%08X", s_txdesc[i][3]));
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);        // Copy data
    s_txdesc[s_txno][2] = (uint32_t) len;     // Set data len
    s_txdesc[s_txno][3] = BIT(28) | BIT(29);  // FD, LD
    s_txdesc[s_txno][3] |= BIT(31);           // Set OWN bit - let DMA take over
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  ETH->DMACSR |= BIT(2) | BIT(1);  // Clear any prior TBU, TPS
  ETH->DMACTDTPR = (uint32_t) (uintptr_t) &s_txdesc[s_txno];  // and resume
  return len;
  (void) ifp;
}

static bool mg_tcpip_driver_stm32h_up(struct mg_tcpip_if *ifp) {
  uint32_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  bool up = bsr & BIT(2) ? 1 : 0;
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    uint32_t scsr = eth_read_phy(PHY_ADDR, PHY_CSCR);
    uint32_t maccr = ETH->MACCR | BIT(14) | BIT(13);  // 100M, Full-duplex
    if ((scsr & BIT(3)) == 0) maccr &= ~BIT(14);      // 10M
    if ((scsr & BIT(4)) == 0) maccr &= ~BIT(13);      // Half-duplex
    ETH->MACCR = maccr;  // IRQ handler does not fiddle with this register
    MG_DEBUG(("Link is %uM %s-duplex", maccr & BIT(14) ? 100 : 10,
              maccr & BIT(13) ? "full" : "half"));
  }
  return up;
}

void ETH_IRQHandler(void);
static uint32_t s_rxno;
void ETH_IRQHandler(void) {
  if (ETH->DMACSR & BIT(6)) {            // Frame received, loop
    ETH->DMACSR = BIT(15) | BIT(6);      // Clear flag
    for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
      if (s_rxdesc[s_rxno][3] & BIT(31)) break;  // exit when done
      if (((s_rxdesc[s_rxno][3] & (BIT(28) | BIT(29))) ==
           (BIT(28) | BIT(29))) &&
          !(s_rxdesc[s_rxno][3] & BIT(15))) {  // skip partial/errored frames
        uint32_t len = s_rxdesc[s_rxno][3] & (BIT(15) - 1);
        // MG_DEBUG(("%lx %lu %lx %08lx", s_rxno, len, s_rxdesc[s_rxno][3],
        // ETH->DMACSR));
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
      }
      s_rxdesc[s_rxno][3] = BIT(31) | BIT(30) | BIT(24);  // OWN, IOC, BUF1V
      if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
    }
  }
  ETH->DMACSR = BIT(7) | BIT(8);  // Clear possible RBU RPS while processing
  ETH->DMACRDTPR =
      (uint32_t) (uintptr_t) &s_rxdesc[ETH_DESC_CNT - 1];  // and resume RX
}

struct mg_tcpip_driver mg_tcpip_driver_stm32h = {
    mg_tcpip_driver_stm32h_init, mg_tcpip_driver_stm32h_tx, NULL,
    mg_tcpip_driver_stm32h_up};
#endif
