#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_SAME54) && MG_ENABLE_DRIVER_SAME54

struct mg_tcpip_driver_same54_data {
    int mdc_cr;
};

#ifndef MG_MAC_ADDRESS
#define MG_MAC_ADDRESS MG_MAC_ADDRESS_RANDOM
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 5
#endif

#define MG_TCPIP_DRIVER_DATA                                \
  static struct mg_tcpip_driver_same54_data driver_data = { \
      .mdc_cr = MG_DRIVER_MDC_CR,                            \
  };

#define MG_TCPIP_DRIVER_CODE &mg_tcpip_driver_same54
#define MG_TCPIP_DRIVER_NAME "same54"

#endif
