// Copyright (c) 2022-2026 Cesanta Software Limited

#pragma once

#include <stm32n657xx.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern bool hal_timer_expired(volatile uint64_t *t, uint64_t prd, uint64_t now);
extern uint64_t hal_get_tick(void);
extern size_t hal_ram_free(void);
extern size_t hal_ram_used(void);
extern uint32_t SystemCoreClock;

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

// UM3417 7.12
#ifndef HAL_ETH_PINS
#define HAL_ETH_PINS                                                 \
  PIN('F', 4), PIN('F', 5), PIN('F', 7), PIN('F', 10), PIN('F', 11), \
      PIN('F', 12), PIN('F', 13), PIN('F', 14), PIN('F', 15), PIN('G', 11),
#endif

#ifndef HAL_SYS_FREQUENCY
#define HAL_SYS_FREQUENCY (600U * 1000U * 1000U)
#endif

#if ((HAL_SYS_FREQUENCY % (8U * 1000U * 1000U)) != 0U)
#error "HAL_SYS_FREQUENCY must be a multiple of 8 MHz"
#endif

#if (HAL_SYS_FREQUENCY == 0U || HAL_SYS_FREQUENCY > (1600U * 1000U * 1000U))
#error "HAL_SYS_FREQUENCY must be between 8 MHz and 1600 MHz"
#endif

enum {
  HAL_HSI_FREQUENCY = 64000000U,
  HAL_PLL1_MAX_FREQUENCY = 1600000000U,
  HAL_PLL1_M = 1U,
  HAL_PLL1_P1 = 1U,
  HAL_PLL1_P2 = 1U,
  HAL_ETH_TARGET_FREQUENCY = 100000000U,
  HAL_XSPI_TARGET_FREQUENCY = 50000000U,
  HAL_HPRE = 4U,
  HAL_PPRE1 = 1U,
  HAL_PPRE2 = 1U,
  HAL_PPRE4 = 1U,
  HAL_PPRE5 = 1U,
};

#define HAL_SYSCLK_IC_DIV (HAL_PLL1_MAX_FREQUENCY / HAL_SYS_FREQUENCY)
#define HAL_PLL1_FREQUENCY (HAL_SYS_FREQUENCY * HAL_SYSCLK_IC_DIV)
#define HAL_PLL1_N (HAL_PLL1_FREQUENCY / HAL_HSI_FREQUENCY)
#define HAL_PLL1_N_FRAC                                                        \
  ((uint32_t) (((((uint64_t) HAL_PLL1_FREQUENCY) % HAL_HSI_FREQUENCY) << 24) / \
               HAL_HSI_FREQUENCY))
#define HAL_AHB_FREQUENCY (HAL_SYS_FREQUENCY / HAL_HPRE)
#define HAL_APB1_FREQUENCY (HAL_AHB_FREQUENCY / HAL_PPRE1)
#define HAL_APB2_FREQUENCY (HAL_AHB_FREQUENCY / HAL_PPRE2)
#define HAL_ETH_IC12_DIV                                  \
  ((HAL_PLL1_FREQUENCY + HAL_ETH_TARGET_FREQUENCY - 1U) / \
   HAL_ETH_TARGET_FREQUENCY)
#define HAL_ETH_KERNEL_FREQUENCY (HAL_PLL1_FREQUENCY / HAL_ETH_IC12_DIV)
#define HAL_XSPI_IC3_DIV                                   \
  ((HAL_PLL1_FREQUENCY + HAL_XSPI_TARGET_FREQUENCY - 1U) / \
   HAL_XSPI_TARGET_FREQUENCY)
#define HAL_XSPI_KERNEL_FREQUENCY (HAL_PLL1_FREQUENCY / HAL_XSPI_IC3_DIV)

enum {
  HAL_GPIO_MODE_INPUT = 0U,
  HAL_GPIO_MODE_OUTPUT = 1U,
  HAL_GPIO_MODE_AF = 2U,
  HAL_GPIO_MODE_ANALOG = 3U,
};
enum {
  HAL_GPIO_OTYPE_PUSH_PULL = 0U,
  HAL_GPIO_OTYPE_OPEN_DRAIN = 1U,
};
enum {
  HAL_GPIO_SPEED_LOW = 0U,
  HAL_GPIO_SPEED_MEDIUM = 1U,
  HAL_GPIO_SPEED_HIGH = 2U,
  HAL_GPIO_SPEED_VERY_HIGH = 3U,
};
enum {
  HAL_GPIO_PULL_NONE = 0U,
  HAL_GPIO_PULL_UP = 1U,
  HAL_GPIO_PULL_DOWN = 2U,
};

static inline GPIO_TypeDef *hal_gpio_bank(uint16_t pin) {
  uint8_t bank = (uint8_t) PINBANK(pin);
  return (GPIO_TypeDef *) (GPIOA_BASE + 0x400 * bank);  // Using secure base
}

static inline void hal_gpio_enable_clock(uint16_t pin) {
  const uint32_t bank = (uint32_t) PINBANK(pin);
  RCC->AHB4ENR |= BIT(bank);
  (void) RCC->AHB4ENR;  // Read back to ensure the write completes before access
}

static inline void hal_gpio_set_secure_nonpriv(uint16_t pin) {
  const uint32_t mask = BIT((uint32_t) PINNO(pin));
  GPIO_TypeDef *gpio = hal_gpio_bank(pin);
  gpio->SECCFGR |= mask;    // Secure
  gpio->PRIVCFGR &= ~mask;  // Clear privileged-only protection
}

static inline void hal_gpio_init(uint16_t pin, uint8_t mode, uint8_t otype,
                                 uint8_t speed, uint8_t pull, uint8_t af) {
  GPIO_TypeDef *gpio = hal_gpio_bank(pin);
  const uint32_t n = (uint32_t) PINNO(pin);
  const uint32_t shift2 = n * 2U;
  const uint32_t shift4 = (n & 7U) * 4U;
  if (gpio == NULL || n > 15U) return;
  hal_gpio_enable_clock(pin);
  CLRSET(gpio->OTYPER, BIT(n), ((uint32_t) (otype & 0x1U)) << n);
  CLRSET(gpio->OSPEEDR, 3UL << shift2, ((uint32_t) (speed & 0x3U)) << shift2);
  CLRSET(gpio->PUPDR, 3UL << shift2, ((uint32_t) (pull & 0x3U)) << shift2);
  if (mode == HAL_GPIO_MODE_AF) {
    CLRSET(gpio->AFR[n >> 3], 0xFUL << shift4,
           ((uint32_t) (af & 0xFU)) << shift4);
  }
  CLRSET(gpio->MODER, 3UL << shift2, ((uint32_t) (mode & 0x3U)) << shift2);
}

static inline void hal_gpio_toggle(uint16_t pin) {
  GPIO_TypeDef *gpio = hal_gpio_bank(pin);
  const uint32_t mask = BIT(PINNO(pin));
  if (gpio == NULL) return;
  gpio->BSRR = mask << ((gpio->ODR & mask) ? 16U : 0U);
}

static inline int hal_gpio_read(uint16_t pin) {
  GPIO_TypeDef *gpio = hal_gpio_bank(pin);
  if (gpio == NULL) return 0;
  return (gpio->IDR & BIT(PINNO(pin))) ? 1 : 0;
}

static inline void hal_gpio_write(uint16_t pin, bool val) {
  GPIO_TypeDef *gpio = hal_gpio_bank(pin);
  if (gpio == NULL) return;
  gpio->BSRR = BIT(PINNO(pin)) << (val ? 0U : 16U);
}

static inline void hal_gpio_input(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE, 0U);
}

static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE, 0U);
}

static inline void hal_gpio_af(uint16_t pin, uint8_t af, uint8_t pull) {
  hal_gpio_init(pin, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, pull, af);
}

static inline void hal_gpio_analog(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_ANALOG, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_LOW, HAL_GPIO_PULL_NONE, 0U);
}

static inline bool hal_uart_init(USART_TypeDef *uart, uint16_t tx, uint16_t rx,
                                 unsigned long baud) {
  uint32_t freq = 0U;
  // uint32_t ready = USART_ISR_TEACK | USART_ISR_REACK;

  if (baud == 0U) return false;

  if (uart == USART1) {
    RCC->APB2ENSR = RCC_APB2ENSR_USART1ENS;
    (void) RCC->APB2ENR;
    CLRSET(RCC->CCIPR13, RCC_CCIPR13_USART1SEL, 0U);
    RCC->APB2RSTSR = RCC_APB2RSTSR_USART1RSTS;
    RCC->APB2RSTCR = RCC_APB2RSTCR_USART1RSTC;
    freq = HAL_APB2_FREQUENCY;
  } else if (uart == USART2) {
    RCC->APB1ENSR1 = RCC_APB1ENSR1_USART2ENS;
    (void) RCC->APB1ENR1;
    CLRSET(RCC->CCIPR13, RCC_CCIPR13_USART2SEL, 0U);
    RCC->APB1RSTSR1 = RCC_APB1RSTSR1_USART2RSTS;
    RCC->APB1RSTCR1 = RCC_APB1RSTCR1_USART2RSTC;
    freq = HAL_APB1_FREQUENCY;
  } else if (uart == USART3) {
    RCC->APB1ENSR1 = RCC_APB1ENSR1_USART3ENS;
    (void) RCC->APB1ENR1;
    CLRSET(RCC->CCIPR13, RCC_CCIPR13_USART3SEL, 0U);
    RCC->APB1RSTSR1 = RCC_APB1RSTSR1_USART3RSTS;
    RCC->APB1RSTCR1 = RCC_APB1RSTCR1_USART3RSTC;
    freq = HAL_APB1_FREQUENCY;
  } else {
    return false;
  }

  hal_gpio_init(tx, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, 0, 7u);
  hal_gpio_init(rx, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, 0, 7u);

  uart->CR1 = uart->CR2 = uart->CR3 = uart->PRESC = 0;
  uart->BRR = (uint32_t) ((freq + baud / 2U) / baud);
  uart->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

  return false;
}

static inline bool hal_uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  uart->TDR = byte;
  for (volatile uint32_t n = 0; n < 999U; n++) {
    if ((uart->ISR & USART_ISR_TXE_TXFNF) != 0U) return true;
  }
  return false;
}

static inline void hal_uart_write_buf(USART_TypeDef *uart, char *buf,
                                      size_t len) {
  while (len-- > 0 && hal_uart_write_byte(uart, *(uint8_t *) buf++)) {
  }
}

static inline int hal_uart_read_ready(USART_TypeDef *uart) {
  return (uart->ISR & USART_ISR_RXNE_RXFNE) ? 1 : 0;
}

static inline uint8_t hal_uart_read_byte(USART_TypeDef *uart) {
  return (uint8_t) (uart->RDR);
}

static inline unsigned int hal_div2prescval(unsigned int div) {
  // STM32N6 encodes AHB/APB prescalers as log2(divider): 0 -> /1, 1 -> /2 ...
  unsigned int val = 0;
  while (div > 1U) {
    div >>= 1U;
    val++;
  }
  return val;
}

static inline bool hal_rng_init(void) {
  RCC->AHB3ENR |= RCC_AHB3ENR_RNGEN;
  RNG->CR &= ~RNG_CR_RNGEN;
  CLRSET(RNG->CR, RNG_CR_CED | RNG_CR_CONDRST, RNG_CR_CONDRST);
  RNG->CR &= ~RNG_CR_CONDRST;
  while (RNG->CR & RNG_CR_CONDRST) (void) 0;
  RNG->CR |= RNG_CR_RNGEN;
  if (RNG->SR & RNG_SR_SEIS) return false;
  while ((RNG->SR & RNG_SR_DRDY) == 0) (void) 0;
  return true;
}

static inline uint32_t hal_rng_read(void) {
  while ((RNG->SR & RNG_SR_DRDY) == 0) (void) 0;
  return RNG->DR;
}

static inline void hal_eth_set_secure(uint32_t cid, uint32_t master_index,
                                      uint32_t slave_index, bool priv) {
  const uint32_t slave_reg = (slave_index >> 28) & 0xFU;
  const uint32_t slave_mask = BIT(slave_index & 0x1FU);

  RIFSC->RIMC_ATTRx[master_index] =
      ((cid & 0x7U) << RIFSC_RIMC_ATTRx_MCID_Pos) | RIFSC_RIMC_ATTRx_MSEC |
      (priv ? RIFSC_RIMC_ATTRx_MPRIV : 0);

  RIFSC->RISC_SECCFGRx[slave_reg] |= slave_mask;
  if (priv) {
    RIFSC->RISC_PRIVCFGRx[slave_reg] |= slave_mask;
  } else {
    RIFSC->RISC_PRIVCFGRx[slave_reg] &= ~slave_mask;
  }
}

static inline void hal_ethernet_init(void) {
  uint16_t pins[] = {HAL_ETH_PINS};

  // Hold ETH1 in reset while clock path is configured
  RCC->AHB5RSTSR = RCC_AHB5RSTSR_ETH1RSTS;
  (void) RCC->AHB5RSTSR;

  // IC12 from the PLL1-based clock tree used by hal_clock_init()
  CLRSET(RCC->IC12CFGR, RCC_IC12CFGR_IC12SEL | RCC_IC12CFGR_IC12INT,
         RCC_IC12CFGR_IC12SEL_1 | ((((uint32_t) (HAL_ETH_IC12_DIV - 1U))
                                    << RCC_IC12CFGR_IC12INT_Pos) &
                                   RCC_IC12CFGR_IC12INT));
  RCC->DIVENSR = RCC_DIVENSR_IC12ENS;
  (void) RCC->DIVENR;

  // Keep ETH1SEL_2, ETH1CLKSEL_1. Clear ETH1REFCLKSEL ETH1GTXCLKSEL
  CLRSET(RCC->CCIPR2,
         RCC_CCIPR2_ETH1CLKSEL | RCC_CCIPR2_ETH1SEL | RCC_CCIPR2_ETH1REFCLKSEL |
             RCC_CCIPR2_ETH1GTXCLKSEL,
         RCC_CCIPR2_ETH1CLKSEL_1 | RCC_CCIPR2_ETH1SEL_2);
  (void) RCC->CCIPR2;

  // Enable ETH1 clocks
  RCC->AHB5ENSR = RCC_AHB5ENSR_ETH1MACENS | RCC_AHB5ENSR_ETH1TXENS |
                  RCC_AHB5ENSR_ETH1RXENS | RCC_AHB5ENSR_ETH1ENS;
  (void) RCC->AHB5ENR;

  // Release reset
  RCC->AHB5RSTCR = RCC_AHB5RSTCR_ETH1RSTC;
  (void) RCC->AHB5RSTSR;

  // Set ETH1 in secure privileged mode
  hal_eth_set_secure(1U, 6U, 0x1000001CU, true);

  // Configure pins in RMII AF
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    hal_gpio_init(pins[i], HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_VERY_HIGH, HAL_GPIO_PULL_NONE, 11);
  }

  NVIC_EnableIRQ(ETH1_IRQn);
}

static inline bool hal_clock_init(uint32_t cpu_freq) {
  uint32_t cpu_divider = 0U;

  // Keep the PLL high enough for the SYS/XSPI tree, then derive CPUCLK from
  // IC1. HAL_SYS_FREQUENCY is restricted to 8 MHz steps so PLL1 DIVNFRAC stays
  // exact.
  if (cpu_freq == 0U || (cpu_freq % 8000000U) != 0U) return false;
  if ((HAL_PLL1_FREQUENCY % cpu_freq) != 0U) return false;
  cpu_divider = HAL_PLL1_FREQUENCY / cpu_freq;
  if (cpu_divider == 0U || cpu_divider > 256U) return false;

  RCC->AHB4ENSR = RCC_AHB4ENSR_PWRENS;
  (void) RCC->AHB4ENR;

  PWR->CR1 &= ~(PWR_CR1_SDEN | PWR_CR1_VDD18SMPSVMEN);
  CLRSET(PWR->VOSCR, PWR_VOSCR_VOS, PWR_VOSCR_VOS);
  for (volatile uint32_t n = 0; n < 1000000U; n++) {
    const uint32_t voscr = PWR->VOSCR;
    const uint32_t requested = (voscr & PWR_VOSCR_VOS) >> PWR_VOSCR_VOS_Pos;
    const uint32_t applied = (voscr & PWR_VOSCR_ACTVOS) >> PWR_VOSCR_ACTVOS_Pos;
    if ((voscr & PWR_VOSCR_VOSRDY) != 0U || requested == applied) break;
  }

  CLRSET(RCC->HSICFGR, RCC_HSICFGR_HSIDIV, 0U);
  RCC->CR |= RCC_CR_HSION;
  while ((RCC->SR & RCC_SR_HSIRDY) == 0U) (void) 0;

  if ((RCC->CFGR1 & RCC_CFGR1_CPUSWS) == RCC_CFGR1_CPUSWS ||
      (RCC->CFGR1 & RCC_CFGR1_SYSSWS) == RCC_CFGR1_SYSSWS) {
    CLRSET(RCC->CFGR1, RCC_CFGR1_CPUSW | RCC_CFGR1_SYSSW, 0U);
    while ((RCC->CFGR1 & RCC_CFGR1_CPUSWS) != 0U) (void) 0;
    while ((RCC->CFGR1 & RCC_CFGR1_SYSSWS) != 0U) (void) 0;
  }

  RCC->CR &= ~RCC_CR_PLL1ON;
  while ((RCC->SR & RCC_SR_PLL1RDY) != 0U) (void) 0;
  CLRSET(
      RCC->PLL1CFGR1,
      RCC_PLL1CFGR1_PLL1SEL | RCC_PLL1CFGR1_PLL1DIVM | RCC_PLL1CFGR1_PLL1DIVN,
      (((uint32_t) HAL_PLL1_M << RCC_PLL1CFGR1_PLL1DIVM_Pos) &
       RCC_PLL1CFGR1_PLL1DIVM) |
          (((uint32_t) HAL_PLL1_N << RCC_PLL1CFGR1_PLL1DIVN_Pos) &
           RCC_PLL1CFGR1_PLL1DIVN));
  RCC->PLL1CFGR3 |= RCC_PLL1CFGR3_PLL1MODSSDIS;
  RCC->PLL1CFGR3 &= ~(RCC_PLL1CFGR3_PLL1MODDSEN | RCC_PLL1CFGR3_PLL1DACEN);
  CLRSET(RCC->PLL1CFGR2, RCC_PLL1CFGR2_PLL1DIVNFRAC,
         ((uint32_t) HAL_PLL1_N_FRAC << RCC_PLL1CFGR2_PLL1DIVNFRAC_Pos) &
             RCC_PLL1CFGR2_PLL1DIVNFRAC);
  CLRSET(RCC->PLL1CFGR3,
         RCC_PLL1CFGR3_PLL1MODSSRST | RCC_PLL1CFGR3_PLL1DACEN |
             RCC_PLL1CFGR3_PLL1MODDSEN | RCC_PLL1CFGR3_PLL1PDIV1 |
             RCC_PLL1CFGR3_PLL1PDIV2 | RCC_PLL1CFGR3_PLL1PDIVEN,
         (((uint32_t) HAL_PLL1_P1 << RCC_PLL1CFGR3_PLL1PDIV1_Pos) &
          RCC_PLL1CFGR3_PLL1PDIV1) |
             (((uint32_t) HAL_PLL1_P2 << RCC_PLL1CFGR3_PLL1PDIV2_Pos) &
              RCC_PLL1CFGR3_PLL1PDIV2) |
             RCC_PLL1CFGR3_PLL1MODSSRST | RCC_PLL1CFGR3_PLL1PDIVEN |
             (HAL_PLL1_N_FRAC != 0U
                  ? RCC_PLL1CFGR3_PLL1MODDSEN | RCC_PLL1CFGR3_PLL1DACEN
                  : 0U));
  RCC->CR |= RCC_CR_PLL1ON;
  while ((RCC->SR & RCC_SR_PLL1RDY) == 0U) (void) 0;

  CLRSET(RCC->CFGR2,
         RCC_CFGR2_HPRE | RCC_CFGR2_PPRE1 | RCC_CFGR2_PPRE2 | RCC_CFGR2_PPRE4 |
             RCC_CFGR2_PPRE5,
         ((uint32_t) hal_div2prescval(HAL_HPRE) << RCC_CFGR2_HPRE_Pos) |
             ((uint32_t) hal_div2prescval(HAL_PPRE1) << RCC_CFGR2_PPRE1_Pos) |
             ((uint32_t) hal_div2prescval(HAL_PPRE2) << RCC_CFGR2_PPRE2_Pos) |
             ((uint32_t) hal_div2prescval(HAL_PPRE4) << RCC_CFGR2_PPRE4_Pos) |
             ((uint32_t) hal_div2prescval(HAL_PPRE5) << RCC_CFGR2_PPRE5_Pos));
  CLRSET(RCC->IC2CFGR, RCC_IC2CFGR_IC2SEL | RCC_IC2CFGR_IC2INT,
         (((uint32_t) (HAL_SYSCLK_IC_DIV - 1U) << RCC_IC2CFGR_IC2INT_Pos) &
          RCC_IC2CFGR_IC2INT));
  CLRSET(RCC->IC1CFGR, RCC_IC1CFGR_IC1SEL | RCC_IC1CFGR_IC1INT,
         (((uint32_t) (cpu_divider - 1U) << RCC_IC1CFGR_IC1INT_Pos) &
          RCC_IC1CFGR_IC1INT));
  CLRSET(RCC->IC6CFGR, RCC_IC6CFGR_IC6SEL | RCC_IC6CFGR_IC6INT,
         (((uint32_t) (HAL_SYSCLK_IC_DIV - 1U) << RCC_IC6CFGR_IC6INT_Pos) &
          RCC_IC6CFGR_IC6INT));
  CLRSET(RCC->IC11CFGR, RCC_IC11CFGR_IC11SEL | RCC_IC11CFGR_IC11INT,
         (((uint32_t) (HAL_SYSCLK_IC_DIV - 1U) << RCC_IC11CFGR_IC11INT_Pos) &
          RCC_IC11CFGR_IC11INT));
  RCC->DIVENSR = RCC_DIVENSR_IC1ENS | RCC_DIVENSR_IC2ENS | RCC_DIVENSR_IC6ENS |
                 RCC_DIVENSR_IC11ENS;

  // Keep the system buses on the Cube-compatible IC2/IC6/IC11 path so XIP
  // timings stay unchanged, and drive only CPUCLK from IC1.
  CLRSET(RCC->CFGR1, RCC_CFGR1_CPUSW | RCC_CFGR1_SYSSW,
         RCC_CFGR1_CPUSW | RCC_CFGR1_SYSSW);
  while ((RCC->CFGR1 & RCC_CFGR1_CPUSWS) != RCC_CFGR1_CPUSWS) (void) 0;
  while ((RCC->CFGR1 & RCC_CFGR1_SYSSWS) != RCC_CFGR1_SYSSWS) (void) 0;

  SystemCoreClock = cpu_freq;
  return true;
}

static inline void hal_system_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();
}
