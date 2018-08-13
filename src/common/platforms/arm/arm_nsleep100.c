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

#include <math.h>

#include "mgos.h"
#include "mgos_system.h"

void (*mgos_nsleep100)(uint32_t n);
uint32_t mgos_nsleep100_loop_count = 0;
/* Provided by arm_nsleep100_{m4,m7}.S for M4 and M7 respectively. */
extern void mgos_nsleep100_impl(uint32_t n);

void mgos_nsleep100_cal(void) {
  uint32_t cpu_freq = mgos_get_cpu_freq();
  /* # of instruction cycles per 100 ns */
  mgos_nsleep100_loop_count =
      roundf((100.0f / 1000000000.0f) / (1.0f / cpu_freq));
  mgos_nsleep100 = mgos_nsleep100_impl;
}
