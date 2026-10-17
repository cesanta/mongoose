// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// FRDM-MCXW71 hardware abstraction layer. MCXW716C reference: MCUXpresso SDK,
// https://github.com/nxp-mcuxpresso/mcux-devices-mcx, MCXW/MCXW716C

#pragma once

#include "MCXW716C.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))

#define SIRC_FREQUENCY 6000000UL  // Slow IRC (FRO6M), Hz

extern uint32_t SystemCoreClock;  // CPU clock, Hz. Set by hal_clock_init()

// MRCC peripheral clock source, MRCC_xx[MUX], see fsl_clock.h
enum { MRCC_SRC_FRO6M = 2, MRCC_SRC_FRO192M = 3 };

// Fast IRC frequency, Hz, or 0 if FIRC is not running. See CLOCK_GetFircFreq()
static inline uint32_t hal_firc_freq(void) {
  static const uint32_t freqs[] = {48000000, 64000000, 96000000, 192000000};
  if ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) == 0) return 0;
  return freqs[SCG0->FIRCCFG & SCG_FIRCCFG_RANGE_MASK];
}

// Work out the CPU clock from the SCG clock source and core divider. The
// clock is left as configured at reset
static inline void hal_clock_init(void) {
  uint32_t csr = SCG0->CSR, freq = 0;
  uint32_t src = (csr & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT;
  uint32_t div = ((csr & SCG_CSR_DIVCORE_MASK) >> SCG_CSR_DIVCORE_SHIFT) + 1;
  if (src == 2) freq = SIRC_FREQUENCY;   // Slow IRC
  if (src == 3) freq = hal_firc_freq();  // Fast IRC
  if (freq == 0) freq = SIRC_FREQUENCY;  // Unknown source, assume reset value
  SystemCoreClock = freq / div;
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}

// Enable peripheral clock and take it out of reset, from given clock source.
// See CLOCK_SetIpSrc() and CLOCK_EnableClock() in fsl_clock.h
static inline void hal_periph_clock(volatile uint32_t *mrcc, uint32_t src) {
  uint32_t v = (*mrcc & ~(MRCC_CC_MASK | MRCC_MUX_MASK | MRCC_DIV_MASK)) |
               MRCC_MUX(src);
  *mrcc = v;                                   // Source, clock still off
  *mrcc = v | 1U;                              // CC = 1: clock on
  if (*mrcc & MRCC_PR_MASK) *mrcc |= MRCC_RSTB_MASK;  // Release reset
  __DSB();
}

// LPUART1 is wired to the MCU-Link debug probe USB serial port.
// TX = PTC3, RX = PTC2, both pin mux alternative 3. See board.h, pin_mux.c
static inline void hal_uart_init(LPUART_Type *uart, unsigned long baud) {
  uint32_t src = MRCC_SRC_FRO192M, freq = hal_firc_freq();
  uint32_t best_osr = 16, best_sbr = 1, best_err = UINT32_MAX, osr;
  if (freq == 0) src = MRCC_SRC_FRO6M, freq = SIRC_FREQUENCY;  // FIRC off
  hal_periph_clock(&MRCC->MRCC_PORTC, 0);
  PORTC->PCR[2] = PORT_PCR_MUX(3);  // RX
  PORTC->PCR[3] = PORT_PCR_MUX(3);  // TX
  hal_periph_clock(&MRCC->MRCC_LPUART1, src);
  // Find the oversampling ratio (4..32) that gives the lowest baud rate error
  for (osr = 4; osr <= 32; osr++) {
    uint32_t sbr = (freq + osr * baud / 2) / (osr * baud), actual, err;
    if (sbr == 0 || sbr > 0x1fff) continue;
    actual = freq / (osr * sbr);
    err = actual > baud ? actual - baud : baud - actual;
    if (err < best_err) best_err = err, best_osr = osr, best_sbr = sbr;
  }
  uart->CTRL = 0;  // Disable while configuring
  uart->BAUD = LPUART_BAUD_OSR(best_osr - 1) | LPUART_BAUD_SBR(best_sbr) |
               (best_osr < 8 ? LPUART_BAUD_BOTHEDGE_MASK : 0);
  uart->CTRL = LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
}

static inline void hal_uart_write_byte(LPUART_Type *uart, uint8_t byte) {
  while ((uart->STAT & LPUART_STAT_TDRE_MASK) == 0) (void) 0;
  uart->DATA = byte;
}

static inline void hal_uart_write_buf(LPUART_Type *uart, const char *buf,
                                      size_t len) {
  while (len-- > 0) hal_uart_write_byte(uart, *(const uint8_t *) buf++);
}

// WDOG0 runs after reset: disable it, as SystemInit() in system_MCXW716C.c
static inline void hal_wdog_disable(void) {
  while ((WDOG0->CS & WDOG_CS_RCS_MASK) == 0) (void) 0;
  if (WDOG0->CS & WDOG_CS_CMD32EN_MASK) {
    WDOG0->CNT = 0xD928C520U;  // Unlock sequence
  } else {
    WDOG0->CNT = 0xC520U;
    WDOG0->CNT = 0xD928U;
  }
  while ((WDOG0->CS & WDOG_CS_ULK_MASK) == 0) (void) 0;
  WDOG0->TOVAL = 0xFFFF;
  WDOG0->CS = (WDOG0->CS & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
  while ((WDOG0->CS & WDOG_CS_RCS_MASK) == 0) (void) 0;
}

static inline void hal_sys_init(void) {
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  hal_wdog_disable();
}

uint64_t hal_get_tick(void);
bool hal_timer_expired(volatile uint64_t *t, uint64_t period, uint64_t now);
size_t hal_ram_used(void);
size_t hal_ram_free(void);
