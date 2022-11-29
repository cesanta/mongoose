// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
// https://www.ti.com/lit/pdf/spms433

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK) (R) = ((R) & ~(CLEARMASK)) | (SETMASK)
#define PIN(bank, num) ((bank << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) pinbank(pin >> 8)
// This MCU doesn't have GPIOI nor GPIOO
static inline unsigned int pinbank(unsigned int bank) {
  bank = bank > 'O' ? bank - 2 : bank > 'I' ? bank - 1 : bank;
  return bank - 'A';
}

// 5.5, Table 5-12: configure flash (and EEPROM) timing in accordance to clock
// freq
enum {
  PLL_CLK = 25,
  PLL_M = 96,
  PLL_N = 5,
  PLL_Q = 1,
  PSYSDIV = 4
};  // Run at 120 Mhz
#define PLL_FREQ (PLL_CLK * PLL_M / PLL_N / PLL_Q / PSYSDIV)
#define FLASH_CLKHIGH 6
#define FLASH_WAITST 5
#define FREQ (PLL_FREQ * 1000000)

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

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

struct nvic {
  volatile uint32_t ISER[8], RESERVED0[24], ICER[8], RSERVED1[24], ISPR[8],
      RESERVED2[24], ICPR[8], RESERVED3[24], IABR[8], RESERVED4[56], IP[240],
      RESERVED5[644], STIR;
};
#define NVIC ((struct nvic *) 0xe000e100)
static inline void nvic_set_prio(int irq, uint32_t prio) {
  NVIC->IP[irq] = prio << 4;
}
static inline void nvic_enable_irq(int irq) {
  NVIC->ISER[irq >> 5] = (uint32_t) (1 << (irq & 31));
}

struct scb {
  volatile uint32_t CPUID, ICSR, VTOR, AIRCR, SCR, CCR, SHPR[3], SHCSR, CFSR,
      HFSR, DFSR, MMFAR, BFAR, AFSR, ID_PFR[2], ID_DFR, ID_AFR, ID_MFR[4],
      ID_ISAR[5], RESERVED0[1], CLIDR, CTR, CCSIDR, CSSELR, CPACR,
      RESERVED3[93], STIR, RESERVED4[15], MVFR0, MVFR1, MVFR2, RESERVED5[1],
      ICIALLU, RESERVED6[1], ICIMVAU, DCIMVAC, DCISW, DCCMVAU, DCCMVAC, DCCSW,
      DCCIMVAC, DCCISW, RESERVED7[6], ITCMCR, DTCMCR, AHBPCR, CACR, AHBSCR,
      RESERVED8[1], ABFSR;
};
#define SCB ((struct scb *) 0xe000ed00)

struct sysctl {
  volatile uint32_t DONTCARE0[31], MOSCCTL, RESERVED0[12], RSCLKCFG,
      RESERVED1[3], MEMTIM0, DONTCARE1[39], PLLFREQ0, PLLFREQ1, PLLSTAT,
      DONTCARE2[241], SREPHY, DONTCARE3[26], SREMAC, DONTCARE4[26], RCGCGPIO,
      DONTCARE5[3], RCGCUART, DONTCARE6[5], RCGCEPHY, DONTCARE7[26], RCGCEMAC,
      DONTCARE8[228], PREPHY, DONTCARE9[26], PREMAC;
};
#define SYSCTL ((struct sysctl *) 0x400FE000)

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum { GPIO_OTYPE_PUSH_PULL, GPIO_OTYPE_OPEN_DRAIN };
enum { GPIO_SPEED_LOW, GPIO_SPEED_HIGH };
enum { GPIO_PULL_NONE, GPIO_PULL_UP, GPIO_PULL_DOWN };

// 10.3, 10.6
struct gpio {
  volatile uint32_t GPIODATA[256], GPIODIR, GPIOIS, GPIOIBE, GPIOIEV, GPIOIM,
      GPIORIS, GPIOMIS, GPIOICR, GPIOAFSEL, RESERVED1[55], GPIODR2R, GPIODR4R,
      GPIODR8R, GPIOODR, GPIOPUR, GPIOPDR, GPIOSLR, GPIODEN, GPIOLOCK, GPIOCR,
      GPIOAMSEL, GPIOPCTL, GPIOADCCTL, GPIODMACTL, GPIOSI, GPIODR12R,
      GPIOWAKEPEN, GPIOWAKELVL, GPIOWAKESTAT, RESERVED2[669], GPIOPP, GPIOPC,
      RESERVED3[2], GPIOPeriphID4, GPIOPeriphID5, GPIOPeriphID6, GPIOPeriphID7,
      GPIOPeriphID0, GPIOPeriphID1, GPIOPeriphID2, GPIOPeriphID3, GPIOPCellID0,
      GPIOPCellID1, GPIOPCellID2, GPIOPCellID3;
};
#define GPIO(N) ((struct gpio *) (0x40058000 + 0x1000 * (N)))

static struct gpio *gpio_bank(uint16_t pin) {
  return GPIO(PINBANK(pin));
}
static inline void gpio_toggle(uint16_t pin) {
  struct gpio *gpio = gpio_bank(pin);
  uint8_t mask = BIT(PINNO(pin));
  gpio->GPIODATA[mask] ^= mask;
}
static inline int gpio_read(uint16_t pin) {
  return gpio_bank(pin)->GPIODATA[BIT(PINNO(pin))] ? 1 : 0;
}
static inline void gpio_write(uint16_t pin, bool val) {
  struct gpio *gpio = gpio_bank(pin);
  uint8_t mask = BIT(PINNO(pin));
  gpio->GPIODATA[mask] = val ? mask : 0;
}
static inline void gpio_init(uint16_t pin, uint8_t mode, uint8_t type,
                             uint8_t speed, uint8_t pull, uint8_t af) {
  struct gpio *gpio = gpio_bank(pin);
  uint8_t n = (uint8_t) (PINNO(pin));
  SYSCTL->RCGCGPIO |= BIT(PINBANK(pin));  // Enable GPIO clock
  if (mode == GPIO_MODE_ANALOG) {
    gpio->GPIOAMSEL |= BIT(PINNO(pin));
    return;
  }
  if (mode == GPIO_MODE_INPUT) {
    gpio->GPIODIR &= ~BIT(PINNO(pin));
  } else if (mode == GPIO_MODE_OUTPUT) {
    gpio->GPIODIR |= BIT(PINNO(pin));
  } else {  // GPIO_MODE_AF
    SETBITS(gpio->GPIOPCTL, 15UL << ((n & 7) * 4),
            ((uint32_t) af) << ((n & 7) * 4));
    gpio->GPIOAFSEL |= BIT(PINNO(pin));
  }
  gpio->GPIODEN |= BIT(PINNO(pin));  // Enable pin as digital function
  if (type == GPIO_OTYPE_OPEN_DRAIN)
    gpio->GPIOODR |= BIT(PINNO(pin));
  else  // GPIO_OTYPE_PUSH_PULL
    gpio->GPIOODR &= ~BIT(PINNO(pin));
  if (speed == GPIO_SPEED_LOW)
    gpio->GPIOSLR |= BIT(PINNO(pin));
  else  // GPIO_SPEED_HIGH
    gpio->GPIOSLR &= ~BIT(PINNO(pin));
  if (pull == GPIO_PULL_UP) {
    gpio->GPIOPUR |= BIT(PINNO(pin));  // setting one...
  } else if (pull == GPIO_PULL_DOWN) {
    gpio->GPIOPDR |= BIT(PINNO(pin));  // ...just clears the other
  } else {
    gpio->GPIOPUR &= ~BIT(PINNO(pin));
    gpio->GPIOPDR &= ~BIT(PINNO(pin));
  }
}
static inline void gpio_input(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_INPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_UP, 0);  // EK does not have pull-up resistors
}
static inline void gpio_output(uint16_t pin) {
  gpio_init(pin, GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL, GPIO_SPEED_HIGH,
            GPIO_PULL_NONE, 0);
}

static inline void gpio_irq_attach(uint16_t pin) {
  uint8_t irqvecs[] = {16, 17, 18, 19, 20, 30, 31, 32,
                       51, 52, 53, 72, 73, 76, 84};
  struct gpio *gpio = gpio_bank(pin);
  gpio->GPIOIS &= ~BIT(PINNO(pin));    // edge sensitive
  gpio->GPIOIBE |= BIT(PINNO(pin));    // both edges
  gpio->GPIOIM |= BIT(PINNO(pin));     // enable pin irq
  int irqvec = irqvecs[PINBANK(pin)];  // IRQ vector index, 2.5.2
  nvic_set_prio(irqvec, 3);
  nvic_enable_irq(irqvec);
}

struct uart {
  volatile uint32_t UARTDR, UARTRSR, RESERVED0[4], UARTFR, RESERVED1, UARTILPR,
      UARTIBRD, UARTFBRD, UARTLCRH, UARTCTL, UARTIFLS, UARTIM, UARTRIS, UARTMIS,
      UARTICR, UARTDMACTL, RESERVED2[22], UART9BITADDR, UART9BITAMASK,
      RESERVED3[965], UARTPP, RESERVED4, UARTCC, RESERVED5, UARTPeriphID4,
      UARTPeriphID5, UARTPeriphID6, UARTPeriphID7, UARTPeriphID0, UARTPeriphID1,
      UARTPeriphID2, UARTPeriphID3, UARTPCellID0, UARTPCellID1, UARTPCellID2,
      UARTPCellID3;
};
#define UARTECR UARTRSR
#define USART_BASE 0x4000C000
#define USART_OFFSET 0x1000
#define USART(N) ((struct uart *) (USART_BASE + USART_OFFSET * (N)))
#define UARTNO(u) ((uint8_t)(((unsigned int) (u) - USART_BASE) / USART_OFFSET))

#define UART0 USART(0)

static inline void uart_init(struct uart *uart, unsigned long baud) {
  struct uarthw {
    uint16_t rx, tx;  // pins
    uint8_t af;       // Alternate function
  };
  // af, rx, tx for UART0
  struct uarthw uarthw[1] = {{PIN('A', 0), PIN('A', 1), 1}};

  if (uart != UART0) return;  // uarthw is not populated for other UARTs

  uint8_t uartno = UARTNO(uart);
  SYSCTL->RCGCUART |= BIT(uartno);  // Enable peripheral clock

  gpio_init(uarthw[uartno].tx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL,
            GPIO_SPEED_HIGH, 0, uarthw[uartno].af);
  gpio_init(uarthw[uartno].rx, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL,
            GPIO_SPEED_HIGH, 0, uarthw[uartno].af);
  // (16.3.2) ClkDiv = 16 (HSE=0)
  // BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
  // UARTFBRD[DIVFRAC] = integer(BRDF * 64 + 0.5)
  // must write in this order
  uart->UARTCTL = 0;                    // Disable this UART, clear HSE
  uart->UARTIBRD = FREQ / (16 * baud);  // Baud rate, integer part
  uart->UARTFBRD =
      ((FREQ % (16 * baud)) >> 26) & 0x3F;    // Baud rate, fractional part
  uart->UARTLCRH = (3 << 5);                  // 8N1, no FIFOs;
  uart->UARTCTL |= BIT(0) | BIT(9) | BIT(8);  // Set UARTEN, RXE, TXE
}
static inline void uart_write_byte(struct uart *uart, uint8_t byte) {
  uart->UARTDR = byte;
  while ((uart->UARTFR & BIT(7)) == 0) spin(1);
}
static inline void uart_write_buf(struct uart *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
static inline int uart_read_ready(struct uart *uart) {
  return uart->UARTFR & BIT(6);  // If RXFF bit is set, data is ready
}
static inline uint8_t uart_read_byte(struct uart *uart) {
  return (uint8_t) (uart->UARTDR & 0xFF);
}

static inline void clock_init(void) {                 // Set clock frequency
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  asm("DSB");
  asm("ISB");
  SETBITS(SYSCTL->MOSCCTL, BIT(3) | BIT(2),
          BIT(4));  // Enable MOSC circuit (clear NOXTAL and PWRDN, set >10MHz
                    // range)
  SETBITS(SYSCTL->MEMTIM0,
          BIT(21) | BIT(5) | 0x1F << 21 | 0xF << 16 | 0x1F << 5 | 0xF << 0,
          FLASH_CLKHIGH << 22 | FLASH_WAITST << 16 | FLASH_CLKHIGH << 5 |
              FLASH_WAITST << 0);  // Configure flash timing (not yet applied)
  SETBITS(SYSCTL->RSCLKCFG, 0xF << 24 | (BIT(9) - 1),
          3 << 24);  // Clear PLL divider, set MOSC as PLL source
  SYSCTL->PLLFREQ1 = (PLL_Q - 1) << 8 | (PLL_N - 1)
                                            << 0;  // Set PLL_Q and PLL_N
  SYSCTL->PLLFREQ0 =
      BIT(23) | PLL_M << 0;  // Set PLL_Q, power up PLL (if it were on, we'd
                             // need to set NEWFREQ in RSCLKCFG instead)
  while ((SYSCTL->PLLSTAT & BIT(0)) == 0) spin(1);  // Wait for lock
  SYSCTL->RSCLKCFG |=
      BIT(31) | BIT(28) |
      (PSYSDIV - 1) << 0;  // Update memory timing, use PLL, set clock divisor
}
