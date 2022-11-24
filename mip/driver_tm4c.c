#include "mip.h"

#if MG_ENABLE_MIP && defined(MG_ENABLE_DRIVER_TM4C) && MG_ENABLE_DRIVER_TM4C
struct tm4c_emac {
  volatile uint32_t EMACCFG, EMACFRAMEFLTR, EMACHASHTBLH, EMACHASHTBLL,
      EMACMIIADDR, EMACMIIDATA, EMACFLOWCTL, EMACVLANTG, RESERVED0, EMACSTATUS,
      EMACRWUFF, EMACPMTCTLSTAT, RESERVED1[2], EMACRIS, EMACIM, EMACADDR0H,
      EMACADDR0L, EMACADDR1H, EMACADDR1L, EMACADDR2H, EMACADDR2L, EMACADDR3H,
      EMACADDR3L, RESERVED2[31], EMACWDOGTO, RESERVED3[8], EMACMMCCTRL,
      EMACMMCRXRIS, EMACMMCTXRIS, EMACMMCRXIM, EMACMMCTXIM, RESERVED4,
      EMACTXCNTGB, RESERVED5[12], EMACTXCNTSCOL, EMACTXCNTMCOL, RESERVED6[4],
      EMACTXOCTCNTG, RESERVED7[6], EMACRXCNTGB, RESERVED8[4], EMACRXCNTCRCERR,
      EMACRXCNTALGNERR, RESERVED9[10], EMACRXCNTGUNI, RESERVED10[239],
      EMACVLNINCREP, EMACVLANHASH, RESERVED11[93], EMACTIMSTCTRL, EMACSUBSECINC,
      EMACTIMSEC, EMACTIMNANO, EMACTIMSECU, EMACTIMNANOU, EMACTIMADD,
      EMACTARGSEC, EMACTARGNANO, EMACHWORDSEC, EMACTIMSTAT, EMACPPSCTRL,
      RESERVED12[12], EMACPPS0INTVL, EMACPPS0WIDTH, RESERVED13[294],
      EMACDMABUSMOD, EMACTXPOLLD, EMACRXPOLLD, EMACRXDLADDR, EMACTXDLADDR,
      EMACDMARIS, EMACDMAOPMODE, EMACDMAIM, EMACMFBOC, EMACRXINTWDT,
      RESERVED14[8], EMACHOSTXDESC, EMACHOSRXDESC, EMACHOSTXBA, EMACHOSRXBA,
      RESERVED15[218], EMACPP, EMACPC, EMACCC, RESERVED16, EMACEPHYRIS,
      EMACEPHYIM, EMACEPHYIMSC;
};
#undef EMAC
#define EMAC ((struct tm4c_emac *) (uintptr_t) 0x400EC000)

#undef BIT
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
enum { EPHY_ADDR = 0, EPHYBMCR = 0, EPHYBMSR = 1 };  // PHY constants

static inline void tm4cspin(volatile uint32_t count) {
  while (count--) (void) 0;
}

static uint32_t emac_read_phy(uint8_t addr, uint8_t reg) {
  EMAC->EMACMIIADDR &= (0xf << 2);
  EMAC->EMACMIIADDR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6);
  EMAC->EMACMIIADDR |= BIT(0);
  while (EMAC->EMACMIIADDR & BIT(0)) tm4cspin(1);
  return EMAC->EMACMIIDATA;
}

static void emac_write_phy(uint8_t addr, uint8_t reg, uint32_t val) {
  EMAC->EMACMIIDATA = val;
  EMAC->EMACMIIADDR &= (0xf << 2);
  EMAC->EMACMIIADDR |= ((uint32_t) addr << 11) | ((uint32_t) reg << 6) | BIT(1);
  EMAC->EMACMIIADDR |= BIT(0);
  while (EMAC->EMACMIIADDR & BIT(0)) tm4cspin(1);
}

// TODO(scaprile) TEST
static uint32_t get_sysclk(void) {
  struct sysctl {
    volatile uint32_t DONTCARE0[44], RSCLKCFG, DONTCARE1[43], PLLFREQ0,
        PLLFREQ1;
  } *sysctl = (struct sysctl *) 0x400FE000;
  uint32_t clk = 0, piosc = 16000000 /* 16 MHz */, mosc = 25000000 /* 25MHz */;
  uint32_t oscsrc = (sysctl->RSCLKCFG & (0xf << 20)) >> 20;
  if (oscsrc == 0)
    clk = piosc;
  else if (oscsrc == 3)
    clk = mosc;
  else
    MG_ERROR(("Unsupported clock source"));
  if (sysctl->RSCLKCFG & (1 << 28)) {  // USEPLL
    uint32_t fin, vco, mdiv, n, q, psysdiv;
    q = (sysctl->PLLFREQ1 & (0x1f << 8)) >> 8;
    n = (sysctl->PLLFREQ1 & (0x1f << 0)) >> 0;
    fin = clk / ((q + 1) * (n + 1));
    mdiv = (sysctl->PLLFREQ0 & (0x3ff << 0)) >>
           0;  // mint + (mfrac / 1024); MFRAC not supported
    psysdiv = (sysctl->RSCLKCFG & (0x3f << 0)) >> 0;
    vco = (uint32_t) ((uint64_t) fin * mdiv);
    return vco / (psysdiv + 1);
  }
  uint32_t osysdiv = (sysctl->RSCLKCFG & (0xf << 16)) >> 16;
  return clk / (osysdiv + 1);
}

//  Guess CR from SYSCLK. MDC clock is generated from SYSCLK (AHB); as per
//  802.3, it must not exceed 2.5MHz (also 20.4.2.6) As the AHB clock can be
//  (and usually is) derived from the PIOSC (internal RC), and it can go above
//  specs, the datasheets specify a range of frequencies and activate one of a
//  series of dividers to keep the MDC clock safely below 2.5MHz. We guess a
//  divider setting based on SYSCLK with a +5% drift. If the user uses a
//  different clock from our defaults, needs to set the macros on top Valid for
//  TM4C129x (20.7) (4.5% worst case drift)
// The PHY receives the main oscillator (MOSC) (20.3.1)
static int guess_mdc_cr(void) {
  uint8_t crs[] = {2, 3, 0, 1};      // EMAC->MACMIIAR::CR values
  uint8_t div[] = {16, 26, 42, 62};  // Respective HCLK dividers
  uint32_t sysclk = get_sysclk();    // Guess system SYSCLK
  int result = -1;                   // Invalid CR value
  if (sysclk < 25000000) {
    MG_ERROR(("SYSCLK too low"));
  } else {
    for (int i = 0; i < 4; i++) {
      if (sysclk / div[i] <= 2375000UL /* 2.5MHz - 5% */) {
        result = crs[i];
        break;
      }
    }
    if (result < 0) MG_ERROR(("SYSCLK too high"));
  }
  MG_DEBUG(("SYSCLK: %u, CR: %d", sysclk, result));
  return result;
}

static bool mip_driver_tm4c_init(uint8_t *mac, void *userdata) {
  struct mip_driver_tm4c *d = (struct mip_driver_tm4c *) userdata;
  // Init RX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = BIT(31);                            // Own
    s_rxdesc[i][1] = sizeof(s_rxbuf[i]) | BIT(14);       // 2nd address chained
    s_rxdesc[i][2] = (uint32_t) (uintptr_t) s_rxbuf[i];  // Point to data buffer
    s_rxdesc[i][3] =
        (uint32_t) (uintptr_t) s_rxdesc[(i + 1) % ETH_DESC_CNT];  // Chain
    // MG_DEBUG(("%d %p", i, s_rxdesc[i]));
  }

  // Init TX descriptors
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i][2] = (uint32_t) (uintptr_t) s_txbuf[i];  // Buf pointer
    s_txdesc[i][3] =
        (uint32_t) (uintptr_t) s_txdesc[(i + 1) % ETH_DESC_CNT];  // Chain
  }

  EMAC->EMACDMABUSMOD |= BIT(0);                            // Software reset
  while ((EMAC->EMACDMABUSMOD & BIT(0)) != 0) tm4cspin(1);  // Wait until done

  // Set MDC clock divider. If user told us the value, use it. Otherwise, guess
  int cr = (d == NULL || d->mdc_cr < 0) ? guess_mdc_cr() : d->mdc_cr;
  EMAC->EMACMIIADDR = ((uint32_t) cr & 0xf) << 2;

  // NOTE(cpq): we do not use extended descriptor bit 7, and do not use
  // hardware checksum. Therefore, descriptor size is 4, not 8
  // EMAC->EMACDMABUSMOD = BIT(13) | BIT(16) | BIT(22) | BIT(23) | BIT(25);
  EMAC->EMACIM = BIT(3) | BIT(9);  // Mask timestamp & PMT IT
  EMAC->EMACFLOWCTL = BIT(7);      // Disable zero-quanta pause
  // EMAC->EMACFRAMEFLTR = BIT(31);   // Receive all
  // EMAC->EMACPC defaults to internal PHY (EPHY) in MMI mode
  emac_write_phy(EPHY_ADDR, EPHYBMCR, BIT(15));  // Reset internal PHY (EPHY)
  emac_write_phy(EPHY_ADDR, EPHYBMCR, BIT(12));  // Set autonegotiation
  EMAC->EMACRXDLADDR = (uint32_t) (uintptr_t) s_rxdesc;  // RX descriptors
  EMAC->EMACTXDLADDR = (uint32_t) (uintptr_t) s_txdesc;  // TX descriptors
  EMAC->EMACDMAIM = BIT(6) | BIT(16);                    // RIE, NIE
  EMAC->EMACCFG = BIT(2) | BIT(3) | BIT(11) | BIT(14);   // RE, TE, Duplex, Fast
  EMAC->EMACDMAOPMODE =
      BIT(1) | BIT(13) | BIT(21) | BIT(25);  // SR, ST, TSF, RSF
  EMAC->EMACADDR0H = ((uint32_t) mac[5] << 8U) | mac[4];
  EMAC->EMACADDR0L = (uint32_t) (mac[3] << 24) | ((uint32_t) mac[2] << 16) |
                     ((uint32_t) mac[1] << 8) | mac[0];
  // NOTE(scaprile) There are 3 additional slots for filtering, disabled by
  // default. This also applies to the STM32 driver (at least for F7)

  return true;
}

static void mip_driver_tm4c_setrx(void (*rx)(void *, size_t, void *),
                                  void *rxdata) {
  s_rx = rx;
  s_rxdata = rxdata;
}

static uint32_t s_txno;
static size_t mip_driver_tm4c_tx(const void *buf, size_t len, void *userdata) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("frame too big, %ld", (long) len));
    len = 0;  // fail
  } else if ((s_txdesc[s_txno][0] & BIT(31))) {
    MG_ERROR(("no descriptors available"));
    len = 0;  // fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);     // Copy data
    s_txdesc[s_txno][1] = (uint32_t) len;  // Set data len
    s_txdesc[s_txno][0] =
        BIT(20) | BIT(28) | BIT(29) | BIT(30);  // Chain,FS,LS,IC
    s_txdesc[s_txno][0] |= BIT(31);  // Set OWN bit - let DMA take over
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  uint32_t sr = EMAC->EMACDMARIS;
  if (sr & BIT(2)) EMAC->EMACDMARIS = BIT(2), EMAC->EMACTXPOLLD = 0;  // Resume
  if (sr & BIT(5)) EMAC->EMACDMARIS = BIT(5), EMAC->EMACTXPOLLD = 0;  // if busy
  if (len == 0)
    MG_ERROR(("E: D0 %lx SR %lx", (long) s_txdesc[0][0], (long) sr));
  return len;
  (void) userdata;
}

static bool mip_driver_tm4c_up(void *userdata) {
  uint32_t bmsr = emac_read_phy(EPHY_ADDR, EPHYBMSR);
  (void) userdata;
  return (bmsr & BIT(2)) ? 1 : 0;
}

void EMAC0_IRQHandler(void);
void EMAC0_IRQHandler(void) {
  qp_mark(QP_IRQTRIGGERED, 0);
  volatile uint32_t sr = EMAC->EMACDMARIS;
  if (sr & BIT(6)) {  // Frame received, loop
    for (uint32_t i = 0; i < ETH_DESC_CNT; i++) {
      if (s_rxdesc[i][0] & BIT(31)) continue;
      uint32_t len = ((s_rxdesc[i][0] >> 16) & (BIT(14) - 1));
      // MG_DEBUG(("%lu %lu %lx %lx", i, len, s_rxdesc[i][0], sr));
      if (s_rx != NULL) s_rx(s_rxbuf[i], len > 4 ? len - 4 : len, s_rxdata);
      s_rxdesc[i][0] = BIT(31);
    }
  }
  if (sr & BIT(7)) {
    EMAC->EMACRXPOLLD = 0;  // Resume RX
    //      uint32_t *p = (uint32_t *)(EMAC->EMACHOSRXDESC);
    //      MG_DEBUG(("RU: %p %c", p, (*p & BIT(31))? '1':'0'));
  }
  EMAC->EMACDMARIS = sr & ~(BIT(2) | BIT(7));  // Clear status
  //  if (EMAC->EMACDMARIS & BIT(7)) {
  //      uint32_t *p = (uint32_t *)(EMAC->EMACHOSRXDESC);
  //      MG_ERROR(("OOPS: %p %c", p, (*p & BIT(31))? '1':'0'));
  //  }
}

struct mip_driver mip_driver_tm4c = {mip_driver_tm4c_init, mip_driver_tm4c_tx,
                                     NULL, mip_driver_tm4c_up,
                                     mip_driver_tm4c_setrx};
#endif
