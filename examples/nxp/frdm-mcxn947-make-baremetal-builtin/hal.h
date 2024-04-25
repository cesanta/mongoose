// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LED1 PIN(0, 0)
#define LED2 LED1
#define LED3 LED1

#ifndef UART_DEBUG
#define UART_DEBUG 0
#endif

#include "MCXN947_cm33_core0.h"

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(bank, num) ((bank << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

void hal_init(void);
size_t hal_ram_free(void);
size_t hal_ram_used(void);

static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

#define SYS_FREQUENCY 48000000UL

//static GPIO_TypeDef *gpio_bank(uint16_t pin) { return GPIO(PINBANK(pin)); }

static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
	(void) pin, (void) mode, (void) type, (void) speed, (void) pull, (void) af;
}

static inline void gpio_input(uint16_t pin) {
	(void) pin;
}

static inline void gpio_output(uint16_t pin) {
	(void) pin;
}

static inline bool gpio_read(uint16_t pin) {
	(void) pin;
    return false;
}

static inline void gpio_write(uint16_t pin, bool value) {
	(void) pin, (void) value;
}

static inline void gpio_toggle(uint16_t pin) {
  gpio_write(pin, !gpio_read(pin));
}

static inline void uart_init(void *uart, unsigned long baud) {
  (void) uart, (void) baud;
}

static inline void uart_write_byte(void *uart, uint8_t byte) {
	(void) uart, (void) byte;
}

static inline void uart_write_buf(void *uart, char *buf, size_t len) {
	while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

static inline void rng_init(void) {
}
static inline uint32_t rng_read(void) {
   return 0;
}

static inline void ethernet_init(void) {
}

static inline void clock_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  asm("DSB");
  asm("ISB");
}
