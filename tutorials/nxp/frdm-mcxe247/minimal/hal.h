// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

// MCXE24x Series Reference Manual (MCXE24XRM)
// FRDM-MCXE247 Board User Manual: UM12286

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "MCXE247.h"

#include <fsl_clock.h>

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
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

#define SYS_FREQUENCY 80000000UL

enum { HAL_GPIO_MODE_INPUT, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_MODE_AF };
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
  if (gpio == GPIOA) {
    CLOCK_EnableClock(kCLOCK_PortA);
  } else if (gpio == GPIOB) {
    CLOCK_EnableClock(kCLOCK_PortB);
  } else if (gpio == GPIOC) {
    CLOCK_EnableClock(kCLOCK_PortC);
  } else if (gpio == GPIOD) {
    CLOCK_EnableClock(kCLOCK_PortD);
  } else if (gpio == GPIOE) {
    CLOCK_EnableClock(kCLOCK_PortE);
  }
  if (dopull)
    --pull;
  port->PCR[PINNO(pin)] = PORT_PCR_MUX(af) | PORT_PCR_DSE(1) |
                          PORT_PCR_PE(dopull) | PORT_PCR_PS(pull);
  if (mode == HAL_GPIO_MODE_INPUT) {
    gpio->PDDR &= ~mask;
  } else if (mode == HAL_GPIO_MODE_OUTPUT) {
    gpio->PDDR |= mask;
  }

  (void) type, (void) speed;
}

static inline void hal_gpio_input(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_LOW,
            HAL_GPIO_PULL_NONE, 1);
}
static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_LOW,
            HAL_GPIO_PULL_NONE, 1);
}

static inline bool hal_gpio_read(uint16_t pin) {
  GPIO_Type *gpio = hal_gpio_bank(pin);
  return gpio->PDIR & PINNO(pin);
}

static inline void hal_gpio_write(uint16_t pin, bool value) {
  GPIO_Type *gpio = hal_gpio_bank(pin);
  if (value) {
    gpio->PDOR |= (1 << PINNO(pin));
  } else {
    gpio->PDOR &= ~(1 << PINNO(pin));
  }
}

static inline void hal_gpio_toggle(uint16_t pin) {
  GPIO_Type *gpio = hal_gpio_bank(pin);
  uint32_t mask = (uint32_t)BIT(PINNO(pin));
  gpio->PTOR = mask;
}

static inline void hal_uart_init(LPUART_Type *uart, unsigned long baud) {
  uint8_t af = 3;   // Alternate function
  uint16_t pr = 0, pt = 0;  // pins
  uint32_t freq = 8000000; // 8MHz (System Oscillator clock)
  uint32_t lpuart_clk;
  if (uart == LPUART2)
    lpuart_clk = kCLOCK_Lpuart2, pt = PIN('E', 12), pr = PIN('D', 17);
  if (uart == LPUART1)
    lpuart_clk = kCLOCK_Lpuart1, pt = PIN('C', 9), pr = PIN('C', 8);

  CLOCK_SetIpSrc(lpuart_clk, kCLOCK_IpSrcSysOscAsync);
  hal_gpio_init(pt, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_LOW,
            HAL_GPIO_PULL_UP, af);
  hal_gpio_init(pr, HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_LOW,
            HAL_GPIO_PULL_UP, af);
  CLOCK_EnableClock(lpuart_clk);
  uart->GLOBAL |= LPUART_GLOBAL_RST_MASK; // reset, CTRL = 0, defaults
  uart->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;
  // use an oversample ratio of 23
  CLRSET(uart->BAUD,
         LPUART_BAUD_OSR_MASK | LPUART_BAUD_SBR_MASK | LPUART_BAUD_SBNS_MASK,
         LPUART_BAUD_OSR(23 - 1) | LPUART_BAUD_SBR(freq / (23 * baud)));
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

static inline void hal_ethernet_init(void) {
  // enable peripheral clock
  uint32_t reg = (*(volatile uint32_t *)(uint32_t)kCLOCK_Enet);
  reg = (reg & ~(PCC_CLKCFG_PCS_MASK | PCC_CLKCFG_FRAC_MASK | PCC_CLKCFG_PCD_MASK)) | PCC_CLKCFG_PCS(6) |
          PCC_CLKCFG_PCD(1) | PCC_CLKCFG_FRAC(1);
  (*(volatile uint32_t *)(uint32_t)kCLOCK_Enet) = reg & ~PCC_CLKCFG_CGC_MASK;
  (*(volatile uint32_t *)(uint32_t)kCLOCK_Enet) = reg;
  CLOCK_EnableClock(kCLOCK_Enet);

  // init pins
  hal_gpio_init(PIN('C', 3), HAL_GPIO_MODE_OUTPUT, 0, 0, HAL_GPIO_PULL_NONE, 1); // PHY_RST
  hal_gpio_write(PIN('C', 3), 1);

  hal_gpio_init(PIN('B', 4), HAL_GPIO_MODE_AF, 0, 0, HAL_GPIO_PULL_NONE, 5); // MII_RMII_MDIO
  hal_gpio_init(PIN('C', 0), HAL_GPIO_MODE_AF, 0, 0, HAL_GPIO_PULL_NONE, 4); // MII_RMII_RXD1
  hal_gpio_init(PIN('C', 1), HAL_GPIO_MODE_AF, 0, 0, HAL_GPIO_PULL_NONE, 5); // MII_RMII_RXD0
  hal_gpio_init(PIN('C', 17), HAL_GPIO_MODE_AF, 0, 0, HAL_GPIO_PULL_NONE, 5); // MII_RMII_RX_DV
  hal_gpio_init(PIN('C', 2), HAL_GPIO_MODE_AF, 0, 0, HAL_GPIO_PULL_NONE, 5); // MII_RMII_TXD0
  hal_gpio_init(PIN('D', 11), HAL_GPIO_MODE_AF, 0, 0, HAL_GPIO_PULL_NONE, 5); // MII_RMII_TX_CLK
  hal_gpio_init(PIN('D', 12), HAL_GPIO_MODE_AF, 0, 0, HAL_GPIO_PULL_NONE, 5); // MII_RMII_TX_EN
  hal_gpio_init(PIN('D', 7), HAL_GPIO_MODE_AF, 0, 0, HAL_GPIO_PULL_NONE, 5); // MII_RMII_TXD1
  hal_gpio_init(PIN('E', 8), HAL_GPIO_MODE_AF, 0, 0, HAL_GPIO_PULL_NONE, 5); // MII_RMII_MDC

  // reset PHY
  hal_gpio_write(PIN('C', 3), 0);
  spin(50000);
  hal_gpio_write(PIN('C', 3), 1);

  NVIC_EnableIRQ(ENET_Receive_IRQn);
}

static inline void hal_rtc_init(void) {
  // Configure RTC clock to generate seeds
  // Reconfigure it there as needed.
  SIM->LPOCLKS = 0x13; // LPO32K_CLK, LPO32KCLKEN, LPO1KCLKEN
  CLOCK_EnableClock(kCLOCK_Rtc0);
  // If Time Invalid Flag set, reset the RTC
  if (RTC->SR & RTC_SR_TIF_MASK) {
    RTC->CR |= RTC_CR_SWR_MASK;
    RTC->CR &= ~RTC_CR_SWR_MASK;
  }
  RTC->IER &= ~(RTC_IER_TSIC_MASK | RTC_IER_TSIE_MASK);
  RTC->SR |= RTC_SR_TCE_MASK; // Enable timer
}

static inline void hal_clock_init(void) {
  // init SysOsc
  SCG->SOSCCSR = SCG_SOSCCSR_SOSCERR_MASK;
  SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV1(1) | SCG_SOSCDIV_SOSCDIV2(1);
  SCG->SOSCCFG = BIT(2) | SCG_SOSCCFG_RANGE(2); // work mode, range
  SCG->SOSCCSR |= SCG_SOSCCSR_SOSCEN_MASK;
  while ((SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) == 0) spin(1);

  // init FIRC
  SCG->SIRCCSR = 0U;
  SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(0) | SCG_SIRCDIV_SIRCDIV2(2);
  SCG->SIRCCFG = SCG_SIRCCFG_RANGE(1);
  SCG->SIRCCSR = SCG_SIRCCSR_SIRCEN_MASK | BIT(0);
  while ((SCG->SIRCCSR & SCG_SIRCCSR_SIRCVLD_MASK) == 0) spin(1);
  // Change to use SIRC as system clock source
  SCG->RCCR = SCG_RCCR_DIVSLOW(3) | SCG_RCCR_SCS(2);
  while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) != 2) spin(1);
  SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV1(1) | SCG_FIRCDIV_FIRCDIV2(1);
  SCG->FIRCCFG = SCG_FIRCCFG_RANGE(0);
  SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;
  while ((SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) == 0) spin(1);
  SCG->RCCR = SCG_RCCR_DIVSLOW(3) | SCG_RCCR_SCS(3); // Change to FIRC
  while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) != 3) spin(1);

  // init SIRC
  SCG->SIRCCSR = 0U;
  SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(1) | SCG_SIRCDIV_SIRCDIV2(2);
  SCG->SIRCCFG = SCG_SIRCCFG_RANGE(1);
  SCG->SIRCCSR = SCG_SIRCCSR_SIRCEN_MASK | BIT(2);
  while ((SCG->SIRCCSR & SCG_SIRCCSR_SIRCVLD_MASK) == 0) spin(1);

  // init SysPLL
  SCG->SPLLCSR = SCG_SPLLCSR_SPLLERR_MASK;
  SCG->SPLLDIV = SCG_SPLLDIV_SPLLDIV1(2) | SCG_SPLLDIV_SPLLDIV2(0);
  SCG->SPLLCFG = SCG_SPLLCFG_PREDIV(0) | SCG_SPLLCFG_MULT(24);
  SCG->SPLLCSR = (uint32_t)SCG_SPLLCSR_SPLLEN_MASK ;
  while ((SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK) == 0) spin(1);

  // Set SCG to SPLL mode
  SCG->RCCR = SCG_RCCR_DIVSLOW(4) | SCG_RCCR_DIVBUS(1) | SCG_RCCR_DIVCORE(1) | SCG_RCCR_SCS(6);
  while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) != 6) spin(1);
  // Set SCG CLKOUT selection.
  SCG->CLKOUTCNFG = SCG_CLKOUTCNFG_CLKOUTSEL(3);
}

// TRNG is part of ELS, which requires flash partitioning to work
// Use our PRNG when the OS does not provide an alternative
#define MG_HAL_ENABLE_PRNG 1
static inline uint32_t hal_entropy_init(void) {
  hal_rtc_init(); // Initialize RTC
  spin(100000);
  uint32_t seed = RTC->TPR; // get seed
  spin(100000);
  seed |= (RTC->TPR << 16);
  return seed;
}

static inline bool hal_entropy_get(uint32_t *ent) {
  *ent = RTC->TPR;
  return true;
}

#if defined(__MCUXPRESSO)
#undef MG_HAL_DISABLE_SYSTEM_INIT
#define MG_HAL_DISABLE_SYSTEM_INIT 1
#undef MG_HAL_SBRK_NONE
#define MG_HAL_SBRK_NONE 1
#endif

static inline void hal_system_init(void) {
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= (3 << 20) | (3 << 22); // Enable FPU access
#endif
  __DSB();
  __ISB();

  // Disable watchdog
  if ((WDOG->CS & WDOG_CS_CMD32EN_MASK) != 0) {
    WDOG->CNT = WDOG_UPDATE_KEY;
  } else {
    WDOG->CNT = WDOG_UPDATE_KEY & 0xFFFFU;
    WDOG->CNT = (WDOG_UPDATE_KEY >> 16U) & 0xFFFFU;
  }
  WDOG->TOVAL = 0xFFFFU;
  WDOG->CS = (uint32_t) ((WDOG->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;

  LMEM->PCCCR |= LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_INVW0_MASK;
  LMEM->PCCCR |= LMEM_PCCCR_GO_MASK;
  // Wait until the command completes
  while (LMEM->PCCCR & LMEM_PCCCR_GO_MASK) spin(1);
  // Enable cache
  LMEM->PCCCR |= LMEM_PCCCR_ENCACHE_MASK;
    __ISB();

  // Errata ERR050877 workaround. Enable FZ mode in the FPSCR
  __set_FPSCR(__get_FPSCR() | FPSCR_FZ_MASK);

  SYSMPU->CESR &= ~SYSMPU_CESR_VLD(1); // disable MPU
}
