// Copyright (c) 2022-2023 Cesanta Software Limited
// All rights reserved
//
// MCU manual: RM0444, board manual: UM2591
// https://www.st.com/resource/en/reference_manual/rm0444-stm32g0x1-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// https://www.st.com/resource/en/user_manual/um2591-stm32g0-nucleo32-board-mb1455-stmicroelectronics.pdf
// Alternate functions: https://www.st.com/resource/en/datasheet/stm32g031c6.pdf

#pragma once

// #define LED PIN('B', 3)
#define LED PIN('C', 6)
#ifndef UART_DEBUG
#define UART_DEBUG USART2
#endif

#include <stm32g031xx.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define CPU_FREQUENCY 16000000
#define AHB_FREQUENCY CPU_FREQUENCY
#define APB_FREQUENCY CPU_FREQUENCY
// #define APB1_FREQUENCY (AHB_FREQUENCY / (BIT(PPRE1 - 3)))

static inline void spin(volatile uint32_t n) {
  while (n--) (void) 0;
}

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_OTYPE_PUSH_PULL, GPIO_OTYPE_OPEN_DRAIN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_INSANE };
enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };

#define GPIO(N) ((GPIO_TypeDef *) ((GPIOA_BASE) + 0x400 * (N)))

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
  RCC->IOPENR |= BIT(PINBANK(pin));  // Enable GPIO clock
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

static inline bool uart_init(USART_TypeDef *uart, unsigned long baud) {
  uint8_t af = 1;           // Alternate function
  uint16_t rx = 0, tx = 0;  // pins
  uint32_t freq = 0;        // Bus frequency

  if (uart == USART1) {
    freq = CPU_FREQUENCY, RCC->APBENR2 |= RCC_APBENR2_USART1EN;
    tx = PIN('A', 9), rx = PIN('A', 10);
  } else if (uart == USART2) {
    freq = CPU_FREQUENCY, RCC->APBENR1 |= RCC_APBENR1_USART2EN;
    tx = PIN('A', 2), rx = PIN('A', 3);
  } else {
    return false;
  }
  gpio_init(tx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  gpio_init(rx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH, 0, af);
  uart->CR1 = 0;                            // Disable UART
  uart->BRR = freq / baud;                  // Set baud rate
  uart->CR1 = USART_CR1_RE | USART_CR1_TE;  // Set mode to TX & RX
  uart->CR1 |= USART_CR1_UE;                // Enable UART
  return true;
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

#ifndef RNG
struct rng {
  volatile uint32_t CR, SR, DR;
};
#define RNG ((struct rng *) 0x40025000)  // RM0444  2.2.2  Table 6
#endif

static inline void rng_init(void) {
  RCC->CCIPR |= 2U << 26U; // RNG clock source. Documented in 5.4.21
  RCC->AHBENR |= BIT(18);  // RM0444 5.4.25 Table 36
  RNG->CR |= BIT(2);       // 19.7.1
}
static inline uint32_t rng_read(void) {
  while ((RNG->SR & BIT(0)) == 0) (void) 0;
  return RNG->DR;
}

// Bit-bang SPI implementation
struct spi {
  uint16_t miso, mosi, clk, cs;  // Pins
  int spin;                      // Number of NOP spins for bitbanging
};

static inline void spi_begin(struct spi *spi) {
  gpio_write(spi->cs, 0);
  // printf("%s\n", __func__);
}

static inline void spi_end(struct spi *spi) {
  gpio_write(spi->cs, 1);
  // printf("%s\n", __func__);
}

static inline void spi_init(struct spi *spi) {
  gpio_input(spi->miso);
  gpio_output(spi->mosi);
  gpio_output(spi->clk);
  gpio_output(spi->cs);
  gpio_write(spi->cs, 1);
  // printf("%s\n", __func__);
}

// Send a byte, and return a received byte
static inline uint8_t spi_txn(struct spi *spi, uint8_t write_byte) {
  unsigned count = spi->spin <= 0 ? 9 : (unsigned) spi->spin;
  uint8_t rx = 0, tx = write_byte;
  for (int i = 0; i < 8; i++) {
    gpio_write(spi->mosi, tx & 0x80U);   // Set mosi
    gpio_write(spi->clk, 1);             // Clock high
    spin(count);                         // Wait half cycle
    rx <<= 1U;                           // Shift alreay read bits
    if (gpio_read(spi->miso)) rx |= 1U;  // Read next bit
    gpio_write(spi->clk, 0);             // Clock low
    spin(count);                         // Wait half cycle
    tx <<= 1U;                           // Discard written bit
  }
  // printf("%s %02x %02x\n", __func__, (int) write_byte, (int) rx);
  return rx;  // Return the received byte
}

#define UUID ((uint32_t *) UID_BASE)  // Unique 96-bit chip ID. TRM 59.1

// Helper macro for MAC generation, byte reads not allowed
#define GENERATE_LOCALLY_ADMINISTERED_MAC()                         \
  {                                                                 \
    2, UUID[0] & 255, (UUID[0] >> 10) & 255, (UUID[0] >> 19) & 255, \
        UUID[1] & 255, UUID[2] & 255                                \
  }
