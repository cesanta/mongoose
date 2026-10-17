// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"

// Initialise the transceiver the way NXP XCVR_Init() does it: start the RF
// oscillator, wait for the radio to wake up, enable its clocks and load the
// 802.15.4 register setup. Then select the legacy 802.15.4 demodulator and
// CCA from RX digital. See PhyHwInit() in NXP ieee_802_15_4 PHY sources,
// PhyPacketProcessor.c
bool hal_radio_init(void) {
  uint32_t stat = RFMC_RF2P4GHZ_STAT_RST_STAT_MASK |
                  RFMC_RF2P4GHZ_STAT_LP_ACK_STAT_MASK;
  size_t i, n = 1000000;
  RFMC->CTRL &= ~RFMC_CTRL_RFMC_RST_MASK;                // RFMC out of reset
  RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_XO_EN_MASK;  // Start RF oscillator
  RFMC->RF2P4GHZ_CTRL &=
      ~(RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK | RFMC_RF2P4GHZ_CTRL_RF_POR_MASK |
        RFMC_RF2P4GHZ_CTRL_RST_MASK);  // Release radio reset, no low power
  while ((RFMC->RF2P4GHZ_STAT & stat) != 0 ||
         (RFMC->XO_STAT & RFMC_XO_STAT_XTAL_RDY_MASK) == 0) {
    if (--n == 0) return false;  // Radio did not wake up
  }
  RADIO_CTRL->LL_CTRL = RADIO_CTRL_LL_CTRL_ACTIVE_LL_MASK;
  RFMC->RF2P4GHZ_CTRL |=
      RFMC_RF2P4GHZ_CTRL_BLE_LP_EN_MASK | RFMC_RF2P4GHZ_CTRL_BLE_WKUP_MASK;
  for (i = 0; i < sizeof(s_radio_regs) / sizeof(s_radio_regs[0]); i++) {
    *s_radio_regs[i].reg = s_radio_regs[i].val;
  }
  XCVR_MISC->XCVR_CTRL &= ~XCVR_MISC_XCVR_CTRL_DEMOD_SEL_MASK;
  XCVR_MISC->XCVR_CTRL |= XCVR_MISC_XCVR_CTRL_DEMOD_SEL(2);
  XCVR_ZBDEMOD->CCA_LQI_SRC |= XCVR_ZBDEMOD_CCA_LQI_SRC_CCA1_FROM_RX_DIG(1);
  return true;
}

// Random bytes for Mongoose, MG_ENABLE_CUSTOM_RANDOM. Entropy is radio noise:
// receiver RSSI, sampled while the radio receives, mixed with SysTick by
// SHA-256. Good enough for random addresses and nonces, not a certified TRNG
bool mg_random(void *buf, size_t len) {
  static uint32_t counter;
  uint8_t digest[32], *p = (uint8_t *) buf;
  size_t i, n;
  while (len > 0) {
    mg_sha256_ctx ctx;
    mg_sha256_init(&ctx);
    counter++;
    mg_sha256_update(&ctx, (uint8_t *) &counter, sizeof(counter));
    for (i = 0; i < 64; i++) {
      uint32_t sample[2] = {XCVR_RX_DIG->NB_RSSI_RES0, SysTick->VAL};
      mg_sha256_update(&ctx, (uint8_t *) sample, sizeof(sample));
      for (n = 0; n < 50; n++) __NOP();  // Let the RSSI update
    }
    mg_sha256_final(digest, &ctx);
    n = len < sizeof(digest) ? len : sizeof(digest);
    memcpy(p, digest, n), p += n, len -= n;
  }
  return true;
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
