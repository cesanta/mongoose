// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// RM0456
// https://www.st.com/resource/en/reference_manual/rm0456-stm32u5-series-armbased-32bit-mcus-stmicroelectronics.pdf
// https://www.st.com/resource/en/user_manual/um2861-stm32u5-nucleo144-board-mb1549-stmicroelectronics.pdf

#pragma once

#define LED_1 PIN('C', 7)  // On-board LED pin (green)
#define LED_2 PIN('B', 7)  // On-board LED pin (blue)
#define LED_3 PIN('G', 2)  // On-board LED pin (red)
#define LED LED_1          // Use green LED for blinking

#include <stm32u5a5xx.h>

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

enum {
  HAL_D1PRE = 1,  // actual divisor value
  HAL_HPRE = 1,   // actual divisor value
  HAL_D2PRE = 1,
  HAL_D3PRE = 1,
};
enum { PLL1_M = 1, PLL1_N = 80, PLL1_P = 2, PLL1_Q = 2, PLL1_R = 2 };
#define MSIS_FREQUENCY (4 * 1024 * 1024)  // 4 Mhz
#define SYS_FREQUENCY \
  (MSIS_FREQUENCY * PLL1_N / PLL1_M / PLL1_P / HAL_D1PRE)  // 160MHz
#define AHB_FREQUENCY (SYS_FREQUENCY / HAL_HPRE)
#define APB2_FREQUENCY (AHB_FREQUENCY / HAL_D2PRE)
#define APB1_FREQUENCY (AHB_FREQUENCY / HAL_D1PRE)

static inline void spin(volatile uint32_t n) {
  while (n--) (void) 0;
}

enum {
  HAL_GPIO_MODE_INPUT,
  HAL_GPIO_MODE_OUTPUT,
  HAL_GPIO_MODE_AF,
  HAL_GPIO_MODE_ANALOG
};
enum { HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_OTYPE_OPEN_DRAIN };
enum {
  HAL_GPIO_SPEED_LOW,
  HAL_GPIO_SPEED_MEDIUM,
  HAL_GPIO_SPEED_HIGH,
  HAL_GPIO_SPEED_INSANE
};
enum { HAL_GPIO_PULL_NONE, HAL_GPIO_PULL_UP, HAL_GPIO_PULL_DOWN };

#define HAL_GPIO(N) ((GPIO_TypeDef *) (GPIOA_BASE_NS + 0x400 * (N)))

static GPIO_TypeDef *hal_gpio_bank(uint16_t pin) {
  return HAL_GPIO(PINBANK(pin));
}
static inline void hal_gpio_toggle(uint16_t pin) {
  GPIO_TypeDef *gpio = hal_gpio_bank(pin);
  uint32_t mask = BIT(PINNO(pin));
  gpio->BSRR = mask << (gpio->ODR & mask ? 16 : 0);
}
static inline void hal_gpio_write(uint16_t pin, bool val) {
  GPIO_TypeDef *gpio = hal_gpio_bank(pin);
  gpio->BSRR = BIT(PINNO(pin)) << (val ? 0 : 16);
}
static inline void hal_gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                                 uint8_t speed, uint8_t pull, uint8_t af) {
  GPIO_TypeDef *gpio = hal_gpio_bank(pin);
  uint8_t n = (uint8_t) (PINNO(pin));
  RCC->AHB2ENR1 |= BIT(PINBANK(pin));  // Enable GPIO clock
  CLRSET(gpio->OTYPER, 1UL << n, ((uint32_t) type) << n);
  CLRSET(gpio->OSPEEDR, 3UL << (n * 2), ((uint32_t) speed) << (n * 2));
  CLRSET(gpio->PUPDR, 3UL << (n * 2), ((uint32_t) pull) << (n * 2));
  CLRSET(gpio->AFR[n >> 3], 15UL << ((n & 7) * 4),
         ((uint32_t) af) << ((n & 7) * 4));
  CLRSET(gpio->MODER, 3UL << (n * 2), ((uint32_t) mode) << (n * 2));
}
static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE, 0);
}

static inline void hal_rng_init(void) {
  RCC->CCIPR2 |= RCC_CCIPR2_RNGSEL_0;   // RNG clock source pll1_q_ck
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_RNGEN;  // Enable RNG clock
  RNG->CR = RNG_CR_RNGEN;               // Enable RNG
}

static inline uint32_t hal_rng_read(void) {
  while ((RNG->SR & RNG_SR_DRDY) == 0) (void) 0;
  return RNG->DR;
}

// OTG_HS and its embedded high-speed PHY, as ST's CubeU5 USBX examples for
// this board set them up. The PHY's reference clock is the board's 16 MHz
// HSE. PA11 and PA12 are the PHY's own pins: no GPIO setup. The Mongoose
// driver, usb_dwc2, then runs the core at full speed
static inline void hal_usb_init(void) {
  RCC->CR |= RCC_CR_HSEON;  // 16 MHz HSE: the PHY's reference clock
  while ((RCC->CR & RCC_CR_HSERDY) == 0) spin(1);
  RCC->CCIPR2 &= ~RCC_CCIPR2_USBPHYCSEL;  // USB PHY clock: HSE
  RCC->APB3ENR |= RCC_APB3ENR_SYSCFGEN;
  CLRSET(SYSCFG->OTGHSPHYCR, SYSCFG_OTGHSPHYCR_CLKSEL,
         SYSCFG_OTGHSPHYCR_CLKSEL_0 | SYSCFG_OTGHSPHYCR_CLKSEL_1);  // 16 MHz
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_OTGEN | RCC_AHB2ENR1_USBPHYCEN;
  PWR->SVMCR |= PWR_SVMCR_USV;  // VDDUSB valid
  PWR->VOSR |= PWR_VOSR_USBPWREN | PWR_VOSR_USBBOOSTEN;  // PHY supply
  SYSCFG->OTGHSPHYCR |= SYSCFG_OTGHSPHYCR_EN;            // PHY on
}

static inline unsigned int hal_ahb_div2prescval(unsigned int div) {
  if (div == 1) return 0;
  unsigned int val = 7;
  while (div >>= 1) ++val;
  return val;
}

static inline unsigned int hal_apb_div2prescval(unsigned int div) {
  if (div <= 1) return 0;
  unsigned int val = 0;
  while (div >>= 1) ++val;
  return val + 2;
}

static inline void hal_clock_init(void) {
  FLASH->ACR |= BIT(8);  // prefetch
  RCC->AHB3ENR |= RCC_AHB3ENR_PWREN;
  PWR->SVMCR |= PWR_SVMCR_IO2SV;       // Enable VDDIO2 supply
  PWR->UCPDR |= PWR_UCPDR_UCPD_DBDIS;  // Disable the internal Pull-Up in Dead
                                       // Battery pins of UCPD peripheral
  PWR->CR3 |= PWR_CR3_REGSEL;          // enable SMPS
  while ((PWR->SVMSR & BIT(1)) == 0) spin(1);
  if ((PWR->SVMSR & PWR_SVMSR_ACTVOS) != (PWR_VOSR_VOS_0 | PWR_VOSR_VOS_1)) {
    // control voltage scaling
    PWR->VOSR |= PWR_VOSR_VOS_0 | PWR_VOSR_VOS_1 | PWR_VOSR_BOOSTEN;
    while ((PWR->VOSR & PWR_VOSR_VOSRDY) == 0) spin(1);
    while ((PWR->SVMSR & PWR_SVMSR_ACTVOSRDY) == 0) spin(1);
  }
  PWR->DBPR |= PWR_DBPR_DBP;     // enable backup access
  CLRSET(RCC->BDCR, 3 << 3, 0);  // RCC_LSEDRIVE_LOW
  RCC->BDCR |= RCC_BDCR_LSEON;   // LSE enable
  while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0) spin(1);
  RCC->BDCR |= RCC_BDCR_LSESYSEN;  // LSESYS enable
  while ((RCC->BDCR & RCC_BDCR_LSESYSRDY) == 0) spin(1);
  PWR->VOSR &= ~PWR_VOSR_BOOSTEN;
  RCC->CR &= ~BIT(24);  // configure PLL1
  while ((RCC->CR & RCC_CR_PLL1RDY) != 0) spin(1);
  RCC->PLL1CFGR = BIT(0) | ((PLL1_M - 1) << 8) | BIT(16) | BIT(17) |
                  BIT(18);  // source, PLL1_M, enable PLL1_P, PLL1_Q
  RCC->PLL1DIVR = (PLL1_N - 1) | ((PLL1_P - 1) << 9) | ((PLL1_Q - 1) << 16) |
                  ((PLL1_R - 1) << 24);
  PWR->VOSR |= PWR_VOSR_BOOSTEN;
  RCC->CR |= BIT(24);  // PLL1ON
  while ((RCC->CR & RCC_CR_PLL1RDY) == 0) spin(1);
  CLRSET(FLASH->ACR, 0xf, 4);  // flash latency 4
  RCC->CFGR1 |= 3;             // set PLL as sysclk source
  CLRSET(RCC->CFGR2, 0xf, hal_ahb_div2prescval(HAL_HPRE));          // HPRE
  CLRSET(RCC->CFGR2, 0x70, hal_apb_div2prescval(HAL_D1PRE) << 4);   // D1PRE
  CLRSET(RCC->CFGR2, 0x700, hal_apb_div2prescval(HAL_D2PRE) << 8);  // D2PRE
  CLRSET(RCC->CFGR3, 0x70, hal_apb_div2prescval(HAL_D3PRE) << 4);   // D3PRE
}

static inline void hal_system_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();

  SCB->VTOR = FLASH_BASE;  // Vector Table Relocation in Internal FLASH
}
