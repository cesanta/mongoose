// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include <stdint.h>
#include <stdio.h>

#include "hardfault.h"

#define MG_SCB_CPUID (*(volatile uint32_t *) 0xe000ed00)
#define MG_SCB_SHCSR (*(volatile uint32_t *) 0xe000ed24)
#define MG_SCB_CFSR (*(volatile uint32_t *) 0xe000ed28)
#define MG_SCB_HFSR (*(volatile uint32_t *) 0xe000ed2c)
#define MG_SCB_DFSR (*(volatile uint32_t *) 0xe000ed30)
#define MG_SCB_MMFAR (*(volatile uint32_t *) 0xe000ed34)
#define MG_SCB_BFAR (*(volatile uint32_t *) 0xe000ed38)
#define MG_SCB_AFSR (*(volatile uint32_t *) 0xe000ed3c)
#define MG_SCB_ABFSR (*(volatile uint32_t *) 0xe000efa8)

#define MG_SHCSR_SECUREFAULTENA (1U << 19)
#define MG_SHCSR_SECUREFAULTPENDED (1U << 20)

#define MG_CPU_CORTEX_M0 0xc20
#define MG_CPU_CORTEX_M23 0xd20
#define MG_CPU_CORTEX_M3 0xc23
#define MG_CPU_CORTEX_M4 0xc24
#define MG_CPU_CORTEX_M7 0xc27
#define MG_CPU_CORTEX_M0P 0xc60
#define MG_CPU_CORTEX_M33 0xd21
#define MG_CPU_CORTEX_M35P 0xd31
#define MG_CPU_CORTEX_M52 0xd24
#define MG_CPU_CORTEX_M55 0xd22
#define MG_CPU_CORTEX_M85 0xd23

#if defined(__ARM_ARCH_8M_BASE__) || defined(__ARM_ARCH_8M_MAIN__) || \
    defined(__ARM_ARCH_8_1M_MAIN__)
#define MG_ARM_HAS_SPLIM 1
#else
#define MG_ARM_HAS_SPLIM 0
#endif

extern uint32_t __StackLimit __attribute__((weak));
extern uint32_t __StackTop __attribute__((weak));
extern uint32_t _sstack __attribute__((weak));
extern uint32_t _estack __attribute__((weak));
extern int hardfault_stack_range(const void *ptr, uintptr_t *lo, uintptr_t *hi)
    __attribute__((weak));

struct exc_return_desc {
  uint32_t lr;
  const char *desc;
  uint8_t fp_frame;
};

static const struct exc_return_desc s_exc_return[] = {
    {0xfffffff1, "Handler Mode (MSP)", 0},
    {0xfffffff9, "Thread Mode (MSP)", 0},
    {0xfffffffd, "Thread Mode (PSP)", 0},
    {0xffffffe1, "Handler Mode (MSP, FP)", 1},
    {0xffffffe9, "Thread Mode (MSP, FP)", 1},
    {0xffffffed, "Thread Mode (PSP, FP)", 1},
    {0, "Unknown", 0},
};

static const char *core_name(uint32_t part) {
  switch (part) {
    case MG_CPU_CORTEX_M0:
      return "Cortex-M0";
    case MG_CPU_CORTEX_M0P:
      return "Cortex-M0+";
    case MG_CPU_CORTEX_M23:
      return "Cortex-M23";
    case MG_CPU_CORTEX_M3:
      return "Cortex-M3";
    case MG_CPU_CORTEX_M4:
      return "Cortex-M4";
    case MG_CPU_CORTEX_M7:
      return "Cortex-M7";
    case MG_CPU_CORTEX_M33:
      return "Cortex-M33";
    case MG_CPU_CORTEX_M35P:
      return "Cortex-M35P";
    case MG_CPU_CORTEX_M52:
      return "Cortex-M52";
    case MG_CPU_CORTEX_M55:
      return "Cortex-M55";
    case MG_CPU_CORTEX_M85:
      return "Cortex-M85";
    default:
      return "Unknown";
  }
}

static int has_fault_status(uint32_t part) {
  return part == MG_CPU_CORTEX_M3 || part == MG_CPU_CORTEX_M4 ||
         part == MG_CPU_CORTEX_M7 || part == MG_CPU_CORTEX_M33 ||
         part == MG_CPU_CORTEX_M35P || part == MG_CPU_CORTEX_M52 ||
         part == MG_CPU_CORTEX_M55 || part == MG_CPU_CORTEX_M85;
}

static int is_v8m(uint32_t part) {
  return part == MG_CPU_CORTEX_M23 || part == MG_CPU_CORTEX_M33 ||
         part == MG_CPU_CORTEX_M35P || part == MG_CPU_CORTEX_M52 ||
         part == MG_CPU_CORTEX_M55 || part == MG_CPU_CORTEX_M85;
}

static const struct exc_return_desc *exc_return_desc(uint32_t lr) {
  size_t i;
  for (i = 0; i < sizeof(s_exc_return) / sizeof(s_exc_return[0]) - 1; i++) {
    if (s_exc_return[i].lr == lr) break;
  }
  return &s_exc_return[i];
}

static int stack_range(const void *ptr, uintptr_t *lo, uintptr_t *hi) {
  if (hardfault_stack_range != 0 && hardfault_stack_range(ptr, lo, hi)) {
    return *lo < *hi;
  } else if (&__StackLimit != 0 && &__StackTop != 0) {
    *lo = (uintptr_t) &__StackLimit;
    *hi = (uintptr_t) &__StackTop;
  } else if (&_sstack != 0 && &_estack != 0) {
    *lo = (uintptr_t) &_sstack;
    *hi = (uintptr_t) &_estack;
  } else {
    return 0;
  }
  return *lo < *hi;
}

static int stack_valid(const void *ptr, size_t words) {
  uintptr_t lo, hi, p = (uintptr_t) ptr;
  if ((p & 3) != 0) return 0;
  if (!stack_range(ptr, &lo, &hi)) return -1;
  return p >= lo && p <= hi && words <= (hi - p) / sizeof(uint32_t);
}

static void print_v8m_status(uint32_t part) {
  if (is_v8m(part)) {
    uint32_t shcsr = MG_SCB_SHCSR;
#if MG_ARM_HAS_SPLIM
    uint32_t msplim, psplim;
    __asm volatile("mrs %0, msplim" : "=r"(msplim));
    __asm volatile("mrs %0, psplim" : "=r"(psplim));
    printf("MSPLIM = %08lX\n", (unsigned long) msplim);
    printf("PSPLIM = %08lX\n", (unsigned long) psplim);
#endif
    printf("SHCSR = %08lX\n", (unsigned long) shcsr);
    printf("SecureFault: %s",
           shcsr & MG_SHCSR_SECUREFAULTENA ? "enabled" : "disabled");
    if (shcsr & MG_SHCSR_SECUREFAULTPENDED) printf(", pended");
    printf("\n");
    // TODO: Add SFSR/SFAR capture if we need deeper SecureFault diagnostics.
  }
}

void hardfault_handler(uint32_t stack[], uint32_t lr) {
  uint32_t cpuid = MG_SCB_CPUID;
  uint32_t part = (cpuid >> 4) & 0xfff;
  int valid = stack_valid(stack, 8);
  struct crash_record *r = &g_hardfault_record;
  volatile uint32_t *words = (volatile uint32_t *) r;

  for (size_t i = 0; i < sizeof(*r) / sizeof(*words); i++) words[i] = 0;
  r->cpuid = cpuid;
  r->sp = (uint32_t) (uintptr_t) stack;
  r->exc_return = lr;
  r->stack_valid = valid;
  if (valid != 0) {
    r->r0 = stack[0];
    r->r1 = stack[1];
    r->r2 = stack[2];
    r->r3 = stack[3];
    r->r12 = stack[4];
    r->lr = stack[5];
    r->pc = stack[6];
    r->psr = stack[7];
  }

  if (has_fault_status(part)) {
    r->cfsr = MG_SCB_CFSR;
    r->hfsr = MG_SCB_HFSR;
    r->dfsr = MG_SCB_DFSR;
    r->afsr = MG_SCB_AFSR;
    r->bfar = MG_SCB_BFAR;
    r->mmfar = MG_SCB_MMFAR;
    if (part == MG_CPU_CORTEX_M7) r->abfsr = MG_SCB_ABFSR;
  }

  if (valid > 0) {
    const struct exc_return_desc *er = exc_return_desc(lr);
    uint32_t *frame = stack + (er->fp_frame ? 26 : 8);
    if (stack[7] & (1 << 9)) frame++;
    r->caller_sp = (uint32_t) (uintptr_t) frame;
    r->caller_valid = stack_valid(frame, 4);
    if (r->caller_valid > 0) {
      for (size_t i = 0; i < 4; i++) r->caller[i] = frame[i];
    }
  }
  r->magic = CRASH_RECORD_MAGIC;
  hal_crash_store(r);

  printf("*** HardFault ***\n");
  printf("SP  = %08lX", (unsigned long) stack);
  if (valid == 0) printf(" INVALID");
  if (valid < 0) printf(" UNKNOWN-RANGE");
  printf("\n");
  printf("EXC_RETURN = %08lX: %s\n", (unsigned long) lr,
         exc_return_desc(lr)->desc);
  printf("CPUID = %08lX: %s\n", (unsigned long) cpuid, core_name(part));
  print_v8m_status(part);
  if (valid == 0) {
    for (;;) (void) 0;
  }

  printf("R0  = %08lX\n", (unsigned long) stack[0]);
  printf("R1  = %08lX\n", (unsigned long) stack[1]);
  printf("R2  = %08lX\n", (unsigned long) stack[2]);
  printf("R3  = %08lX\n", (unsigned long) stack[3]);
  printf("R12 = %08lX\n", (unsigned long) stack[4]);
  printf("LR  = %08lX\n", (unsigned long) stack[5]);
  printf("PC  = %08lX\n", (unsigned long) stack[6]);
  printf("PSR = %08lX\n", (unsigned long) stack[7]);

  if (has_fault_status(part)) {
    uint32_t cfsr = MG_SCB_CFSR;
    uint32_t hfsr = MG_SCB_HFSR;
    uint32_t dfsr = MG_SCB_DFSR;
    uint32_t afsr = MG_SCB_AFSR;
    uint32_t bfar = MG_SCB_BFAR;
    uint32_t mmfar = MG_SCB_MMFAR;
    uint16_t ufsr = (uint16_t) (cfsr >> 16);
    uint8_t bfsr = (uint8_t) (cfsr >> 8);
    uint8_t mmfsr = (uint8_t) cfsr;

    printf("CFSR = %08lX\n", (unsigned long) cfsr);
    printf("HFSR = %08lX\n", (unsigned long) hfsr);
    printf("DFSR = %08lX\n", (unsigned long) dfsr);
    printf("AFSR = %08lX\n", (unsigned long) afsr);
    if (hfsr & (1U << 30)) printf("FORCED\n");
    if (hfsr & (1U << 1)) printf("VECTTBL\n");
    if (ufsr) printf("Usage fault, UFSR = %04X\n", ufsr);
    if (mmfsr & (1U << 7))
      printf("Memory management fault, MMFAR = %08lX\n",
             (unsigned long) mmfar);
    if (bfsr & (1U << 7))
      printf("BFARVALID, BFAR = %08lX\n", (unsigned long) bfar);
    if (bfsr & (1U << 1)) printf("PRECISE\n");
    if (bfsr & (1U << 2)) {
      printf("IMPRECISE\n");
      if (part == MG_CPU_CORTEX_M7)
        printf("ABFSR = %08lX\n", (unsigned long) MG_SCB_ABFSR);
    }
  }

  if (valid > 0) {
    const struct exc_return_desc *er = exc_return_desc(lr);
    uint32_t *frame = stack + (er->fp_frame ? 26 : 8);
    if (stack[7] & (1 << 9)) frame++;
    valid = stack_valid(frame, 4);
    printf("CALLER SP = %08lX", (unsigned long) frame);
    if (valid == 0) printf(" INVALID");
    if (valid < 0) printf(" UNKNOWN-RANGE");
    printf("\n");
    if (valid > 0) {
      printf("CALLER[0] = %08lX\n", (unsigned long) frame[0]);
      printf("CALLER[1] = %08lX\n", (unsigned long) frame[1]);
      printf("CALLER[2] = %08lX\n", (unsigned long) frame[2]);
      printf("CALLER[3] = %08lX\n", (unsigned long) frame[3]);
    }
  }
  for (;;) (void) 0;
}

__attribute__((naked)) void HardFault_Handler(void) {
  __asm volatile(
      "mov r0, lr \n"
      "movs r1, #4 \n"
      "tst r0, r1 \n"
      "beq 1f \n"
      "mrs r0, psp \n"
      "b 2f \n"
      "1: \n"
      "mrs r0, msp \n"
      "2: \n"
      "mov r1, lr \n"
      "b hardfault_handler \n");
}
