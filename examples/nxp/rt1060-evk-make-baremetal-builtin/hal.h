// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
// https://cache.nxp.com/secured/assets/documents/en/reference-manual/IMXRT1060XRM.pdf
// https://cache.nxp.com/secured/assets/documents/en/user-guide/MIMXRT1060EVKBUM.pdf

#pragma once

#include "MIMXRT1062.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - '0') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

// Use LED for blinking, D8: GPIO_AD_B0_08. GPIO1.8 (schematics, RM)
#define LED PIN('1', 8)

#ifndef UART_DEBUG
#define UART_DEBUG LPUART1
#endif

// No settable constants, see sysinit.c
#define SYS_FREQUENCY 600000000UL

static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

enum { CLOCK_OFF = 0U, CLOCK_ON_RUN = 1U, CLOCK_ON_RUN_WAIT = 3U };
static inline void clock_periph(uint32_t index, uint32_t shift, uint32_t val) {
  volatile uint32_t *r = &CCM->CCGR0;
  SETBITS(r[index], 3UL << shift, val << shift);
}

// which peripheral feeds the pin
static inline void gpio_mux_config(uint16_t index, uint8_t af) {
  IOMUXC->SW_MUX_CTL_PAD[index] = af;
}

// which pin feeds the peripheral (2nd stage)
static inline void periph_mux_config(uint16_t index, uint8_t in) {
  IOMUXC->SELECT_INPUT[index] = in;
}

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT };
enum { GPIO_OTYPE_PUSH_PULL, GPIO_OTYPE_OPEN_DRAIN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_MEDIUM_, GPIO_SPEED_HIGH };
enum { GPIO_PULL_NONE, GPIO_PULL_DOWN, GPIO_PULL_UP };
static inline GPIO_Type *gpio_bank(uint16_t pin) {
  static const GPIO_Type *g[] = {NULL, GPIO1, GPIO2, GPIO3, NULL, GPIO5};
  return (GPIO_Type *) g[PINBANK(pin)];
}

// pin driver/pull-up/down configuration (ignore "keeper")
static inline void gpio_pad_config(uint16_t index, uint8_t type, uint8_t speed,
                                   uint8_t pull) {
  bool dopull = pull > 0;
  if (dopull) --pull;
  IOMUXC->SW_PAD_CTL_PAD[index] =
      IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_ODE(type) |
      IOMUXC_SW_PAD_CTL_PAD_SRE(speed >= GPIO_SPEED_HIGH) |
      IOMUXC_SW_PAD_CTL_PAD_PUE(1) | IOMUXC_SW_PAD_CTL_PAD_PKE(dopull) |
      IOMUXC_SW_PAD_CTL_PAD_PUS(pull) | IOMUXC_SW_PAD_CTL_PAD_DSE(7);
}

static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull) {
  GPIO_Type *gpio = gpio_bank(pin);
  uint8_t bit = (uint8_t) PINNO(pin);
  uint32_t mask = (uint32_t) BIT(PINNO(pin));

  clock_periph(4, CCM_CCGR4_CG1_SHIFT, CLOCK_ON_RUN_WAIT);  // iomuxc_ipg_clk_s
  switch (PINBANK(pin)) {
    case 1:
      gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_00 + bit, 5);
      gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_00 + bit, type, speed,
                      pull);
      clock_periph(1, CCM_CCGR1_CG13_SHIFT, CLOCK_ON_RUN_WAIT);
      break;
    case 2:
      gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_00 + bit, 5);
      gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_00 + bit, type, speed,
                      pull);
      clock_periph(0, CCM_CCGR0_CG15_SHIFT, CLOCK_ON_RUN_WAIT);
      break;
    case 3:
      gpio_mux_config(bit < 12 ? kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B1_00 + bit
                      : bit < 18
                          ? kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B0_00 + bit - 12
                          : kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_32 + bit - 18,
                      5);
      gpio_pad_config(bit < 12 ? kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B1_00 + bit
                      : bit < 18
                          ? kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B0_00 + bit - 12
                          : kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_32 + bit - 18,
                      type, speed, pull);
      clock_periph(2, CCM_CCGR2_CG13_SHIFT, CLOCK_ON_RUN_WAIT);
      break;
    case 5:
      // TODO(): support sw_mux
      clock_periph(1, CCM_CCGR1_CG15_SHIFT, CLOCK_ON_RUN_WAIT);
      break;
    default:
      break;
  }

  gpio->IMR &= ~mask;
  if (mode == GPIO_MODE_INPUT) {
    gpio->GDIR &= ~mask;
  } else {
    gpio->GDIR |= mask;
  }
}
static inline void gpio_input(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_INPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_MEDIUM,
            GPIO_PULL_NONE);
}
static inline void gpio_output(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_MEDIUM,
            GPIO_PULL_NONE);
}

static inline bool gpio_read(uint16_t pin) {
  GPIO_Type *gpio = gpio_bank(pin);
  uint32_t mask = (uint32_t) BIT(PINNO(pin));
  return gpio->DR & mask;
}
static inline void gpio_write(uint16_t pin, bool value) {
  GPIO_Type *gpio = gpio_bank(pin);
  uint32_t mask = (uint32_t) BIT(PINNO(pin));
  if (value) {
    gpio->DR |= mask;
  } else {
    gpio->DR &= ~mask;
  }
}
static inline void gpio_toggle(uint16_t pin) {
  gpio_write(pin, !gpio_read(pin));
}

// 14.5 Table 14-4: uart_clk_root
// see sysinit.c for clocks, (14.7.9: defaults to pll3_80m = PLL3/6/1 = 80MHz)
static inline void uart_init(LPUART_Type *uart, unsigned long baud) {
  uint8_t af = 2;                           // Alternate function
  uint16_t mr = 0, pr = 0, mt = 0, pt = 0;  // pins
  uint32_t freq = 80000000;                 // uart_clk_root frequency
  if (uart == LPUART1)
    mt = kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_12,
    pt = kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_12,
    mr = kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_13,
    pr = kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_13;
  if (uart == LPUART3)
    mt = kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_06,
    pt = kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_06,
    mr = kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_07,
    pr = kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_07;

  if (uart == LPUART1) clock_periph(5, CCM_CCGR5_CG12_SHIFT, CLOCK_ON_RUN_WAIT);
  if (uart == LPUART3) clock_periph(0, CCM_CCGR0_CG6_SHIFT, CLOCK_ON_RUN_WAIT);
  clock_periph(4, CCM_CCGR4_CG1_SHIFT, CLOCK_ON_RUN_WAIT);  // iomuxc_ipg_clk_s
  gpio_mux_config(mt, af);
  gpio_pad_config(pt, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_MEDIUM, GPIO_PULL_UP);
  gpio_mux_config(mr, af);
  gpio_pad_config(pr, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_MEDIUM, GPIO_PULL_UP);

  uart->GLOBAL |= LPUART_GLOBAL_RST_MASK;  // reset, CTRL = 0, defaults
  uart->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;
  SETBITS(uart->BAUD,
          LPUART_BAUD_OSR_MASK | LPUART_BAUD_SBR_MASK | LPUART_BAUD_SBNS_MASK,
          LPUART_BAUD_OSR(16 - 1) |
              LPUART_BAUD_SBR(freq / (16 * baud)));  // Rx sample at 16x
  SETBITS(uart->CTRL,
          LPUART_CTRL_PE_MASK | LPUART_CTRL_M_MASK | LPUART_CTRL_ILT_MASK |
              LPUART_CTRL_IDLECFG_MASK,
          LPUART_CTRL_IDLECFG(1) | LPUART_CTRL_ILT(1) |
              LPUART_BAUD_SBNS(0));  // no parity, idle 2 chars after 1 stop bit
  uart->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
}

static inline void uart_write_byte(LPUART_Type *uart, uint8_t byte) {
  uart->DATA = byte;
  while ((uart->STAT & LPUART_STAT_TDRE_MASK) == 0) spin(1);
}
static inline void uart_write_buf(LPUART_Type *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int uart_read_ready(LPUART_Type *uart) {
  (void) uart;
  return uart->STAT & LPUART_STAT_RDRF_MASK;
}
static inline uint8_t uart_read_byte(LPUART_Type *uart) {
  return (uint8_t) (uart->DATA & 255);
}

static inline void rng_init(void) {
  clock_periph(6, CCM_CCGR6_CG6_SHIFT, CLOCK_ON_RUN_WAIT);  // trng_clk
  SETBITS(TRNG->MCTL,
          TRNG_MCTL_PRGM_MASK | TRNG_MCTL_ERR_MASK | TRNG_MCTL_RST_DEF_MASK,
          TRNG_MCTL_PRGM(1) | TRNG_MCTL_ERR(1) |
              TRNG_MCTL_RST_DEF(1));  // reset to default values
  SETBITS(TRNG->MCTL, TRNG_MCTL_PRGM_MASK | TRNG_MCTL_ERR_MASK,
          TRNG_MCTL_PRGM(0));            // set to run mode
  (void) TRNG->ENT[TRNG_ENT_COUNT - 1];  // start new entropy generation
  (void) TRNG->ENT[0];                   // defect workaround
}
static inline uint32_t rng_read(void) {
  static uint8_t idx = 0;
  while ((TRNG->MCTL & TRNG_MCTL_ENT_VAL_MASK) == 0) (void) 0;
  uint32_t data = TRNG->ENT[idx++];  // read data
  idx %= TRNG_ENT_COUNT;             // stay within array limits
  if (idx == 0)                      // we've just read TRNG_ENT_COUNT - 1
    (void) TRNG->ENT[0];             // defect workaround
  return data;
}

// - PHY has no xtal, XI driven from ENET_REF_CLK1 (labeled as ENET_TX_REF_CLK
// (GPIO_AD_B1_10)), generated by the MCU
// - PHY RST connected to GPIO1.9 (GPIO_AD_B0_09); INTRP/NAND_TREE connected to
// GPIO1.10 (GPIO_AD_B0_10)
// - 41.4 REF_CLK1 is RMII mode reference clock for Rx, Tx, and SMI; it is I/O
// - 11.3.2 IOMUXC_GPR_GPR1
//   - bit 13: ENET1_CLK_SEL --> 0 ENET1 TX reference clock driven by
//   ref_enetpll and output via ENET_REF_CLK1 (labeled as ENET_REF_CLK
//   elsewhere)
//   - bit 17: ENET1_TX_CLK_DIR --> 1 ENET1_TX_CLK output driver enabled
// - 14.6.1.3.6 Ethernet PLL (PLL6)
// - 14.8.14: configure PLL6 to generate 50MHz clocks for ENET and RMII.
static inline void ethernet_init(void) {
  // setup PLL and clock ENET from it
  SETBITS(CCM_ANALOG->PLL_ENET, CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC_MASK,
          CCM_ANALOG_PLL_ENET_BYPASS_MASK |
              CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC(0));  // bypass to 24MHz osc
  SETBITS(
      CCM_ANALOG->PLL_ENET,
      CCM_ANALOG_PLL_ENET_DIV_SELECT_MASK | CCM_ANALOG_PLL_ENET_POWERDOWN_MASK,
      CCM_ANALOG_PLL_ENET_DIV_SELECT(1) | CCM_ANALOG_PLL_ENET_ENABLE_MASK);
  while ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK_MASK) == 0)
    spin(1);  // wait until it is stable
  CCM_ANALOG->PLL_ENET &= ~CCM_ANALOG_PLL_ENET_BYPASS_MASK;

  gpio_init(PIN('1', 9), GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL,
            GPIO_SPEED_MEDIUM, GPIO_PULL_UP);  // set GPIO1.9 as GPIO (PHY \RST)
  gpio_write(PIN('1', 9), 0);                  // reset PHY

  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_10,
                  6);  // set for ENET_REF_CLK
  IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_10] |=
      IOMUXC_SW_MUX_CTL_PAD_SION(1);  // loop signal back from pin
  periph_mux_config(kIOMUXC_ENET_IPG_CLK_RMII_SELECT_INPUT,
                    1);  // drive peripheral from B1_10, so RMII clock is taken
                         // from ENET_REF_CLK
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_10, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_HIGH, GPIO_PULL_UP);
  IOMUXC_GPR->GPR1 |=
      IOMUXC_GPR_GPR1_ENET1_TX_CLK_DIR(1);  // Set ENET_TX_CLK as output
  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_04, 3);  // set for RXDATA0
  periph_mux_config(kIOMUXC_ENET0_RXDATA_SELECT_INPUT,
                    1);  // drive peripheral from B1_04
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_04, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_HIGH, GPIO_PULL_UP);
  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_05, 3);  // set for RXDATA1
  periph_mux_config(kIOMUXC_ENET1_RXDATA_SELECT_INPUT,
                    1);  // drive peripheral from B1_05
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_05, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_HIGH, GPIO_PULL_UP);
  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_06, 3);  // set for CRS
  periph_mux_config(kIOMUXC_ENET_RXEN_SELECT_INPUT,
                    1);  // drive peripheral from B1_06
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_06, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_HIGH, GPIO_PULL_UP);
  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_07, 3);  // set for TXDATA0
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_07, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_HIGH, GPIO_PULL_UP);
  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_08, 3);  // set for TXDATA1
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_08, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_HIGH, GPIO_PULL_UP);
  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_09, 3);  // set for TXEN
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_09, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_HIGH, GPIO_PULL_UP);
  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_11, 3);  // set for RXERR
  periph_mux_config(kIOMUXC_ENET_RXERR_SELECT_INPUT,
                    1);  // drive peripheral from B0_12
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_11, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_HIGH, GPIO_PULL_UP);
  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_40, 4);  // set for MDC
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_40, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_MEDIUM, GPIO_PULL_UP);
  gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_41, 4);  // set for MDIO
  periph_mux_config(kIOMUXC_ENET_MDIO_SELECT_INPUT,
                    1);  // drive peripheral from EMC_41
  gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_41, GPIO_OTYPE_PUSH_PULL,
                  GPIO_SPEED_MEDIUM, GPIO_PULL_UP);

  gpio_init(PIN('1', 10), GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL,
            GPIO_SPEED_MEDIUM,
            GPIO_PULL_UP);      // set GPIO1.10 as GPIO (PHY INTRP/NAND_TREE)
  gpio_write(PIN('1', 10), 1);  // prevent NAND_TREE
  spin(10000);                  // keep PHY RST low for a while
  gpio_write(PIN('1', 9), 1);   // deassert RST
  gpio_init(PIN('1', 10), GPIO_MODE_INPUT, 0, GPIO_SPEED_MEDIUM,
            GPIO_PULL_UP);  // setup IRQ (pulled-up)(not used)

  clock_periph(1, CCM_CCGR1_CG5_SHIFT, CLOCK_ON_RUN_WAIT);  // enet_ipg_clk
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
