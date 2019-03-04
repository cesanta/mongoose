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

#include "rom_functions.h"

uint32_t set_baud_rate(uint32_t uart_no, uint32_t old_baud_rate,
                       uint32_t new_baud_rate) {
  uint32_t uart_reg = REG_READ(UART_CLKDIV_REG(uart_no));
  uint32_t uart_div = uart_reg & UART_CLKDIV_M;
  uint32_t fraction = (uart_reg >> UART_CLKDIV_FRAG_S) & UART_CLKDIV_FRAG_V;
  uart_div = (uart_div << 4) + fraction;
  uint32_t master_freq = uart_div * old_baud_rate;
  uart_div_modify(uart_no, master_freq / new_baud_rate);
  return uart_div;
}
