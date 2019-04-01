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

.arch armv7e-m
.syntax unified
.thumb

/* These are required to satisfy TI linker. */
.eabi_attribute Tag_ABI_align_needed, 1
.eabi_attribute Tag_ABI_align_preserved, 1

.global mgos_nsleep100_impl
.global mgos_nsleep100_loop_count

.section .text.IRAM.mgos_nsleep100_impl
.type mgos_nsleep100_impl, %function
.align 4

mgos_nsleep100_impl:
      ldr     r3, =mgos_nsleep100_loop_count
      ldr     r3, [r3]
      /* Because of speculative fetch, the core loop only takes 1 cycle. */
      muls    r0, r3
      cbz     r0, xxx
lxx:
      subs    r0, #1
      bne     lxx
xxx:
      bx      lr
.align 4
.size mgos_nsleep100_impl, . - mgos_nsleep100_impl
