// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#pragma once

#define UART_DEBUG USART1

#define BTN_PIN PIN('B', 3)    // On-board user button
#define LED1_PIN PIN('A', 15)  // On-board red LED
#define LED2_PIN PIN('B', 4)   // On-board blue LED
#define LED_PIN LED2_PIN

#include <ch32v30x.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

extern uint32_t SystemCoreClock;

void hal_init(void);
size_t hal_ram_free(void);
size_t hal_ram_used(void);

static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

enum {
  GPIO_MODE_INPUT,
  GPIO_MODE_OUTPUT_10M,
  GPIO_MODE_OUTPUT_2M,
  GPIO_MODE_OUTPUT_50M
};
enum { GPIO_OTYPE_PP, GPIO_OTYPE_OD, GPIO_OTYPE_AF_PP, GPIO_AF_OD };
enum { GPIO_ITYPE_ANALOG, GPIO_ITYPE_FLOAT, GPIO_ITYPE_PUPD };
#define GPIO(N) ((GPIO_TypeDef *) (size_t) (GPIOA_BASE + 0x400 * (N)))

static GPIO_TypeDef *gpio_bank(uint16_t pin) {
  return GPIO(PINBANK(pin));
}
static inline void gpio_toggle(uint16_t pin) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint32_t mask = BIT(PINNO(pin));
  gpio->BSHR = mask << (gpio->OUTDR & mask ? 16 : 0);
}
static inline bool gpio_read(uint16_t pin) {
  return gpio_bank(pin)->INDR & BIT(PINNO(pin)) ? true : false;
}
static inline void gpio_write(uint16_t pin, bool val) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  gpio->BSHR = BIT(PINNO(pin)) << (val ? 0 : 16);
}
static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t cfg) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint8_t bank = (uint8_t) PINBANK(pin), no = (uint8_t) PINNO(pin);
  RCC->APB2PCENR |= BIT(bank + 2);  // Enable GPIO clock, section 3.4.7
  if (mode != GPIO_MODE_INPUT && cfg == GPIO_OTYPE_AF_PP) {
    RCC->APB2PCENR |= BIT(0);  // Enable AFIO
  }
  volatile uint32_t *r = &gpio->CFGLR;
  if (no > 7) {
    r = &gpio->CFGHR;
    no = (uint8_t) (no - 8);
  }
  uint8_t v = (uint8_t) ((mode & 3U) | ((cfg & 3U) << 2));
  CLRSET(*r, 15U << (no * 4), v << (no * 4));
}
static inline void gpio_input(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_INPUT, GPIO_ITYPE_PUPD);
}
static inline void gpio_output(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_OUTPUT_50M, GPIO_OTYPE_PP);
}

static inline void uart_init(USART_TypeDef *uart, unsigned baud) {
  uint16_t rx = 0, tx = 0;  // pins
  uint32_t freq = 0;        // Bus frequency. UART1 is on APB2, rest on APB1

  if (uart == USART1) freq = SystemCoreClock, RCC->APB2PCENR |= BIT(14);
  if (uart == USART2) freq = SystemCoreClock, RCC->APB1PCENR |= BIT(17);
  if (uart == USART3) freq = SystemCoreClock, RCC->APB1PCENR |= BIT(18);

  if (uart == USART1) tx = PIN('A', 9), rx = PIN('A', 10);
  // if (uart == USART1) tx = PIN('B', 6), rx = PIN('B', 7);
  if (uart == USART2) tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == USART3) tx = PIN('B', 10), rx = PIN('B', 11);

  gpio_init(tx, GPIO_MODE_OUTPUT_50M, GPIO_OTYPE_AF_PP);
  gpio_init(rx, GPIO_MODE_INPUT, GPIO_ITYPE_PUPD);
  uart->CTLR1 = 0;                        // Disable this UART
  unsigned div = freq / baud * 100 / 16;  // 18.3
  uart->BRR = (uint16_t) (((div / 100) << 4) | (div * 16 / 100));
  uart->CTLR1 = BIT(13) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  uart->DATAR = byte;
  volatile int timeout = 999;
  while ((uart->STATR & BIT(7)) == 0 && timeout--) spin(1);
}
static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int uart_read_ready(USART_TypeDef *uart) {
  return uart->STATR & BIT(5);  // If RXNE bit is set, data is ready
}
static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
  return (uint8_t) (uart->DATAR & 255U);
}

static inline void ethernet_init(void) {
  RCC->CTLR &= ~BIT(28);                         // PLL3 off
  CLRSET(RCC->CFGR2, 15U << 4, 1U << 4);         // 3.4.12: PREDIV2 = 2
  CLRSET(RCC->CFGR2, 15U << 12, 13U << 12);      // 3.4.12: PLL3MUL = 15
  RCC->CTLR |= BIT(28);                          // PLL3 on
  EXTEN->EXTEN_CTR |= EXTEN_ETH_10M_EN;          // Enable built-in 10M PHY
  RCC->AHBPCENR |= BIT(14) | BIT(15) | BIT(16);  // Enable MAC, TX, RX
  NVIC_EnableIRQ(ETH_IRQn);                      // Enable Ethernet interrupt
}

// opt: 0: 128/192, 1: 96/224, 2: 64/256, 3: 32/288
static inline void set_ram_size(uint8_t opt) {
  // Unlock flash option byte, RM 32.6.1
  FLASH->KEYR = 0x45670123;
  FLASH->KEYR = 0xcdef89ab;
  FLASH->OBKEYR = 0x45670123;
  FLASH->OBKEYR = 0xcdef89ab;
  FLASH->CTLR |= 1U << 4;  // Enable options byte programming
  unsigned val = *(uint16_t *) 0x1ffff802;
  val &= ~(3U << 6);
  val |= ((opt & 3U)) << 6;
  FLASH->CTLR |= FLASH_CTLR_PG;               // Set programming bit
  *(uint16_t *) 0x1ffff802 = (uint16_t) val;  // Write half-word
  spin(9999);
  FLASH->CTLR &= ~(1U << 4);
}

static inline void rng_init(void) {
  RNG->CR |= RNG_CR_RNGEN;
}
static inline uint32_t rng_read(void) {
  return RNG->DR;
}

// Helper macro for MAC generation
#define ROM_MAC ((uint8_t *) (0X1ffff7e8 + 5))
#define GENERATE_LOCALLY_ADMINISTERED_MAC() \
  { 2, ROM_MAC[0], ROM_MAC[1], ROM_MAC[2], ROM_MAC[3], ROM_MAC[4] }
