// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "MCXN947_cm33_core0.h"

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(bank, num) ((bank << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

void hal_init(void);
size_t hal_ram_free(void);
size_t hal_ram_used(void);
bool hal_timer_expired(volatile uint64_t *t, uint64_t period, uint64_t now);
uint64_t hal_get_tick(void);

static inline void spin(volatile uint32_t count) {
  while (count--)
    (void)0;
}

#define SYS_FREQUENCY 150000000UL

enum { HAL_GPIO_MODE_INPUT, HAL_GPIO_MODE_OUTPUT };
enum { HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_OTYPE_OPEN_DRAIN };
enum { HAL_GPIO_SPEED_LOW, HAL_GPIO_SPEED_HIGH };
enum { HAL_GPIO_PULL_NONE, HAL_GPIO_PULL_DOWN, HAL_GPIO_PULL_UP };
static inline GPIO_Type *hal_gpio_bank(uint16_t pin) {
  static GPIO_Type *const g[] = GPIO_BASE_PTRS;
  return g[PINBANK(pin)];
}

static inline void hal_gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
  static PORT_Type *const p[] = PORT_BASE_PTRS;
  PORT_Type *port = p[PINBANK(pin)];
  GPIO_Type *gpio = hal_gpio_bank(pin);
  uint32_t mask = (uint32_t)BIT(PINNO(pin));
  bool dopull = pull > 0;
  if (dopull)
    --pull;
  if (gpio != GPIO5) {
    SYSCON->AHBCLKCTRL0 |=
        (1 << (SYSCON_AHBCLKCTRL0_GPIO0_SHIFT + PINBANK(pin))) |
        (1 << (SYSCON_AHBCLKCTRL0_PORT0_SHIFT + PINBANK(pin)));
  };
  port->PCR[PINNO(pin)] = PORT_PCR_IBE(1) | PORT_PCR_MUX(af) | PORT_PCR_DSE(1) |
                          PORT_PCR_ODE(type) |
                          PORT_PCR_SRE(speed != HAL_GPIO_SPEED_HIGH) |
                          PORT_PCR_PE(dopull) | PORT_PCR_PS(pull);
  gpio->ICR[PINNO(pin)] = GPIO_ICR_ISF_MASK;
  if (mode == HAL_GPIO_MODE_INPUT) {
    gpio->PDDR &= ~mask;
  } else {
    gpio->PDDR |= mask;
  }
}

static inline void hal_gpio_input(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_LOW,
            HAL_GPIO_PULL_NONE, 0);
}
static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_LOW,
            HAL_GPIO_PULL_NONE, 0);
}

static inline bool hal_gpio_read(uint16_t pin) {
  GPIO_Type *gpio = hal_gpio_bank(pin);
  return gpio->PDR[PINNO(pin)];
}

static inline void hal_gpio_write(uint16_t pin, bool value) {
  GPIO_Type *gpio = hal_gpio_bank(pin);
  if (value) {
    gpio->PDR[PINNO(pin)] = 1;
  } else {
    gpio->PDR[PINNO(pin)] = 0;
  }
}

static inline void hal_gpio_toggle(uint16_t pin) {
  GPIO_Type *gpio = hal_gpio_bank(pin);
  uint32_t mask = (uint32_t)BIT(PINNO(pin));
  gpio->PTOR = mask;
}

// MCU-Link UART (P1_9/8; FC4_P1/0)
// Arduino J1_2/4 UART (P4_3/2; FC2_P3/2)
// 33.3.23 LP_FLEXCOMM clocking
// 66.2.4 LP_FLEXCOMM init
// 66.5 LPUART
static inline void hal_uart_init(LPUART_Type *uart, unsigned long baud) {
  static LP_FLEXCOMM_Type *const f[] = LP_FLEXCOMM_BASE_PTRS;
  uint8_t af = 2, fc = 0;   // Alternate function, FlexComm instance
  uint16_t pr = 0, pt = 0;  // pins
  uint32_t freq = 12000000; // fro_12_m
  if (uart == LPUART2)
    fc = 2, pt = PIN(4, 3), pr = PIN(4, 2);
  if (uart == LPUART4)
    fc = 4, pt = PIN(1, 9), pr = PIN(1, 8);

  SYSCON->AHBCLKCTRL1 |= (1 << (SYSCON_AHBCLKCTRL1_FC0_SHIFT + fc));
  SYSCON->PRESETCTRL1 |= (1 << (SYSCON_PRESETCTRL1_FC0_RST_SHIFT + fc));
  SYSCON->PRESETCTRL1 &= ~(1 << (SYSCON_PRESETCTRL1_FC0_RST_SHIFT + fc));
  SYSCON->FCCLKSEL[fc] = SYSCON_FCCLKSEL_SEL(2); // clock from FRO_12M / 1
  SYSCON->FLEXCOMMCLKDIV[fc] = SYSCON_FLEXCOMMXCLKDIV_FLEXCOMMCLKDIV_DIV(0);
  LP_FLEXCOMM_Type *flexcomm = f[fc];
  flexcomm->PSELID = LP_FLEXCOMM_PSELID_PERSEL(1); // configure as UART
  hal_gpio_init(pt, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_LOW,
            HAL_GPIO_PULL_UP, af);
  hal_gpio_init(pr, HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_LOW,
            HAL_GPIO_PULL_UP, af);

  uart->GLOBAL |= LPUART_GLOBAL_RST_MASK; // reset, CTRL = 0, defaults
  uart->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;
  // use a weird oversample ratio of 26x to fit specs, standard 16x won't do
  CLRSET(uart->BAUD,
         LPUART_BAUD_OSR_MASK | LPUART_BAUD_SBR_MASK | LPUART_BAUD_SBNS_MASK,
         LPUART_BAUD_OSR(26 - 1) | LPUART_BAUD_SBR(freq / (26 * baud)));
  CLRSET(uart->CTRL,
         LPUART_CTRL_PE_MASK | LPUART_CTRL_M_MASK | LPUART_CTRL_ILT_MASK |
             LPUART_CTRL_IDLECFG_MASK,
         LPUART_CTRL_IDLECFG(1) | LPUART_CTRL_ILT(1) |
             LPUART_BAUD_SBNS(0)); // no parity, idle 2 chars after 1 stop bit
  uart->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
}

static inline void hal_uart_write_byte(LPUART_Type *uart, uint8_t byte) {
  uart->DATA = byte;
  while ((uart->STAT & LPUART_STAT_TDRE_MASK) == 0)
    spin(1);
}
static inline void hal_uart_write_buf(LPUART_Type *uart, char *buf, size_t len) {
  while (len-- > 0)
    hal_uart_write_byte(uart, *(uint8_t *)buf++);
}

// TRNG is part of ELS and there is no info on it
// Use our PRNG when the OS does not provide an alternative
#define MG_HAL_ENABLE_PRNG 1
static inline uint32_t hal_entropy_init(void) {
  // Use RTC as entropy source
  // Configured at clock_init(), reconfigure it there as needed.
  uint32_t seed;
  // generate a random seed
  seed = RTC0->RTC_TEST2;
  for (uint32_t i = 0; i < 100000; i++) spin(1);
  seed |= RTC0->RTC_TEST2 << 16;
  return seed;
}

static inline bool hal_entropy_get(uint32_t *ent) {
  if (SYSCON->AHBCLKCTRL1 & BIT(6)) { // 17.4.1.14
    // if the RTC is still active, use the subsecond counter as seed
    *ent = RTC0->RTC_TEST2;
    return true;
  }
  return false;
}

// - PHY and MAC clocked via a 50MHz oscillator, P1_4 (ENET0_TXCLK)
// - 33.3.30 ENET clocking
// - SMI clocked from AHB module clock (CSR)
// - PHY RST connected to P5_8
// - PHY RXD0,1,DV = 1 on RST enable autonegotiation, no hw pull-ups
static inline void hal_ethernet_init(void) {
  // '0' in clk_rmii, set for RMII mode
  SYSCON->ENETRMIICLKSEL = SYSCON_ENETRMIICLKSEL_SEL(0);
  SYSCON->ENETRMIICLKDIV = SYSCON_ENETRMIICLKDIV_DIV(0);
  SYSCON->AHBCLKCTRL2 |= SYSCON_AHBCLKCTRL2_ENET_MASK;       // enable bus clk
  SYSCON0->PRESETCTRL2 = SYSCON_PRESETCTRL2_ENET_RST_MASK;   // reset MAC
  SYSCON0->PRESETCTRL2 &= ~SYSCON_PRESETCTRL2_ENET_RST_MASK; // then set RMII
  SYSCON0->ENET_PHY_INTF_SEL |= SYSCON_ENET_PHY_INTF_SEL_PHY_SEL_MASK;
  hal_gpio_init(PIN(5, 8), HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_LOW,
            HAL_GPIO_PULL_NONE, 0); // set P5_8 as GPIO (PHY \RST)
  hal_gpio_write(PIN(5, 8), 0);     // reset PHY
  hal_gpio_init(PIN(1, 4), HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_NONE, 9); // set P1_4 as ENET0_TXCLK
  hal_gpio_init(PIN(1, 5), HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_NONE, 9); // set P1_5 as ENET0_TXEN
  hal_gpio_init(PIN(1, 6), HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_NONE, 9); // set P1_6 as ENET0_TXD0
  hal_gpio_init(PIN(1, 7), HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_NONE, 9); // set P1_7 as ENET0_TXD1
  hal_gpio_init(PIN(1, 13), HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_UP, 9); // set P1_13 as ENET0_RXDV
  hal_gpio_init(PIN(1, 14), HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_UP, 9); // set P1_14 as ENET0_RXD0
  hal_gpio_init(PIN(1, 15), HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_UP, 9); // set P1_15 as ENET0_RXD1
  hal_gpio_init(PIN(1, 20), HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_NONE, 9); // set P1_20 as ENET0_MDC
  hal_gpio_init(PIN(1, 21), HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_NONE, 9);  // set P1_21 as ENET0_MDIO
  spin(10000);                   // keep PHY RST low for a while
  hal_gpio_write(PIN(5, 8), 1);      // deassert RST
  NVIC_EnableIRQ(ETHERNET_IRQn); // Setup Ethernet IRQ handler
}

#include "fsl_clock.h"
#include "fsl_spc.h"

// 33.2 Figure 127 SCG main clock
static inline void hal_clock_init(void) {
  SYSCON->AHBCLKCTRL2 |= SYSCON_AHBCLKCTRL2_SCG_MASK;   // enable SCG clk
  CLRSET(SCG0->RCCR, SCG_CSR_SCS_MASK, SCG_CSR_SCS(2)); // clock main_clock
  spc_active_mode_dcdc_option_t dcdc = {
      .DCDCVoltage = kSPC_DCDC_OverdriveVoltage,
      .DCDCDriveStrength = kSPC_DCDC_NormalDriveStrength};
  SPC_SetActiveModeDCDCRegulatorConfig(SPC0, &dcdc); // Set DCDC to 1.2 V
  spc_active_mode_core_ldo_option_t ldo = {
      .CoreLDOVoltage = kSPC_CoreLDO_OverDriveVoltage,
      .CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength};
  SPC_SetActiveModeCoreLDORegulatorConfig(SPC0, &ldo); // Set LDO_CORE to 1.2 V
  CLRSET(FMU0->FCTRL, FMU_FCTRL_RWSC_MASK, FMU_FCTRL_RWSC(3)); // Set Flash WS
  spc_sram_voltage_config_t sram = {.operateVoltage = kSPC_sramOperateAt1P2V,
                                    .requestVoltageUpdate = true};
  SPC_SetSRAMOperateVoltage(SPC0, &sram); // Set SRAM timing for 1.2V
  CLOCK_SetupFROHFClocking(48000000U);    // Enable FRO HF
  const pll_setup_t pll0 = {.pllctrl = SCG_APLLCTRL_SOURCE(1U) |
                                       SCG_APLLCTRL_SELI(27U) |
                                       SCG_APLLCTRL_SELP(13U),
                            .pllndiv = SCG_APLLNDIV_NDIV(8U),
                            .pllpdiv = SCG_APLLPDIV_PDIV(1U),
                            .pllmdiv = SCG_APLLMDIV_MDIV(50U),
                            .pllRate = 150000000U};
  CLOCK_SetPLL0Freq(&pll0);                             // Setup PLL0 (APLL),
  CLOCK_SetPll0MonitorMode(0);                          // disable monitor mode
  CLRSET(SCG0->RCCR, SCG_CSR_SCS_MASK, SCG_CSR_SCS(5)); // clock main_clock
  SYSCON->AHBCLKDIV = SYSCON_AHBCLKDIV_DIV(0);          // /1
  // We configure the RTC to get rng seeds, reconfigure it here as needed
  CLOCK_SetupClk16KClocking(kCLOCK_Clk16KToVbat | kCLOCK_Clk16KToMain);
  CLOCK_EnableClock(kCLOCK_Rtc0);
}

#if defined(__MCUXPRESSO)
#undef MG_HAL_DISABLE_SYSTEM_INIT
#define MG_HAL_DISABLE_SYSTEM_INIT 1
#undef MG_HAL_SBRK_NONE
#define MG_HAL_SBRK_NONE 1
#endif

static inline void hal_system_init(void) {
  // Init IRAM (MCUXPresso startup handles this, CMSIS startup does not)
  extern long _siram, _eiram, _siiram;
  for (long *dst = &_siram, *src = &_siiram; dst < &_eiram;) *dst++ = *src++;

  SCB->CPACR |=
#if 0
      (3UL << 0 * 2) | (3UL << 1 * 2) |   // Enable PowerQuad (CPO/CP1)
#endif
      (3UL << 10 * 2) | (3UL << 11 * 2); // Enable FPU
  __DSB();
  __ISB();
  SYSCON->LPCAC_CTRL &= ~SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK; // enable LPCAC
// Read TRM 36.1 and decide whether you really want to enable aGDET and dGDET
#if 1
  // Disable aGDET and dGDET
  CLRSET(ITRC0->OUT_SEL[4][0], 0xC0000U, 0x2 << 18);
  CLRSET(ITRC0->OUT_SEL[4][1], 0xC0000U, 0x2 << 18);
  SPC0->ACTIVE_CFG |= SPC_ACTIVE_CFG_GLITCH_DETECT_DISABLE_MASK;
  SPC0->GLITCH_DETECT_SC &= ~SPC_GLITCH_DETECT_SC_LOCK_MASK;
  SPC0->GLITCH_DETECT_SC = 0x3C;
  SPC0->GLITCH_DETECT_SC |= SPC_GLITCH_DETECT_SC_LOCK_MASK;
  CLRSET(ITRC0->OUT_SEL[4][0], 0x3, 0x2);
  CLRSET(ITRC0->OUT_SEL[4][1], 0x3, 0x2);
  GDET0->GDET_ENABLE1 = 0;
  GDET1->GDET_ENABLE1 = 0;
#endif
}
