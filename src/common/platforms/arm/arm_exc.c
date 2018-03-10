/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"

#include "common/platform.h"

#include "mgos_core_dump.h"
#include "mgos_hal.h"

struct arm_exc_frame {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t pc;
  uint32_t xpsr;
#ifdef ARM_HAVE_FPU
  uint32_t s[16];
  uint32_t fpscr;
  uint32_t reserved;
#endif
} __attribute__((packed));

struct arm_gdb_reg_file {
  uint32_t r[13];
  uint32_t sp;
  uint32_t lr;
  uint32_t pc;
  uint32_t cpsr;
  uint64_t d[16];
  uint32_t fpscr;
} __attribute__((packed));

#if ARM_HAVE_FPU
static void save_s16_s31(uint32_t *dst) {
  __asm volatile(
      "\
      vmov r1, s16\n str  r1, [%0, 0]\n\
      vmov r1, s17\n str  r1, [%0, 4]\n\
      vmov r1, s18\n str  r1, [%0, 8]\n\
      vmov r1, s19\n str  r1, [%0, 12]\n\
      vmov r1, s20\n str  r1, [%0, 16]\n\
      vmov r1, s21\n str  r1, [%0, 20]\n\
      vmov r1, s22\n str  r1, [%0, 24]\n\
      vmov r1, s23\n str  r1, [%0, 28]\n\
      vmov r1, s24\n str  r1, [%0, 32]\n\
      vmov r1, s25\n str  r1, [%0, 36]\n\
      vmov r1, s26\n str  r1, [%0, 40]\n\
      vmov r1, s27\n str  r1, [%0, 44]\n\
      vmov r1, s28\n str  r1, [%0, 48]\n\
      vmov r1, s29\n str  r1, [%0, 52]\n\
      vmov r1, s30\n str  r1, [%0, 56]\n\
      vmov r1, s31\n str  r1, [%0, 60]\n\
      "
      :
      : "r"(dst)
      : "r1");
}

static void print_fpu_regs(const uint32_t *regs, int off, int n) {
  for (int i = 0, j = off; i < n; i++, j++) {
    if (j % 4 == 0) mgos_cd_putc('\n');
    mgos_cd_printf("  S%d: %s0x%08lx", j, (j < 10 ? " " : ""), regs[i]);
  }
}
#endif

void arm_exc_handler_bottom(uint8_t isr_no, struct arm_exc_frame *ef,
                            struct arm_gdb_reg_file *rf) {
  char buf[8];
  const char *name;
  portDISABLE_INTERRUPTS();
  switch (isr_no) {
    case 0:
      name = "ThreadMode";
      break;

    case 1:
    case 7:
    case 8:
    case 9:
    case 10:
    case 13:
      name = "Reserved";
      break;

    case 2:
      name = "NMI";
      break;
    case 3:
      name = "HardFault";
      break;
    case 4:
      name = "MemManage";
      break;
    case 5:
      name = "BusFault";
      break;
    case 6:
      name = "UsageFault";
      break;
    case 11:
      name = "SVCall";
      break;
    case 12:
      name = "ReservedDebug";
      break;
    case 14:
      name = "PendSV";
      break;
    case 15:
      name = "SysTick";
      break;
    default: {
      sprintf(buf, "IRQ%u", isr_no - 16);
      name = buf;
    }
  }
  mgos_cd_printf("\n\n--- Exception %u (%s) ---\n", isr_no, name);
  if (rf != NULL) {
    mgos_cd_printf("  R0:  0x%08lx  R1:  0x%08lx  R2:  0x%08lx  R3:  0x%08lx\n",
                   rf->r[0], rf->r[1], rf->r[2], rf->r[3]);
    mgos_cd_printf("  R4:  0x%08lx  R5:  0x%08lx  R6:  0x%08lx  R7:  0x%08lx\n",
                   rf->r[4], rf->r[5], rf->r[6], rf->r[7]);
    mgos_cd_printf("  R8:  0x%08lx  R9:  0x%08lx  R10: 0x%08lx  R11: 0x%08lx\n",
                   rf->r[8], rf->r[9], rf->r[10], rf->r[11]);
    mgos_cd_printf("  R12: 0x%08lx  SP:  0x%08lx  LR:  0x%08lx  PC:  0x%08lx\n",
                   rf->r[12], rf->sp, rf->lr, rf->pc);
    mgos_cd_printf("  PSR: 0x%08lx\n", rf->cpsr);
  }
  memset(rf->d, 0, sizeof(rf->d));
#if ARM_HAVE_FPU
  rf->fpscr = ef->fpscr;
  memcpy((uint8_t *) rf->d, ef->s, sizeof(ef->s));
  print_fpu_regs((uint32_t *) rf->d, 0, ARRAY_SIZE(ef->s));
  save_s16_s31(ef->s);
  memcpy(((uint8_t *) rf->d) + sizeof(ef->s), ef->s, sizeof(ef->s));
  print_fpu_regs((uint32_t *) (((uint8_t *) rf->d) + sizeof(ef->s)), 16,
                 ARRAY_SIZE(ef->s));
  mgos_cd_putc('\n');
  mgos_cd_printf("FPSCR: 0x%08lx\n", rf->fpscr);
#else
  rf->fpscr = 0;
#endif
  mgos_cd_emit_header();
  mgos_cd_emit_section(MGOS_CORE_DUMP_SECTION_REGS, rf, sizeof(*rf));
  mgos_cd_emit_section("SRAM", (void *) SRAM_BASE_ADDR, SRAM_SIZE);
  mgos_cd_emit_footer();
#ifdef MGOS_HALT_ON_EXCEPTION
  mgos_cd_printf("Halting\n");
  while (1) {
    mgos_wdt_feed();
  }
#else
  mgos_cd_printf("Rebooting\n");
  mgos_dev_system_restart();
#endif
}
