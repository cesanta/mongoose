/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "uart.h"
#include "ets_sys.h"

#define UART_CLKDIV_26MHZ(B) (52000000 + B / 2) / B

void set_baud_rate(uint32_t uart_no, uint32_t baud_rate) {
  uint32_t div = UART_CLKDIV_26MHZ(baud_rate);
  WRITE_PERI_REG(UART_CLKDIV_REG(uart_no), div & 0xfffff);
}
