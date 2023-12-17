// Copyright (c) 2022-2023 Cesanta Software Limited
// All rights reserved
//
// Datasheet: RM0433, devboard manual: UM2407
// https://www.st.com/resource/en/reference_manual/rm0433-stm32h742-stm32h743753-and-stm32h750-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// Alternate functions: https://www.st.com/resource/en/datasheet/stm32h743vi.pdf

#pragma once

#include <stm32h743xx.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define LED1 PIN('B', 0)   // On-board LED pin (green)
#define LED2 PIN('E', 1)   // On-board LED pin (yellow)
#define LED3 PIN('B', 14)  // On-board LED pin (red)

#define LED LED2  // Use yellow LED for blinking

// System clock (2.1, Figure 1; 8.5, Figure 45; 8.5.5, Figure 47; 8.5.6, Figure
// 49) CPU_FREQUENCY <= 480 MHz; hclk = CPU_FREQUENCY / HPRE ; hclk <= 240 MHz;
// APB clocks <= 120 MHz. D1 domain bus matrix (and so flash) runs at hclk
// frequency. Configure flash latency (WS) in accordance to hclk freq (4.3.8,
// Table 17) The Ethernet controller is in D2 domain and runs at hclk frequency
enum {
  D1CPRE = 1,  // actual divisor value
  HPRE = 2,    // actual divisor value
  D1PPRE = 4,  // register values, divisor value = BIT(value - 3) = / 2
  D2PPRE1 = 4,
  D2PPRE2 = 4,
  D3PPRE = 4
};
// PLL1_P: odd division factors are not allowed (8.7.13) (according to Cube, '1'
// is also an "odd division factor").
// Make sure your chip is revision 'V', otherwise set PLL1_N = 400
enum { PLL1_HSI = 64, PLL1_M = 32, PLL1_N = 480, PLL1_P = 2 };
#define FLASH_LATENCY 0x24  // WRHIGHFREQ LATENCY
#define CPU_FREQUENCY ((PLL1_HSI * PLL1_N / PLL1_M / PLL1_P / D1CPRE) * 1000000)
// #define CPU_FREQUENCY ((PLL1_HSI / D1CPRE) * 1000000)
#define AHB_FREQUENCY (CPU_FREQUENCY / HPRE)
#define APB2_FREQUENCY (AHB_FREQUENCY / (BIT(D2PPRE2 - 3)))
#define APB1_FREQUENCY (AHB_FREQUENCY / (BIT(D2PPRE1 - 3)))

static inline void spin(volatile uint32_t n) {
  while (n--) (void) 0;
}

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_OTYPE_PUSH_PULL, GPIO_OTYPE_OPEN_DRAIN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_INSANE };
enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };

#define GPIO(N) ((GPIO_TypeDef *) (0x40000000 + 0x18020000UL + 0x400 * (N)))

static GPIO_TypeDef *gpio_bank(uint16_t pin) {
  return GPIO(PINBANK(pin));
}
static inline void gpio_toggle(uint16_t pin) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint32_t mask = BIT(PINNO(pin));
  gpio->BSRR = mask << (gpio->ODR & mask ? 16 : 0);
}
static inline int gpio_read(uint16_t pin) {
  return gpio_bank(pin)->IDR & BIT(PINNO(pin)) ? 1 : 0;
}
static inline void gpio_write(uint16_t pin, bool val) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  gpio->BSRR = BIT(PINNO(pin)) << (val ? 0 : 16);
}
static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint8_t n = (uint8_t) (PINNO(pin));
  RCC->AHB4ENR |= BIT(PINBANK(pin));  // Enable GPIO clock
  SETBITS(gpio->OTYPER, 1UL << n, ((uint32_t) type) << n);
  SETBITS(gpio->OSPEEDR, 3UL << (n * 2), ((uint32_t) speed) << (n * 2));
  SETBITS(gpio->PUPDR, 3UL << (n * 2), ((uint32_t) pull) << (n * 2));
  SETBITS(gpio->AFR[n >> 3], 15UL << ((n & 7) * 4),
          ((uint32_t) af) << ((n & 7) * 4));
  SETBITS(gpio->MODER, 3UL << (n * 2), ((uint32_t) mode) << (n * 2));
}
static inline void gpio_input(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_INPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
}
static inline void gpio_output(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
}

#ifndef UART_DEBUG
#define UART_DEBUG USART3
#endif

// D2 Kernel clock (8.7.21) USART1 defaults to pclk2 (APB2), while USART2,3
// default to pclk1 (APB1). Even if using other kernel clocks, the APBx clocks
// must be enabled for CPU access, as the kernel clock drives the BRR, not the
// APB bus interface
static inline void uart_init(USART_TypeDef *uart, unsigned long baud) {
  uint8_t af = 7;           // Alternate function
  uint16_t rx = 0, tx = 0;  // pins
  uint32_t freq = 0;        // Bus frequency. UART1 is on APB2, rest on APB1

  if (uart == USART1) freq = APB2_FREQUENCY, RCC->APB2ENR |= BIT(4);
  if (uart == USART2) freq = APB1_FREQUENCY, RCC->APB1LENR |= BIT(17);
  if (uart == USART3) freq = APB1_FREQUENCY, RCC->APB1LENR |= BIT(18);

  if (uart == USART1) tx = PIN('A', 9), rx = PIN('A', 10);
  if (uart == USART2) tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == USART3) tx = PIN('D', 8), rx = PIN('D', 9);

#if 0  // CONSTANT BAUD RATE FOR REMOTE DEBUGGING WHILE SETTING THE PLL
  SETBITS(RCC->D2CCIP2R, 7 << 3, 3 << 3);  // use HSI for UART1
  freq = 64000000;
#endif

  gpio_init(tx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  gpio_init(rx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  uart->CR1 = 0;                         // Disable this UART
  uart->BRR = freq / baud;               // Set baud rate
  uart->CR1 = BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & BIT(7)) == 0) spin(1);
}
static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int uart_read_ready(USART_TypeDef *uart) {
  return uart->ISR & BIT(5);  // If RXNE bit is set, data is ready
}
static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
  return (uint8_t) (uart->RDR & 255);
}

static inline void rng_init(void) {
  RCC->D2CCIP2R |= RCC_D2CCIP2R_RNGSEL_0;  // RNG clock source pll1_q_ck
  RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;       // Enable RNG clock
  RNG->CR = RNG_CR_RNGEN;                  // Enable RNG
}

static inline uint32_t rng_read(void) {
  while ((RNG->SR & RNG_SR_DRDY) == 0) (void) 0;
  return RNG->DR;
}

static inline char chiprev(void) {
  uint16_t rev = (uint16_t) (((uint32_t) DBGMCU->IDCODE) >> 16);
  if (rev == 0x1003) return 'Y';
  if (rev == 0x2003) return 'V';
  return '?';
}

static inline unsigned int div2prescval(unsigned int div) {
  // 0 --> /1; 8 --> /2 ... 11 --> /16;  12 --> /64 ... 15 --> /512
  if (div == 1) return 0;
  if (div > 16) div /= 2;
  unsigned int val = 7;
  while (div >>= 1) ++val;
  return val;
}

static inline unsigned int pllrge(unsigned int f) {
  unsigned int val = 0;
  while (f >>= 1) ++val;
  return val - 1;
}

static inline void clock_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();
  PWR->CR3 |= BIT(1);                          // select LDO (reset value)
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
  PWR->D3CR |= BIT(15) | BIT(14);              // Select VOS1
  uint32_t f = PWR->D3CR;  // fake read to wait for bus clocking
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
  SYSCFG->PWRCR |= BIT(0);                     // ODEN
  f = SYSCFG->PWRCR;
  while ((PWR->CSR1 & BIT(13)) == 0) spin(1);  // ACTVOSRDY
  (void) f;
  SETBITS(
      RCC->D1CFGR, (0x0F << 8) | (7 << 4) | (0x0F << 0),
      (div2prescval(D1CPRE) << 8) | (D1PPRE << 4) | (div2prescval(HPRE) << 0));
  RCC->D2CFGR = (D2PPRE2 << 8) | (D2PPRE1 << 4);
  RCC->D3CFGR = (D3PPRE << 4);
  SETBITS(RCC->PLLCFGR, 3 << 2,
          pllrge(PLL1_HSI / PLL1_M)
              << 2);  // keep reset config (DIVP1EN, !PLL1VCOSEL), PLL1RGE
  SETBITS(RCC->PLL1DIVR, (0x7F << 9) | (0x1FF << 0),
          ((PLL1_P - 1) << 9) | ((PLL1_N - 1) << 0));  // Set PLL1_P PLL1_N
  SETBITS(RCC->PLLCKSELR, 0x3F << 4,
          PLL1_M << 4);  // Set PLL1_M (source defaults to HSI)
  RCC->CR |= BIT(24);    // Enable PLL1
  while ((RCC->CR & BIT(25)) == 0) spin(1);  // Wait until done
  RCC->CFGR |= (3 << 0);                     // Set clock source to PLL1
  while ((RCC->CFGR & (7 << 3)) != (3 << 3)) spin(1);  // Wait until done
  FLASH->ACR = FLASH_LATENCY;                          // default is larger
}

static inline void ethernet_init(void) {
  // Initialise Ethernet. Enable MAC GPIO pins, see
  // https://www.st.com/resource/en/user_manual/um2407-stm32h7-nucleo144-boards-mb1364-stmicroelectronics.pdf
  uint16_t pins[] = {PIN('A', 1),  PIN('A', 2),  PIN('A', 7),
                     PIN('B', 13), PIN('C', 1),  PIN('C', 4),
                     PIN('C', 5),  PIN('G', 11), PIN('G', 13)};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    gpio_init(pins[i], GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_INSANE,
              GPIO_PULL_NONE, 11);  // 11 is the Ethernet function
  }
  RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;         // Enable SYSCFG
  SETBITS(SYSCFG->PMCR, 7 << 21, 4 << 21);      // Use RMII (12.3.1)
  RCC->AHB1ENR |= BIT(15) | BIT(16) | BIT(17);  // Enable Ethernet clocks
  NVIC_EnableIRQ(ETH_IRQn);                     // Setup Ethernet IRQ handler
}

#define UUID ((uint8_t *) UID_BASE)  // Unique 96-bit chip ID. TRM 61.1

// Helper macro for MAC generation
#define GENERATE_LOCALLY_ADMINISTERED_MAC()                        \
  {                                                                \
    2, UUID[0] ^ UUID[1], UUID[2] ^ UUID[3], UUID[4] ^ UUID[5],    \
        UUID[6] ^ UUID[7] ^ UUID[8], UUID[9] ^ UUID[10] ^ UUID[11] \
  }
