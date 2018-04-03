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

#if CS_PLATFORM == CS_P_MSP432

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>

int gettimeofday(struct timeval *tp, void *tzp) {
  uint32_t ticks = Clock_getTicks();
  tp->tv_sec = ticks / 1000;
  tp->tv_usec = (ticks % 1000) * 1000;
  return 0;
}

#endif /* CS_PLATFORM == CS_P_MSP432 */
