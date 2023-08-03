// Copyright (c) 2022 Cesanta Software Limited
// SPDX-License-Identifier: MIT
//
// https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/DataSheets/SAM-D5x-E5x-Family-Data-Sheet-DS60001507.pdf
// https://ww1.microchip.com/downloads/en/DeviceDoc/70005321A.pdf

#ifndef LED_PIN
#define LED_PIN PIN('C', 18)
#endif

#ifndef BUTTON_PIN
#define BUTTON_PIN PIN('B', 31)
#endif

#ifndef UART_DEBUG
#define UART_DEBUG USART1
#endif

#pragma once
#include <sam.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

static inline uint32_t clock_sys_freq(void) {
  return 48000000U;
}

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_OTYPE_PUSH_PULL, GPIO_OTYPE_OPEN_DRAIN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH, GPIO_SPEED_INSANE };
enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };
#define GPIO(N) ((port_group_registers_t *) (PORT_BASE_ADDRESS + 0x80 * (N)))
typedef port_group_registers_t GPIO_TypeDef;
static inline GPIO_TypeDef *gpio_bank(uint16_t pin) {
  return GPIO(PINBANK(pin));
}
static inline void gpio_toggle(uint16_t pin) {
  gpio_bank(pin)->PORT_OUTTGL = BIT(PINNO(pin));
}
static inline bool gpio_read(uint16_t pin) {
  return gpio_bank(pin)->PORT_IN & BIT(PINNO(pin));
}
static inline void gpio_write(uint16_t pin, bool val) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  if (val) {
    gpio->PORT_OUTSET = BIT(PINNO(pin));
  } else {
    gpio->PORT_OUTCLR = BIT(PINNO(pin));
  }
}
static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
  (void) type, (void) speed, (void) pull, (void) af;
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint32_t mask = BIT(PINNO(pin));
  MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_PORT_Msk;
  if (mode == GPIO_MODE_INPUT) {
    gpio->PORT_DIRCLR = mask;
  } else {
    gpio->PORT_DIRSET = mask;
  }
}
static inline void gpio_input(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_INPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
}
static inline void gpio_output(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
}

typedef sercom_usart_int_registers_t USART_TypeDef;
#define USART1 ((USART_TypeDef *) SERCOM0_BASE_ADDRESS)
#define USART2 ((USART_TypeDef *) SERCOM1_BASE_ADDRESS)
#define USART3 ((USART_TypeDef *) SERCOM2_BASE_ADDRESS)
static inline bool uart_init(USART_TypeDef *uart, unsigned long baud) {
  uint16_t rx = 0, tx = 0;  // Pins
  uint8_t rx_mux = 0, tx_mux = 0;
  if (uart == USART1) {
    MCLK_REGS->MCLK_APBAMASK |= MCLK_APBAMASK_SERCOM0_Msk;
    GCLK_REGS->GCLK_PCHCTRL[SERCOM0_GCLK_ID_CORE] =
        GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN_Msk;
    GCLK_REGS->GCLK_PCHCTRL[SERCOM0_GCLK_ID_SLOW] =
        GCLK_PCHCTRL_GEN_GCLK3 | GCLK_PCHCTRL_CHEN_Msk;
    tx = PIN('A', 4), rx = PIN('A', 5);
    rx_mux = MUX_PA05D_SERCOM0_PAD1, tx_mux = MUX_PA04D_SERCOM0_PAD0;
  } else if (uart == USART2) {
    MCLK_REGS->MCLK_APBAMASK |= MCLK_APBAMASK_SERCOM1_Msk;
    tx = PIN('C', 27), rx = PIN('C', 28);
  } else if (uart == USART3) {
    MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_SERCOM2_Msk;
    tx = PIN('A', 9), rx = PIN('A', 8);
  } else {
    return false;
  }
  gpio_bank(rx)->PORT_WRCONFIG =
      PORT_WRCONFIG_PMUX(rx_mux) | PORT_WRCONFIG_WRPMUX(1) |
      PORT_WRCONFIG_PMUXEN(1) | PORT_WRCONFIG_WRPINCFG(1) | BIT(PINNO(rx));
  gpio_bank(tx)->PORT_WRCONFIG =
      PORT_WRCONFIG_PMUX(tx_mux) | PORT_WRCONFIG_WRPMUX(1) |
      PORT_WRCONFIG_PMUXEN(1) | PORT_WRCONFIG_WRPINCFG(1) | BIT(PINNO(tx));
  uart->SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD(1) |
                       SERCOM_USART_INT_CTRLA_MODE(1 /* INT_CLK */) |
                       SERCOM_USART_INT_CTRLA_RXPO(1 /* PAD1 */) |
                       SERCOM_USART_INT_CTRLA_TXPO(0 /* PAD0 */) |
                       SERCOM_USART_INT_CTRLA_SAMPR(1);
  uart->SERCOM_BAUD = (uint16_t) (clock_sys_freq() / (16 * baud));
  uart->SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_RXEN(1) |
                       SERCOM_USART_INT_CTRLB_TXEN(1) |
                       SERCOM_USART_INT_CTRLB_CHSIZE(0);
  while (uart->SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk) spin(1);
  uart->SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE(1);
  while (uart->SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_ENABLE_Msk) spin(1);
  return true;
}
static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  while (!(uart->SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk)) spin(1);
  uart->SERCOM_DATA = byte;
}
static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline bool uart_read_ready(USART_TypeDef *uart) {
  return (uart->SERCOM_INTFLAG & SERCOM_USART_EXT_INTFLAG_RXC_Msk);
}
static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
  return (uint8_t) (uart->SERCOM_DATA & 255U);
}

static inline void rng_init(void) {
  MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_TRNG_Msk;
  TRNG_REGS->TRNG_CTRLA = TRNG_CTRLA_ENABLE_Msk;
}
static inline uint32_t rng_read(void) {
  while ((TRNG_REGS->TRNG_INTFLAG & TRNG_INTFLAG_DATARDY_Msk) == 0) spin(1);
  return TRNG_REGS->TRNG_DATA;
}

#define UID_BASE_W0 0x008061FC    // Word 0 location of the 128-bit chip ID
#define UID_BASE_W1_3 0x00806010  // Words 1-3 location of the 128-bit chip ID

#define UUID(n) ((n >= 0 && n <= 3) ? \
                     (((uint8_t *) UID_BASE_W0)[n]) : \
                     (((uint8_t *) UID_BASE_W1_3)[n - 4]))

#define GENERATE_LOCALLY_ADMINISTERED_MAC()                         \
  {                                                                 \
    2, UUID(0) ^ UUID(1) ^ UUID(2), UUID(3) ^ UUID(4) ^ UUID(5),    \
        UUID(6) ^ UUID(7) ^ UUID(8), UUID(9) ^ UUID(10) ^ UUID(11), \
        UUID(12) ^ UUID(13) ^ UUID(14) ^ UUID(15)                   \
  }

static inline bool timer_expired(volatile uint64_t *t, uint64_t prd,
                                 uint64_t now) {
  if (now + prd < *t) *t = 0;                    // Time wrapped? Reset timer
  if (*t == 0) *t = now + prd;                   // Firt poll? Set expiration
  if (*t > now) return false;                    // Not expired yet, return
  *t = (now - *t) > prd ? now + prd : *t + prd;  // Next expiration time
  return true;                                   // Expired, return true
}

static inline void clock_init(void) {
  SCB->CPACR |= (15U << 20);                // Enable FPU
  SysTick_Config(clock_sys_freq() / 1000);  // Sys tick every 1ms
}

static inline void gpio_set_irq_handler(uint16_t pin, void (*fn)(void *),
                                        void *arg) {
  (void) pin, (void) fn, (void) arg;
}

static inline void ethernet_init(void) {
  uint16_t pins[] = {PIN('A', 12), PIN('A', 13), PIN('A', 14), PIN('A', 15),
                     PIN('A', 17), PIN('A', 18), PIN('A', 19), PIN('C', 11),
                     PIN('C', 12), PIN('C', 20)};
  uint32_t af[] = {MUX_PA12L_GMAC_GRX1,  MUX_PA13L_GMAC_GRX0,
                   MUX_PA14L_GMAC_GTXCK, MUX_PA15L_GMAC_GRXER,
                   MUX_PA17L_GMAC_GTXEN, MUX_PA18L_GMAC_GTX0,
                   MUX_PA19L_GMAC_GTX1,  MUX_PC11L_GMAC_GMDC,
                   MUX_PC12L_GMAC_GMDIO, MUX_PC20L_GMAC_GRXDV};

  MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_PORT_Msk;

  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    int bank = PINBANK(pins[i]), no = PINNO(pins[i]);
    PORT_REGS->GROUP[bank].PORT_PINCFG[no] |= PORT_PINCFG_PMUXEN_Msk;
    volatile uint8_t *m = &PORT_REGS->GROUP[bank].PORT_PMUX[no / 2], v = m[0];
    if (no & 1) {
      m[0] = (uint8_t) ((v & ~0xf0) | PORT_PMUX_PMUXO(af[i]));
    } else {
      m[0] = (uint8_t) ((v & ~0x0f) | PORT_PMUX_PMUXE(af[i]));
    }
  }

  PORT_REGS->GROUP[0].PORT_PINCFG[17] |= PORT_PINCFG_DRVSTR_Msk;
  PORT_REGS->GROUP[0].PORT_PINCFG[18] |= PORT_PINCFG_DRVSTR_Msk;
  PORT_REGS->GROUP[0].PORT_PINCFG[19] |= PORT_PINCFG_DRVSTR_Msk;

  // Reset PHY
  uint16_t phy_pin = PIN('C', 21);
  gpio_output(phy_pin);
  gpio_write(phy_pin, false);
  spin(999);
  gpio_write(phy_pin, true);
  spin(999);
}
