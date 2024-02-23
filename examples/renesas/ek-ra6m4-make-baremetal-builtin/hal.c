// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include <stdint.h>
#include <string.h>
#include <sys/stat.h>  // For _fstat()

#include "hal.h"

uint32_t SystemCoreClock = SYS_FREQUENCY;
static volatile uint64_t s_ticks;  // Milliseconds since boot

extern void _estack(void);  // Defined in link.ld
void Reset_Handler(void);
void SysTick_Handler(void);
extern void EDMAC_IRQHandler(void);

// 16 ARM and 200 peripheral handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 10])(void) = {
    _estack,         Reset_Handler,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    SysTick_Handler, EDMAC_IRQHandler};

void Reset_Handler(void) {
  extern long _sbss[], _ebss[], _sdata[], _edata[], _sidata[];
  extern int main(void);

  SCB->VTOR = (uint32_t) tab;
  for (long *dst = _sbss; dst < _ebss; dst++) *dst = 0;
  for (long *dst = _sdata, *src = _sidata; dst < _edata;) *dst++ = *src++;

  SystemInit();
  main();
  while (1) (void) 0;
}

void SystemInit(void) {  // Called automatically by startup code
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();
  clock_init();                            // Set system clock to SYS_FREQUENCY
  rng_init();                              // Initialise random number generator
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}

void SysTick_Handler(void) {  // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

#if 0
void mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}
#endif

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

void hal_init(void) {
  uart_init(UART_DEBUG, 115200);  // Initialise UART
  gpio_output(LED1);              // Initialise LED
  gpio_output(LED2);              // Initialise LED
  gpio_output(LED3);              // Initialise LED
  ethernet_init();                // Initialise Ethernet pins
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
