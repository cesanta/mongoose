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

#ifndef CS_COMMON_PLATFORMS_ESP8266_STUBS_UART_H_
#define CS_COMMON_PLATFORMS_ESP8266_STUBS_UART_H_

#include <stdint.h>

#include "uart_register.h"

uint32_t set_baud_rate(uint32_t uart_no, uint32_t old_baud_rate,
                       uint32_t new_baud_rate);

#define UART_FIFO_REG(i) (REG_UART_BASE(i) + 0x0)
#define UART_CONF1_REG(i) (REG_UART_BASE(i) + 0x24)
#define UART_RX_TOUT_EN (BIT(31))
#define UART_RX_TOUT_THRHD_S 24
#define UART_RXFIFO_FULL_THRHD_S 0
#define UART_INT_ST_REG(i) (REG_UART_BASE(i) + 0x8)
#define UART_INT_ENA_REG(i) (REG_UART_BASE(i) + 0xC)
#define UART_RXFIFO_TOUT_INT_ENA (BIT(8))
#define UART_RXFIFO_FULL_INT_ENA (BIT(0))

#define UART_INT_CLR_REG(i) (REG_UART_BASE(i) + 0x10)

#define UART_CLKDIV_REG(i) (REG_UART_BASE(i) + 0x14)

#define UART_STATUS_REG(i) (REG_UART_BASE(i) + 0x1C)

#define UART_CONF0_REG(i) (REG_UART_BASE(i) + 0x20)
#define UART_RXFIFO_RST (BIT(17))

#define ETS_UART0_INUM ETS_UART_INUM

#endif /* CS_COMMON_PLATFORMS_ESP8266_STUBS_UART_H_ */
