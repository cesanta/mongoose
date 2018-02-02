/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "uart.h"

#include "rom_functions.h"

void set_baud_rate(uint32_t uart_no, uint32_t baud_rate) {
  uint32_t master_freq = ets_get_detected_xtal_freq() << 4;
  master_freq += (baud_rate / 2);
  uint32_t div = master_freq / baud_rate;
  uart_div_modify(uart_no, div);
}
