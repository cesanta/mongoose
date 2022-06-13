// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mip.h"

#if MG_ENABLE_MIP && defined(__arm__)

struct stm32_eth {
  uint32_t MACCR, MACFFR, MACHTHR, MACHTLR, MACMIIAR, MACMIIDR, MACFCR,
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
static void *s_userdata;                             // Driver data
enum { PHY_ADDR = 0, PHY_BCR = 0, PHY_BSR = 1 };     // PHY constants

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

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

static void mip_driver_stm32_init(void *userdata) {
  s_userdata = userdata;

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
  ETH->MACIMR = BIT(3) | BIT(9);              // Mask timestamp & PMT IT
  ETH->MACMIIAR = 4 << 2;                     // MDC clock 150-216 MHz, 38.8.1
  ETH->MACFCR = BIT(7);                       // Disable zero quarta pause
  ETH->MACFFR = BIT(31);                      // Receive all
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(15));  // Reset PHY
  eth_write_phy(PHY_ADDR, PHY_BCR, BIT(12));  // Set autonegotiation
  ETH->DMARDLAR = (uint32_t) (uintptr_t) s_rxdesc;     // RX descriptors
  ETH->DMATDLAR = (uint32_t) (uintptr_t) s_txdesc;     // RX descriptors
  ETH->DMAIER = BIT(6) | BIT(16);                      // RIE, NISE
  ETH->MACCR = BIT(2) | BIT(3) | BIT(11) | BIT(14);    // RE, TE, Duplex, Fast
  ETH->DMAOMR = BIT(1) | BIT(13) | BIT(21) | BIT(25);  // SR, ST, TSF, RSF
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

static bool mip_driver_stm32_status(void *userdata) {
  uint32_t bsr = eth_read_phy(PHY_ADDR, PHY_BSR);
  return bsr & BIT(2) ? 1 : 0;
  (void) userdata;
}

void ETH_IRQHandler(void);
void ETH_IRQHandler(void) {
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
                                      .rxcb = mip_driver_stm32_setrx,
                                      .status = mip_driver_stm32_status};
#endif  // MG_ENABLE_MIP
