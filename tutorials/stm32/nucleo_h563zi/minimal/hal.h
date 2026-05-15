// Copyright (c) 2022-2026 Cesanta Software Limited
// All rights reserved
//
// Datasheet: RM0481, devboard manual: UM3115
// https://www.st.com/resource/en/reference_manual/rm0481-stm32h563h573-and-stm32h562-armbased-32bit-mcus-stmicroelectronics.pdf
// Alternate functions: https://www.st.com/resource/en/datasheet/stm32h563vi.pdf

#pragma once

#include <stm32h563xx.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hal_init(void);
size_t hal_ram_free(void);
size_t hal_ram_used(void);
bool hal_timer_expired(volatile uint64_t *t, uint64_t prd, uint64_t now);
uint64_t hal_get_tick(void);

#define BIT(x) (1UL << (x))
#define CLRSET(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define HAL_ETH_PINS  \
  PIN('A', 1), /* ETH_REF_CLK */ \
  PIN('A', 2), /* ETH_MDIO */ \
  PIN('A', 7), /* ETH_CRS_DV */ \
  PIN('B', 15), /* ETH_TXD1 */ \
  PIN('C', 1), /* ETH_MDC */ \
  PIN('C', 4), /* ETH_RXD0 */ \
  PIN('C', 5), /* ETH_RXD1 */ \
  PIN('G', 11), /* ETH_TX_EN */ \
  PIN('G', 13) /* ETH_TXD0 */

// System clock (11.4, Figure 48; 11.4.5, Figure 51; 11.4.8
// SYS_FREQUENCY <= 250 MHz; (CLOCK_FREQUENCY / HPRE) ; hclk = SYS_FREQUENCY
// APB clocks <= 250 MHz. Configure flash latency (WS) in accordance to hclk
// freq (7.3.4, Table 37)
enum {
  HAL_HPRE = 7,   // register value, divisor value = BIT(value - 7) = / 1
  HAL_PPRE1 = 4,  // register values, divisor value = BIT(value - 3) = / 2
  HAL_PPRE2 = 4,
  HAL_PPRE3 = 4,
};
// Make sure your chip package uses the internal LDO, otherwise set PLL1_N = 200
enum { PLL1_HSI = 64, PLL1_M = 32, PLL1_N = 250, PLL1_P = 2 };
#define SYS_FREQUENCY \
  ((PLL1_HSI * PLL1_N / PLL1_M / PLL1_P / (BIT(HAL_HPRE - 7))) * 1000000)
#define AHB_FREQUENCY SYS_FREQUENCY
#define APB2_FREQUENCY (AHB_FREQUENCY / (BIT(HAL_PPRE2 - 3)))
#define APB1_FREQUENCY (AHB_FREQUENCY / (BIT(HAL_PPRE1 - 3)))

static inline void spin(volatile uint32_t n) {
  while (n--) (void) 0;
}

enum { HAL_GPIO_MODE_INPUT, HAL_GPIO_MODE_OUTPUT, HAL_GPIO_MODE_AF, HAL_GPIO_MODE_ANALOG };
enum { HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_OTYPE_OPEN_DRAIN };
enum { HAL_GPIO_SPEED_LOW, HAL_GPIO_SPEED_MEDIUM, HAL_GPIO_SPEED_HIGH, HAL_GPIO_SPEED_INSANE };
enum { HAL_GPIO_PULL_NONE, HAL_GPIO_PULL_UP, HAL_GPIO_PULL_DOWN };

#define HAL_GPIO(N) ((GPIO_TypeDef *) ((GPIOA_BASE_NS) + 0x400 * (N)))

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

static inline bool hal_uart_init(USART_TypeDef *uart, uint16_t tx_pin,
                                 uint16_t rx_pin, unsigned long baud) {
  uint32_t freq = 0;        // Bus frequency. UART1 is on APB2, rest on APB1

  if (uart == USART1) {
    freq = APB2_FREQUENCY, RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  } else if (uart == USART2) {
    freq = APB1_FREQUENCY, RCC->APB1LENR |= RCC_APB1LENR_USART2EN;
  } else if (uart == USART3) {
    freq = APB1_FREQUENCY, RCC->APB1LENR |= RCC_APB1LENR_USART3EN;
  } else {
    return false;
  }
  hal_gpio_init(tx_pin, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH, 0, 7U);
  hal_gpio_init(rx_pin, HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_HIGH, 0, 7U);
  uart->CR1 = 0;                            // Disable UART
  uart->BRR = freq / baud;                  // Set baud rate
  uart->CR1 = USART_CR1_RE | USART_CR1_TE;  // Set mode to TX & RX
  uart->CR1 |= USART_CR1_UE;                // Enable UART
  return true;
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
  RCC->CCIPR5 |= RCC_CCIPR5_RNGSEL_0;  // RNG clock source pll1_q_ck
  RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;   // Enable RNG clock
  RNG->CR |= RNG_CR_RNGEN;             // Enable RNG
}
static inline uint32_t hal_rng_read(void) {
  while ((RNG->SR & RNG_SR_DRDY) == 0) spin(1);
  return RNG->DR;
}

static inline bool hal_ldo_is_on(void) {
  return (PWR->SCCR & PWR_SCCR_LDOEN) == PWR_SCCR_LDOEN;
}

// Hw pull-ups on PHY RXD0,1,DV to enable autonegotiation
static inline void hal_ethernet_init(void) {
  // Initialise Ethernet. Enable MAC GPIO pins, see UM3115 section 10.7
  uint16_t pins[] = {HAL_ETH_PINS};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    hal_gpio_init(pins[i], HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_INSANE,
              HAL_GPIO_PULL_NONE, 11);  // 11 is the Ethernet function
  }
  NVIC_EnableIRQ(ETH_IRQn);           // Setup Ethernet IRQ handler
  RCC->APB3ENR |= RCC_APB3ENR_SBSEN;  // Enable SBS clock
  CLRSET(SBS->PMCR, SBS_PMCR_ETH_SEL_PHY, SBS_PMCR_ETH_SEL_PHY_2);  // RMII
  RCC->AHB1ENR |= RCC_AHB1ENR_ETHEN | RCC_AHB1ENR_ETHRXEN | RCC_AHB1ENR_ETHTXEN;
}

#define UUID ((uint32_t *) UID_BASE)  // Unique 96-bit chip ID. TRM 59.1

// Helper macro for MAC generation, byte reads not allowed
#define GENERATE_LOCALLY_ADMINISTERED_MAC()                         \
  {                                                                 \
    2, UUID[0] & 255, (UUID[0] >> 10) & 255, (UUID[0] >> 19) & 255, \
        UUID[1] & 255, UUID[2] & 255                                \
  }

static inline void hal_system_init(void) {
  SCB->CPACR |= ((3UL << 20U) | (3UL << 22U));  // Enable FPU
  __DSB();
  __ISB();
}

static inline void hal_clock_init(void) {
  // Set flash latency. RM0481, section 7.11.1, section 7.3.4 table 37
  CLRSET(FLASH->ACR, (FLASH_ACR_WRHIGHFREQ_Msk | FLASH_ACR_LATENCY_Msk),
         FLASH_ACR_LATENCY_5WS | FLASH_ACR_WRHIGHFREQ_1);

  if (hal_ldo_is_on()) {
    PWR->VOSCR = PWR_VOSCR_VOS_0 | PWR_VOSCR_VOS_1;  // Select VOS0
  } else {
    PWR->VOSCR = PWR_VOSCR_VOS_1;  // Select VOS1
  }
  uint32_t f = PWR->VOSCR;  // fake read to wait for bus clocking
  while ((PWR->VOSSR & PWR_VOSSR_ACTVOSRDY) == 0) spin(1);
  (void) f;
  RCC->CR = RCC_CR_HSION;                          // Clear HSI clock divisor
  while ((RCC->CR & RCC_CR_HSIRDY) == 0) spin(1);  // Wait until done
  RCC->CFGR2 = (HAL_PPRE3 << 12) | (HAL_PPRE2 << 8) | (HAL_PPRE1 << 4) | (HAL_HPRE << 0);
  RCC->PLL1DIVR =
      ((PLL1_P - 1) << 9) | ((PLL1_N - 1) << 0);  // Set PLL1_P PLL1_N
  // Enable P and Q divider outputs; set PLL1_M, select HSI as source,
  // !PLL1VCOSEL, PLL1RGE=0
  RCC->PLL1CFGR =
      RCC_PLL1CFGR_PLL1QEN | RCC_PLL1CFGR_PLL1PEN | (PLL1_M << 8) | (1 << 0);
  RCC->CR |= RCC_CR_PLL1ON;                         // Enable PLL1
  while ((RCC->CR & RCC_CR_PLL1RDY) == 0) spin(1);  // Wait until done
  RCC->CFGR1 |= (3 << 0);                           // Set clock source to PLL1
  while ((RCC->CFGR1 & (7 << 3)) != (3 << 3)) spin(1);  // Wait until done

  SystemCoreClock = SYS_FREQUENCY;
}
