// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include <sys/stat.h>  // For _fstat()
#include "hal.h"

extern void SystemCoreClockUpdate(void);

void SysTick_Init(void) {
  SysTick->CMP = SystemCoreClock / 1000 - 1;
  SysTick->CNT = 0;
  SysTick->SR = 0;
  SysTick->CTLR = BIT(0) | BIT(1) | BIT(2) | BIT(3);
  NVIC_EnableIRQ(SysTicK_IRQn);
}

#if 0
void SystemInit(void) {  // Called automatically by startup code
  SystemCoreClockUpdate();
}
#endif

static volatile uint64_t s_ticks;  // Milliseconds since boot
__attribute__((interrupt())) void SysTick_Handler(void) {
  s_ticks++;
  SysTick->SR = 0;
}

void mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

void hal_init(void) {
  SystemCoreClockUpdate();
  SysTick_Init();
  uart_init(UART_DEBUG, 115200);
  gpio_output(LED_PIN);
  ethernet_init();
  rng_init();
}

// Newlib syscalls. Implemented are: _sbrk() for malloc, and _write()
int _fstat(int fd, struct stat *st) {
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
  for (;;) (void) 0;
}

void _kill(int pid, int sig) {
  (void) pid, (void) sig;
}

int _getpid(void) {
  return -1;
}

int _write(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  if (fd == 1) uart_write_buf(UART_DEBUG, ptr, (size_t) len);
  return -1;
}

int _read(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  return -1;
}

int _link(const char *a, const char *b) {
  (void) a, (void) b;
  return -1;
}

int _unlink(const char *a) {
  (void) a;
  return -1;
}

int _stat(const char *path, struct stat *st) {
  (void) path, (void) st;
  return -1;
}

int mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}

void _init(void) {
}
