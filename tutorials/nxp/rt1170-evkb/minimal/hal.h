// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved
// https://www.nxp.com/webapp/Download?colCode=IMXRT1170RM
// https://www.nxp.com/webapp/Download?colCode=MIMXRT1170EVKBHUG

#pragma once

#include "MIMXRT1176_cm7.h"
#include "fsl_clock.h"
#include "fsl_dcdc.h"
#include "fsl_pmu.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - '0') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

void hal_init(void);
size_t hal_ram_free(void);
size_t hal_ram_used(void);

bool hal_timer_expired(volatile uint64_t *t, uint64_t period, uint64_t now);
uint64_t hal_get_tick(void);

// No settable constants, see sysinit.c
#define SYS_FREQUENCY 996000000UL

static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

// Use "Unassigned/Domain Mode"
static inline void hal_clock_periph(uint32_t index, bool val) {
  CCM->LPCG[index].DIRECT = val ? 1 : 0;  // (15.9.1.25)
}

// which peripheral feeds the pin
static inline void hal_gpio_mux_config(uint16_t index, uint8_t af) {
  IOMUXC->SW_MUX_CTL_PAD[index] = af;
}

// which pin feeds the peripheral (2nd stage)
static inline void hal_periph_mux_config(uint16_t index, uint8_t in) {
  IOMUXC->SELECT_INPUT[index] = in;
}

// CM7_GPIOx not supported
enum { HAL_GPIO_MODE_INPUT, HAL_GPIO_MODE_OUTPUT };
enum { HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_OTYPE_OPEN_DRAIN };
enum { HAL_GPIO_SPEED_LOW, HAL_GPIO_SPEED_MEDIUM, HAL_GPIO_SPEED_MEDIUM_, HAL_GPIO_SPEED_HIGH };
enum { HAL_GPIO_PULL_NONE, HAL_GPIO_PULL_UP, HAL_GPIO_PULL_DOWN };
static inline GPIO_Type *hal_gpio_bank(uint16_t pin) {
  static const GPIO_Type *g[] = {NULL,   GPIO1,  GPIO2,  GPIO3, GPIO4,
                                 GPIO5,  GPIO6,  GPIO7,  GPIO8, GPIO9,
                                 GPIO10, GPIO11, GPIO12, GPIO13};
  return (GPIO_Type *) g[PINBANK(pin)];
}

// pin driver/pull-up/down configuration (allow both cores)
static inline void hal_gpio_pad_config(uint16_t index, uint8_t type, uint8_t speed,
                                   uint8_t pull) {
  if (index < kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_00 ||
      (index >= kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B1_00 &&
       index < kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_00)) {
    IOMUXC->SW_PAD_CTL_PAD[index] =
        IOMUXC_SW_PAD_CTL_PAD_PDRV(speed == HAL_GPIO_SPEED_LOW) |
        IOMUXC_SW_PAD_CTL_PAD_ODE(type) | IOMUXC_SW_PAD_CTL_PAD_PULL(pull);
  } else {
    bool dopull = pull > 0;
    if (dopull) pull = (pull == HAL_GPIO_PULL_UP);
    IOMUXC->SW_PAD_CTL_PAD[index] =
        IOMUXC_SW_PAD_CTL_PAD_DSE(speed != HAL_GPIO_SPEED_LOW) |
        IOMUXC_SW_PAD_CTL_PAD_ODE(type) | IOMUXC_SW_PAD_CTL_PAD_PUE(dopull) |
        IOMUXC_SW_PAD_CTL_PAD_PUS(pull) |
        IOMUXC_SW_PAD_CTL_PAD_SRE(speed >= HAL_GPIO_SPEED_HIGH);
  }
}

static inline void hal_gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull) {
  GPIO_Type *gpio = hal_gpio_bank(pin);
  uint8_t bit = (uint8_t) PINNO(pin);
  uint32_t mask = (uint32_t) BIT(PINNO(pin));

  hal_clock_periph(51, 1);     // clk_enable_gpio (15.5.4 Table 15-5)
  hal_clock_periph(49, 1);     // clk_enable_iomuxc (15.5.4 Table 15-5)
  switch (PINBANK(pin)) {  // (11.1.1 Table 11-1)
    case 1:
      hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_00 + bit, 5);
      hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_B1_00 + bit, type, speed,
                      pull);
      break;
    case 2:
      IOMUXC_GPR->GPR40 = 0;  // select GPIO2
      IOMUXC_GPR->GPR41 = 0;  // select GPIO2
      hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_32 + bit, 5);
      hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_B1_32 + bit, type, speed,
                      pull);
      break;
    case 3:
      IOMUXC_GPR->GPR42 = 0;  // select GPIO3
      IOMUXC_GPR->GPR43 = 0;  // select GPIO3
      hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_01 + bit, 5);
      hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_01 + bit, type, speed,
                      pull);
      break;
    case 4:
      hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_33 + bit, 5);
      hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_33 + bit, type, speed,
                      pull);
      break;
    default:
      // TODO(): support GPIO5-13, 10-15 requires redefinition of PIN() macro
      break;
  }

  gpio->IMR &= ~mask;
  if (mode == HAL_GPIO_MODE_INPUT) {
    gpio->GDIR &= ~mask;
  } else {
    gpio->GDIR |= mask;
  }
}
static inline void hal_gpio_input(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_MEDIUM,
            HAL_GPIO_PULL_NONE);
}
static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_MEDIUM,
            HAL_GPIO_PULL_NONE);
}

static inline bool hal_gpio_read(uint16_t pin) {
  GPIO_Type *gpio = hal_gpio_bank(pin);
  uint32_t mask = (uint32_t) BIT(PINNO(pin));
  return gpio->DR & mask;
}
static inline void hal_gpio_write(uint16_t pin, bool value) {
  GPIO_Type *gpio = hal_gpio_bank(pin);
  uint32_t mask = (uint32_t) BIT(PINNO(pin));
  if (value) {
    gpio->DR |= mask;
  } else {
    gpio->DR &= ~mask;
  }
}
static inline void hal_gpio_toggle(uint16_t pin) {
  hal_gpio_write(pin, !hal_gpio_read(pin));
}

// 15.3 Table 15-2: uart_clk_root
// 15.4: lpuart*_clk_root 15.5.3 Table 15-4; 15.9.1.2; select OSC_24M
static inline void hal_uart_init(LPUART_Type *uart, unsigned long baud) {
  uint8_t af = 0;                           // Alternate function
  uint16_t mr = 0, pr = 0, mt = 0, pt = 0;  // pins
  uint32_t freq = 24000000;                 // uart_clk_root frequency
  if (uart == LPUART1) {
    af = 0;
    mt = kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_24;
    pt = kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_24;
    mr = kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_25;
    pr = kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_25;
    // configure clock root source and divisor /1, enable peripheral clock
    CCM->CLOCK_ROOT[25].CONTROL =
        CCM_CLOCK_ROOT_CONTROL_MUX(1) | CCM_CLOCK_ROOT_CONTROL_DIV(0);
    hal_clock_periph(86, 1);  // (15.5.4 Table 15-5)
    hal_periph_mux_config(kIOMUXC_LPUART1_LPUART_RXD_SELECT_INPUT, 0);
    hal_periph_mux_config(kIOMUXC_LPUART1_LPUART_TXD_SELECT_INPUT, 0);
  } else if (uart == LPUART2) {
    af = 2;
    mt = kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_10;
    pt = kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_10;
    mr = kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_11;
    pr = kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_11;
    CCM->CLOCK_ROOT[26].CONTROL =
        CCM_CLOCK_ROOT_CONTROL_MUX(1) | CCM_CLOCK_ROOT_CONTROL_DIV(0);
    hal_clock_periph(87, 1);  // (15.5.4 Table 15-5)
  }
  hal_clock_periph(49, 1);  // clk_enable_iomuxc (15.5.4 Table 15-5)
  hal_gpio_mux_config(mt, af);
  hal_gpio_pad_config(pt, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_MEDIUM, HAL_GPIO_PULL_NONE);
  hal_gpio_mux_config(mr, af);
  hal_gpio_pad_config(pr, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_MEDIUM, HAL_GPIO_PULL_UP);

  uart->GLOBAL |= LPUART_GLOBAL_RST_MASK;  // reset, CTRL = 0, defaults
  uart->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;
  uart->BAUD = LPUART_BAUD_OSR(16 - 1) |
               LPUART_BAUD_SBR(freq / (16 * baud));  // Rx sample at 16x
  uart->CTRL = LPUART_CTRL_IDLECFG(1) |
               LPUART_CTRL_ILT(1);  // no parity, idle 2 chars after 1 stop bit
  uart->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
}

static inline void hal_uart_write_byte(LPUART_Type *uart, uint8_t byte) {
  uart->DATA = byte;
  while ((uart->STAT & LPUART_STAT_TDRE_MASK) == 0) spin(1);
}
static inline void hal_uart_write_buf(LPUART_Type *uart, char *buf, size_t len) {
  while (len-- > 0) hal_uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int hal_uart_read_ready(LPUART_Type *uart) {
  (void) uart;
  return uart->STAT & LPUART_STAT_RDRF_MASK;
}
static inline uint8_t hal_uart_read_byte(LPUART_Type *uart) {
  return (uint8_t) (uart->DATA & 255);
}

// TRNG:no info
#ifndef MG_HAL_DISABLE_RANDOM
#define MG_HAL_DISABLE_RANDOM 1
#endif
static inline void hal_rng_init(void) {}
static inline uint32_t hal_rng_read(void) { return 42; }

static inline void hal_lpsr_mux_config(uint16_t index, uint8_t af) {
  IOMUXC_LPSR->SW_MUX_CTL_PAD[index] = af;
}

static inline void hal_lpsr_pad_config(uint16_t index, uint8_t type, uint8_t speed,
                                   uint8_t pull) {
  bool dopull = pull > 0;
  if (dopull) pull = (pull == HAL_GPIO_PULL_UP);
  IOMUXC_LPSR->SW_PAD_CTL_PAD[index] =
      IOMUXC_LPSR_SW_PAD_CTL_PAD_DSE(speed != HAL_GPIO_SPEED_LOW) |
      IOMUXC_LPSR_SW_PAD_CTL_PAD_ODE_LPSR(type) |
      IOMUXC_SW_PAD_CTL_PAD_PUE(dopull) | IOMUXC_LPSR_SW_PAD_CTL_PAD_PUS(pull) |
      IOMUXC_LPSR_SW_PAD_CTL_PAD_SRE(speed >= HAL_GPIO_SPEED_HIGH);
}

// - 15.4 clock tree
// - 15.3 Table 15-2: ENET1_CLK_ROOT <= 50MHz
// - PHY has no xtal, XI driven from ENET_REF_CLK (labeled as ENET_TX_REF_CLK
// (GPIO_DISP_B2_05)), generated by the MCU
// - PHY RST connected to GPIO12.12 (GPIO_LPSR_12);
// - 60.4 REF_CLK is RMII mode reference clock for Rx, Tx, and SMI; it is I/O
static inline void hal_ethernet_init(void) {
  const clock_sys_pll1_config_t pll1 = {.pllDiv2En = true};
  CLOCK_InitSysPll1(&pll1);  // setup PLL1 and clock ENET from it
  // configure clock root source PLL1/2 and divisor /10, enable peripheral clock
  CCM->CLOCK_ROOT[51].CONTROL =
      CCM_CLOCK_ROOT_CONTROL_MUX(4) | CCM_CLOCK_ROOT_CONTROL_DIV(10 - 1);
  hal_clock_periph(112, 1);  // clk_enable_enet (15.5.4 Table 15-5)

  hal_clock_periph(51, 1);  // clk_enable_gpio (15.5.4 Table 15-5)
  hal_clock_periph(50, 1);  // clk_enable_iomuxc_lpsr (15.5.4 Table 15-5)
  hal_clock_periph(49, 1);  // clk_enable_iomuxc (15.5.4 Table 15-5)
  hal_lpsr_mux_config(kIOMUXC_LPSR_SW_MUX_CTL_PAD_GPIO_LPSR_12,
                  10);  // set GPIO12.12 as GPIO (PHY \RST)
  hal_lpsr_pad_config(kIOMUXC_LPSR_SW_PAD_CTL_PAD_GPIO_LPSR_12,
                  HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_MEDIUM, HAL_GPIO_PULL_UP);
  GPIO12->IMR &= ~BIT(12);
  GPIO12->GDIR |= BIT(12);
  GPIO12->DR &= ~BIT(12);  // reset PHY

  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_05,
                  2);  // set for ENET_REF_CLK
  IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_05] |=
      IOMUXC_SW_MUX_CTL_PAD_SION(1);  // loop signal back from pin
  hal_periph_mux_config(kIOMUXC_ENET_IPG_CLK_RMII_SELECT_INPUT,
                    1);  // drive peripheral from DISP_B2_05, so RMII clock is
                         // taken from ENET_REF_CLK
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_05, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE);
  IOMUXC_GPR->GPR4 |=
      IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR(1);  // Set ENET_REF_CLK as output

  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_06,
                  1);  // set for RXDATA0
  hal_periph_mux_config(kIOMUXC_ENET_MAC0_RXDATA_SELECT_INPUT_0,
                    1);  // drive peripheral from DISP_B2_06
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_06, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_DOWN);

  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_07,
                  1);  // set for RXDATA1
  hal_periph_mux_config(kIOMUXC_ENET_MAC0_RXDATA_SELECT_INPUT_1,
                    1);  // drive peripheral from DISP_B2_07
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_07, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_DOWN);

  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_08, 1);  // set for CRS
  hal_periph_mux_config(kIOMUXC_ENET_MAC0_RXEN_SELECT_INPUT,
                    1);  // drive peripheral from DISP_B2_08
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_08, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_DOWN);

  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_09, 1);  // set for RXERR
  hal_periph_mux_config(kIOMUXC_ENET_MAC0_RXERR_SELECT_INPUT,
                    1);  // drive peripheral from DISP_B2_09
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_09, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_DOWN);

  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_02,
                  1);  // set for TXDATA0
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_02, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE);
  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_03,
                  1);  // set for TXDATA1
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_03, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE);
  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_DISP_B2_04, 1);  // set for TXEN
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_DISP_B2_04, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE);

  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_32, 3);  // set for MDC
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_32, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_MEDIUM, HAL_GPIO_PULL_NONE);
  hal_gpio_mux_config(kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_33, 3);  // set for MDIO
  hal_periph_mux_config(kIOMUXC_ENET_MAC0_MDIO_SELECT_INPUT,
                    1);  // drive peripheral from AD_33
  hal_gpio_pad_config(kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_33, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_MEDIUM, HAL_GPIO_PULL_UP);

  IOMUXC_GPR->GPR28 &= ~IOMUXC_GPR_GPR28_CACHE_ENET_MASK;  // ERR050396
  hal_gpio_init(PIN('3', 11), HAL_GPIO_MODE_INPUT, 0, HAL_GPIO_SPEED_MEDIUM,
            HAL_GPIO_PULL_UP);  // setup IRQ (pulled-up)(not used)
  spin(10000);              // keep PHY RST low for a while
  GPIO12->DR |= BIT(12);    // deassert RST

  hal_clock_periph(112, 1);       // clk_enable_enet (15.5.4 Table 15-5)
  NVIC_EnableIRQ(ENET_IRQn);  // Setup Ethernet IRQ handler
}

// Helper macro for MAC generation, byte reads not allowed
#define GENERATE_LOCALLY_ADMINISTERED_MAC()                                    \
  {                                                                            \
    2, OCOTP->FUSEN[16].FUSE & 255, (OCOTP->FUSEN[16].FUSE >> 10) & 255,       \
        ((OCOTP->FUSEN[16].FUSE >> 19) ^ (OCOTP->FUSEN[17].FUSE >> 19)) & 255, \
        (OCOTP->FUSEN[17].FUSE >> 10) & 255, OCOTP->FUSEN[17].FUSE & 255       \
  }

static inline void hal_clock_init(void) {
  // 79.5.2: Disable watchdogS after reset (unlocked)
  RTWDOG3->CS &= ~RTWDOG_CS_EN_MASK;
  RTWDOG3->TOVAL = 0xFFFF;
  while (RTWDOG3->CS & RTWDOG_CS_ULK_MASK) spin(1);  // wait for lock
  while ((RTWDOG3->CS & RTWDOG_CS_RCS_MASK) == 0)
    spin(1);  // wait for new config
  RTWDOG4->CS &= ~RTWDOG_CS_EN_MASK;
  RTWDOG4->TOVAL = 0xFFFF;
  while (RTWDOG4->CS & RTWDOG_CS_ULK_MASK) spin(1);  // wait for lock
  while ((RTWDOG4->CS & RTWDOG_CS_RCS_MASK) == 0)
    spin(1);  // wait for new config
  ANADIG_OSC->OSC_24M_CTRL |=
      ANADIG_OSC_OSC_24M_CTRL_OSC_EN(1) | ANADIG_OSC_OSC_24M_CTRL_LP_EN(1);
  while ((ANADIG_OSC->OSC_24M_CTRL &
          ANADIG_OSC_OSC_24M_CTRL_OSC_24M_STABLE_MASK) == 0)
    spin(1);               // wait until it is stable
  DCDC_BootIntoDCM(DCDC);  // Enable "overdrive" mode as needed
  DCDC_SetVDD1P0BuckModeTargetVoltage(DCDC, kDCDC_1P0BuckTarget1P125V);
  PMU_EnableBodyBias(ANADIG_PMU, kPMU_FBB_CM7,
                     (OCOTP->FUSEN[7].FUSE & BIT(4)) == 0);
  const clock_arm_pll_config_t armpll = {.loopDivider = 166, .postDivider = 0};
  CLOCK_InitArmPll(&armpll);  // Set clock to 996MHz
  CCM->CLOCK_ROOT[0].CONTROL = CCM_CLOCK_ROOT_CONTROL_MUX(4);  // /1
}

#if defined(__MCUXPRESSO)
#undef MG_HAL_DISABLE_SYSTEM_INIT
#define MG_HAL_DISABLE_SYSTEM_INIT 1
#undef MG_HAL_SBRK_NONE
#define MG_HAL_SBRK_NONE 1
#endif

static inline void hal_system_init(void) {
  extern long _siram, _eiram, _siiram;
  for (long *dst = &_siram, *src = &_siiram; dst < &_eiram;) *dst++ = *src++;

  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();
}

static inline void hal_flash_init(void) {  // QSPI in FlexSPI
  // set pins
  hal_clock_periph(49, 1);  // clk_enable_iomuxc (15.5.4 Table 15-5)
}
