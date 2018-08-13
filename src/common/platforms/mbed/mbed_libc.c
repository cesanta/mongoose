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

#include "common/platform.h"

#if CS_PLATFORM == CS_P_MBED

long timezone;

/*
 * The GCC ARM toolchain for implements a weak
 * gettimeofday stub that should be implemented
 * to hook the OS time source. But mbed OS doesn't do it;
 * the mbed doc only talks about C date and time functions:
 *
 * https://docs.mbed.com/docs/mbed-os-api-reference/en/5.1/APIs/tasks/Time/
 *
 * gettimeof day is a BSD API.
 */
int _gettimeofday(struct timeval *tv, void *tzvp) {
  tv->tv_sec = time(NULL);
  tv->tv_usec = 0;
  return 0;
}

int inet_aton(const char *cp, struct in_addr *inp) {
  /* We don't have aton, but have pton in mbed */
  return inet_pton(AF_INET, cp, inp);
}

in_addr_t inet_addr(const char *cp) {
  in_addr_t ret;
  if (inet_pton(AF_INET, cp, &ret) != 1) {
    return 0;
  }

  return ret;
}

#endif /* CS_PLATFORM == CS_P_MBED */
