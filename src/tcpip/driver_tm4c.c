#include "tcpip.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_TM4C) && MG_ENABLE_DRIVER_TM4C
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
static struct mg_tcpip_if *s_ifp;                    // MIP interface
enum {
  EPHY_ADDR = 0,
  EPHYBMCR = 0,
  EPHYBMSR = 1,
  EPHYSTS = 16
};  // PHY constants

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

static uint32_t get_sysclk(void) {
  struct sysctl {
    volatile uint32_t DONTCARE0[44], RSCLKCFG, DONTCARE1[43], PLLFREQ0,
        PLLFREQ1;
  } *sysctl = (struct sysctl *) 0x400FE000;
  uint32_t clk = 0, piosc = 16000000 /* 16 MHz */, mosc = 25000000 /* 25MHz */;
  if (sysctl->RSCLKCFG & (1 << 28)) {  // USEPLL
    uint32_t fin, vco, mdiv, n, q, psysdiv;
    uint32_t pllsrc = (sysctl->RSCLKCFG & (0xf << 24)) >> 24;
    if (pllsrc == 0) {
      clk = piosc;
    } else if (pllsrc == 3) {
      clk = mosc;
    } else {
      MG_ERROR(("Unsupported clock source"));
    }
    q = (sysctl->PLLFREQ1 & (0x1f << 8)) >> 8;
    n = (sysctl->PLLFREQ1 & (0x1f << 0)) >> 0;
    fin = clk / ((q + 1) * (n + 1));
    mdiv = (sysctl->PLLFREQ0 & (0x3ff << 0)) >>
           0;  // mint + (mfrac / 1024); MFRAC not supported
    psysdiv = (sysctl->RSCLKCFG & (0x3f << 0)) >> 0;
    vco = (uint32_t) ((uint64_t) fin * mdiv);
    return vco / (psysdiv + 1);
  }
  uint32_t oscsrc = (sysctl->RSCLKCFG & (0xf << 20)) >> 20;
  if (oscsrc == 0) {
    clk = piosc;
  } else if (oscsrc == 3) {
    clk = mosc;
  } else {
    MG_ERROR(("Unsupported clock source"));
  }
  uint32_t osysdiv = (sysctl->RSCLKCFG & (0xf << 16)) >> 16;
  return clk / (osysdiv + 1);
}

//  Guess CR from SYSCLK. MDC clock is generated from SYSCLK (AHB); as per
//  802.3, it must not exceed 2.5MHz (also 20.4.2.6) As the AHB clock can be
//  derived from the PIOSC (internal RC), and it can go above  specs, the
//  datasheets specify a range of frequencies and activate one of a series of
//  dividers to keep the MDC clock safely below 2.5MHz. We guess a divider
//  setting based on SYSCLK with a +5% drift. If the user uses a different clock
//  from our defaults, needs to set the macros on top Valid for TM4C129x (20.7)
//  (4.5% worst case drift)
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

static bool mg_tcpip_driver_tm4c_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_tm4c_data *d =
      (struct mg_tcpip_driver_tm4c_data *) ifp->driver_data;
  s_ifp = ifp;

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
  EMAC->EMACADDR0H = ((uint32_t) ifp->mac[5] << 8U) | ifp->mac[4];
  EMAC->EMACADDR0L = (uint32_t) (ifp->mac[3] << 24) |
                     ((uint32_t) ifp->mac[2] << 16) |
                     ((uint32_t) ifp->mac[1] << 8) | ifp->mac[0];
  // NOTE(scaprile) There are 3 additional slots for filtering, disabled by
  // default. This also applies to the STM32 driver (at least for F7)
  return true;
}

static uint32_t s_txno;
static size_t mg_tcpip_driver_tm4c_tx(const void *buf, size_t len,
                                      struct mg_tcpip_if *ifp) {
  if (len > sizeof(s_txbuf[s_txno])) {
    MG_ERROR(("Frame too big, %ld", (long) len));
    len = 0;  // fail
  } else if ((s_txdesc[s_txno][0] & BIT(31))) {
    MG_ERROR(("No descriptors available"));
    // printf("D0 %lx SR %lx\n", (long) s_txdesc[0][0], (long)
    // EMAC->EMACDMARIS);
    len = 0;  // fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);     // Copy data
    s_txdesc[s_txno][1] = (uint32_t) len;  // Set data len
    s_txdesc[s_txno][0] =
        BIT(20) | BIT(28) | BIT(29) | BIT(30);  // Chain,FS,LS,IC
    s_txdesc[s_txno][0] |= BIT(31);  // Set OWN bit - let DMA take over
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  EMAC->EMACDMARIS = BIT(2) | BIT(5);  // Clear any prior TU/UNF
  EMAC->EMACTXPOLLD = 0;               // and resume
  return len;
  (void) ifp;
}

static bool mg_tcpip_driver_tm4c_up(struct mg_tcpip_if *ifp) {
  uint32_t bmsr = emac_read_phy(EPHY_ADDR, EPHYBMSR);
  bool up = (bmsr & BIT(2)) ? 1 : 0;
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    uint32_t sts = emac_read_phy(EPHY_ADDR, EPHYSTS);
    uint32_t emaccfg = EMAC->EMACCFG | BIT(14) | BIT(11);  // 100M, Full-duplex
    if (sts & BIT(1)) emaccfg &= ~BIT(14);                 // 10M
    if ((sts & BIT(2)) == 0) emaccfg &= ~BIT(11);          // Half-duplex
    EMAC->EMACCFG = emaccfg;  // IRQ handler does not fiddle with this register
    MG_DEBUG(("Link is %uM %s-duplex", emaccfg & BIT(14) ? 100 : 10,
              emaccfg & BIT(11) ? "full" : "half"));
  }
  return up;
}

void EMAC0_IRQHandler(void);
static uint32_t s_rxno;
void EMAC0_IRQHandler(void) {
  if (EMAC->EMACDMARIS & BIT(6)) {        // Frame received, loop
    EMAC->EMACDMARIS = BIT(16) | BIT(6);  // Clear flag
    for (uint32_t i = 0; i < 10; i++) {   // read as they arrive but not forever
      if (s_rxdesc[s_rxno][0] & BIT(31)) break;  // exit when done
      if (((s_rxdesc[s_rxno][0] & (BIT(8) | BIT(9))) == (BIT(8) | BIT(9))) &&
          !(s_rxdesc[s_rxno][0] & BIT(15))) {  // skip partial/errored frames
        uint32_t len = ((s_rxdesc[s_rxno][0] >> 16) & (BIT(14) - 1));
        //  printf("%lx %lu %lx %.8lx\n", s_rxno, len, s_rxdesc[s_rxno][0],
        //  EMAC->EMACDMARIS);
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
      }
      s_rxdesc[s_rxno][0] = BIT(31);
      if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
    }
  }
  EMAC->EMACDMARIS = BIT(7);  // Clear possible RU while processing
  EMAC->EMACRXPOLLD = 0;      // and resume RX
}

struct mg_tcpip_driver mg_tcpip_driver_tm4c = {mg_tcpip_driver_tm4c_init,
                                               mg_tcpip_driver_tm4c_tx, NULL,
                                               mg_tcpip_driver_tm4c_up};
#endif
