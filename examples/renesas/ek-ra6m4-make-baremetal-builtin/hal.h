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

#define BIT(x) (1UL << (x))
#define SETBITS(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define PIN(port, num) ((((port) - '0') << 8) | (num))
#define PIN_NUM(pin) (pin & 255)
#define PIN_PORT(pin) (pin >> 8)

#define LED1 PIN('4', 15)  // blue
#define LED2 PIN('4', 4)   // green
#define LED3 PIN('4', 0)   // red
#define LED LED2           // Use green LED for blinking

#define rng_read() 0
#define rng_init()
#define GENERATE_MAC_ADDRESS() \
  { 2, 3, 4, 5, 6, 7 }

// Clock
// Board UM: P212 P213 24MHz xtal
// - (8.1 Table 8.1) (50.3.1 Table 50.14): PLL 120 ~ 200 MHz; input 8 ~ 24 MHz.
// System clock (ICLK) <= 200 MHz; PCLKA <= 100 MHz; PCLKB <= 50 MHz
// - (8.2.2): ICLK >= other clocks of interest for us
// - (8.7.3): FlashIF Clock (FCLK) (8.1 Table 8.1): 4 MHz to 50 MHz(P/E), up to
// 50 MHz (R)
// - (8.2.9): MOSCCR Set the PRCR.PRC0 bit to 1 (write enabled) before rewriting
// this register.
// - (10.2.9) OPCCR : Operating Power Control Register, defaults to high-speed
// mode
// - (45.2.4) 1 wait state for SRAM (default);
#define SYS_FREQUENCY 200000000
#define PLL_MUL 49    // x25 ((n+1)/2)
#define PLL_DIV 2     // /3 (n+1)
#define ICLK_DIV 0    // /1 -> 200MHz (/(2^n))
#define FCLK_DIV 2    // /4 -> 50MHz
#define PCLKAD_DIV 1  // /2 -> 100MHz
#define PCLKBC_DIV 2  // /4 -> 50MHz
#define FLASH_MHZ (SYS_FREQUENCY / ((1 << FCLK_DIV) * 1000000))
static inline void clock_init(void) {
  R_SYSTEM->PRCR = 0xA501;  // enable writing to osc control regs (12.2.1)
  R_SYSTEM->MOSCWTCR = 9;   // (8.2.5, 8.2.4)
  R_SYSTEM->MOSCCR = 0;     // enable main oscillator, default cfg (20~24MHz)
  while (R_SYSTEM->OSCSF_b.MOSCSF == 0) (void) 0;  // wait until it stabilizes
  R_SYSTEM->PLLCCR =
      (PLL_MUL << R_SYSTEM_PLLCCR_PLLMUL_Pos) |
      (PLL_DIV
       << R_SYSTEM_PLLCCR_PLIDIV_Pos);  // config PLL for MOSC /3 x 25 (8.2.4)
  R_SYSTEM->PLLCR = 0;                  // enable PLL
  R_SYSTEM->SCKDIVCR =
      (FCLK_DIV << R_SYSTEM_SCKDIVCR_FCK_Pos) |
      (ICLK_DIV << R_SYSTEM_SCKDIVCR_ICK_Pos) |
      (PCLKAD_DIV << R_SYSTEM_SCKDIVCR_BCK_Pos) |
      (PCLKAD_DIV << R_SYSTEM_SCKDIVCR_PCKA_Pos) |
      (PCLKBC_DIV << R_SYSTEM_SCKDIVCR_PCKB_Pos) |
      (PCLKBC_DIV << R_SYSTEM_SCKDIVCR_PCKC_Pos) |
      (PCLKAD_DIV << R_SYSTEM_SCKDIVCR_PCKD_Pos);  // set dividers
  R_FCACHE->FLWT = 3;          // flash: 3 wait states (47.4.3)
  R_FLAD->FCKMHZ = FLASH_MHZ;  // flash: read speed optimization (47.4.28)
  R_FACI_HP->FPCKAR = (0x1E << 8) + FLASH_MHZ;    // flash: write (47.4.26)
  while (R_SYSTEM->OSCSF_b.PLLSF == 0) (void) 0;  // PLL stabilization
  while (R_FLAD->FCKMHZ != FLASH_MHZ) (void) 0;   // flash module magic
  R_SYSTEM->SCKSCR = 5;                           // select PLL (8.2.3)
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
  } else {  // GPIO_MODE_AF
    // TODO(): only non-analog supported (19.2.5)
    R_PMISC->PWPR_b.B0WI = 0;  // (19.5.1--> (RM says this is PFS...)
    R_PMISC->PWPR_b.PFSWE = 1;
    R_PFS->PORT[PIN_PORT(pin)].PIN[PIN_NUM(pin)].PmnPFS_b.PMR = 0;
    R_PFS->PORT[PIN_PORT(pin)].PIN[PIN_NUM(pin)].PmnPFS_b.PSEL =
        af & (R_PFS_PORT_PIN_PmnPFS_PSEL_Msk >> R_PFS_PORT_PIN_PmnPFS_PSEL_Pos);
    R_PFS->PORT[PIN_PORT(pin)].PIN[PIN_NUM(pin)].PmnPFS_b.PMR = 1;
    R_PFS->PORT[PIN_PORT(pin)].PIN[PIN_NUM(pin)].PmnPFS_b.DSCR = 2;  // high
    R_PMISC->PWPR_b.PFSWE = 0;
    R_PMISC->PWPR_b.B0WI = 1;  // <--19.5.1)
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

#ifndef UART_DEBUG
#define UART_DEBUG R_SCI7
#endif
#define UART_TX PIN('6', 13)  // SCI7
#define UART_RX PIN('6', 14)

// UART
// (datasheet, after BSP rev.eng.) SCI uses PCLKA in this chip
static inline void uart_init(R_SCI0_Type *uart, unsigned baud) {
  if (uart != R_SCI7) return;
  R_MSTP->MSTPCRB_b.MSTPB24 = 0;  // enable SCI7
  (void) R_MSTP->MSTPCRB;         // (10.10.15)
  uart->SCR = 0;                  // (29.3.7 -->) disable SCI
  uart->FCR_b.FM = 0;             // disable FIFO
  uart->SIMR1 = 0;                // disable I2C (default)
  uart->SPMR = 0;                 // disable SPI (default)
  uart->SCMR_b.SMIF = 0;          // no smartcard (default)
  uart->SMR = 0;                  // async 8N1, use PCLK
  uart->SCMR_b.CHR1 = 1;          // disable 9-bit mode (default)
  uart->SEMR = 0;  // 16x clocking (other SCIs need internal clock setting)
  uart->SPTR = 3;  // no inversions, high on idle
  uart->BRR = (uint8_t) (SYS_FREQUENCY / (32 * (1 << PCLKAD_DIV) * baud));
  gpio_init(UART_TX, GPIO_MODE_AF, 5);  // (19.6)
  gpio_init(UART_RX, GPIO_MODE_AF, 5);
  (void) uart, (void) baud;
  uart->SCR = R_SCI0_SCR_TE_Msk | R_SCI0_SCR_RE_Msk;  // enable Tx, Rx
}

static inline void uart_write_byte(R_SCI0_Type *uart, uint8_t byte) {
  uart->TDR = byte;
  while (uart->SSR_b.TDRE == 0) (void) 0;
}

static inline void uart_write_buf(R_SCI0_Type *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

static inline int uart_read_ready(R_SCI0_Type *uart) {
  return uart->SSR_b.RDRF;  // If RDRF bit is set, data is ready
}
static inline uint8_t uart_read_byte(R_SCI0_Type *uart) {
  return uart->RDR;
}

// Ethernet
// - (13.3.2 Table 13.4) Event table: Event 0x16F, ETHER_EINT0
// - (13.1) ICU (13.3.1) Interrupt Vector Table (13.5)
// - Choose a IELSRx, that is IRQn=x in NVIC, write event number
static inline void ethernet_init(void) {
  R_MSTP->MSTPCRB_b.MSTPB15 = 0;  // enable ETHERC0 and EDMAC0
  (void) R_MSTP->MSTPCRB;         // (10.10.15)
  // (Board manual: 4.3.3 Table 1, 6.1 Table 20) (RM: 19.6)
  R_SYSTEM->PRCR = 0xA502;  // enable writing to next reg (12.2.1)
  R_SYSTEM->VBTICTLR = 0;   // enable MDC pins (19.5.5) (11.2.6)
  gpio_init(PIN('4', 1), GPIO_MODE_AF, 0x17);  // MDC
  gpio_init(PIN('4', 2), GPIO_MODE_AF, 0x17);  // MDIO
  gpio_output(PIN('4', 3));                    // PHY RST
  gpio_write(PIN('0', 2), 1);                  // prevent NAND_TREE
  gpio_write(PIN('4', 3), 0);                  // assert RST
  gpio_init(PIN('4', 5), GPIO_MODE_AF, 0x17);  // TX_EN
  gpio_init(PIN('4', 6), GPIO_MODE_AF, 0x17);  // TXD1
  gpio_init(PIN('7', 0), GPIO_MODE_AF, 0x17);  // TXD0
  gpio_init(PIN('7', 1), GPIO_MODE_AF, 0x17);  // REF50CK0
  gpio_init(PIN('7', 2), GPIO_MODE_AF, 0x17);  // RXD0
  gpio_init(PIN('7', 3), GPIO_MODE_AF, 0x17);  // RXD1
  gpio_init(PIN('7', 4), GPIO_MODE_AF, 0x17);  // RX_ER
  gpio_init(PIN('7', 5), GPIO_MODE_AF, 0x17);  // CRS_DV
  R_PMISC->PFENET_b.PHYMODE0 = 0;              // select RMII
  for (volatile int i = 0; i < 0x2000; i++)
    (void) 0;                  // keep PHY RST low for a while
  gpio_write(PIN('4', 3), 1);  // deassert RST
  gpio_input(PIN('0', 2));     // PHY IRQ, not used
  NVIC_EnableIRQ(0);           // (13.5.1) no CMSIS support
  R_ICU->IELSR[0] = 0x16f;     // (13.2.15)(13.5.4.1)
}
