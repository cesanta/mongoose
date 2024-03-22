#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_XMC) && MG_ENABLE_DRIVER_XMC

struct mg_tcpip_driver_xmc_data {
  // 13.2.8.1 Station Management Functions
  // MDC clock divider (). MDC clock is derived from ETH MAC clock
  // It must not exceed 2.5MHz
  // ETH Clock range  DIVIDER       mdc_cr VALUE
  // --------------------------------------------
  //                                     -1  <-- tell driver to guess the value
  // 60-100 MHz       ETH Clock/42        0
  // 100-150 MHz      ETH Clock/62        1
  // 20-35 MHz        ETH Clock/16        2
  // 35-60 MHz        ETH Clock/26        3
  // 150-250 MHz      ETH Clock/102       4
  // 250-300 MHz      ETH Clock/124       5
  // 110, 111 Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5
  uint8_t phy_addr;
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 4
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_xmc_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                       \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                    \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_xmc;                        \
    mif_.driver_data = &driver_data_;                             \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: xmc, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
