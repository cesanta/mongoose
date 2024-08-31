// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include "hal.h"

#if 0
static volatile uint64_t s_ticks;  // Milliseconds since boot
void SysTick_Handler(void) {       // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

bool mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
}

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return s_ticks;           // Return number of milliseconds since boot
}
#endif

void hal_init(void) {
  clock_init();                            // Set system clock to SYS_FREQUENCY
  SystemCoreClock = SYS_FREQUENCY;         // Update SystemCoreClock global var
  //SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
  // rng_init();  // TRNG is part of ELS and there is no info on that

  uart_init(UART_DEBUG, 115200);  // Initialise UART
  gpio_output(LED1);              // Initialise LED1
  gpio_write(LED1, 1);
  gpio_output(LED2);  // Initialise LED2
  gpio_write(LED2, 1);
  gpio_output(LED3);  // Initialise LED3
  gpio_write(LED3, 1);
  ethernet_init();  // Initialise Ethernet pins
}

#if defined(__ARMCC_VERSION)
// Keil specific - implement IO printf redirection
int fputc(int c, FILE *stream) {
  if (stream == stdout || stream == stderr) uart_write_byte(UART_DEBUG, c);
  return c;
}
#elif defined(__GNUC__)
// ARM GCC specific. ARM GCC is shipped with Newlib C library.
// Implement newlib syscalls:
//      _sbrk() for malloc
//      _write() for printf redirection
//      the rest are just stubs
#include <sys/stat.h>  // For _fstat()

#if !defined(__MCUXPRESSO)
uint32_t SystemCoreClock;
// evaluate your use of Secure/non-Secure and modify accordingly
void SystemInit(void) {  // Called automatically by startup code
  SCB->CPACR |=
#if 0
      (3UL << 0 * 2) | (3UL << 1 * 2) |   // Enable PowerQuad (CPO/CP1)
#endif
      (3UL << 10 * 2) | (3UL << 11 * 2);  // Enable FPU
  __DSB();
  __ISB();
  SYSCON->LPCAC_CTRL &= ~SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK;  // enable LPCAC
// Read TRM 36.1 and decide whether you really want to enable aGDET and dGDET
#if 1
  // Disable aGDET and dGDET
  ITRC0->OUT_SEL[4][0] =
      (ITRC0->OUT_SEL[4][0] & ~ITRC_OUTX_SEL_OUTX_SELY_OUT_SEL_IN9_SELn_MASK) |
      (ITRC_OUTX_SEL_OUTX_SELY_OUT_SEL_IN9_SELn(0x2));
  ITRC0->OUT_SEL[4][1] =
      (ITRC0->OUT_SEL[4][1] & ~ITRC_OUTX_SEL_OUTX_SELY_OUT_SEL_IN9_SELn_MASK) |
      (ITRC_OUTX_SEL_OUTX_SELY_OUT_SEL_IN9_SELn(0x2));
  SPC0->ACTIVE_CFG |= SPC_ACTIVE_CFG_GLITCH_DETECT_DISABLE_MASK;
  SPC0->GLITCH_DETECT_SC &= ~SPC_GLITCH_DETECT_SC_LOCK_MASK;
  SPC0->GLITCH_DETECT_SC = 0x3C;
  SPC0->GLITCH_DETECT_SC |= SPC_GLITCH_DETECT_SC_LOCK_MASK;
  ITRC0->OUT_SEL[4][0] =
      (ITRC0->OUT_SEL[4][0] & ~ITRC_OUTX_SEL_OUTX_SELY_OUT_SEL_IN0_SELn_MASK) |
      (ITRC_OUTX_SEL_OUTX_SELY_OUT_SEL_IN0_SELn(0x2));
  ITRC0->OUT_SEL[4][1] =
      (ITRC0->OUT_SEL[4][1] & ~ITRC_OUTX_SEL_OUTX_SELY_OUT_SEL_IN0_SELn_MASK) |
      (ITRC_OUTX_SEL_OUTX_SELY_OUT_SEL_IN0_SELn(0x2));
  GDET0->GDET_ENABLE1 = 0;
  GDET1->GDET_ENABLE1 = 0;
#endif
}
#endif

int _fstat(int fd, struct stat *st) {
  (void) fd, (void) st;
  return -1;
}

#if !defined(__MCUXPRESSO)
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
  prev_heap = s_current_heap_end;
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

int mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}

void _init(void) {
}
#endif  // __GNUC__
