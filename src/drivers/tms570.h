#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_TMS570) && MG_ENABLE_DRIVER_TMS570
struct mg_tcpip_driver_tms570_data {
  int mdc_cr;
  int phy_addr;
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 1
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                               \
  do {                                                          \
    static struct mg_tcpip_driver_tms570_data driver_data_;     \
    static struct mg_tcpip_if mif_;                             \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                     \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                  \
    mif_.ip = MG_TCPIP_IP;                                      \
    mif_.mask = MG_TCPIP_MASK;                                  \
    mif_.gw = MG_TCPIP_GW;                                      \
    mif_.driver = &mg_tcpip_driver_tms570;                      \
    mif_.driver_data = &driver_data_;                           \
    MG_SET_MAC_ADDRESS(mif_.mac);                               \
    mg_tcpip_init(mgr, &mif_);                                  \
    MG_INFO(("Driver: tms570, MAC: %M", mg_print_mac, mif_.mac));\
  } while (0)
#endif

