// Copyright (c) 2013-2026 Cesanta Software Limited
// Datasheet: RM0522, devboard manual: UM3616
// https://www.st.com/resource/en/reference_manual/rm0522-stm32c5-series-armbased-32bit-mcus-stmicroelectronics.pdf
// https://www.st.com/resource/en/user_manual/um3616-stm32c5-nucleo144-board-mb2310-stmicroelectronics.pdf

#pragma once

#include <stm32c5xx.h>

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

// Ethernet RMII pins and their alternate functions. UM3616 7.11, Table 9
#define HAL_ETH_PINS                  \
  {PIN('A', 1), 10},  /* REF_CLK */ \
  {PIN('C', 1), 10},  /* MDC */     \
  {PIN('E', 12), 10}, /* MDIO */    \
  {PIN('D', 1), 10},  /* CRS_DV */  \
  {PIN('C', 4), 12},  /* RXD0 */    \
  {PIN('C', 5), 13},  /* RXD1 */    \
  {PIN('G', 11), 10}, /* TX_EN */   \
  {PIN('G', 13), 10}, /* TXD0 */    \
  {PIN('G', 12), 10}  /* TXD1 */

// System clock (9.4.2, 9.4.8). HSI runs at 144 MHz, the maximum system clock:
// SYSCLK = HSIS, no PLL. hclk = SYSCLK, APB clocks = hclk. After reset, the
// system runs on HSIDIV3, 48 MHz. Flash latency for 144 MHz: 6.3.6, Table 20
#define SYS_FREQUENCY 144000000
#define AHB_FREQUENCY SYS_FREQUENCY
#define APB2_FREQUENCY AHB_FREQUENCY
#define APB1_FREQUENCY AHB_FREQUENCY
#define HAL_FLASH_LATENCY (4 | FLASH_ACR_WRHIGHFREQ_1)  // 4 WS, WRHIGHFREQ 2

enum { HAL_GPIO_MODE_INPUT, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_MODE_AF, HAL_GPIO_MODE_ANALOG };
enum { HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_OTYPE_OPEN_DRAIN };
enum { HAL_GPIO_SPEED_LOW, HAL_GPIO_SPEED_MEDIUM, HAL_GPIO_SPEED_HIGH, HAL_GPIO_SPEED_INSANE };
enum { HAL_GPIO_PULL_NONE, HAL_GPIO_PULL_UP, HAL_GPIO_PULL_DOWN };

#define HAL_GPIO(N) ((GPIO_TypeDef *) (GPIOA_BASE + 0x400 * (N)))

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
  RCC->AHB2ENR |= BIT(PINBANK(pin));  // Enable GPIO clock
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

// USART1 kernel clock defaults to pclk2 (APB2), USART2,3 default to pclk1
// (APB1), see RCC_CCIPR1
static inline void hal_uart_init(USART_TypeDef *uart, uint16_t tx,
                                 uint16_t rx, unsigned long baud) {
  uint8_t af = 7;           // Alternate function
  uint32_t freq = 0;        // Bus frequency. UART1 is on APB2, rest on APB1

  if (uart == USART1) freq = APB2_FREQUENCY, RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  if (uart == USART2) freq = APB1_FREQUENCY, RCC->APB1LENR |= RCC_APB1LENR_USART2EN;
  if (uart == USART3) freq = APB1_FREQUENCY, RCC->APB1LENR |= RCC_APB1LENR_USART3EN;

  hal_gpio_init(tx, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH, 0, af);
  hal_gpio_init(rx, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH, 0, af);
  uart->CR1 = 0;                         // Disable this UART
  uart->BRR = freq / baud;               // Set baud rate
  uart->CR1 = BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
static inline void hal_uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & USART_ISR_TXE_TXFNF) == 0) (void) 0;
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

// RNG clock (rng_clk) is CK48, which is disabled after reset. Use HSIDIV3,
// 48 MHz. It stays enabled after the system clock switches to HSIS
static inline void hal_rng_init(void) {
  CLRSET(RCC->CCIPR2, RCC_CCIPR2_CK48SEL_Msk, RCC_CCIPR2_CK48SEL_1);
  RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;  // Enable RNG AHB clock
  RNG->CR |= RNG_CR_RNGEN;            // Enable RNG
}

static inline uint32_t hal_rng_read(void) {
  while ((RNG->SR & RNG_SR_DRDY) == 0) (void) 0;
  return RNG->DR;
}

static inline void hal_ethernet_init(void) {
  // Initialise Ethernet. Enable MAC GPIO pins, see UM3616 Table 9
  uint16_t pins[][2] = {HAL_ETH_PINS};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    hal_gpio_init(pins[i][0], HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_INSANE,
        HAL_GPIO_PULL_NONE, (uint8_t) pins[i][1]);
  }
  NVIC_EnableIRQ(ETH1_IRQn);          // Setup Ethernet IRQ handler
  RCC->APB3ENR |= RCC_APB3ENR_SBSEN;  // Enable SBS clock
  CLRSET(SBS->PMCR, SBS_PMCR_ETH1_SEL_PHY_Msk, SBS_PMCR_ETH1_SEL_PHY_2);  // RMII
  RCC->AHB1ENR |= RCC_AHB1ENR_ETH1CKEN | RCC_AHB1ENR_ETH1EN |
                  RCC_AHB1ENR_ETH1TXEN | RCC_AHB1ENR_ETH1RXEN;
}

static inline void hal_clock_init(void) {
  CLRSET(FLASH->ACR, FLASH_ACR_LATENCY_Msk | FLASH_ACR_WRHIGHFREQ_Msk,
         HAL_FLASH_LATENCY);
  while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) != (HAL_FLASH_LATENCY & 15)) (void) 0;
  RCC->CR1 |= RCC_CR1_HSISON;                        // Enable HSIS, 144 MHz
  while ((RCC->CR1 & RCC_CR1_HSISRDY) == 0) (void) 0;  // Wait until ready
  CLRSET(RCC->CFGR1, RCC_CFGR1_SW_Msk, RCC_CFGR1_SW_0);  // SYSCLK = HSIS
  while ((RCC->CFGR1 & RCC_CFGR1_SWS_Msk) != RCC_CFGR1_SWS_0) (void) 0;
  SystemCoreClock = SYS_FREQUENCY;         // Update SystemCoreClock global var
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}

static inline void hal_system_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();
}
