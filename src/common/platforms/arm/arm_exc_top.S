/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */

.arch armv7e-m
.syntax unified
.thumb

/* These are required to satisfy TI linker. */
.eabi_attribute Tag_ABI_align_needed, 1
.eabi_attribute Tag_ABI_align_preserved, 1

.global arm_exc_handler_top
.global arm_exc_handler_bottom

/*
 * Determines the stack pointer, populates most of the GDB frame 
 * and hands off to the C routine.
 */
.section .text.arm_exc_handler_top
.type arm_exc_handler_top, %function
.align 8

arm_exc_handler_top:
  tst lr, #4
  ite eq
  mrseq r1, msp
  mrsne r1, psp
  // r1 -> arm_exc_frame prepared for us by the CPU
#if ARM_HAVE_FPU
  add r0, r1, #104  // sizeof(arm_exc_frame)
  sub sp, #328      // sizeof(arm_gdb_reg_file)
#else
  add r0, r1, #32   // sizeof(arm_exc_frame)
  sub sp, #328      // sizeof(arm_gdb_reg_file)
#endif
  mov r2, sp
  // r0 -> original sp, r2 -> arm_gdb_reg_file to fill
  // r3 - scratch
  ldr r3,  [r1, #0]   // r0
  str r3,  [r2, #0]
  ldr r3,  [r1, #4]   // r2
  str r3,  [r2, #4]
  ldr r3,  [r1, #8]   // r1
  str r3,  [r2, #8]
  ldr r3,  [r1, #12]  // r3
  str r3,  [r2, #12]
  str r4,  [r2, #16]  // r4
  str r5,  [r2, #20]  // r5
  str r6,  [r2, #24]  // r6
  str r7,  [r2, #28]  // r7
  str r8,  [r2, #32]  // r8
  str r9,  [r2, #36]  // r9
  str r10, [r2, #40]  // r10
  str r11, [r2, #44]  // r11
  ldr r3,  [r1, #16]  // r12
  str r3,  [r2, #48]
  str r0,  [r2, #52]  // sp
  ldr r3,  [r1, #20]  // lr
  str r3,  [r2, #56]
  ldr r3,  [r1, #24]  // pc
  str r3,  [r2, #60]
  ldr r3,  [r1, #28]  // xpsr
  str r3,  [r2, #64]

  mrs r0, ipsr
  b arm_exc_handler_bottom
.size arm_exc_handler_top, . - arm_exc_handler_top
