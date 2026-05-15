// Copyright (c) 2022-2024 Cesanta Software Limited
// https://www.st.com/resource/en/reference_manual/dm00124865-stm32f75xxx-and-stm32f74xxx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
// https://www.st.com/resource/en/datasheet/stm32f746zg.pdf

#pragma once

#include <stm32f756xx.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define HAL_ETH_PINS                  \
  PIN('A', 1),      /* ETH_REF_CLK */ \
      PIN('A', 2),  /* ETH_MDIO */    \
      PIN('A', 7),  /* ETH_CRS_DV */  \
      PIN('C', 1),  /* ETH_MDC */     \
      PIN('C', 4),  /* ETH_RXD0 */    \
      PIN('C', 5),  /* ETH_RXD1 */    \
      PIN('B', 13), /* ETH_TXD1 */    \
      PIN('G', 11), /* ETH_TX_EN */   \
      PIN('G', 13)  /* ETH_TXD0 */

extern bool hal_timer_expired(volatile uint64_t *t, uint64_t prd, uint64_t now);
extern uint64_t hal_get_tick(void);
extern size_t hal_ram_free(void);
extern size_t hal_ram_used(void);
extern uint32_t SystemCoreClock;

/* System clock
5.3.3: APB1 clock <= 54MHz; APB2 clock <= 108MHz
3.3.2, Table 5: configure flash latency (WS) in accordance to clock freq
38.4: The AHB clock frequency must be at least 25 MHz when the Ethernet
controller is used */
enum {
  HAL_APB1_PRE = 5 /* AHB clock / 4 */,
  HAL_APB2_PRE = 4 /* AHB clock / 2 */
};
enum { PLL_HSI = 16, PLL_M = 8, PLL_N = 216, PLL_P = 2 };  // Run at 216 Mhz
#define FLASH_LATENCY 7
#define SYS_FREQUENCY ((PLL_HSI * PLL_N / PLL_M / PLL_P) * 1000000)
#define APB2_FREQUENCY (SYS_FREQUENCY / (BIT(HAL_APB2_PRE - 3)))
#define APB1_FREQUENCY (SYS_FREQUENCY / (BIT(HAL_APB1_PRE - 3)))

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
#define HAL_GPIO(N) ((GPIO_TypeDef *) (0x40020000 + 0x400 * (N)))

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
  RCC->AHB1ENR |= BIT(PINBANK(pin));  // Enable GPIO clock
  CLRSET(gpio->OTYPER, 1UL << n, ((uint32_t) type) << n);
  CLRSET(gpio->OSPEEDR, 3UL << (n * 2), ((uint32_t) speed) << (n * 2));
  CLRSET(gpio->PUPDR, 3UL << (n * 2), ((uint32_t) pull) << (n * 2));
  CLRSET(gpio->AFR[n >> 3], 15UL << ((n & 7) * 4),
         ((uint32_t) af) << ((n & 7) * 4));
  CLRSET(gpio->MODER, 3UL << (n * 2), ((uint32_t) mode) << (n * 2));
}
static inline void hal_gpio_input(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_INPUT, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE, 0);
}
static inline void hal_gpio_output(uint16_t pin) {
  hal_gpio_init(pin, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, HAL_GPIO_PULL_NONE, 0);
}

static inline void hal_irq_exti_attach(uint16_t pin) {
  uint8_t bank = (uint8_t) (PINBANK(pin)), n = (uint8_t) (PINNO(pin));
  SYSCFG->EXTICR[n / 4] &= ~(15UL << ((n % 4) * 4));
  SYSCFG->EXTICR[n / 4] |= (uint32_t) (bank << ((n % 4) * 4));
  EXTI->IMR |= BIT(n);
  EXTI->RTSR |= BIT(n);
  EXTI->FTSR |= BIT(n);
  int irqvec = n < 5 ? 6 + n : n < 10 ? 23 : 40;  // IRQ vector index, 10.1.2
  NVIC_SetPriority(irqvec, 3);
  NVIC_EnableIRQ(irqvec);
}

static inline void hal_uart_init(USART_TypeDef *uart, unsigned long baud) {
  uint8_t af = 7;           // Alternate function
  uint16_t rx = 0, tx = 0;  // pins
  uint32_t freq = 0;        // Bus frequency. UART1 is on APB2, rest on APB1

  if (uart == USART1) freq = APB2_FREQUENCY, RCC->APB2ENR |= BIT(4);
  if (uart == USART2) freq = APB1_FREQUENCY, RCC->APB1ENR |= BIT(17);
  if (uart == USART3) freq = APB1_FREQUENCY, RCC->APB1ENR |= BIT(18);

  if (uart == USART1) tx = PIN('A', 9), rx = PIN('A', 10);
  if (uart == USART2) tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == USART3) tx = PIN('D', 8), rx = PIN('D', 9);

  hal_gpio_init(tx, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, 0, af);
  hal_gpio_init(rx, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL,
                HAL_GPIO_SPEED_HIGH, 0, af);
  uart->CR1 = 0;                          // Disable this UART
  uart->BRR = freq / baud;                // Set baud rate
  uart->CR1 |= BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
static inline void hal_uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & BIT(7)) == 0) (void) 0;
}
static inline void hal_uart_write_buf(USART_TypeDef *uart, char *buf,
                                      size_t len) {
  while (len-- > 0) hal_uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int hal_uart_read_ready(USART_TypeDef *uart) {
  return uart->ISR & BIT(5);  // If RXNE bit is set, data is ready
}
static inline uint8_t hal_uart_read_byte(USART_TypeDef *uart) {
  return (uint8_t) (uart->RDR & 255);
}

static inline void hal_rng_init(void) {
  RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
  RNG->CR |= RNG_CR_RNGEN;
}
static inline uint32_t hal_rng_read(void) {
  while ((RNG->SR & RNG_SR_DRDY) == 0) (void) 0;
  return RNG->DR;
}

// Hw pull-ups on PHY RXD0,1,DV to enable autonegotiation
static inline void hal_ethernet_init(void) {
  // Initialise Ethernet. Enable MAC GPIO pins, see
  // https://www.farnell.com/datasheets/2014265.pdf section 6.10
  uint16_t pins[] = {HAL_ETH_PINS};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    hal_gpio_init(pins[i], HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL,
                  HAL_GPIO_SPEED_INSANE, HAL_GPIO_PULL_NONE,
                  11);  // 11 is the Ethernet function
  }
  NVIC_EnableIRQ(ETH_IRQn);                // Setup Ethernet IRQ handler
  SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL;  // Use RMII. Goes first!
  RCC->AHB1ENR |=
      RCC_AHB1ENR_ETHMACEN | RCC_AHB1ENR_ETHMACTXEN | RCC_AHB1ENR_ETHMACRXEN;
}

static inline void hal_clock_init(void) {
  FLASH->ACR |= FLASH_LATENCY | BIT(8) | BIT(9);  // Flash latency, prefetch
  RCC->PLLCFGR &= ~((BIT(17) - 1));               // Clear PLL multipliers
  RCC->PLLCFGR |= (((PLL_P - 2) / 2) & 3) << 16;  // Set PLL_P
  RCC->PLLCFGR |= PLL_M | (PLL_N << 6);           // Set PLL_M and PLL_N
  RCC->CR |= BIT(24);                             // Enable PLL
  while ((RCC->CR & BIT(25)) == 0) (void) 0;      // Wait until done
  RCC->CFGR = (HAL_APB1_PRE << 10) | (HAL_APB2_PRE << 13);  // Set prescalers
  RCC->CFGR |= 2;                          // Set clock source to PLL
  while ((RCC->CFGR & 12) == 0) (void) 0;  // Wait until done
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;    // Enable SYSCFG
  SystemCoreClock = SYS_FREQUENCY;         // Update SystemCoreClock global var
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}

static inline void hal_system_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();
}
