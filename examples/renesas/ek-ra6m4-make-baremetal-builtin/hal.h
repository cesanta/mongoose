// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved
//
// MCU and ek-ra6m4 eval board datasheets:
// https://www.renesas.com/us/en/document/man/ra6m4-group-user-s-manual-hardware
// https://www.renesas.com/us/en/document/man/ek-ra6m5-v1-users-manual

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "bsp_exceptions.h"

#include "R7FA6M4AF.h"

extern void hal_init(void);

#define LED1 PIN('E', 15)
#define LED2 PIN('E', 4)
#define LED3 PIN('E', 0)
#define UART_DEBUG R_SCI0
#define UART_TX PIN('G', 13)
#define UART_RX PIN('G', 12)

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(port, num) ((((port) - 'A') << 8) | (num))
#define PIN_NUM(pin) (pin & 255)
#define PIN_PORT(pin) (pin >> 8)

// No-op HAL API implementation for a device with GPIO and UART
#define rng_read() 0
#define rng_init()
#define uart_read_ready(uart) 0
#define GENERATE_MAC_ADDRESS() \
  { 2, 3, 4, 5, 6, 7 }

// Clock
#define SYS_FREQUENCY 2000000
static inline void clock_init(void) {
  SCB->CPACR = (uint32_t) 15U << 20;
}

// GPIO
#define GPIO(N) ((R_PORT0_Type *) (R_PORT0_BASE + 0x20 * (N)))
enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF };

static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t af) {
  R_PORT0_Type *gpio = GPIO(PIN_PORT(pin));
  if (mode == GPIO_MODE_OUTPUT) {
    gpio->PCNTR1 |= BIT(PIN_NUM(pin));
  } else if (mode == GPIO_MODE_INPUT) {
    gpio->PCNTR1 &= ~BIT(PIN_NUM(pin));
  } else {
    (void) af;
  }
}

static inline void gpio_output(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_OUTPUT, 0);
}

static inline void gpio_input(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_INPUT, 0);
}

static inline bool gpio_read(uint16_t pin) {
  R_PORT0_Type *gpio = GPIO(PIN_PORT(pin));
  return gpio->PCNTR2 & BIT(PIN_NUM(pin)) ? true : false;
}

static inline void gpio_write(uint16_t pin, bool val) {
  R_PORT0_Type *gpio = GPIO(PIN_PORT(pin));
  gpio->PCNTR3 = BIT(PIN_NUM(pin)) << (val ? 0 : 16U);
}

static inline void gpio_toggle(uint16_t pin) {
  gpio_write(pin, !gpio_read(pin));
}

// UART
static inline void uart_init(R_SCI0_Type *uart, unsigned baud) {
  gpio_output(UART_TX);
  gpio_input(UART_RX);
  (void) uart, (void) baud;
}

static inline void uart_write_byte(void *uart, uint8_t byte) {
  uint16_t pin = UART_TX;
  uint32_t baud = 9600;
  uint32_t interval = SYS_FREQUENCY / baud;  // Time to send 1 bit
  uint8_t bits[] = {
      0,  // Start bit
      (byte >> 0) & 1U,
      (byte >> 1) & 1U,
      (byte >> 2) & 1U,
      (byte >> 3) & 1U,
      (byte >> 4) & 1U,
      (byte >> 5) & 1U,
      (byte >> 6) & 1U,
      (byte >> 7) & 1U,
      1,  // Stop bit
  };
  uint32_t t = SysTick->VAL;  // Current timer value

  for (uint8_t i = 0; i < 10; i++) {
    gpio_write(pin, bits[i]);
    if (t <= interval) {
      while (SysTick->VAL <= t) (void) 0;
      t = SysTick->LOAD + 1 - interval;
      while (SysTick->VAL > t) (void) 0;
    } else {
      t -= interval;
      while (SysTick->VAL > t) (void) 0;
    }
  }
  (void) uart;
}

static inline void uart_write_buf(void *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

// Ethernet
static inline void ethernet_init(void) {
}
