// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include "hal.h"

#define MG_HAL_SYSTICK_NONE      1
#define MG_HAL_SYSTICK_FREERTOS  2
#ifndef MG_HAL_SYSTICK
#define MG_HAL_SYSTICK  0
#endif

#if MG_HAL_SYSTICK == MG_HAL_SYSTICK_FREERTOS
#include <FreeRTOS.h>
#include <task.h>
extern void xPortSysTickHandler(void);
void SysTick_Handler(void) {
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	xPortSysTickHandler();
}
#elif MG_HAL_SYSTICK != MG_HAL_SYSTICK_NONE
static volatile uint64_t s_ticks;  // Milliseconds since boot
void SysTick_Handler(void) {       // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}
#endif

#ifndef MG_HAL_DISABLE_RANDOM
#ifdef MG_HAL_ENABLE_PRNG
static void rng_init(void);
static uint32_t rng_read(void);
#endif
bool mg_random(void *buf, size_t len) {  // Use on-board RNG or custom PRNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true; // TODO(): change rng_read() sig: check chip RNG, return false
}
#endif

#ifndef MG_HAL_DISABLE_MILLIS
uint64_t mg_millis(void) {  // Return number of milliseconds since boot
  // handle architectures with non-atomic 64-bit access, see #3041
  uint64_t ticks = s_ticks;            // first read
  if (ticks == s_ticks) return ticks;  // second read, same value => OK
  return s_ticks;                      // changed while reading, read again
}
#endif

#ifndef MG_HAL_DISABLE_SYSTEM_INIT
uint32_t SystemCoreClock;
void SystemInit(void) {  // Called automatically by startup code
  system_init();
}
#endif

#ifdef MG_HAL_ARCH_ARMCGT
#include <lowlev.h>

int _write(int fd, const char *ptr, unsigned int len) {
  (void) fd, (void) ptr, (void) len;
  if (fd == 1) uart_write_buf(UART_DEBUG, ptr, (size_t) len);
  return len;
}
#endif

void hal_init(void) {
#ifndef MG_HAL_DISABLE_CLOCK
  clock_init();                            // Set system clock to SYS_FREQUENCY
  SystemCoreClock = SYS_FREQUENCY;         // Update SystemCoreClock global var
#endif
#if MG_HAL_SYSTICK != MG_HAL_SYSTICK_NONE
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
#endif
#ifndef MG_HAL_DISABLE_RANDOM
  rng_init();                              // Initialise random number generator
#endif

#ifndef MG_HAL_DISABLE_UART
  uart_init(UART_DEBUG, 115200);  // Initialise UART
#ifdef MG_HAL_ARCH_ARMCGT
  _stream[1].dfd = 1;
  _stream[1].dev->WRITE = _write; // redirect stream to _write syscall
#endif
#endif
  leds_init();
#ifndef MG_HAL_DISABLE_ETHERNET
  ethernet_init();                // Initialise Ethernet pins
#endif
}

#ifdef MG_HAL_ENABLE_GENMAC
void hal_genmac(unsigned char *mac) {
  ethernet_genmac(mac);
}
#endif

#ifdef MG_HAL_ENABLE_PRNG
static uint32_t s_hal_rng_seed;
static void rng_init(void) {
  s_hal_rng_seed = entropy_init();
}

static uint32_t rng_read() {
  uint32_t seed = s_hal_rng_seed;
  uint32_t ent;
  if (entropy_get(&ent)) {
    // Factor both previous seed and current ent value into a new seed
    ent *= 0x85ebca6b;
    ent ^= (ent << 16);
    ent ^= (ent >> 5);
    seed ^= ent;
    seed *= 0x5bd1e995;
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    seed += ent;
    seed ^= seed >> 16;
  } else {
    // apply complex bit permutations / arithmetical ops for better randomness
    seed ^= seed >> 3;
    seed *= 2654435761U;
    seed ^= seed << 13;
    seed *= 0x85ebca6b;
    seed ^= seed >> 16;
    seed *= 0xc2b2ae35;
    seed ^= seed >> 13;
  }
  s_hal_rng_seed = seed;
  return seed;
}
#endif


#if defined(__ARMCC_VERSION)
// Keil specific - implement IO printf redirection
int fputc(int c, FILE *stream) {
  if (stream == stdout || stream == stderr) uart_write_byte(UART_DEBUG, c);
  return c;
}
#elif defined(__GNUC__) && !defined(MG_HAL_DISABLE_NEWLIB)
// ARM GCC specific. ARM GCC is shipped with Newlib C library.
// Implement newlib syscalls:
//      _sbrk() for malloc
//      _write() for printf redirection
//      the rest are just stubs
#include <sys/stat.h>  // For _fstat()

int _fstat(int fd, struct stat *st) {
  (void) fd, (void) st;
  return -1;
}

#ifndef MG_HAL_SBRK_NONE
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
#ifndef MG_HAL_SBRK_SIMPLEST
  unsigned char *heap_end = (unsigned char *) ((size_t) &heap_end - 256);
#endif
  prev_heap = s_current_heap_end;
#ifndef MG_HAL_SBRK_SIMPLEST
  // Check how much space we  got from the heap end to the stack end
  if (s_current_heap_end + incr > heap_end) return (void *) -1;
#endif
  s_current_heap_end += incr;
  return prev_heap;
}
#endif

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

void _init(void) {
}
#endif                 // __GNUC__
