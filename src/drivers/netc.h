#pragma once

#if defined(MG_ENABLE_TCPIP) && MG_ENABLE_TCPIP && \
    defined(MG_ENABLE_DRIVER_NETC) && MG_ENABLE_DRIVER_NETC

struct mg_tcpip_driver_netc_data {
  int mdc_cr;
  uint8_t phy_addr;

  // Selects the physical Ethernet link (ETH0 through ETH3)
  // Its switch port is bridged to pseudo-port 4,
  // which connects internally to ENETC1
  // IMXRT1180 RM 53.4.1.1 NETC Block diagram
  uint8_t link;
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 5
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 48
#endif

#ifndef MG_TCPIP_NETC_LINK
#define MG_TCPIP_NETC_LINK 0
#endif

struct mg_tcpip_driver;
extern struct mg_tcpip_driver mg_tcpip_driver_netc;

#define MG_TCPIP_DRIVER_INIT(mgr)                              \
  do {                                                         \
    static struct mg_tcpip_driver_netc_data driver_data_;      \
    static struct mg_tcpip_if mif_;                            \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                    \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                 \
    driver_data_.link = MG_TCPIP_NETC_LINK;                    \
    mif_.ip = MG_TCPIP_IP;                                     \
    mif_.mask = MG_TCPIP_MASK;                                 \
    mif_.gw = MG_TCPIP_GW;                                     \
    mif_.driver = &mg_tcpip_driver_netc;                       \
    mif_.driver_data = &driver_data_;                          \
    MG_SET_MAC_ADDRESS(mif_.mac);                              \
    mg_tcpip_init(mgr, &mif_);                                 \
    MG_INFO(("Driver: netc, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
