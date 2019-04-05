/*
 * Copyright (c) 2014-2019 Cesanta Software Limited
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

#include <stdint.h>

struct arm_exc_frame {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t pc;
  uint32_t xpsr;
#if __FPU_PRESENT
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
  uint32_t xpsr;
#if __FPU_PRESENT
  uint64_t d[16];
  uint32_t fpscr;
#endif
  // MSP and PSP are our extension.
  uint32_t msp;
  uint32_t psp;
} __attribute__((packed));

void arm_exc_handler_bottom(uint8_t isr_no, struct arm_exc_frame *ef,
                            struct arm_gdb_reg_file *rf);
void arm_exc_dump_regs(void);
