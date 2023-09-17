// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
// https://www.nxp.com/webapp/Download?colCode=IMXRT1020RM
// https://cache.nxp.com/secured/assets/documents/en/user-guide/MIMXRT1020EVKHUG.pdf

#pragma once

#include "MIMXRT1021.h"
// #include "drivers/fsl_clock.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define LED PIN('A', 6)  // Use LED for blinking, GPIO_AD_B0_06. RM tbl 9-1

#ifndef UART_DEBUG
#define UART_DEBUG LPUART1
#endif

#define SYS_FREQUENCY 16000000

static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

static inline GPIO_Type *gpio_bank(uint16_t pin) {
  switch (PINBANK(pin)) {
    case 1: return (GPIO_Type *) GPIO1_BASE;
    case 2: return (GPIO_Type *) GPIO2_BASE;
    case 3: return (GPIO_Type *) GPIO3_BASE;
    case 5: return (GPIO_Type *) GPIO5_BASE;
    default: return NULL;
  }
}

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
// enum { GPIO_OTYPE_PUSH_PULL, GPIO_OTYPE_OPEN_DRAIN };
// enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };

#if 0
static inline void CLOCK_ControlGate(clock_ip_name_t name,
                                     clock_gate_value_t value) {
  uint32_t index = ((uint32_t) name) >> 8U;
  uint32_t shift = ((uint32_t) name) & 0x1FU;
  volatile uint32_t *reg;
  assert(index <= 6UL);
  reg = (volatile uint32_t *) ((uint32_t) ((volatile uint32_t *) &CCM->CCGR0) +
                               sizeof(volatile uint32_t *) * index);
  SDK_ATOMIC_LOCAL_CLEAR_AND_SET(reg, (3UL << shift),
                                 (((uint32_t) value) << shift));
}

static inline void CLOCK_EnableClock(clock_ip_name_t name) {
  CLOCK_ControlGate(name, kCLOCK_ClockNeededRunWait);
}
#endif

enum { CLOCK_OFF = 0U, CLOCK_ON_RUN = 1U, CLOCK_ON_RUN_WAIT = 3U };
static inline void clock_periph(uint32_t index, uint32_t shift, uint32_t val) {
  volatile uint32_t *r = &CCM->CCGR0;
  SETBITS(r[index], 3UL << shift, val << shift);
}

static inline void gpio_init(uint16_t pin, uint8_t mode) {
  GPIO_Type *gpio = gpio_bank(pin);
  uint32_t mask = (uint32_t) BIT(PINNO(pin));

  // Enable clock
  switch (PINBANK(pin)) {
    case 1: clock_periph(1, CCM_CCGR1_CG13_SHIFT, CLOCK_ON_RUN_WAIT); break;
    case 2: clock_periph(0, CCM_CCGR0_CG15_SHIFT, CLOCK_ON_RUN_WAIT); break;
    case 3: clock_periph(2, CCM_CCGR2_CG13_SHIFT, CLOCK_ON_RUN_WAIT); break;
    case 5: clock_periph(1, CCM_CCGR1_CG15_SHIFT, CLOCK_ON_RUN_WAIT); break;
    default: break;
  }

  gpio->IMR &= ~mask;
  if (mode == GPIO_MODE_INPUT) {
    gpio->GDIR &= ~mask;
  } else {
    gpio->GDIR |= mask;
  }
}
static inline void gpio_input(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_INPUT);
}
static inline void gpio_output(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_OUTPUT);
}
static inline bool gpio_read(uint16_t pin) {
  GPIO_Type *gpio = gpio_bank(pin);
  uint32_t mask = (uint32_t) BIT(PINNO(pin));
  return gpio->DR & mask;
}
static inline void gpio_write(uint16_t pin, bool value) {
  GPIO_Type *gpio = gpio_bank(pin);
  uint32_t mask = (uint32_t) BIT(PINNO(pin));
  if (value) {
    gpio->DR |= mask;
  } else {
    gpio->DR &= ~mask;
  }
}
static inline void gpio_toggle(uint16_t pin) {
  gpio_write(pin, !gpio_read(pin));
}

static inline void uart_init(LPUART_Type *uart, unsigned long baud) {
  (void) uart, (void) baud;
}
#if 0

static inline void uart_init(LPUART_Type *uart, unsigned long baud) {
  uint8_t af = 7;           // Alternate function
  uint16_t rx = 0, tx = 0;  // pins
  uint32_t freq = 0;        // Bus frequency. UART1 is on APB2, rest on APB1

  if (uart == USART1) freq = APB2_FREQUENCY, RCC->APB2ENR |= BIT(4);
  if (uart == USART2) freq = APB1_FREQUENCY, RCC->APB1ENR |= BIT(17);
  if (uart == USART3) freq = APB1_FREQUENCY, RCC->APB1ENR |= BIT(18);

  if (uart == USART1) tx = PIN('A', 9), rx = PIN('A', 10);
  if (uart == USART2) tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == USART3) tx = PIN('D', 8), rx = PIN('D', 9);

  gpio_init(tx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  gpio_init(rx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  uart->CR1 = 0;                          // Disable this UART
  uart->BRR = freq / baud;                // Set baud rate
  uart->CR1 |= BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
#endif
static inline void uart_write_byte(LPUART_Type *uart, uint8_t byte) {
  // uart->TDR = byte;
  // while ((uart->ISR & BIT(7)) == 0) spin(1);
  (void) uart, (void) byte;
}
static inline void uart_write_buf(LPUART_Type *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int uart_read_ready(LPUART_Type *uart) {
  (void) uart;
  // return uart->ISR & BIT(5);  // If RXNE bit is set, data is ready
  return 0;
}
static inline uint8_t uart_read_byte(LPUART_Type *uart) {
  (void) uart;
  // return (uint8_t) (uart->RDR & 255);
  return 0;
}

static inline void rng_init(void) {
}
static inline uint32_t rng_read(void) {
  return 42;
}
static inline void ethernet_init(void) {
}
