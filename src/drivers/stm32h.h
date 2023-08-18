#pragma once

struct mg_tcpip_driver_stm32h_data {
  // MDC clock divider. MDC clock is derived from HCLK, must not exceed 2.5MHz
  //    HCLK range    DIVIDER    mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    HCLK/42        0
  //    100-150 MHz   HCLK/62        1
  //    20-35 MHz     HCLK/16        2
  //    35-60 MHz     HCLK/26        3
  //    150-250 MHz   HCLK/102       4  <-- value for Nucleo-H* on max speed driven by HSI
  //    250-300 MHz   HCLK/124       5  <-- value for Nucleo-H* on max speed driven by CSI
  //    110, 111 Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5
};
