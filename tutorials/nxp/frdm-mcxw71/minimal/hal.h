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

uint64_t hal_get_tick(void);
bool hal_timer_expired(volatile uint64_t *t, uint64_t period, uint64_t now);
size_t hal_ram_used(void);
size_t hal_ram_free(void);
bool hal_radio_init(void);  // Set up the 2.4 GHz transceiver for 802.15.4
extern uint32_t SystemCoreClock;  // CPU clock, Hz. Set by hal_clock_init()


#define BIT(x) (1UL << (x))
#define CLRSET(reg, clear, set) ((reg) = ((reg) & ~(clear)) | (set))
#define SIRC_FREQUENCY 6000000UL  // Slow IRC (FRO6M), Hz

// MRCC peripheral clock source, MRCC_xx[MUX], see fsl_clock.h
enum { MRCC_SRC_FRO6M = 2, MRCC_SRC_FRO192M = 3 };

// Fast IRC frequency, Hz, or 0 if FIRC is not running. See CLOCK_GetFircFreq()
static inline uint32_t hal_firc_freq(void) {
  static const uint32_t freqs[] = {48000000, 64000000, 96000000, 192000000};
  if ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) == 0) return 0;
  return freqs[SCG0->FIRCCFG & SCG_FIRCCFG_RANGE_MASK];
}

// Switch the system clock: source (2 SIRC, 3 FIRC), core and slow dividers
// as divider - 1. Wait until the switch is done. See CLOCK_SetRunModeSysClkConfig
static inline void hal_set_sysclk(uint32_t src, uint32_t divcore,
                                  uint32_t divslow) {
  SCG0->RCCR = SCG_RCCR_SCS(src) | SCG_RCCR_DIVCORE(divcore) |
               SCG_RCCR_DIVSLOW(divslow);
  while (((SCG0->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) != src) (void) 0;
}

// Run the core at 96 MHz from FIRC, slow clock 24 MHz. Order as in NXP
// BOARD_BootClockRUN(), clock_config.c: raise the core voltage and memory
// timing first, then the clock. FIRC can only be changed when not in use
static inline void hal_clock_96mhz(void) {
  while (SPC0->SC & SPC_SC_BUSY_MASK) (void) 0;
  CLRSET(SPC0->ACTIVE_CFG, SPC_ACTIVE_CFG_CORELDO_VDD_LVL_MASK,
         SPC_ACTIVE_CFG_CORELDO_VDD_LVL(2));  // Normal voltage
  while (SPC0->SC & SPC_SC_BUSY_MASK) (void) 0;
  CLRSET(FMU0->FCTRL, FMU_FCTRL_RWSC_MASK, FMU_FCTRL_RWSC(2));  // Wait states
  CLRSET(SPC0->SRAMCTL, SPC_SRAMCTL_VSM_MASK, SPC_SRAMCTL_VSM(2));  // 1.1 V
  SPC0->SRAMCTL |= SPC_SRAMCTL_REQ_MASK;
  while ((SPC0->SRAMCTL & SPC_SRAMCTL_ACK_MASK) == 0) (void) 0;
  SPC0->SRAMCTL &= ~SPC_SRAMCTL_REQ_MASK;
  hal_set_sysclk(2, 0, 3);                 // Run from SIRC meanwhile
  SCG0->FIRCCSR &= ~SCG_FIRCCSR_LK_MASK;   // Unlock
  SCG0->FIRCCSR = SCG_FIRCCSR_FIRCERR_MASK;  // Disable, clear error
  SCG0->FIRCCFG = SCG_FIRCCFG_RANGE(2);    // 96 MHz
  SCG0->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;
  while ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) == 0) (void) 0;
  hal_set_sysclk(3, 0, 3);                 // FIRC, core / 1, slow / 4
}

// Set the CPU clock to 96 MHz, then work out SystemCoreClock from the SCG
// clock source and core divider
static inline void hal_clock_init(void) {
  uint32_t csr, freq = 0;
  hal_clock_96mhz();
  csr = SCG0->CSR;
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

// 802.15.4 (O-QPSK 250 kbps) radio register setup for the MCXW71 transceiver.
// Values are taken from the NXP XCVR driver, release/25.06.00, configs/gen45:
// XCVR_RadioGenRegSetup() and XCVR_RadioGenRBMESetup() with
// xcvr_oqpsk_802p15p4_250kbps_full_config and xcvr_ble_uncoded_config.
// https://github.com/nxp-mcuxpresso/mcuxsdk-middleware-xcvr
// Copyright 2016 Freescale Semiconductor, Inc.
// Copyright 2018-2023 NXP
// SPDX-License-Identifier: BSD-3-Clause

// One radio register write
struct w71_radio_reg {
  volatile uint32_t *reg;  // Register address
  uint32_t val;            // Value to write
};

// Register writes, in the order the NXP driver performs them
static const struct w71_radio_reg s_radio_regs[] = {
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[0].DMD_WAVE_REG0, 0x0420b3d3},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[0].DMD_WAVE_REG1, 0x31d78f00},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[0].DMD_WAVE_REG2, 0x00025a6d},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[1].DMD_WAVE_REG0, 0x0420b3d3},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[1].DMD_WAVE_REG1, 0x38d78f00},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[1].DMD_WAVE_REG2, 0x0000403c},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[2].DMD_WAVE_REG0, 0x3c00420b},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[2].DMD_WAVE_REG1, 0x082c8100},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[2].DMD_WAVE_REG2, 0x0003c004},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[3].DMD_WAVE_REG0, 0x3c00420b},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[3].DMD_WAVE_REG1, 0x0f2c8100},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[3].DMD_WAVE_REG2, 0x0001b5d3},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[4].DMD_WAVE_REG0, 0x0403ce35},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[4].DMD_WAVE_REG1, 0x31d78f00},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[4].DMD_WAVE_REG2, 0x00025a6d},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[5].DMD_WAVE_REG0, 0x0403ce35},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[5].DMD_WAVE_REG1, 0x38d78f00},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[5].DMD_WAVE_REG2, 0x0000403c},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[6].DMD_WAVE_REG0, 0x3ce35c6d},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[6].DMD_WAVE_REG1, 0x082c8100},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[6].DMD_WAVE_REG2, 0x0003c004},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[7].DMD_WAVE_REG0, 0x3ce35c6d},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[7].DMD_WAVE_REG1, 0x0f2c8100},
    {&XCVR_2P4GHZ_PHY->DEMOD_WAVE[7].DMD_WAVE_REG2, 0x0001b5d3},
    {&XCVR_2P4GHZ_PHY->DMD_CTRL0, 0x6d940101},
    {&XCVR_2P4GHZ_PHY->DMD_CTRL1, 0xa8049008},
    {&XCVR_2P4GHZ_PHY->FSK_CFG0, 0x15001502},
    {&XCVR_2P4GHZ_PHY->FSK_CFG1, 0x0f080020},
    {&XCVR_2P4GHZ_PHY->FSK_CFG2, 0x40000000},
    {&XCVR_2P4GHZ_PHY->FSK_FAD_CFG, 0x00000000},
    {&XCVR_2P4GHZ_PHY->FSK_FAD_CTRL, 0x00000000},
    {&XCVR_2P4GHZ_PHY->FSK_PD_CFG0, 0x0000ff0c},
    {&XCVR_2P4GHZ_PHY->FSK_PD_CFG1, 0x00000055},
    {&XCVR_2P4GHZ_PHY->FSK_PD_CFG2, 0x00f600f6},
    {&XCVR_2P4GHZ_PHY->FSK_PD_PH[0], 0x00030503},
    {&XCVR_2P4GHZ_PHY->FSK_PD_PH[1], 0x003d3b3d},
    {&XCVR_2P4GHZ_PHY->FSK_PT, 0x00000048},
    {&XCVR_2P4GHZ_PHY->LR_AA_CFG, 0x06020696},
    {&XCVR_2P4GHZ_PHY->LR_PD_CFG, 0x0001019a},
    {&XCVR_2P4GHZ_PHY->LR_PD_PH[0], 0x3d383028},
    {&XCVR_2P4GHZ_PHY->LR_PD_PH[1], 0x20283038},
    {&XCVR_2P4GHZ_PHY->LR_PD_PH[2], 0x03081018},
    {&XCVR_2P4GHZ_PHY->LR_PD_PH[3], 0x20181008},
    {&XCVR_2P4GHZ_PHY->MISC, 0x03ff00a0},
    {&XCVR_2P4GHZ_PHY->PREPHY_MISC, 0x00000c10},
    {&XCVR_2P4GHZ_PHY->RTT_CTRL, 0x00004166},
    {&XCVR_2P4GHZ_PHY->RTT_REF, 0x00806348},
    {&XCVR_2P4GHZ_PHY->SM_CFG, 0x00a02432},
    {&RADIO_CTRL->LL_CTRL, 0x00000001},
    {&XCVR_ANALOG->LDO_0, 0x04440400},
    {&XCVR_ANALOG->LDO_1, 0x00000005},
    {&XCVR_ANALOG->PLL, 0x00000040},
    {&XCVR_ANALOG->RX_0, 0x01000002},
    {&XCVR_ANALOG->RX_1, 0x00022008},
    {&XCVR_ANALOG->TX_DAC_PA, 0x01000000},
    {&XCVR_ANALOG->XO_DIST, 0x00000000},
    {&XCVR_MISC->IPS_FO_ADDR[0], 0x00000370},
    {&XCVR_MISC->IPS_FO_ADDR[1], 0x0000002c},
    {&XCVR_MISC->IPS_FO_DRS0_DATA[0], 0x00000000},
    {&XCVR_MISC->IPS_FO_DRS0_DATA[1], 0x00000000},
    {&XCVR_MISC->IPS_FO_DRS1_DATA[0], 0x00000000},
    {&XCVR_MISC->IPS_FO_DRS1_DATA[1], 0x00000000},
    {&XCVR_MISC->LDO_TRIM_0, 0x00020000},
    {&XCVR_MISC->LDO_TRIM_1, 0x00000000},
    {&XCVR_MISC->XCVR_CTRL, 0x00130180},
    {&XCVR_PLL_DIG->CHAN_MAP, 0x00040000},
    {&XCVR_PLL_DIG->CHAN_MAP_EXT, 0x00100000},
    {&XCVR_PLL_DIG->DATA_RATE_OVRD_CTRL1, 0x00000089},
    {&XCVR_PLL_DIG->DATA_RATE_OVRD_CTRL2, 0x00180000},
    {&XCVR_PLL_DIG->DELAY_MATCH, 0x00050102},
    {&XCVR_PLL_DIG->HPMCAL_CTRL, 0x00002000},
    {&XCVR_PLL_DIG->HPM_BUMP, 0x00440144},
    {&XCVR_PLL_DIG->HPM_CAL_TIMING, 0x00000000},
    {&XCVR_PLL_DIG->HPM_CTRL, 0x00840000},
    {&XCVR_PLL_DIG->HPM_SDM_RES, 0x00ff0000},
    {&XCVR_PLL_DIG->LOCK_DETECT, 0x0e1a1800},
    {&XCVR_PLL_DIG->LPM_CTRL, 0x88880a00},
    {&XCVR_PLL_DIG->LPM_SDM_CTRL1, 0x00260000},
    {&XCVR_PLL_DIG->MOD_CTRL, 0x80000000},
    {&XCVR_PLL_DIG->PLL_DATARATE_CTRL, 0x04126644},
    {&XCVR_PLL_DIG->TUNING_CAP_RX_CTRL, 0x00000000},
    {&XCVR_PLL_DIG->TUNING_CAP_TX_CTRL, 0x00ffffa3},
    {&XCVR_RX_DIG->ACQ_FILT_0_3, 0x737f0502},
    {&XCVR_RX_DIG->ACQ_FILT_0_3_DRS, 0x0f7f3a01},
    {&XCVR_RX_DIG->ACQ_FILT_10_11, 0x00ef0061},
    {&XCVR_RX_DIG->ACQ_FILT_10_11_DRS, 0x012b0021},
    {&XCVR_RX_DIG->ACQ_FILT_4_7, 0xf8221478},
    {&XCVR_RX_DIG->ACQ_FILT_4_7_DRS, 0x3c06617f},
    {&XCVR_RX_DIG->ACQ_FILT_8_9, 0x01d501bc},
    {&XCVR_RX_DIG->ACQ_FILT_8_9_DRS, 0x019401f3},
    {&XCVR_RX_DIG->AGC_CTRL, 0x587c8d19},
    {&XCVR_RX_DIG->AGC_CTRL_STAT, 0x0000872c},
    {&XCVR_RX_DIG->AGC_IDX0_GAIN_CFG, 0x000000f2},
    {&XCVR_RX_DIG->AGC_IDX0_GAIN_VAL, 0x000003fd},
    {&XCVR_RX_DIG->AGC_IDX0_THR, 0x0000001e},
    {&XCVR_RX_DIG->AGC_IDX10_GAIN_CFG, 0x00004a03},
    {&XCVR_RX_DIG->AGC_IDX10_GAIN_VAL, 0x000000eb},
    {&XCVR_RX_DIG->AGC_IDX10_THR, 0x0096001e},
    {&XCVR_RX_DIG->AGC_IDX11_GAIN_CFG, 0x00007e03},
    {&XCVR_RX_DIG->AGC_IDX11_GAIN_VAL, 0x000000fa},
    {&XCVR_RX_DIG->AGC_IDX11_THR, 0x00780000},
    {&XCVR_RX_DIG->AGC_IDX1_GAIN_CFG, 0x000001b2},
    {&XCVR_RX_DIG->AGC_IDX1_GAIN_VAL, 0x0000000b},
    {&XCVR_RX_DIG->AGC_IDX1_THR, 0x015e001e},
    {&XCVR_RX_DIG->AGC_IDX2_GAIN_CFG, 0x00000123},
    {&XCVR_RX_DIG->AGC_IDX2_GAIN_VAL, 0x00000026},
    {&XCVR_RX_DIG->AGC_IDX2_THR, 0x00c8001e},
    {&XCVR_RX_DIG->AGC_IDX3_GAIN_CFG, 0x00000113},
    {&XCVR_RX_DIG->AGC_IDX3_GAIN_VAL, 0x0000003c},
    {&XCVR_RX_DIG->AGC_IDX3_THR, 0x00c8001e},
    {&XCVR_RX_DIG->AGC_IDX4_GAIN_CFG, 0x00000083},
    {&XCVR_RX_DIG->AGC_IDX4_GAIN_VAL, 0x00000057},
    {&XCVR_RX_DIG->AGC_IDX4_THR, 0x00c8001e},
    {&XCVR_RX_DIG->AGC_IDX5_GAIN_CFG, 0x00000183},
    {&XCVR_RX_DIG->AGC_IDX5_GAIN_VAL, 0x0000006d},
    {&XCVR_RX_DIG->AGC_IDX5_THR, 0x00fa001e},
    {&XCVR_RX_DIG->AGC_IDX6_GAIN_CFG, 0x00000203},
    {&XCVR_RX_DIG->AGC_IDX6_GAIN_VAL, 0x00000087},
    {&XCVR_RX_DIG->AGC_IDX6_THR, 0x012c001e},
    {&XCVR_RX_DIG->AGC_IDX7_GAIN_CFG, 0x00000603},
    {&XCVR_RX_DIG->AGC_IDX7_GAIN_VAL, 0x000000a0},
    {&XCVR_RX_DIG->AGC_IDX7_THR, 0x012c001e},
    {&XCVR_RX_DIG->AGC_IDX8_GAIN_CFG, 0x00000e03},
    {&XCVR_RX_DIG->AGC_IDX8_GAIN_VAL, 0x000000b7},
    {&XCVR_RX_DIG->AGC_IDX8_THR, 0x012c001e},
    {&XCVR_RX_DIG->AGC_IDX9_GAIN_CFG, 0x00002203},
    {&XCVR_RX_DIG->AGC_IDX9_GAIN_VAL, 0x000000d2},
    {&XCVR_RX_DIG->AGC_IDX9_THR, 0x0096001e},
    {&XCVR_RX_DIG->AGC_MIS_GAIN_CFG, 0x00000001},
    {&XCVR_RX_DIG->AGC_THR_FAST, 0x0190000f},
    {&XCVR_RX_DIG->AGC_THR_FAST_DRS, 0x0190001e},
    {&XCVR_RX_DIG->AGC_THR_MIS, 0x00960028},
    {&XCVR_RX_DIG->AGC_TIMING0, 0x04102398},
    {&XCVR_RX_DIG->AGC_TIMING0_DRS, 0x40002000},
    {&XCVR_RX_DIG->AGC_TIMING1, 0x190242a7},
    {&XCVR_RX_DIG->AGC_TIMING1_DRS, 0x000002a7},
    {&XCVR_RX_DIG->AGC_TIMING2, 0xe000007f},
    {&XCVR_RX_DIG->AGC_TIMING2_DRS, 0x0000007f},
    {&XCVR_RX_DIG->CTRL0, 0x68063083},
    {&XCVR_RX_DIG->CTRL0_DRS, 0x000030c0},
    {&XCVR_RX_DIG->CTRL1, 0x00000180},
    {&XCVR_RX_DIG->DCOC_CTRL0, 0x04332d28},
    {&XCVR_RX_DIG->DCOC_CTRL0_DRS, 0x00000114},
    {&XCVR_RX_DIG->DCOC_CTRL1, 0x00000000},
    {&XCVR_RX_DIG->DEMOD_FILT_0_1, 0x01fe01fd},
    {&XCVR_RX_DIG->DEMOD_FILT_0_1_DRS, 0x00ff0003},
    {&XCVR_RX_DIG->DEMOD_FILT_2_4, 0x0ba20824},
    {&XCVR_RX_DIG->DEMOD_FILT_2_4_DRS, 0x124221ff},
    {&XCVR_RX_DIG->NB_RSSI_CTRL0, 0xed472234},
    {&XCVR_RX_DIG->NB_RSSI_CTRL1, 0x00000000},
    {&XCVR_RX_DIG->RCCAL_CTRL0, 0x0000003a},
    {&XCVR_RX_DIG->RSSI_GLOBAL_CTRL, 0x80008330},
    {&XCVR_RX_DIG->WB_RSSI_CTRL, 0x00223342},
    {&XCVR_TSM->CTRL, 0xff000000},
    {&XCVR_TSM->END_OF_SEQ, 0x5c5a7270},
    {&XCVR_TSM->FAST_CTRL1, 0x24006f00},
    {&XCVR_TSM->FAST_CTRL2, 0x66284208},
    {&XCVR_TSM->FAST_CTRL3, 0x18001800},
    {&XCVR_TSM->RECYCLE_COUNT, 0x00001258},
    {&XCVR_TSM->TIMING00, 0xffffffff},
    {&XCVR_TSM->TIMING01, 0xffffffff},
    {&XCVR_TSM->TIMING02, 0xffffffff},
    {&XCVR_TSM->TIMING03, 0xffffffff},
    {&XCVR_TSM->TIMING04, 0xffffffff},
    {&XCVR_TSM->TIMING05, 0xffffffff},
    {&XCVR_TSM->TIMING06, 0xffffffff},
    {&XCVR_TSM->TIMING07, 0xffffffff},
    {&XCVR_TSM->TIMING08, 0xffffffff},
    {&XCVR_TSM->TIMING09, 0x5700ffff},
    {&XCVR_TSM->TIMING10, 0x11081108},
    {&XCVR_TSM->TIMING11, 0x5b137113},
    {&XCVR_TSM->TIMING12, 0x5b19714f},
    {&XCVR_TSM->TIMING13, 0x572dffff},
    {&XCVR_TSM->TIMING14, 0x0000716f},
    {&XCVR_TSM->TIMING15, 0x5b58716f},
    {&XCVR_TSM->TIMING16, 0x59580000},
    {&XCVR_TSM->TIMING17, 0x5b580000},
    {&XCVR_TSM->TIMING18, 0x5b580000},
    {&XCVR_TSM->TIMING19, 0x11061106},
    {&XCVR_TSM->TIMING20, 0x11001100},
    {&XCVR_TSM->TIMING21, 0x19001100},
    {&XCVR_TSM->TIMING22, 0x58006f00},
    {&XCVR_TSM->TIMING23, 0x58006f00},
    {&XCVR_TSM->TIMING24, 0x5b007100},
    {&XCVR_TSM->TIMING25, 0x5c007200},
    {&XCVR_TSM->TIMING26, 0x5c007200},
    {&XCVR_TSM->TIMING27, 0x5c007200},
    {&XCVR_TSM->TIMING28, 0x5c007200},
    {&XCVR_TSM->TIMING29, 0xffff7200},
    {&XCVR_TSM->TIMING30, 0x5c00ffff},
    {&XCVR_TSM->TIMING31, 0x5b087108},
    {&XCVR_TSM->TIMING32, 0x1911ffff},
    {&XCVR_TSM->TIMING33, 0x23115a11},
    {&XCVR_TSM->TIMING34, 0x5b117111},
    {&XCVR_TSM->TIMING35, 0x5b117111},
    {&XCVR_TSM->TIMING36, 0x5b117111},
    {&XCVR_TSM->TIMING37, 0x5b117111},
    {&XCVR_TSM->TIMING38, 0x5b117111},
    {&XCVR_TSM->TIMING39, 0x5b2b7111},
    {&XCVR_TSM->TIMING40, 0xffff7111},
    {&XCVR_TSM->TIMING41, 0xffff7111},
    {&XCVR_TSM->TIMING42, 0x5b127112},
    {&XCVR_TSM->TIMING43, 0x5b12ffff},
    {&XCVR_TSM->TIMING44, 0x5b12ffff},
    {&XCVR_TSM->TIMING45, 0xffff7112},
    {&XCVR_TSM->TIMING46, 0xffff716f},
    {&XCVR_TSM->TIMING47, 0x5b137157},
    {&XCVR_TSM->TIMING48, 0x5b23715a},
    {&XCVR_TSM->TIMING49, 0x5b23715a},
    {&XCVR_TSM->TIMING50, 0x5b2bffff},
    {&XCVR_TSM->TIMING51, 0x5b2bffff},
    {&XCVR_TSM->TIMING52, 0xffffffff},
    {&XCVR_TSM->WU_LATENCY, 0x00060006},
    {&XCVR_TX_DIG->DATARATE_CONFIG_FILTER_CTRL, 0x00010000},
    {&XCVR_TX_DIG->DATARATE_CONFIG_FSK_CTRL, 0x08001800},
    {&XCVR_TX_DIG->DATARATE_CONFIG_GFSK_CTRL, 0x00000804},
    {&XCVR_TX_DIG->DATA_PADDING_CTRL, 0x00001012},
    {&XCVR_TX_DIG->DATA_PADDING_CTRL_1, 0x00001600},
    {&XCVR_TX_DIG->DATA_PADDING_CTRL_2, 0x08001800},
    {&XCVR_TX_DIG->FSK_CTRL, 0x04001c00},
    {&XCVR_TX_DIG->GFSK_COEFF_0_1, 0x00040001},
    {&XCVR_TX_DIG->GFSK_COEFF_2_3, 0x0028000d},
    {&XCVR_TX_DIG->GFSK_COEFF_4_5, 0x00c00063},
    {&XCVR_TX_DIG->GFSK_COEFF_6_7, 0x0176012c},
    {&XCVR_TX_DIG->GFSK_CTRL, 0x000003fd},
    {&XCVR_TX_DIG->IMAGE_FILTER_CTRL, 0x00000000},
    {&XCVR_TX_DIG->PA_CTRL, 0x00020126},
    {&XCVR_TX_DIG->PA_RAMP_TBL0, 0x06040201},
    {&XCVR_TX_DIG->PA_RAMP_TBL1, 0x14100c09},
    {&XCVR_TX_DIG->PA_RAMP_TBL2, 0x26211c18},
    {&XCVR_TX_DIG->PA_RAMP_TBL3, 0x3c38322c},
    {&XCVR_TX_DIG->SWITCH_TX_CTRL, 0x00000000},
    {&XCVR_TX_DIG->TXDIG_CTRL, 0x00000007},
    {&XCVR_2P4GHZ_PHY->DMD_CTRL1, 0x8c049008},
    {&RBME->CRCW_CFG, 0x00000001},
    {&RBME->CRCW_CFG2, 0x00000000},
    {&RBME->CRCW_CFG3, 0x00000403},
    {&RBME->CRC_INIT, 0x55555500},
    {&RBME->CRC_PHR_SZ, 0x00000002},
    {&RBME->CRC_POLY, 0x00065b00},
    {&RBME->CRC_XOR_OUT, 0x00000000},
    {&RBME->FCP_CFG, 0x00000000},
    {&RBME->FEC_BSZ_OV_B4SP, 0x00000000},
    {&RBME->FEC_CFG1, 0x00000000},
    {&RBME->FEC_CFG2, 0x00000000},
    {&RBME->FRAME_OVER_SZ, 0x00000000},
    {&RBME->NPAYL_OVER_SZ, 0x00000000},
    {&RBME->PKT_SZ, 0x00244000},
    {&RBME->SPREAD_CFG, 0x00000000},
    {&RBME->WHITEN_CFG, 0x00410705},
    {&RBME->WHITEN_POLY, 0x00000004},
    {&RBME->WHITEN_SZ_THR, 0x00000000},
};
