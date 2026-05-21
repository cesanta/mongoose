// Copyright (c) 2022-2023 Cesanta Software Limited
// All rights reserved
//
// RM0399
// https://www.st.com/resource/en/reference_manual/rm0399-stm32h745755-and-stm32h747757-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// https://www.st.com/resource/en/datasheet/stm32h747xi.pdf
// UM2411
// https://www.st.com/resource/en/user_manual/um2411-discovery-kit-with-stm32h747xi-mcu-stmicroelectronics.pdf

#pragma once

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

#define HAL_ETH_PINS  \
  PIN('A', 1), /* ETH_REF_CLK */ \
  PIN('A', 2), /* ETH_MDIO */ \
  PIN('A', 7), /* ETH_CRS_DV */ \
  PIN('C', 1), /* ETH_MDC */ \
  PIN('C', 4), /* ETH_RXD0 */ \
  PIN('C', 5), /* ETH_RXD1 */ \
  PIN('G', 11), /* ETH_TX_EN */ \
  PIN('G', 12), /* ETH_TXD1 */ \
  PIN('G', 13) /* ETH_TXD0 */

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

void hal_init(void);
size_t hal_ram_free(void);
size_t hal_ram_used(void);

// System clock (2.1, Figure 1; 8.5, Figure 45; 8.5.5, Figure 47; 8.5.6, Figure
// 49)
// - This board defaults to work with an SMPS regulator, so:
//   - SYS_FREQUENCY <= 400 MHz; hclk = SYS_FREQUENCY / HPRE ; hclk <= 200 MHz;
//   APB clocks <= 100 MHz.
// - D1 domain bus matrix (and so flash) runs at hclk frequency. Configure flash
// latency (WS) in accordance to hclk freq (4.3.8, Table 17)
// - The Ethernet controller is in D2 domain and runs at hclk frequency
enum {
  HAL_D1CPRE = 1, // actual divisor value
  HAL_HPRE = 2,   // actual divisor value
  HAL_D1PPRE = 4, // register values, divisor value = BIT(value - 3) = / 2
  HAL_D2PPRE1 = 4,
  HAL_D2PPRE2 = 4,
  HAL_D3PPRE = 4
};
// PLL1_P: odd division factors are not allowed (8.7.13) (according to Cube, '1'
// is also an "odd division factor").
// Make sure your chip is revision 'V', otherwise set PLL1_N = 400
enum { PLL1_HSI = 64, PLL1_M = 32, PLL1_N = 400, PLL1_P = 2 };
#define SYS_FREQUENCY ((PLL1_HSI * PLL1_N / PLL1_M / PLL1_P / HAL_D1CPRE) * 1000000)
//#define SYS_FREQUENCY ((PLL1_HSI / HAL_D1CPRE) * 1000000)
//#define SYS_FREQUENCY 64000000
#define AHB_FREQUENCY (SYS_FREQUENCY / HAL_HPRE)
#define APB2_FREQUENCY (AHB_FREQUENCY / (BIT(HAL_D2PPRE2 - 3)))
#define APB1_FREQUENCY (AHB_FREQUENCY / (BIT(HAL_D2PPRE1 - 3)))

static inline void spin(volatile uint32_t n) {
  while (n--) (void) 0;
}

enum { HAL_GPIO_MODE_INPUT, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_MODE_AF, HAL_GPIO_MODE_ANALOG };
enum { HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_OTYPE_OPEN_DRAIN };
enum { HAL_GPIO_SPEED_LOW, HAL_GPIO_SPEED_MEDIUM, HAL_GPIO_SPEED_HIGH, HAL_GPIO_SPEED_INSANE };
enum { HAL_GPIO_PULL_NONE, HAL_GPIO_PULL_UP, HAL_GPIO_PULL_DOWN };

#define HAL_GPIO(N) ((GPIO_TypeDef *) (0x40000000 + 0x18020000UL + 0x400 * (N)))

static GPIO_TypeDef *hal_gpio_bank(uint16_t pin) {
  return HAL_GPIO(PINBANK(pin));
}
static inline void hal_gpio_toggle(uint16_t pin) {
  GPIO_TypeDef *gpio = hal_gpio_bank(pin);
  uint32_t mask = BIT(PINNO(pin));
  gpio->BSRR = mask << (gpio->ODR & mask ? 16 : 0);
}
static inline int hal_gpio_read(uint16_t pin) {
  return hal_gpio_bank(pin)->IDR & BIT(PINNO(pin)) ? 1 : 0;
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
static inline void hal_gpio_input(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_NONE, 0);
}
static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH,
            HAL_GPIO_PULL_NONE, 0);
}

// D2 Kernel clock (8.7.21) USART1 defaults to pclk2 (APB2), while USART2,3
// default to pclk1 (APB1). Even if using other kernel clocks, the APBx clocks
// must be enabled for CPU access, as the kernel clock drives the BRR, not the
// APB bus interface
static inline void hal_uart_init(USART_TypeDef *uart, unsigned long baud) {
  uint8_t af = 7;           // Alternate function
  uint16_t rx = 0, tx = 0;  // pins
  uint32_t freq = 0;        // Bus frequency. UART1 is on APB2, rest on APB1

  if (uart == USART1) freq = APB2_FREQUENCY, RCC->APB2ENR |= BIT(4);
  if (uart == USART2) freq = APB1_FREQUENCY, RCC->APB1LENR |= BIT(17);
  if (uart == USART3) freq = APB1_FREQUENCY, RCC->APB1LENR |= BIT(18);

  if (uart == USART1) tx = PIN('A', 9), rx = PIN('A', 10);
  if (uart == USART2) tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == USART3) tx = PIN('D', 8), rx = PIN('D', 9);

#if 0 // CONSTANT BAUD RATE FOR REMOTE DEBUGGING WHILE SETTING THE PLL
  CLRSET(RCC->D2CCIP2R, 7 << 3, 3 << 3);  // use HSI for UART1
  freq = 64000000;
#endif

  hal_gpio_init(tx, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH, 0, af);
  hal_gpio_init(rx, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH, 0, af);
  uart->CR1 = 0;                         // Disable this UART
  uart->BRR = freq / baud;               // Set baud rate
  uart->CR1 = BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
static inline void hal_uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & BIT(7)) == 0) spin(1);
}
static inline void hal_uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
  while (len-- > 0) hal_uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int hal_uart_read_ready(USART_TypeDef *uart) {
  return uart->ISR & BIT(5);  // If RXNE bit is set, data is ready
}
static inline uint8_t hal_uart_read_byte(USART_TypeDef *uart) {
  return (uint8_t) (uart->RDR & 255);
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

// Hw pull-ups on PHY RXD0,1,DV to enable autonegotiation
static inline void hal_ethernet_init(void) {
  // Initialise Ethernet. Enable MAC GPIO pins, see UM2411
  uint16_t pins[] = {HAL_ETH_PINS};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    hal_gpio_init(pins[i], HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_INSANE,
              HAL_GPIO_PULL_NONE, 11);  // 11 is the Ethernet function
  }
  NVIC_EnableIRQ(ETH_IRQn);                     // Setup Ethernet IRQ handler
  CLRSET(SYSCFG->PMCR, 7 << 21, 4 << 21);      // Use RMII (13.3.1)
  RCC->AHB1ENR |= BIT(15) | BIT(16) | BIT(17);  // Enable Ethernet clocks
}

static inline char hal_chiprev(void) {
  uint16_t rev = (uint16_t) (((uint32_t) DBGMCU->IDCODE) >> 16);
  if (rev == 0x1003) return 'Y';
  if (rev == 0x2003) return 'V';
  return '?';
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

static inline void hal_clock_init(void) {  // Called automatically by startup code
  // Set flash latency. RM section 4.9.1, section 4.3.8 table 16
  CLRSET(FLASH->ACR, (FLASH_ACR_WRHIGHFREQ_Msk | FLASH_ACR_LATENCY_Msk),
         FLASH_ACR_LATENCY_2WS | FLASH_ACR_WRHIGHFREQ_1);
  CLRSET(PWR->CR3, BIT(1), BIT(2));            // select SMPS
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
  PWR->D3CR |= BIT(15) | BIT(14);              // Select VOS1
  uint32_t f = PWR->D3CR;         // fake read to wait for bus clocking
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
#if 0
  SYSCFG->PWRCR |= BIT(0);                     // ODEN for LDO to go 480MHz
  f = SYSCFG->PWRCR;
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
#endif
  (void) f;
  CLRSET(RCC->D1CFGR, (0x0F << 8) | (7 << 4) | (0x0F << 0),
         (hal_div2prescval(HAL_D1CPRE) << 8) | (HAL_D1PPRE << 4) |
             (hal_div2prescval(HAL_HPRE) << 0));
  RCC->D2CFGR = (HAL_D2PPRE2 << 8) | (HAL_D2PPRE1 << 4);
  RCC->D3CFGR = (HAL_D3PPRE << 4);
  CLRSET(RCC->PLLCFGR, 3 << 2,
         hal_pllrge(PLL1_HSI / PLL1_M)
             << 2); // keep reset config (DIVP1EN, !PLL1VCOSEL), PLL1RGE
  CLRSET(RCC->PLL1DIVR, (0x7F << 9) | (0x1FF << 0),
         ((PLL1_P - 1) << 9) | ((PLL1_N - 1) << 0)); // Set PLL1_P PLL1_N
  CLRSET(RCC->PLLCKSELR, 0x3F << 4,
         PLL1_M << 4); // Set PLL1_M (source defaults to HSI)
  RCC->CR |= BIT(24);  // Enable PLL1
  while ((RCC->CR & BIT(25)) == 0) spin(1);  // Wait until done
  RCC->CFGR |= (3 << 0);                     // Set clock source to PLL1
  while ((RCC->CFGR & (7 << 3)) != (3 << 3)) spin(1);  // Wait until done
  RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;                // Enable SYSCFG
}

static inline void hal_system_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();
}
