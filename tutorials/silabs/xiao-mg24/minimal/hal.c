// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"

// Secure Engine data descriptor, sli_se_datatransfer_t in Simplicity SDK
struct se_dt {
  volatile void *volatile data;  // Data buffer
  volatile void *volatile next;  // Next descriptor, or 1: last one
  volatile uint32_t length;      // Length in bytes, plus flags
};

#define SE_CMD_TRNG_GET_RANDOM 0x07000000UL  // SE mailbox commands
#define SE_DT_STOP 1UL                       // Descriptor flags
#define SE_DT_REALIGN 0x20000000UL
#define SE_RESPONSE_MASK 0x000F0000UL  // RX_HEADER response code, 0: OK

// Fill buf with n random bytes, n a multiple of 4, from the Secure Engine
// TRNG (RM section 11.2.5), through the SE mailbox, as sl_se_get_random()
// and sli_se_mailbox_execute_command() in Simplicity SDK. buf and the
// descriptor are static: on the stack, at the top of RAM, the SE reported a
// bus error. Return false on SE error
static bool se_random(uint32_t *buf, uint32_t n) {
  static struct se_dt out;
  out.data = buf, out.next = (void *) SE_DT_STOP;
  out.length = n | SE_DT_REALIGN;
  while ((SEMAILBOX_HOST->TX_STATUS & SEMAILBOX_TX_STATUS_TXINT) == 0) (void) 0;
  SEMAILBOX_HOST->TX_HEADER = sizeof(uint32_t) * 5;  // 4 words + 1 parameter
  SEMAILBOX_HOST->FIFO = SE_CMD_TRNG_GET_RANDOM;
  SEMAILBOX_HOST->FIFO = 0;                // No input data
  SEMAILBOX_HOST->FIFO = (uint32_t) &out;  // Output data
  SEMAILBOX_HOST->FIFO = n;                // Parameter: number of bytes
  while ((SEMAILBOX_HOST->RX_STATUS & SEMAILBOX_RX_STATUS_RXINT) == 0) (void) 0;
  return (SEMAILBOX_HOST->RX_HEADER & SE_RESPONSE_MASK) == 0;
}

// Random 32 bits, for mg_random(). One SE request fills a pool of 16 words.
// If the SE fails 3 times in a row, the pool is zeros: see se_random()
uint32_t hal_rng_read(void) {
  static uint32_t pool[16];
  static size_t avail;
  if (avail == 0) {
    int i;
    for (i = 0; i < 3 && !se_random(pool, sizeof(pool)); i++) (void) 0;
    if (i == 3) memset(pool, 0, sizeof(pool));
    avail = sizeof(pool) / sizeof(pool[0]);
  }
  return pool[--avail];
}

// RAIL library needs these Simplicity SDK functions. Implemented as in the
// SDK: sl_core_cortexm.c, em_cmu.c, em_emu.c, em_system.c, sl_gpio.c,
// pa_conversions_efr32.c. Atomic sections mask IRQs with priority number 3
// and above, see hal_radio_init() for the IRQ priorities
#define CORE_ATOMIC_BASE_PRIORITY_LEVEL 3

uint32_t CORE_EnterCritical(void) {
  uint32_t state = __get_PRIMASK();
  __disable_irq();
  return state;
}

void CORE_ExitCritical(uint32_t state) {
  if (state == 0) __enable_irq(), __ISB();
}

uint32_t CORE_EnterAtomic(void) {
  uint32_t state = __get_BASEPRI();
  __set_BASEPRI(CORE_ATOMIC_BASE_PRIORITY_LEVEL << (8U - __NVIC_PRIO_BITS));
  return state;
}

void CORE_ExitAtomic(uint32_t state) {
  __set_BASEPRI(state);
  __ISB();
}

// Clock IDs: bits 0..4 bit number, bits 5..6 register, 1 CLKEN0, 2 CLKEN1
void CMU_ClockEnable(uint32_t clock, bool enable) {
  volatile uint32_t *reg = (clock >> 5) == 1   ? &CMU->CLKEN0
                           : (clock >> 5) == 2 ? &CMU->CLKEN1
                                               : NULL;
  if (reg == NULL) return;
  if (enable) {
    *reg |= BIT(clock & 31);
  } else {
    *reg &= ~BIT(clock & 31);
  }
}

void EMU_DCDCSetPFMXTimeoutMaxCtrl(uint32_t value) {
  bool clk_was_on = (CMU->CLKEN0 & CMU_CLKEN0_DCDC) != 0;
  bool was_locked;
  CMU->CLKEN0_SET = CMU_CLKEN0_DCDC;
  was_locked = (DCDC->LOCKSTATUS & DCDC_LOCKSTATUS_LOCK) != 0;
  DCDC->LOCK = DCDC_LOCK_LOCKKEY_UNLOCKKEY;
  while (DCDC->SYNCBUSY & DCDC_SYNCBUSY_PFMXCTRL) (void) 0;
  DCDC->PFMXCTRL = (DCDC->PFMXCTRL & ~_DCDC_PFMXCTRL_IPKTMAXCTRL_MASK) |
                   (value << _DCDC_PFMXCTRL_IPKTMAXCTRL_SHIFT);
  if (was_locked) DCDC->LOCK = ~DCDC_LOCK_LOCKKEY_UNLOCKKEY;
  if (!clk_was_on) CMU->CLKEN0_CLR = CMU_CLKEN0_DCDC;
}

// SYSTEM_ChipRevision_TypeDef, em_system.h
struct chip_rev {
  uint8_t minor;   // Minor revision number
  uint8_t major;   // Major revision number
  uint8_t family;  // Device family number
};

void SYSTEM_ChipRevisionGet(struct chip_rev *rev) {
  uint32_t chiprev;
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
  chiprev = SYSCFG->CHIPREV;
  rev->family = (chiprev & _SYSCFG_CHIPREV_FAMILY_MASK) >>
                _SYSCFG_CHIPREV_FAMILY_SHIFT;
  rev->major = (chiprev & _SYSCFG_CHIPREV_MAJOR_MASK) >>
               _SYSCFG_CHIPREV_MAJOR_SHIFT;
  rev->minor = (chiprev & _SYSCFG_CHIPREV_MINOR_MASK) >>
               _SYSCFG_CHIPREV_MINOR_SHIFT;
}

// sl_gpio_t, sl_device_gpio.h
struct sl_gpio {
  uint8_t port;  // GPIO port number
  uint8_t pin;   // GPIO pin number
};

uint32_t sl_gpio_set_pin(const struct sl_gpio *gpio) {
  GPIO->P_SET[gpio->port].DOUT = BIT(gpio->pin);
  return 0;
}

uint32_t sl_gpio_clear_pin(const struct sl_gpio *gpio) {
  GPIO->P_CLR[gpio->port].DOUT = BIT(gpio->pin);
  return 0;
}

// We set TX power as a raw PA level and load no PA curves. RAIL calls this to
// check per-channel dBm limits: RAIL_TX_POWER_MAX means no limit, 255. Without
// curves the SDK returns 0, see RAIL_ConvertDbmToRaw(), pa_conversions_efr32.c
uint8_t RAIL_ConvertDbmToRaw(void *rail, int mode, int16_t power) {
  (void) rail, (void) mode;
  return power >= 0x7fff ? 255 : 0;  // 0x7fff: RAIL_TX_POWER_MAX
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
  return ms * 1000 + ((uint64_t) (SysTick->LOAD - val) * 1000) /
                         (SysTick->LOAD + 1);
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
