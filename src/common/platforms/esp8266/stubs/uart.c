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

#include "uart.h"
#include "ets_sys.h"

#define UART_CLKDIV_26MHZ(B) (52000000 + B / 2) / B

void set_baud_rate(uint32_t uart_no, uint32_t baud_rate) {
  uint32_t div = UART_CLKDIV_26MHZ(baud_rate);
  WRITE_PERI_REG(UART_CLKDIV_REG(uart_no), div & 0xfffff);
}
