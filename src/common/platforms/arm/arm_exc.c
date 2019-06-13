/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "arm_exc.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"

#include "common/platform.h"

#include "mgos_core_dump.h"
#include "mgos_hal.h"

#ifndef MGOS_ENABLE_CORE_DUMP
#define MGOS_ENABLE_CORE_DUMP 1
#endif

#if __FPU_PRESENT && !defined(MGOS_BOOT_BUILD)
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

static struct arm_gdb_reg_file *s_rf = NULL;

void arm_exc_dump_regs(void) {
  mgos_cd_write_section(MGOS_CORE_DUMP_SECTION_REGS, s_rf, sizeof(*s_rf));
}

void arm_exc_handler_bottom(uint8_t isr_no, struct arm_exc_frame *ef,
                            struct arm_gdb_reg_file *rf) {
  char buf[8];
  const char *name;
#if __MPU_PRESENT
  MPU->CTRL = 0;  // Disable MPU.
#endif
  portDISABLE_INTERRUPTS();
  s_rf = rf;
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
#ifndef MGOS_BOOT_BUILD
      sprintf(buf, "IRQ%u", isr_no - 16);
#endif
      name = buf;
    }
  }
  mgos_cd_printf("\n\n--- Exception %u (%s) ---\n", isr_no, name);
  if (rf != NULL) {
    mgos_cd_printf(
        "  R%d:  0x%08lx  R%d:  0x%08lx  R%d:  0x%08lx  R%d:  0x%08lx\n", 0,
        rf->r[0], 1, rf->r[1], 2, rf->r[2], 3, rf->r[3]);
    mgos_cd_printf(
        "  R%d:  0x%08lx  R%d:  0x%08lx  R%d:  0x%08lx  R%d:  0x%08lx\n", 4,
        rf->r[4], 5, rf->r[5], 6, rf->r[6], 7, rf->r[7]);
    mgos_cd_printf("  R8:  0x%08lx  R9:  0x%08lx  R10: 0x%08lx  R11: 0x%08lx\n",
                   rf->r[8], rf->r[9], rf->r[10], rf->r[11]);
    mgos_cd_printf("  R12: 0x%08lx  SP:  0x%08lx   LR: 0x%08lx  PC:  0x%08lx\n",
                   rf->r[12], rf->sp, rf->lr, rf->pc);
    mgos_cd_printf("  PSR: 0x%08lx MSP:  0x%08lx  PSP: 0x%08lx\n", rf->xpsr,
                   rf->msp, rf->psp);
  }
#if __FPU_PRESENT
  memset(rf->d, 0, sizeof(rf->d));
#if !defined(MGOS_BOOT_BUILD)
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
#endif
#if MGOS_ENABLE_CORE_DUMP
  mgos_cd_write();
#endif
#ifdef MGOS_HALT_ON_EXCEPTION
  mgos_cd_printf("Halting\n");
  while (1) {
    mgos_wdt_feed();
  }
#else
  mgos_cd_printf("Rebooting\n");
  mgos_dev_system_restart();
#endif
  (void) ef;
}
