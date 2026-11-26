// Copyright (c) 2022-2026 Cesanta Software Limited
// All rights reserved
//
// RM0399
// https://www.st.com/resource/en/reference_manual/rm0399-stm32h745755-and-stm32h747757-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// https://www.st.com/resource/en/datasheet/stm32h747xi.pdf
// https://docs.arduino.cc/hardware/portenta-h7/

#pragma once

#define LED_1 PIN('K', 5)  // On-board LED pin (red)
#define LED_2 PIN('K', 6)  // On-board LED pin (green)
#define LED_3 PIN('K', 7)  // On-board LED pin (blue)
#define LED LED_2          // Use green LED for blinking

#include <stm32h747xx.h>

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

// System clock (2.1, Fig 1; 8.5, Fig 45; 8.5.5, Fig 47; 8.5.6, Fig 49)
// - This board defaults to work with an SMPS regulator, so:
//   - SYS_FREQUENCY <= 400 MHz; hclk = SYS_FREQUENCY / HPRE ; hclk <= 200 MHz;
//   APB clocks <= 100 MHz.
// - D1 domain bus matrix (and so flash) runs at hclk frequency. Configure flash
// latency (WS) in accordance to hclk freq (4.3.8, Table 17)
enum {
  HAL_D1CPRE = 1,  // actual divisor value
  HAL_HPRE = 2,    // actual divisor value
  HAL_D1PPRE = 4,  // register values, divisor value = BIT(value - 3) = / 2
  HAL_D2PPRE1 = 4,
  HAL_D2PPRE2 = 4,
  HAL_D3PPRE = 4
};
// PLL1_P: odd division factors are not allowed (8.7.13) (according to Cube, '1'
// is also an "odd division factor").
// Make sure your chip is revision 'V', otherwise set PLL1_N = 400
enum { PLL1_HSI = 64, PLL1_M = 32, PLL1_N = 400, PLL1_P = 2, PLL1_Q = 4 };
#define SYS_FREQUENCY \
  ((PLL1_HSI * PLL1_N / PLL1_M / PLL1_P / HAL_D1CPRE) * 1000000)

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

#define HAL_GPIO(N) ((GPIO_TypeDef *) (0x40000000 + 0x18020000UL + 0x400 * (N)))

static inline GPIO_TypeDef *hal_gpio_bank(uint16_t pin) {
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
  RCC->AHB4ENR |= BIT(PINBANK(pin));  // Enable GPIO clock
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
  RCC->D2CCIP2R |= RCC_D2CCIP2R_RNGSEL_0;  // RNG clock source pll1_q_ck
  RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;       // Enable RNG clock
  RNG->CR = RNG_CR_RNGEN;                  // Enable RNG
}

static inline uint32_t hal_rng_read(void) {
  while ((RNG->SR & RNG_SR_DRDY) == 0) (void) 0;
  return RNG->DR;
}

// USB-C: the OTG_HS core with an external ULPI PHY, on these pins, alternate
// function 10. Pins from Arduino's mbed-os, TARGET_PORTENTA_H7
// PeripheralPins.c. Arduino's bootloader uses the same port for DFU, and
// leaves the PHY running. The OTG_HS core's kernel clock: HSI48
static inline void hal_usb_init(void) {
  uint16_t pins[] = {PIN('A', 3),  PIN('A', 5),  PIN('B', 0), PIN('B', 1),
                     PIN('B', 5),  PIN('B', 10), PIN('B', 11), PIN('B', 12),
                     PIN('B', 13), PIN('C', 0),  PIN('H', 4), PIN('I', 11)};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    hal_gpio_init(pins[i], HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_INSANE, HAL_GPIO_PULL_NONE, 10);
  }
  RCC->CR |= RCC_CR_HSI48ON;
  while ((RCC->CR & RCC_CR_HSI48RDY) == 0) spin(1);
  CLRSET(RCC->D2CCIP2R, RCC_D2CCIP2R_USBSEL, RCC_D2CCIP2R_USBSEL);  // HSI48
  RCC->AHB1ENR |= RCC_AHB1ENR_USB1OTGHSEN | RCC_AHB1ENR_USB1OTGHSULPIEN;
  RCC->AHB1RSTR |= RCC_AHB1RSTR_USB1OTGHSRST;  // A clean core: the bootloader
  RCC->AHB1RSTR &= ~RCC_AHB1RSTR_USB1OTGHSRST;  // used it for DFU
}

static inline unsigned int hal_div2prescval(unsigned int div) {
  // 0 --> /1; 8 --> /2 ... 11 --> /16;  12 --> /64 ... 15 --> /512
  if (div == 1) return 0;
  if (div > 16) div /= 2;
  unsigned int val = 7;
  while (div >>= 1) ++val;
  return val;
}

static inline unsigned int hal_pllrge(unsigned int f) {
  unsigned int val = 0;
  while (f >>= 1) ++val;
  return val - 1;
}

// HSE: 25MHz in PH0; LSE: in PC14; both unused
static inline void hal_clock_init(void) {  // Called automatically by startup
  // Set flash latency. RM section 4.9.1, section 4.3.8 table 16
  CLRSET(FLASH->ACR, (FLASH_ACR_WRHIGHFREQ_Msk | FLASH_ACR_LATENCY_Msk),
         FLASH_ACR_LATENCY_2WS | FLASH_ACR_WRHIGHFREQ_1);
  CLRSET(PWR->CR3, BIT(1), BIT(2));  // select SMPS
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
  PWR->D3CR |= BIT(15) | BIT(14);              // Select VOS1
  uint32_t f = PWR->D3CR;  // fake read to wait for bus clocking
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
  (void) f;
  RCC->CFGR &= ~(3 << 0);  // Set clock source to HSI (bootloader...)
  RCC->CR &= ~BIT(24);     // Disable PLL1
  CLRSET(RCC->D1CFGR, (0x0F << 8) | (7 << 4) | (0x0F << 0),
         (hal_div2prescval(HAL_D1CPRE) << 8) | (HAL_D1PPRE << 4) |
             (hal_div2prescval(HAL_HPRE) << 0));
  RCC->D2CFGR = (HAL_D2PPRE2 << 8) | (HAL_D2PPRE1 << 4);
  RCC->D3CFGR = (HAL_D3PPRE << 4);
  // DIVQ1EN DIVP1EN PLL1RGE !PLL1VCOSEL !PLL1FRACEN
  CLRSET(RCC->PLLCFGR, (3 << 2) | BIT(1) | BIT(0),
         BIT(17) | BIT(16) | hal_pllrge(PLL1_HSI / PLL1_M) << 2);
  CLRSET(RCC->PLL1DIVR, (0x7F << 16) | (0x7F << 9) | (0x1FF << 0),
         ((PLL1_Q - 1) << 16) | ((PLL1_P - 1) << 9) | ((PLL1_N - 1) << 0));
  CLRSET(RCC->PLLCKSELR, (0x3F << 4) | (3 << 0), PLL1_M << 4);  // HSI/M
  RCC->CR |= BIT(24);                                            // PLL1 on
  while ((RCC->CR & BIT(25)) == 0) spin(1);   // Wait until done
  RCC->CFGR |= (3 << 0);                      // Set clock source to PLL1
  while ((RCC->CFGR & (7 << 3)) != (3 << 3)) spin(1);  // Wait until done
  RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;                // Enable SYSCFG
}

static inline void hal_system_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  SCB_DisableDCache();  // The bootloader enables caches, disable them
  SCB_DisableICache();
  __DSB();
  __ISB();
}
