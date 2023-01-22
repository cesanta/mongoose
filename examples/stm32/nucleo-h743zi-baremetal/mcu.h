// Copyright (c) 2022-2023 Cesanta Software Limited
// All rights reserved
//
// Datasheet: RM0433, devboard manual: UM2407
// https://www.st.com/resource/en/reference_manual/rm0433-stm32h742-stm32h743753-and-stm32h750-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// Alternate functions: https://www.st.com/resource/en/datasheet/stm32h743vi.pdf

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

// TODO: Set clock. Now, running @ default of 64 MHz
enum { PLL_HSI = 64, PLL_M = 1, PLL_N = 1, PLL_P = 1 };
#define FLASH_LATENCY 7
#define SYS_FREQUENCY ((PLL_HSI * PLL_N / PLL_M / PLL_P) * 1000000)
#define APB2_FREQUENCY SYS_FREQUENCY
#define APB1_FREQUENCY SYS_FREQUENCY

struct systick {
  volatile uint32_t CTRL, LOAD, VAL, CALIB;
};
#define SYSTICK ((struct systick *) 0xe000e010)
static inline void systick_init(uint32_t ticks) {
  if ((ticks - 1) > 0xffffff) return;  // Systick timer is 24 bit
  SYSTICK->LOAD = ticks - 1;
  SYSTICK->VAL = 0;
  SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2);  // Enable systick
}

struct rcc {
  volatile uint32_t CR, HSICFGR, CRRCR, CSICFGR, CFGR, RESERVED1, D1CFGR,
      D2CFGR, D3CFGR, RESERVED2, PLLCKSELR, PLLCFGR, PLL1DIVR, PLL1FRACR,
      PLL2DIVR, PLL2FRACR, PLL3DIVR, PLL3FRACR, RESERVED3, D1CCIPR, D2CCIP1R,
      D2CCIP2R, D3CCIPR, RESERVED4, CIER, CIFR, CICR, RESERVED5, BDCR, CSR,
      RESERVED6, AHB3RSTR, AHB1RSTR, AHB2RSTR, AHB4RSTR, APB3RSTR, APB1LRSTR,
      APB1HRSTR, APB2RSTR, APB4RSTR, GCR, RESERVED8, D3AMR, RESERVED11[9], RSR,
      AHB3ENR, AHB1ENR, AHB2ENR, AHB4ENR, APB3ENR, APB1LENR, APB1HENR, APB2ENR,
      APB4ENR, RESERVED12, AHB3LPENR, AHB1LPENR, AHB2LPENR, AHB4LPENR,
      APB3LPENR, APB1LLPENR, APB1HLPENR, APB2LPENR, APB4LPENR, RESERVED13[4];
};
#define RCC ((struct rcc *) (0x40000000 + 0x18020000 + 0x4400))

static inline void spin(volatile uint32_t n) {
  while (n--) (void) 0;
}

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_OTYPE_PUSH_PULL, GPIO_OTYPE_OPEN_DRAIN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_INSANE };
enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };

struct gpio {
  volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(N) ((struct gpio *) (0x40000000 + 0x18020000UL + 0x400 * (N)))

static struct gpio *gpio_bank(uint16_t pin) { return GPIO(PINBANK(pin)); }
static inline void gpio_toggle(uint16_t pin) {
  struct gpio *gpio = gpio_bank(pin);
  uint32_t mask = BIT(PINNO(pin));
  gpio->BSRR = mask << (gpio->ODR & mask ? 16 : 0);
}
static inline int gpio_read(uint16_t pin) {
  return gpio_bank(pin)->IDR & BIT(PINNO(pin)) ? 1 : 0;
}
static inline void gpio_write(uint16_t pin, bool val) {
  struct gpio *gpio = gpio_bank(pin);
  gpio->BSRR = BIT(PINNO(pin)) << (val ? 0 : 16);
}
static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
  struct gpio *gpio = gpio_bank(pin);
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

struct uart {
  volatile uint32_t CR1, CR2, CR3, BRR, GTPR, RTOR, RQR, ISR, ICR, RDR, TDR,
      PRESC;
};
#define UART1 ((struct uart *) 0x40011000)
#define UART2 ((struct uart *) 0x40004400)
#define UART3 ((struct uart *) 0x40004800)
#define UART_DEBUG UART1

static inline void uart_init(struct uart *uart, unsigned long baud) {
  uint8_t af = 7;           // Alternate function
  uint16_t rx = 0, tx = 0;  // pins
  uint32_t freq = 0;        // Bus frequency. UART1 is on APB2, rest on APB1

  if (uart == UART1) freq = APB2_FREQUENCY, RCC->APB2ENR |= BIT(4);
  if (uart == UART2) freq = APB1_FREQUENCY, RCC->APB1LENR |= BIT(17);
  if (uart == UART3) freq = APB1_FREQUENCY, RCC->APB1LENR |= BIT(18);

  if (uart == UART1) tx = PIN('A', 9), rx = PIN('A', 10);
  if (uart == UART2) tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == UART3) tx = PIN('D', 8), rx = PIN('D', 9);

  freq = SYS_FREQUENCY;

  gpio_init(tx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  gpio_init(rx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  uart->CR1 = 0;                         // Disable this UART
  uart->BRR = freq / baud;               // Set baud rate
  uart->CR1 = BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
static inline void uart_write_byte(struct uart *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & BIT(7)) == 0) spin(1);
}
static inline void uart_write_buf(struct uart *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int uart_read_ready(struct uart *uart) {
  return uart->ISR & BIT(5);  // If RXNE bit is set, data is ready
}
static inline uint8_t uart_read_byte(struct uart *uart) {
  return (uint8_t) (uart->RDR & 255);
}

static inline void clock_init(void) {
  // TODO: Enable FPU, set flash latency, set clock
}
