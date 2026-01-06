#include "net_builtin.h"

#if MG_ENABLE_TCPIP && (MG_ENABLE_DRIVER_STM32H || MG_ENABLE_DRIVER_MCXN || \
                        MG_ENABLE_DRIVER_STM32N)
// STM32H: vendor modded single-queue Synopsys v4.2
// STM32N: dual-queue GbE Synopsys v5.2 with no hash table option, 64-bit AXI
// MCXNx4x: dual-queue Synopsys v5.2 with no hash table option
// RT1170: ENET_QOS: quad-queue Synopsys v5.1
#if MG_ENABLE_DRIVER_STM32H
#define SYNOPSYS_ENET_V5 0
#define SYNOPSYS_ENET_SINGLEQ 1
#define SYNOPSYS_ENET_NOHASHTABLE 0
#define SYNOPSYS_ENET_GbE 0
#elif MG_ENABLE_DRIVER_STM32N
#define SYNOPSYS_ENET_V5 1
#define SYNOPSYS_ENET_SINGLEQ 0
#define SYNOPSYS_ENET_NOHASHTABLE 1
#define SYNOPSYS_ENET_GbE 1
#elif MG_ENABLE_DRIVER_MCXN
#define SYNOPSYS_ENET_V5 1
#define SYNOPSYS_ENET_SINGLEQ 0
#define SYNOPSYS_ENET_NOHASHTABLE 1
#define SYNOPSYS_ENET_GbE 0
#endif

struct synopsys_enet_qos {
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
#if MG_ENABLE_DRIVER_STM32H
#define ETH ((struct synopsys_enet_qos *) (uintptr_t) 0x40028000UL)
#elif MG_ENABLE_DRIVER_STM32N
#define ETH ((struct synopsys_enet_qos *) (uintptr_t) 0x48036000UL)
#elif MG_ENABLE_DRIVER_MCXN
#define ETH ((struct synopsys_enet_qos *) (uintptr_t) 0x40100000UL)
#endif

#define ETH_PKT_SIZE 1540  // Max frame size
#define ETH_DESC_CNT 4     // Descriptors count
#define ETH_DS 4           // Descriptor size (words)

#if MG_ENABLE_DRIVER_STM32H || MG_ENABLE_DRIVER_STM32N
#define CACHE_LINESZ 32  // must be a whole number of (d)words (see DESC_SZW)
#ifndef SCB
struct m7_scb {
  volatile uint32_t CPUID, RESERVED1[4], CCR, RESERVED2[145], DCIMVAC, DCISW,
      DCCMVAU, DCCMVAC, DCCSW, DCCIMVAC, DCCISW, RESERVED3[10], CACR,
      RESERVED4[3];
};
#define SCB ((struct m7_scb *) (uintptr_t) 0xE000ED00UL)
#endif
// ending ISB is not needed because we don't cache instructions in data space
static inline void MG_CACHE_INVAL(uint8_t *addr, int32_t len) {
#if MG_ENABLE_DRIVER_STM32H
  if ((SCB->CPUID & 0xfff0) != 0xc270) return;  // not a Cortex-M7 => not an H7
#endif
  if ((SCB->CCR & MG_BIT(16)) == 0) return;     // cache not enabled
  MG_DSB();
  while (len > 0) {
    SCB->DCIMVAC = (uint32_t) addr;
    addr += CACHE_LINESZ;
    len -= CACHE_LINESZ;
  }
  MG_DSB();
}
static inline void MG_CACHE_FLUSH(uint8_t *addr, int32_t len) {
#if MG_ENABLE_DRIVER_STM32H
  if ((SCB->CPUID & 0xfff0) != 0xc270) return;  // not a Cortex-M7 => not an H7
#endif
  if ((SCB->CCR & MG_BIT(16)) == 0) return;     // cache not enabled
  MG_DSB();
  while (len > 0) {
    SCB->DCCMVAC = (uint32_t) addr;
    addr += CACHE_LINESZ;
    len -= CACHE_LINESZ;
  }
  MG_DSB();
}
#define ETH_RAM_ALIGNED MG_32BYTE_ALIGNED  // depends on CACHE_LINESZ and ETH
#define CACHE_ALIGN(x) \
  ((((size_t) (x)) + CACHE_LINESZ - 1) & ~(CACHE_LINESZ - 1))
#define DESC_SZ CACHE_ALIGN(4 * ETH_DS)  // grow descriptors to fit a line
#define DESC_SZW (DESC_SZ / 4)
#define BUFF_SZ CACHE_ALIGN(ETH_PKT_SIZE)  // grow buffers to fit n lines
#if MG_ENABLE_DRIVER_STM32H
#define DESC_SKIPW (DESC_SZW - ETH_DS)  // tell DMA the descriptor size, words
#else
// MG_ENABLE_DRIVER_STM32N, DMA is AXI and specs skip in 64-bit double-words
#define DESC_SKIPW ((DESC_SZW - ETH_DS) / 2)
#endif
#else
#define MG_CACHE_FLUSH(a, b)
#define MG_CACHE_INVAL(a, b)
#define ETH_RAM_ALIGNED MG_8BYTE_ALIGNED  // depends on ETH DMA alone
#define DESC_SZ 0
#define DESC_SZW ETH_DS
#define BUFF_SZ ETH_PKT_SIZE
#define DESC_SKIPW 0  // no need to skip, as we're not aligning to cache lines
#endif

// array[rows][cols] = coldata coldata ... for all rows, keeps alignment
static volatile uint32_t s_rxdesc[ETH_DESC_CNT][DESC_SZW] MG_ETH_RAM
    ETH_RAM_ALIGNED;
static volatile uint32_t s_txdesc[ETH_DESC_CNT][DESC_SZW] MG_ETH_RAM
    ETH_RAM_ALIGNED;
static uint8_t s_rxbuf[ETH_DESC_CNT][BUFF_SZ] MG_ETH_RAM ETH_RAM_ALIGNED;
static uint8_t s_txbuf[ETH_DESC_CNT][BUFF_SZ] MG_ETH_RAM ETH_RAM_ALIGNED;

static struct mg_tcpip_if *s_ifp;  // MIP interface

static uint16_t eth_read_phy(uint8_t addr, uint8_t reg) {
  ETH->MACMDIOAR &= (0xF << 8);
  ETH->MACMDIOAR |= ((uint32_t) addr << 21) | ((uint32_t) reg << 16) | 3 << 2;
  ETH->MACMDIOAR |= MG_BIT(0);
  while (ETH->MACMDIOAR & MG_BIT(0)) (void) 0;
  return (uint16_t) ETH->MACMDIODR;
}

static void eth_write_phy(uint8_t addr, uint8_t reg, uint16_t val) {
  ETH->MACMDIODR = val;
  ETH->MACMDIOAR &= (0xF << 8);
  ETH->MACMDIOAR |= ((uint32_t) addr << 21) | ((uint32_t) reg << 16) | 1 << 2;
  ETH->MACMDIOAR |= MG_BIT(0);
  while (ETH->MACMDIOAR & MG_BIT(0)) (void) 0;
}

static bool mg_tcpip_driver_stm32h_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_stm32h_data *d =
      (struct mg_tcpip_driver_stm32h_data *) ifp->driver_data;
  s_ifp = ifp;
  uint8_t phy_addr = d == NULL ? 0 : d->phy_addr;
  uint8_t phy_conf = d == NULL ? MG_PHY_CLOCKS_MAC : d->phy_conf;

  // Init RX descriptors
  memset((char *) s_rxdesc, 0, sizeof(s_rxdesc));  // manual init
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_rxdesc[i][0] = (uint32_t) (uintptr_t) s_rxbuf[i];  // Point to data buffer
    s_rxdesc[i][3] = MG_BIT(31) | MG_BIT(30) | MG_BIT(24);  // OWN, IOC, BUF1V
  }
  MG_CACHE_FLUSH((uint8_t *) s_rxdesc, sizeof(s_rxdesc));

  // Init TX descriptors
  memset((char *) s_txdesc, 0, sizeof(s_txdesc));  // manual init
  for (int i = 0; i < ETH_DESC_CNT; i++) {
    s_txdesc[i][0] = (uint32_t) (uintptr_t) s_txbuf[i];  // Buf pointer
  }
  MG_CACHE_FLUSH((uint8_t *) s_txdesc, sizeof(s_txdesc));

  ETH->DMAMR |= MG_BIT(0);  // Software reset
  for (int i = 0; i < 4; i++)
    (void) 0;  // wait at least 4 clocks before reading
  while ((ETH->DMAMR & MG_BIT(0)) != 0) (void) 0;  // Wait until done

  // Set MDC clock divider. Get user value, else, assume max freq
  int cr = (d == NULL || d->mdc_cr < 0) ? 7 : d->mdc_cr;
  ETH->MACMDIOAR = ((uint32_t) cr & 0xF) << 8;

  // NOTE(scaprile): We do not use timing facilities so the DMA engine does not
  // re-write buffer address
  ETH->DMAMR = 0 << 16;        // use interrupt mode 0 (58.8.1) (reset value)
  ETH->DMASBMR |= MG_BIT(12);  // AAL NOTE(scaprile): is this actually needed
  ETH->MACIER = 0;  // Do not enable additional irq sources (reset value)
  ETH->MACTFCR = MG_BIT(7);  // Disable zero-quanta pause
#if !SYNOPSYS_ENET_V5
  ETH->MACPFR = MG_BIT(10);  // Perfect filtering
#endif
  struct mg_phy phy = {eth_read_phy, eth_write_phy};
  mg_phy_init(&phy, phy_addr, phy_conf);
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
  ETH->DMACCR = DESC_SKIPW << 18;  // DSL (contiguous/sparse descriptor table)
#if SYNOPSYS_ENET_V5
  MG_SET_BITS(ETH->DMACTCR, 0x3F << 16, MG_BIT(16));
  MG_SET_BITS(ETH->DMACRCR, 0x3F << 16, MG_BIT(16));
#endif
  ETH->DMACIER = MG_BIT(6) | MG_BIT(15);  // RIE, NIE
  ETH->MACCR = MG_BIT(0) | MG_BIT(1) | MG_BIT(13) | MG_BIT(14) |
               MG_BIT(15);  // RE, TE, Duplex, Fast, (10/100)/Reserved
#if SYNOPSYS_ENET_SINGLEQ
  ETH->MTLTQOMR |= MG_BIT(1);  // TSF
  ETH->MTLRQOMR |= MG_BIT(5);  // RSF
#else
  ETH->MTLTQOMR |= (7 << 16) | MG_BIT(3) | MG_BIT(1);  // 2KB Q0, TSF
  ETH->MTLRQOMR |= (7 << 20) | MG_BIT(5);              // 2KB Q, RSF
  MG_SET_BITS(ETH->RESERVED6[3], 3, 2);  // Enable RxQ0 (MAC_RXQ_CTRL0)
#endif
  ETH->DMACTCR |= MG_BIT(0);  // ST
  ETH->DMACRCR |= MG_BIT(0);  // SR

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
    return 0;  // Frame is too big
  }
  MG_CACHE_INVAL((uint8_t *) &s_txdesc[s_txno], DESC_SZ);
  if ((s_txdesc[s_txno][3] & MG_BIT(31))) {
    ifp->nerr++;
    MG_ERROR(("No free descriptors: %u %08X %08X %08X", s_txno,
              s_txdesc[s_txno][3], ETH->DMACSR, ETH->DMACTCR));
    MG_CACHE_INVAL((uint8_t *) s_txdesc, sizeof(s_txdesc));
    for (int i = 0; i < ETH_DESC_CNT; i++) MG_ERROR(("%08X", s_txdesc[i][3]));
    len = 0;  // All descriptors are busy, fail
  } else {
    memcpy(s_txbuf[s_txno], buf, len);  // Copy data
    MG_CACHE_FLUSH((uint8_t *) &s_txbuf[s_txno], BUFF_SZ);
    s_txdesc[s_txno][2] = (uint32_t) len;           // Set data len
    s_txdesc[s_txno][3] = MG_BIT(28) | MG_BIT(29);  // FD, LD
    s_txdesc[s_txno][3] |= MG_BIT(31);  // Set OWN bit - let DMA take over
    MG_CACHE_FLUSH((uint8_t *) &s_txdesc[s_txno], DESC_SZ);
    if (++s_txno >= ETH_DESC_CNT) s_txno = 0;
  }
  ETH->DMACSR |= MG_BIT(2) | MG_BIT(1);  // Clear any prior TBU, TPS
  ETH->DMACTDTPR = (uint32_t) (uintptr_t) &s_txdesc[s_txno];  // and resume
  return len;
  (void) ifp;
}

static void mg_tcpip_driver_stm32h_update_hash_table(struct mg_tcpip_if *ifp) {
#if SYNOPSYS_ENET_NOHASHTABLE
  ETH->MACPFR = MG_BIT(4);  // Pass Multicast (pass all multicast frames)
#else
  // TODO(): read database, rebuild hash table
  // add mDNS / DNS-SD multicast address
  ETH->MACA1LR = (uint32_t) mcast_addr[3] << 24 |
                 (uint32_t) mcast_addr[2] << 16 |
                 (uint32_t) mcast_addr[1] << 8 | (uint32_t) mcast_addr[0];
  ETH->MACA1HR = (uint32_t) mcast_addr[5] << 8 | (uint32_t) mcast_addr[4];
  ETH->MACA1HR |= MG_BIT(31);  // AE
#endif
  (void) ifp;
}

static bool mg_tcpip_driver_stm32h_poll(struct mg_tcpip_if *ifp, bool s1) {
  if (ifp->update_mac_hash_table) {
    mg_tcpip_driver_stm32h_update_hash_table(ifp);
    ifp->update_mac_hash_table = false;
  }
  if (!s1) return false;
  struct mg_tcpip_driver_stm32h_data *d =
      (struct mg_tcpip_driver_stm32h_data *) ifp->driver_data;
  uint8_t phy_addr = d == NULL ? 0 : d->phy_addr;
  uint8_t speed = MG_PHY_SPEED_10M;
  bool up = false, full_duplex = false;
  struct mg_phy phy = {eth_read_phy, eth_write_phy};
  up = mg_phy_up(&phy, phy_addr, &full_duplex, &speed);
  if ((ifp->state == MG_TCPIP_STATE_DOWN) && up) {  // link state just went up
    // tmp = reg with flags set to the most likely situation: 100M full-duplex
    // if(link is slow or half) set flags otherwise
    // reg = tmp
    uint32_t maccr = ETH->MACCR | MG_BIT(14) | MG_BIT(13);  // 100M, Full-duplex
#if SYNOPSYS_ENET_GbE
    if (speed == MG_PHY_SPEED_1000M) maccr &= ~MG_BIT(15);  // 1000M
#endif
    if (speed == MG_PHY_SPEED_10M) maccr &= ~MG_BIT(14);  // 10M
    if (full_duplex == false) maccr &= ~MG_BIT(13);       // Half-duplex
    ETH->MACCR = maccr;  // IRQ handler does not fiddle with this register
    MG_DEBUG(("Link is %uM %s-duplex", maccr & MG_BIT(14) ? 100 : 10,
              maccr & MG_BIT(13) ? "full" : "half"));
  }
  return up;
}

static uint32_t s_rxno;
#if MG_ENABLE_DRIVER_MCXN
void ETHERNET_IRQHandler(void);
void ETHERNET_IRQHandler(void) {
#elif MG_ENABLE_DRIVER_STM32H
void ETH_IRQHandler(void);
void ETH_IRQHandler(void) {
#else
void ETH1_IRQHandler(void);
void ETH1_IRQHandler(void) {
#endif
  if (ETH->DMACSR & MG_BIT(6)) {           // Frame received, loop
    ETH->DMACSR = MG_BIT(15) | MG_BIT(6);  // Clear flag
    for (uint32_t i = 0; i < 10; i++) {  // read as they arrive but not forever
      MG_CACHE_INVAL((uint8_t *) &s_rxdesc[s_rxno], DESC_SZ);
      if (s_rxdesc[s_rxno][3] & MG_BIT(31)) break;  // exit when done
      if (((s_rxdesc[s_rxno][3] & (MG_BIT(28) | MG_BIT(29))) ==
           (MG_BIT(28) | MG_BIT(29))) &&
          !(s_rxdesc[s_rxno][3] & MG_BIT(15))) {  // skip partial/errored frames
        uint32_t len = s_rxdesc[s_rxno][3] & (MG_BIT(15) - 1);
        // MG_DEBUG(("%lx %lu %lx %08lx", s_rxno, len, s_rxdesc[s_rxno][3],
        // ETH->DMACSR));
        MG_CACHE_INVAL((uint8_t *) &s_rxbuf[s_rxno], BUFF_SZ);
        mg_tcpip_qwrite(s_rxbuf[s_rxno], len > 4 ? len - 4 : len, s_ifp);
      }
      s_rxdesc[s_rxno][3] =
          MG_BIT(31) | MG_BIT(30) | MG_BIT(24);  // OWN, IOC, BUF1V
      MG_CACHE_FLUSH((uint8_t *) &s_rxdesc[s_rxno], DESC_SZ);
      if (++s_rxno >= ETH_DESC_CNT) s_rxno = 0;
    }
  }
  ETH->DMACSR =
      MG_BIT(7) | MG_BIT(8);  // Clear possible RBU RPS while processing
  ETH->DMACRDTPR =
      (uint32_t) (uintptr_t) &s_rxdesc[ETH_DESC_CNT - 1];  // and resume RX
}

struct mg_tcpip_driver mg_tcpip_driver_stm32h = {
    mg_tcpip_driver_stm32h_init, mg_tcpip_driver_stm32h_tx, NULL,
    mg_tcpip_driver_stm32h_poll};
#endif
