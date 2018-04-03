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

#include <inttypes.h>
#include <string.h>
#include "slip.h"

/* Define the args vector and put it into the ".params" section. */
uint32_t params[3] __attribute__((section(".params")));

/* Define a function called stub_main. Do not return or reboot.
 * Use send_packet to communicate to the host. */

const char *hello = "Hello";

static char buf[1024];
extern uint32_t _bss_start, _bss_end;

void stub_main(void) {
  uint32_t greeting = 0x4941484f;
  SLIP_send(&greeting, 4);
  memset(&_bss_start, 0, (&_bss_end - &_bss_start));
  buf[1] = 123;
  SLIP_send(hello, 5);
  while (1) {
  }
}
