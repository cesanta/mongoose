// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"

#if MG_RADIO == MG_RADIO_BLE
// NXP release/25.06.00, gen45: gfsk_bt_0p5_h_0p5_1mbps_full_config.
// The alternate data-rate bank is 2 Mbit/s, selected by GENFSK ENH_FEATURE.
// Copyright 2016 Freescale Semiconductor, Inc.; 2018-2023 NXP.
// SPDX-License-Identifier: BSD-3-Clause
// These are the differences from the 802.15.4 setup in hal.h. Both use
// xcvr_ble_uncoded_config for RBME (CRC and whitening).
static const struct w71_radio_reg s_ble_regs[] = {
    {&XCVR_MISC->IPS_FO_ADDR[0], 0x0000332c},
    {&XCVR_MISC->IPS_FO_ADDR[1], 0x00000034},
    {&XCVR_MISC->IPS_FO_DRS0_DATA[0], 0x20ff0000},
    {&XCVR_MISC->IPS_FO_DRS1_DATA[0], 0x10ff0000},
    {&XCVR_MISC->XCVR_CTRL, 0x00030140},
    {&XCVR_PLL_DIG->CHAN_MAP, 0x00000000},
    {&XCVR_PLL_DIG->DELAY_MATCH, 0x00040103},
    {&XCVR_PLL_DIG->HPMCAL_CTRL, 0x00000000},
    {&XCVR_PLL_DIG->HPM_BUMP, 0x00440122},
    {&XCVR_PLL_DIG->HPM_SDM_RES, 0x20ff0000},
    {&XCVR_RX_DIG->ACQ_FILT_0_3, 0x757a0202},
    {&XCVR_RX_DIG->ACQ_FILT_0_3_DRS, 0x09023e3e},
    {&XCVR_RX_DIG->ACQ_FILT_10_11, 0x00de0068},
    {&XCVR_RX_DIG->ACQ_FILT_10_11_DRS, 0x00a1006e},
    {&XCVR_RX_DIG->ACQ_FILT_4_7, 0xeb171801},
    {&XCVR_RX_DIG->ACQ_FILT_4_7_DRS, 0xdeeb000b},
    {&XCVR_RX_DIG->ACQ_FILT_8_9, 0x01e901c2},
    {&XCVR_RX_DIG->ACQ_FILT_8_9_DRS, 0x002601f0},
    {&XCVR_RX_DIG->AGC_IDX11_GAIN_CFG, 0x02047e03},
    {&XCVR_RX_DIG->AGC_IDX11_GAIN_VAL, 0x006014fa},
    {&XCVR_RX_DIG->CTRL0, 0x60063083},
    {&XCVR_RX_DIG->CTRL0_DRS, 0x000020c0},
    {&XCVR_RX_DIG->DEMOD_FILT_0_1_DRS, 0x01fe01fd},
    {&XCVR_RX_DIG->DEMOD_FILT_2_4, 0x0bc20c24},
    {&XCVR_RX_DIG->DEMOD_FILT_2_4_DRS, 0x0bc20c23},
    {&XCVR_RX_DIG->RCCAL_CTRL0, 0x0000001a},
    {&XCVR_RX_DIG->RSSI_GLOBAL_CTRL, 0x80008310},
    {&XCVR_TX_DIG->DATARATE_CONFIG_FSK_CTRL, 0x00000000},
    {&XCVR_TX_DIG->DATARATE_CONFIG_GFSK_CTRL, 0x00000830},
    {&XCVR_TX_DIG->DATA_PADDING_CTRL, 0x00001010},
    {&XCVR_TX_DIG->DATA_PADDING_CTRL_1, 0x00001404},
    {&XCVR_TX_DIG->DATA_PADDING_CTRL_2, 0x04001c00},
    {&XCVR_TX_DIG->GFSK_CTRL, 0x00000400},
    {&XCVR_TX_DIG->PA_CTRL, 0x00010126},
    {&XCVR_TX_DIG->TXDIG_CTRL, 0x00000000},
};
#endif

// Initialise the transceiver the way NXP XCVR_Init() does it: start the RF
// oscillator, wait for the radio to wake up, enable its clocks and load the
// register setup for the selected PHY. For 802.15.4, select the legacy
// demodulator and CCA from RX digital. See PhyHwInit() in NXP ieee_802_15_4 PHY
// sources, PhyPacketProcessor.c
void hal_radio_init(void) {
  uint32_t stat =
      RFMC_RF2P4GHZ_STAT_RST_STAT_MASK | RFMC_RF2P4GHZ_STAT_LP_ACK_STAT_MASK;
  size_t i, n = 1000000;
  RFMC->CTRL &= ~RFMC_CTRL_RFMC_RST_MASK;                // RFMC out of reset
  RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_XO_EN_MASK;  // Start RF oscillator
  RFMC->RF2P4GHZ_CTRL &=
      ~(RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK | RFMC_RF2P4GHZ_CTRL_RF_POR_MASK |
        RFMC_RF2P4GHZ_CTRL_RST_MASK);  // Release radio reset, no low power
  while ((RFMC->RF2P4GHZ_STAT & stat) != 0 ||
         (RFMC->XO_STAT & RFMC_XO_STAT_XTAL_RDY_MASK) == 0) {
    if (--n == 0) {
      MG_ERROR(("Radio init failed"));  // Radio did not wake up
      return;
    }
  }
  RADIO_CTRL->LL_CTRL = RADIO_CTRL_LL_CTRL_ACTIVE_LL_MASK;
  RFMC->RF2P4GHZ_CTRL |=
      RFMC_RF2P4GHZ_CTRL_BLE_LP_EN_MASK | RFMC_RF2P4GHZ_CTRL_BLE_WKUP_MASK;
  for (i = 0; i < sizeof(s_radio_regs) / sizeof(s_radio_regs[0]); i++) {
    *s_radio_regs[i].reg = s_radio_regs[i].val;
  }
#if MG_RADIO == MG_RADIO_BLE
  for (i = 0; i < sizeof(s_ble_regs) / sizeof(s_ble_regs[0]); i++) {
    *s_ble_regs[i].reg = s_ble_regs[i].val;
  }
  RADIO_CTRL->LL_CTRL = RADIO_CTRL_LL_CTRL_ACTIVE_LL(2);  // GENFSK engine
  // NXP GENFSK_SetBleWhitenInit(): BLE channel index with bit 6 set.
  RBME->WHITEN_CFG = (RBME->WHITEN_CFG & ~RBME_WHITEN_CFG_WHITEN_INIT_MASK) |
                     RBME_WHITEN_CFG_WHITEN_INIT(MG_RADIO_CHANNEL | 0x40);
#else
  XCVR_MISC->XCVR_CTRL &= ~XCVR_MISC_XCVR_CTRL_DEMOD_SEL_MASK;
  XCVR_MISC->XCVR_CTRL |= XCVR_MISC_XCVR_CTRL_DEMOD_SEL(2);
  XCVR_ZBDEMOD->CCA_LQI_SRC |= XCVR_ZBDEMOD_CCA_LQI_SRC_CCA1_FROM_RX_DIG(1);
#endif
}

// Random 32 bits, for mg_random(). Entropy is radio noise: receiver RSSI,
// sampled while the radio receives, mixed with SysTick by SHA-256. One hash
// fills a pool of 8 words. Good enough for random addresses and nonces, not
// a certified TRNG
uint32_t hal_rng_read(void) {
  static uint32_t counter, pool[8];
  static size_t avail;
  size_t i, n;
  if (avail == 0) {
    mg_sha256_ctx ctx;
    mg_sha256_init(&ctx);
    counter++;
    mg_sha256_update(&ctx, (uint8_t *) &counter, sizeof(counter));
    for (i = 0; i < 64; i++) {
      uint32_t sample[2] = {XCVR_RX_DIG->NB_RSSI_RES0, SysTick->VAL};
      mg_sha256_update(&ctx, (uint8_t *) sample, sizeof(sample));
      for (n = 0; n < 50; n++) __NOP();  // Let the RSSI update
    }
    mg_sha256_final((uint8_t *) pool, &ctx);
    avail = sizeof(pool) / sizeof(pool[0]);
  }
  return pool[--avail];
}

// Added to minimise the bin size: avoid pulling printf and friends
void __assert_func(const char *s, int line, const char *fn, const char *expr) {
  (void) s, (void) line, (void) fn, (void) expr;
  for (;;) (void) 0;  // or trigger the fault handler
}

bool hal_timer_expired(volatile uint64_t *t, uint64_t period, uint64_t now) {
  uint64_t diff = now - *t;         // Wrap-safe elapsed time since last expiry
  if (period == 0) return false;    // Avoid division by zero
  if (diff < period) return false;  // Period has not elapsed yet
  *t += (diff / period) * period;   // Preserve cadence, skip missed periods
  return true;
}

static volatile uint64_t s_ticks;  // Milliseconds since boot
void SysTick_Handler(void) {       // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

uint64_t hal_get_tick(void) {
  return s_ticks;
}

uint64_t mg_micros(void);
// SysTick runs at the core clock and reloads every millisecond. Sample
// the tick counter and the sub-millisecond counter in one critical section.
uint64_t mg_micros(void) {
  uint32_t primask = __get_PRIMASK(), val, pending;
  uint64_t ms;
  __disable_irq();
  ms = hal_get_tick();
  pending = SCB->ICSR & SCB_ICSR_PENDSTSET_Msk;
  val = SysTick->VAL;
  if (pending != (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)) {
    pending = SCB_ICSR_PENDSTSET_Msk;
    val = SysTick->VAL;
  }
  __set_PRIMASK(primask);
  if (pending != 0 && val != 0) ms++;
  return ms * 1000 +
         ((uint64_t) (SysTick->LOAD - val) * 1000) / (SysTick->LOAD + 1);
}

uint32_t SystemCoreClock;

// Called by the startup code before .data and .bss are initialised, so it
// must not write any variables. Clock and SysTick setup is in main()
void SystemInit(void) {
  hal_sys_init();
}

struct stat;
__attribute__((weak)) int _fstat(int fd, struct stat *st) {
  (void) fd, (void) st;
  return -1;
}

extern unsigned char _end[];  // End of data section, start of heap. See link.ld
static unsigned char *s_current_heap_end = _end;

size_t hal_ram_used(void) {
  return (size_t) (s_current_heap_end - _end);
}

size_t hal_ram_free(void) {
  unsigned char endofstack;
  return (size_t) (&endofstack - s_current_heap_end);
}

void *_sbrk(int incr) {
  unsigned char *prev_heap;
  unsigned char *heap_end = (unsigned char *) ((size_t) &heap_end - 256);
  prev_heap = s_current_heap_end;
  // Check how much space we  got from the heap end to the stack end
  if (s_current_heap_end + incr > heap_end) return (void *) -1;
  s_current_heap_end += incr;
  return prev_heap;
}

__attribute__((weak)) int _open(const char *path) {
  (void) path;
  return -1;
}

__attribute__((weak)) int _close(int fd) {
  (void) fd;
  return -1;
}

__attribute__((weak)) int _isatty(int fd) {
  (void) fd;
  return 1;
}

__attribute__((weak)) int _lseek(int fd, int ptr, int dir) {
  (void) fd, (void) ptr, (void) dir;
  return 0;
}

__attribute__((weak)) void _exit(int status) {
  (void) status;
  for (;;) asm volatile("BKPT #0");
}

__attribute__((weak)) void _kill(int pid, int sig) {
  (void) pid, (void) sig;
}

__attribute__((weak)) int _getpid(void) {
  return -1;
}

__attribute__((weak)) int _write(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  return -1;
}

__attribute__((weak)) int _read(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  return -1;
}

__attribute__((weak)) void _init(void) {
}
