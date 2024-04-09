#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_IMXRT) && MG_ENABLE_DRIVER_IMXRT

struct mg_tcpip_driver_imxrt_data {
  // MDC clock divider. MDC clock is derived from IPS Bus clock (ipg_clk),
  // must not exceed 2.5MHz. Configuration for clock range 2.36~2.50 MHz
  // 37.5.1.8.2, Table 37-46 : f = ipg_clk / (2(mdc_cr + 1))
  //    ipg_clk       mdc_cr VALUE
  //    --------------------------
  //                  -1  <-- TODO() tell driver to guess the value
  //    25 MHz         4
  //    33 MHz         6
  //    40 MHz         7
  //    50 MHz         9
  //    66 MHz        13
  int mdc_cr;  // Valid values: -1 to 63

  uint8_t phy_addr;  // PHY address
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 2
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 24
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                \
  do {                                                           \
    static struct mg_tcpip_driver_imxrt_data driver_data_;       \
    static struct mg_tcpip_if mif_;                              \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                      \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                   \
    mif_.ip = MG_TCPIP_IP;                                       \
    mif_.mask = MG_TCPIP_MASK;                                   \
    mif_.gw = MG_TCPIP_GW;                                       \
    mif_.driver = &mg_tcpip_driver_imxrt;                        \
    mif_.driver_data = &driver_data_;                            \
    MG_SET_MAC_ADDRESS(mif_.mac);                                \
    mg_tcpip_init(mgr, &mif_);                                   \
    MG_INFO(("Driver: imxrt, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
