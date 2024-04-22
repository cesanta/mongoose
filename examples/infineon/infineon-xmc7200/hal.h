// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LED1 PIN(16, 1)
#define LED2 PIN(16, 2)
#define LED3 PIN(16, 3)

#include "xmc7200d_e272k8384.h"

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

extern uint32_t SystemCoreClock;

enum {PLL_FEEDBACK = 50, PLL_REF = 1, PLL_OUT = 4};
#define CLK_IMO 8000000UL
#define SYS_FREQUENCY ((CLK_IMO * PLL_FEEDBACK) / (PLL_REF * PLL_OUT))

#ifndef UART_DEBUG
#define UART_DEBUG SCB3
#endif

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_HIGHZ = 0, GPIO_PULLUP = 2, GPIO_PULLDOWN = 3, GPIO_OPENDRAIN_LOW = 4,
       GPIO_OPENDRAIN_HIGH = 5, GPIO_STRONG = 6, GPIO_PULLUP_DOWN = 7};
enum { GPIO_SPEED_LOW = 3, GPIO_SPEED_MEDIUM = 2, GPIO_SPEED_HIGH = 0};
#undef GPIO
#define GPIO_TypeDef GPIO_PRT_Type
#define GPIO(N) ((GPIO_TypeDef *) (GPIO_BASE + 0x80 * (N)))

static GPIO_TypeDef *gpio_bank(uint16_t pin) { return GPIO(PINBANK(pin)); }

static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
	(void) pin, (void) mode, (void) type, (void) speed, (void) pull, (void) af;
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint8_t pinno = PINNO(pin);
  uint32_t msk, pos;

  // configure input / output direction
  if (mode == GPIO_MODE_INPUT) {
    gpio->CFG |= 1 << (4 * pinno + 3); // enable input buffer (IN_ENx)
  }

  if (af || (mode == GPIO_MODE_AF && af)) {
    // configure alternate function
    HSIOM_PRT_Type* hsiom = ((HSIOM_PRT_Type*) HSIOM) + PINBANK(pin);
    volatile uint32_t *port_sel = pinno < 4 ? &hsiom->PORT_SEL0 : &hsiom->PORT_SEL1;
    pos = 8 * (pinno % 4), msk = 0x1f << pos;
    *port_sel &= ~msk, *port_sel |= af << pos;
  }

  // configure driver mode
  msk = 7, pos = 4 * pinno;
  CLRSET(gpio->CFG, msk << pos, type << pos);

  // configure speed
  msk = 3, pos = 2 * pinno + 16;
  CLRSET(gpio->CFG_OUT, msk << pos, speed << pos);

  if (mode == GPIO_MODE_OUTPUT /*&& af == 0*/) {
    gpio->OUT_SET = (1 << pinno);
  }
}

static inline void gpio_input(uint16_t pin) {
	gpio_init(pin, GPIO_MODE_INPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH,
	            0, 0);
}

static inline void gpio_output(uint16_t pin) {
	gpio_init(pin, GPIO_MODE_OUTPUT, GPIO_STRONG, GPIO_SPEED_MEDIUM,
	            0, 0);
}

static inline bool gpio_read(uint16_t pin) {
  GPIO_TypeDef *gpio = gpio_bank(pin);
  uint8_t pinno = PINNO(pin);
  if (gpio->CFG & (1 << (4 * pinno + 3))) {
    // pin mode is input, reading from GPIO_PRT_IN
    return gpio->IN & (1 << pinno);
  } else {
    // pin mode is output, reading from GPIO_PRT_OUT
    return gpio->OUT & (1 << pinno);
  }
}

static inline void gpio_write(uint16_t pin, bool value) {
	GPIO_TypeDef *gpio = gpio_bank(pin);
  uint8_t pinno = PINNO(pin);
  if (value) {
    gpio->OUT_SET = 1 << pinno;
  } else {
    gpio->OUT_CLR = 1 << pinno;
  }
}

static inline void gpio_toggle(uint16_t pin) {
  gpio_write(pin, !gpio_read(pin));
}

static inline void uart_init(volatile CySCB_Type *uart, unsigned long baud) {
  (void) uart, (void) baud;
  uint16_t rx = 0, tx = 0;
  if (uart == SCB3) {
    rx = PIN(13, 0), tx = PIN(13, 1);
  } else {
    return; // unsupported uart
  }

  // set pins
  gpio_init(rx, GPIO_MODE_INPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH, 0, 17);
  gpio_init(tx, GPIO_MODE_OUTPUT, GPIO_STRONG, GPIO_SPEED_HIGH, 0, 17);

  // enable peripheral clock (18.6.2)
  // compute divider first (we choose 24 bit divider 0)
  unsigned long div = 0, frac = 0, ovs = 16;
  div = SYS_FREQUENCY / (baud * ovs);
  if (div == 0) div = 1;
  frac = SYS_FREQUENCY % (baud * ovs);
  frac = (frac * 100) / (baud * ovs);
  PERI_PCLK->GR[1].DIV_CMD = (3 << PERI_PCLK_GR_DIV_CMD_TYPE_SEL_Pos) |
                             PERI_PCLK_GR_DIV_CMD_PA_DIV_SEL_Msk |
                             PERI_PCLK_GR_DIV_CMD_PA_TYPE_SEL_Msk;
  PERI_PCLK->GR[1].DIV_CMD |= PERI_PCLK_GR_DIV_CMD_DISABLE_Msk; // disable divider
  PERI_PCLK->GR[1].DIV_24_5_CTL[0] = (((uint8_t) (div - 1)) << PERI_PCLK_GR_DIV_24_5_CTL_INT24_DIV_Pos) |
                                     (frac << PERI_PCLK_GR_DIV_24_5_CTL_FRAC5_DIV_Pos);
  PERI_PCLK->GR[1].DIV_CMD = (3 << PERI_PCLK_GR_DIV_CMD_TYPE_SEL_Pos) |
                             PERI_PCLK_GR_DIV_CMD_PA_DIV_SEL_Msk |
                             PERI_PCLK_GR_DIV_CMD_PA_TYPE_SEL_Msk;;
  PERI_PCLK->GR[1].DIV_CMD |= PERI_PCLK_GR_DIV_CMD_ENABLE_Msk; // enable divider
  PERI_PCLK->GR[1].CLOCK_CTL[PCLK_SCB3_CLOCK & 0xff] = 3 << PERI_PCLK_GR_CLOCK_CTL_TYPE_SEL_Pos;  // connect SCB3 to div_24_5_ctl[0]

  // configure UART interface
  uart->CTRL = ((ovs - 1) << SCB_CTRL_OVS_Pos) | (2 << SCB_CTRL_MODE_Pos);
  uart->UART_CTRL = 0;
  uart->UART_RX_CTRL = 1 << SCB_UART_RX_CTRL_STOP_BITS_Pos;
  uart->RX_CTRL = 7 << SCB_RX_CTRL_DATA_WIDTH_Pos;
  uart->UART_TX_CTRL = 1 << SCB_UART_TX_CTRL_STOP_BITS_Pos;
  uart->TX_CTRL = 7 << SCB_TX_CTRL_DATA_WIDTH_Pos;
  uart->TX_FIFO_CTRL = 1 << 16;
  uart->TX_FIFO_CTRL = 0;
  uart->CTRL |= 1 << SCB_CTRL_ENABLED_Pos;
}

static inline void uart_write_byte(volatile CySCB_Type *uart, uint8_t byte) {
  (void) byte; (void) uart;
  while((uart->INTR_TX & SCB_INTR_TX_EMPTY_Msk) == 0) spin(1);
  uart->TX_FIFO_WR = byte;
  uart->INTR_TX |= ~SCB_INTR_TX_EMPTY_Msk;
}

static inline void uart_write_buf(volatile CySCB_Type *uart, char *buf, size_t len) {
	while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

static inline void rng_init(void) {
}
static inline uint32_t rng_read(void) {
   return 0;
}

static inline void ethernet_init(void) {
  gpio_init(PIN(26, 0), GPIO_MODE_INPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH, 0, 27); // ETH1_REF_CLK
  gpio_init(PIN(26, 1), GPIO_MODE_OUTPUT, GPIO_STRONG, GPIO_SPEED_HIGH, 0, 27); // ETH1_TX_CTL
  gpio_init(PIN(26, 2), GPIO_MODE_OUTPUT, GPIO_STRONG, GPIO_SPEED_HIGH, 0, 27); // ETH1_TX_CLK
  gpio_init(PIN(26, 3), GPIO_MODE_OUTPUT, GPIO_STRONG, GPIO_SPEED_HIGH, 0, 27); // ETH1_TXD_0
  gpio_init(PIN(26, 4), GPIO_MODE_OUTPUT, GPIO_STRONG, GPIO_SPEED_HIGH, 0, 27); // ETH1_TXD_1
  gpio_init(PIN(26, 5), GPIO_MODE_OUTPUT, GPIO_STRONG, GPIO_SPEED_HIGH, 0, 27); // ETH1_TXD_2
  gpio_init(PIN(26, 6), GPIO_MODE_OUTPUT, GPIO_STRONG, GPIO_SPEED_HIGH, 0, 27); // ETH1_TXD_3
  gpio_init(PIN(26, 7), GPIO_MODE_INPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH, 0, 27); // ETH1_RXD_0

  gpio_init(PIN(27, 0), GPIO_MODE_INPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH, 0, 27); // ETH1_RXD_1
  gpio_init(PIN(27, 1), GPIO_MODE_INPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH, 0, 27); // ETH1_RXD_2
  gpio_init(PIN(27, 2), GPIO_MODE_INPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH, 0, 27); // ETH1_RXD_3
  gpio_init(PIN(27, 3), GPIO_MODE_INPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH, 0, 27); // ETH1_RX_CTL
  gpio_init(PIN(27, 4), GPIO_MODE_INPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH, 0, 27); // ETH1_RX_CLK
  gpio_init(PIN(27, 5), GPIO_MODE_INPUT, GPIO_STRONG, GPIO_SPEED_LOW, 0, 27); // ETH1_MDIO
  gpio_init(PIN(27, 6), GPIO_MODE_OUTPUT, GPIO_STRONG, GPIO_SPEED_LOW, 0, 27); // ETH1_MDC
  //gpio_init(PIN(27, 7), GPIO_MODE_OUTPUT, GPIO_HIGHZ, GPIO_SPEED_HIGH, 0, 0); // ETH1_RST

  CPUSS->CM7_0_SYSTEM_INT_CTL[eth_1_interrupt_eth_0_IRQn] = 0x80000003; // assign CPU interrupt #3
  NVIC->ISER[0] = 1 << 3;

  spin(10000000); // artificial delay to wait for PHY init
}

static inline void clock_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  asm("DSB");
  asm("ISB");
  
  // configure PLL
  CLRSET(SRSS->CLK_PLL400M[0].CONFIG, CLK_PLL400M_CONFIG_BYPASS_SEL_Msk, BIT(29)); // First bypass PLL
  spin(10);
  SRSS->CLK_PLL400M[0].CONFIG &= ~CLK_PLL400M_CONFIG_ENABLE_Msk; // disable the PLL itself

  // IMO source generates a frequency of 8MHz. The final frequency will be
  // calculated as (CLK_IMO * feedback) / (reference * output_div)
  CLRSET(SRSS->CLK_PLL400M[0].CONFIG, CLK_PLL400M_CONFIG_FEEDBACK_DIV_Msk, PLL_FEEDBACK << CLK_PLL400M_CONFIG_FEEDBACK_DIV_Pos);
  CLRSET(SRSS->CLK_PLL400M[0].CONFIG, CLK_PLL400M_CONFIG_REFERENCE_DIV_Msk, PLL_REF << CLK_PLL400M_CONFIG_REFERENCE_DIV_Pos);
  CLRSET(SRSS->CLK_PLL400M[0].CONFIG, CLK_PLL400M_CONFIG_OUTPUT_DIV_Msk, PLL_OUT << CLK_PLL400M_CONFIG_OUTPUT_DIV_Pos);
  CLRSET(SRSS->CLK_PLL400M[0].CONFIG2, CLK_PLL400M_CONFIG2_FRAC_DIV_Msk, 0);
  CLRSET(SRSS->CLK_PLL400M[0].CONFIG2, CLK_PLL400M_CONFIG2_FRAC_DITHER_EN_Msk, 0);
  CLRSET(SRSS->CLK_PLL400M[0].CONFIG2, CLK_PLL400M_CONFIG2_FRAC_EN_Msk, 1 << CLK_PLL400M_CONFIG2_FRAC_EN_Pos);
  CLRSET(SRSS->CLK_PLL400M[0].CONFIG, CLK_PLL400M_CONFIG_BYPASS_SEL_Msk, 0);
  spin(10);
  SRSS->CLK_PLL400M[0].CONFIG |= CLK_PLL400M_CONFIG_ENABLE_Msk; // enable the PLL
  while (SRSS->CLK_PLL400M[0].CONFIG & 1) spin(1);

  // configure PATH1 with source set to IMO
  SRSS->CLK_PATH_SELECT[1] = 0;

  // enable CLK_HFx
  uint8_t clocks[] = {0, 1, 2, 3, 4, 5, 6};
  for (size_t i = 0; i < sizeof(clocks) / sizeof(uint8_t); i++) {
    CLRSET(SRSS->CLK_ROOT_SELECT[clocks[i]], SRSS_CLK_ROOT_SELECT_ROOT_MUX_Msk, 1); // choose PATH1
    CLRSET(SRSS->CLK_ROOT_SELECT[clocks[i]], SRSS_CLK_ROOT_SELECT_DIRECT_MUX_Msk, 1 << SRSS_CLK_ROOT_SELECT_DIRECT_MUX_Pos);
    if (clocks[i] != 0) // CLF_HF0 is already enabled
      SRSS->CLK_ROOT_SELECT[clocks[i]] |= SRSS_CLK_ROOT_SELECT_ENABLE_Msk; // enable clock root
  }

  // set systick frequency
  uint32_t tenms = SYS_FREQUENCY / 100 - 1; // number of cycles executed in 10ms
  CPUSS->SYSTICK_CTL = tenms | (3 << CPUSS_SYSTICK_CTL_CLOCK_SOURCE_Pos); // select CLK_HF as source
}
