// Copyright (c) 2022-2026 Cesanta Software Limited
// Datasheet: RM0468, devboard manual: UM2407
// https://www.st.com/resource/en/reference_manual/rm0468-stm32h723733-stm32h725735-and-stm32h730-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// https://www.st.com/resource/en/user_manual/um2407-stm32h7-nucleo144-boards-mb1364-stmicroelectronics.pdf
// Alternate functions: https://www.st.com/resource/en/datasheet/stm32h723.pdf

#pragma once

#include <stm32h723xx.h>

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

#define HAL_ETH_PINS  PIN('A', 1),  PIN('A', 2),  PIN('A', 7), \
                      PIN('B', 13), PIN('C', 1),  PIN('C', 4), \
                      PIN('C', 5),  PIN('G', 11), PIN('G', 13)

// System clock (2.1, Figure 1; 8.5, Figure 45; 8.5.5, Figure 47; 8.5.6, Figure
// 49; 8.5.8 Table 56; datasheet) CPU_FREQUENCY <= 550 MHz; hclk = CPU_FREQUENCY
// / HAL_HPRE ; hclk <= 275 MHz; APB clocks <= 137.5 MHz. D1 domain bus matrix (and
// so flash) runs at hclk frequency. Configure flash latency (WS) in accordance
// to hclk freq (4.3.8, Table 16) The Ethernet controller is in D2 domain and
// runs at hclk frequency
enum {
  HAL_D1CPRE = 1,  // actual divisor value
  HAL_HPRE = 2,    // actual divisor value
  HAL_D1PPRE = 4,  // register values, divisor value = BIT(value - 3) = / 2
  HAL_D2PPRE1 = 4,
  HAL_D2PPRE2 = 4,
  HAL_D3PPRE = 4
};
// PLL1_P: odd division factors are not allowed (8.7.12) (except for '1')
enum { PLL1_HSI = 64, PLL1_M = 32, PLL1_N = 225, PLL1_P = 1 };
#define HAL_FLASH_LATENCY 0x33  // WRHIGHFREQ LATENCY
#define SYS_FREQUENCY ((PLL1_HSI * PLL1_N / PLL1_M / PLL1_P / HAL_D1CPRE) * 1000000)
#define AHB_FREQUENCY (SYS_FREQUENCY / HAL_HPRE)
#define APB2_FREQUENCY (AHB_FREQUENCY / (BIT(HAL_D2PPRE2 - 3)))
#define APB1_FREQUENCY (AHB_FREQUENCY / (BIT(HAL_D2PPRE1 - 3)))

static inline void hal_spin(volatile uint32_t n) {
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

#if 0  // CONSTANT BAUD RATE FOR REMOTE DEBUGGING WHILE SETTING THE PLL
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
  while ((uart->ISR & BIT(7)) == 0) hal_spin(1);
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

// RNG clock (rng_clk) defaults to HSI48 and is characterized at this frequency.
// Enable at SystemInit. (STM32CubeMX) It won't work with a 275MHz (AHB)
static inline void hal_rng_init(void) {
  RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;  // Enable RNG AHB clock (hal_rng_hclk)
  // 39.6.2 Table 320 init
  RNG->CR = RNG_CR_CONDRST | (0x0F << RNG_CR_RNG_CONFIG1_Pos) |
            (0 << RNG_CR_RNG_CONFIG2_Pos) | (0xD << RNG_CR_RNG_CONFIG3_Pos);
  RNG->HTCR = 0x17590abc;
  RNG->HTCR = 0xaa74;
  RNG->CR &= ~RNG_CR_CONDRST ;
  while(RNG->CR & RNG_CR_CONDRST) hal_spin(1); // 39.7.1
  RNG->CR |= RNG_CR_RNGEN;  // Enable RNG
}

static inline uint32_t hal_rng_read(void) {
  while ((RNG->SR & RNG_SR_DRDY) == 0) (void) 0;
  return RNG->DR;
}

// Hw pull-ups on PHY RXD0,1,DV to enable autonegotiation
static inline void hal_ethernet_init(void) {
  // Initialise Ethernet. Enable MAC GPIO pins, see
  // https://www.st.com/resource/en/user_manual/um2407-stm32h7-nucleo144-boards-mb1364-stmicroelectronics.pdf
  uint16_t pins[] = {HAL_ETH_PINS};
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    hal_gpio_init(pins[i], HAL_GPIO_MODE_AF, HAL_GPIO_OTYPE_PUSH_PULL, HAL_GPIO_SPEED_INSANE,
        HAL_GPIO_PULL_NONE, 11);  // 11 is the Ethernet function
  }
  NVIC_EnableIRQ(ETH_IRQn);                     // Setup Ethernet IRQ handler
  CLRSET(SYSCFG->PMCR, 7 << 21, 4 << 21);      // Use RMII (12.3.1)
  RCC->AHB1ENR |= BIT(15) | BIT(16) | BIT(17);  // Enable Ethernet clocks
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

static inline void hal_clock_init(void) {
  PWR->CR3 |= BIT(1);                          // select LDO (reset value)
  while ((PWR->CSR1 & BIT(13)) == 0) hal_spin(1);  // ACTVOSRDY
  PWR->D3CR &= ~(BIT(15) | BIT(14));           // Select VOS0
  while ((PWR->D3CR & BIT(13)) == 0) hal_spin(1);  // VOSRDY
  CLRSET(
      RCC->D1CFGR, (0x0F << 8) | (7 << 4) | (0x0F << 0),
      (hal_div2prescval(HAL_D1CPRE) << 8) | (HAL_D1PPRE << 4) | (hal_div2prescval(HAL_HPRE) << 0));
  RCC->D2CFGR = (HAL_D2PPRE2 << 8) | (HAL_D2PPRE1 << 4);
  RCC->D3CFGR = (HAL_D3PPRE << 4);
  CLRSET(RCC->PLLCFGR, 3 << 2,
          hal_pllrge(PLL1_HSI / PLL1_M)
              << 2);  // keep reset config (DIVP1EN, !PLL1VCOSEL), PLL1RGE
  CLRSET(RCC->PLL1DIVR, (0x7F << 9) | (0x1FF << 0),
          ((PLL1_P - 1) << 9) | ((PLL1_N - 1) << 0));  // Set PLL1_P PLL1_N
  CLRSET(RCC->PLLCKSELR, 0x3F << 4,
          PLL1_M << 4);          // Set PLL1_M (source defaults to HSI)
  RCC->CR |= BIT(24) | BIT(12);  // Enable PLL1 and HSI48 (for RNG)
  while ((RCC->CR & BIT(25)) == 0) hal_spin(1);  // Wait until done
  RCC->CFGR |= (3 << 0);                     // Set clock source to PLL1
  while ((RCC->CFGR & (7 << 3)) != (3 << 3)) hal_spin(1);  // Wait until done
  FLASH->ACR = HAL_FLASH_LATENCY;                          // default is larger
  RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;      // Enable SYSCFG
  while ((RCC->CR & BIT(13)) == 0) hal_spin(1);  // Make sure HSI48 is ready
  SystemCoreClock = SYS_FREQUENCY;         // Update SystemCoreClock global var
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}

static inline void hal_system_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();
}
