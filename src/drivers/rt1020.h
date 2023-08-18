#pragma once

struct mg_tcpip_driver_imxrt1020_data {
  // MDC clock divider. MDC clock is derived from IPS Bus clock (ipg_clk),
  // must not exceed 2.5MHz. Configuration for clock range 2.36~2.50 MHz
  //    ipg_clk       MSCR       mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    25 MHz        0x04           0
  //    33 MHz        0x06           1
  //    40 MHz        0x07           2
  //    50 MHz        0x09           3
  //    66 MHz        0x0D           4  <-- value for iMXRT1020-EVK at max freq.
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4
};
