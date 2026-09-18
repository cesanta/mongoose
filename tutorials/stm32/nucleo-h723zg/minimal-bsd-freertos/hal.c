// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"

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
};

uint32_t SystemCoreClock = 160000000;
void SystemInit(void) {  // Called automatically by startup code
  hal_system_init();     // Enable FPU
  hal_clock_init();
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}

void ExitRun0Mode(void) {
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

__attribute__((weak)) int _link(const char *a, const char *b) {
  (void) a, (void) b;
  return -1;
}

__attribute__((weak)) int _unlink(const char *a) {
  (void) a;
  return -1;
}

__attribute__((weak)) int _stat(const char *path, struct stat *st) {
  (void) path, (void) st;
  return -1;
}

__attribute__((weak)) int mkdir(const char *path, int mode) {
  (void) path, (void) mode;
  return -1;
}

__attribute__((weak)) void _init(void) {
}
