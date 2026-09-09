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

static int has_fp_frame(uint32_t exc_return) {
  return (exc_return & (1U << 4)) == 0;
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

void hardfault_handler(uint32_t exception_stack[], uint32_t lr,
                       const uint32_t callee_saved[8]) {
  uint32_t cpuid = MG_SCB_CPUID;
  uint32_t part = (cpuid >> 4) & 0xfff;
  size_t fp_words = has_fp_frame(lr) ? 18U : 0U;
  int valid = stack_valid(exception_stack, fp_words + 8U);
  struct crash_record *r = &g_hardfault_record;
  volatile uint32_t *words = (volatile uint32_t *) r;

  for (size_t i = 0; i < sizeof(*r) / sizeof(*words); i++) words[i] = 0;
  r->version = CRASH_RECORD_VERSION;
  r->cpuid = cpuid;
  r->exception_sp = (uint32_t) (uintptr_t) exception_stack;
  r->exc_return = lr;
  r->exception_valid = valid;
  r->r4 = callee_saved[0];
  r->r5 = callee_saved[1];
  r->r6 = callee_saved[2];
  r->r7 = callee_saved[3];
  r->r8 = callee_saved[4];
  r->r9 = callee_saved[5];
  r->r10 = callee_saved[6];
  r->r11 = callee_saved[7];
  if (valid > 0) {
    uint32_t *core_frame = exception_stack + fp_words;
    uint32_t *fault_sp;
    uintptr_t lo, hi, p;
    size_t count;

    r->r0 = core_frame[0];
    r->r1 = core_frame[1];
    r->r2 = core_frame[2];
    r->r3 = core_frame[3];
    r->r12 = core_frame[4];
    r->lr = core_frame[5];
    r->pc = core_frame[6];
    r->psr = core_frame[7];

    fault_sp = core_frame + 8;
    if (r->psr & (1U << 9)) fault_sp++;
    r->sp = (uint32_t) (uintptr_t) fault_sp;
    r->stack_valid = stack_valid(fault_sp, 0);
    p = (uintptr_t) fault_sp;
    if (r->stack_valid > 0 && stack_range(fault_sp, &lo, &hi)) {
      (void) lo;
      count = (hi - p) / sizeof(uint32_t);
      if (count > CRASH_STACK_WORDS) count = CRASH_STACK_WORDS;
      r->stack_word_count = (uint32_t) count;
      for (size_t i = 0; i < count; i++) r->stack_words[i] = fault_sp[i];
    }
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

  r->magic = CRASH_RECORD_MAGIC;
  hal_crash_store(r);

  printf("*** HardFault ***\n");
  printf("EXCEPTION SP = %08lX", (unsigned long) exception_stack);
  if (valid == 0) printf(" INVALID");
  if (valid < 0) printf(" UNKNOWN-RANGE");
  printf("\n");
  printf("EXC_RETURN = %08lX: %s\n", (unsigned long) lr,
         exc_return_desc(lr)->desc);
  printf("CPUID = %08lX: %s\n", (unsigned long) cpuid, core_name(part));
  print_v8m_status(part);
  if (valid <= 0) {
    for (;;) (void) 0;
  }

  printf("R0  = %08lX\n", (unsigned long) r->r0);
  printf("R1  = %08lX\n", (unsigned long) r->r1);
  printf("R2  = %08lX\n", (unsigned long) r->r2);
  printf("R3  = %08lX\n", (unsigned long) r->r3);
  printf("R4  = %08lX\n", (unsigned long) r->r4);
  printf("R5  = %08lX\n", (unsigned long) r->r5);
  printf("R6  = %08lX\n", (unsigned long) r->r6);
  printf("R7  = %08lX\n", (unsigned long) r->r7);
  printf("R8  = %08lX\n", (unsigned long) r->r8);
  printf("R9  = %08lX\n", (unsigned long) r->r9);
  printf("R10 = %08lX\n", (unsigned long) r->r10);
  printf("R11 = %08lX\n", (unsigned long) r->r11);
  printf("R12 = %08lX\n", (unsigned long) r->r12);
  printf("SP  = %08lX\n", (unsigned long) r->sp);
  printf("LR  = %08lX\n", (unsigned long) r->lr);
  printf("PC  = %08lX\n", (unsigned long) r->pc);
  printf("PSR = %08lX\n", (unsigned long) r->psr);

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

  printf("STACK WORDS = %lu\n", (unsigned long) r->stack_word_count);
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
      "stmdb sp!, {r4-r11} \n"
      "mov r2, sp \n"
      "b hardfault_handler \n");
}
