// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
// https://cache.nxp.com/secured/assets/documents/en/reference-manual/IMXRT1060XRM.pdf
// https://cache.nxp.com/secured/assets/documents/en/user-guide/MIMXRT1060EVKBUM.pdf

#pragma once

#include "MIMXRT1062.h"

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"

static inline void ethernet_init(void) {
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_ENET_REF_CLK, 1U);
  //
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0xB0A9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_10_GPIO1_IO10, 0xB0A9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_ENET_RX_DATA00, 0xB0E9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_05_ENET_RX_DATA01, 0xB0E9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_ENET_RX_EN, 0xB0E9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_ENET_TX_DATA00, 0xB0E9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_ENET_TX_DATA01, 0xB0E9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_ENET_TX_EN, 0xB0E9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_10_ENET_REF_CLK, 0xB0E9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_ENET_RX_ER, 0xB0E9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_40_ENET_MDC, 0xB0E9U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_41_ENET_MDIO, 0xB0E9U);
  //
  IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
  gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
  GPIO_PinInit(GPIO1, 9, &gpio_config);
  GPIO_PinInit(GPIO1, 10, &gpio_config);
  GPIO_PinWrite(GPIO1, 9, 0);                  // reset PHY
  GPIO_PinWrite(GPIO1, 10, 1);  // prevent NAND_TREE
  SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk)); // keep PHY RST low for a while
  GPIO_PinWrite(GPIO1, 9, 1);  // deassert RST
  CLOCK_EnableClock(kCLOCK_Enet);
  NVIC_EnableIRQ(ENET_IRQn);  // Setup Ethernet IRQ handler
}

// Helper macro for MAC generation, byte reads not allowed
#define GENERATE_LOCALLY_ADMINISTERED_MAC()                \
  {                                                        \
    2, OCOTP->CFG0 & 255, (OCOTP->CFG0 >> 10) & 255,       \
        ((OCOTP->CFG0 >> 19) ^ (OCOTP->CFG1 >> 19)) & 255, \
        (OCOTP->CFG1 >> 10) & 255, OCOTP->CFG1 & 255       \
  }
// NOTE: You can fuse your own MAC and read it from OCOTP->MAC0, OCOTP->MAC1,
// OCOTP->MAC2

// For internal testing purposes
#ifdef UART_DEBUG
#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
// 14.5 Table 14-4: uart_clk_root
// see sysinit.c for clocks, (14.7.9: defaults to pll3_80m = PLL3/6/1 = 80MHz)
static inline void test_init(void) {
  LPUART_Type *uart = LPUART3;  // hardcode to UART3 GPIO_AD_B1_06,07
  uint32_t freq = 80000000;                 // uart_clk_root frequency
  CCM_ANALOG->PLL_USB1 |= CCM_ANALOG_PLL_USB1_POWER_MASK;  // Power PLL on
  while ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_MASK) == 0)
    (void) 0;  // wait until it is stable
  CCM_ANALOG->PLL_USB1 &= ~CCM_ANALOG_PLL_USB1_BYPASS_MASK;  // Disable Bypass (switch to PLL)
  CCM->CSCDR1 &= ~(CCM_CSCDR1_UART_CLK_SEL_MASK | CCM_CSCDR1_UART_CLK_PODF_MASK);
  CLOCK_EnableClock(kCLOCK_Lpuart3);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_LPUART3_TX, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_LPUART3_RX, 0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_LPUART3_TX, 0x10B0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_LPUART3_RX, 0x10B0U);
  uart->GLOBAL |= LPUART_GLOBAL_RST_MASK;  // reset, CTRL = 0, defaults
  uart->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;
  SETBITS(uart->BAUD,
          LPUART_BAUD_OSR_MASK | LPUART_BAUD_SBR_MASK | LPUART_BAUD_SBNS_MASK,
          LPUART_BAUD_OSR(16 - 1) |
              LPUART_BAUD_SBR(freq / (16 * 115200)));  // Rx sample at 16x
  SETBITS(uart->CTRL,
          LPUART_CTRL_PE_MASK | LPUART_CTRL_M_MASK | LPUART_CTRL_ILT_MASK |
              LPUART_CTRL_IDLECFG_MASK,
          LPUART_CTRL_IDLECFG(1) | LPUART_CTRL_ILT(1) |
              LPUART_BAUD_SBNS(0));  // no parity, idle 2 chars after 1 stop bit
  uart->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
}
static inline void uart_write_byte(LPUART_Type *uart, uint8_t byte) {
  uart->DATA = byte;
  while ((uart->STAT & LPUART_STAT_TDRE_MASK) == 0) (void) 0;
}
static inline void uart_write_buf(LPUART_Type *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
#else
#define test_init()
#endif
