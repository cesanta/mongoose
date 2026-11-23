// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include <string.h>

static volatile uint64_t s_ticks;  // Milliseconds since boot
void SysTick_Handler(void) {       // SysTick IRQ handler, triggered every 1ms
  s_ticks++;
}

uint64_t hal_get_tick(void) {
  return s_ticks;
}

// Mongoose microsecond time source for the mesh, see MG_ENABLE_CUSTOM_MICROS.
// SysTick runs at the core clock and reloads every millisecond. Sample the
// tick counter and the sub-millisecond counter in one critical section
uint64_t mg_micros(void) {
  uint32_t primask = __get_PRIMASK(), val, pending;
  uint64_t ms;
  __disable_irq();
  ms = s_ticks;
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

// Mongoose random source, see MG_ENABLE_CUSTOM_RANDOM
bool mg_random(void *buf, size_t len) {
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = hal_rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
}

// Avoid pulling printf and friends in through assert()
void __assert_func(const char *s, int line, const char *fn, const char *expr) {
  (void) s, (void) line, (void) fn, (void) expr;
  for (;;) (void) 0;
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
  // Check how much space we got from the heap end to the stack end
  if (s_current_heap_end + incr > heap_end) return (void *) -1;
  s_current_heap_end += incr;
  return prev_heap;
}

// Newlib stubs
struct stat;
int _fstat(int fd, struct stat *st) {
  (void) fd, (void) st;
  return -1;
}

int _open(const char *path) {
  (void) path;
  return -1;
}

int _close(int fd) {
  (void) fd;
  return -1;
}

int _isatty(int fd) {
  (void) fd;
  return 1;
}

int _lseek(int fd, int ptr, int dir) {
  (void) fd, (void) ptr, (void) dir;
  return 0;
}

void _exit(int status) {
  (void) status;
  for (;;) asm volatile("BKPT #0");
}

void _kill(int pid, int sig) {
  (void) pid, (void) sig;
}

int _getpid(void) {
  return -1;
}

int _write(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  return -1;
}

int _read(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  return -1;
}

void _init(void) {
}
