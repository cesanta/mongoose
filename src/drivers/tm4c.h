#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_TM4C) && MG_ENABLE_DRIVER_TM4C

struct mg_tcpip_driver_tm4c_data {
  // MDC clock divider. MDC clock is derived from SYSCLK, must not exceed 2.5MHz
  //    SYSCLK range   DIVIDER   mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    SYSCLK/42      0
  //    100-150 MHz   SYSCLK/62      1  <-- value for EK-TM4C129* on max speed
  //    20-35 MHz     SYSCLK/16      2
  //    35-60 MHz     SYSCLK/26      3
  //    0x4-0xF Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3
};

#ifndef MG_MAC_ADDRESS
#define MG_MAC_ADDRESS MG_MAC_ADDRESS_RANDOM
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 1
#endif

#define MG_TCPIP_DRIVER_DATA                                \
  static struct mg_tcpip_driver_tm4c_data driver_data = { \
      .mdc_cr = MG_DRIVER_MDC_CR,                            \
  };

#define MG_TCPIP_DRIVER_CODE &mg_tcpip_driver_tm4c
#define MG_TCPIP_DRIVER_NAME "tm4c"


#endif
