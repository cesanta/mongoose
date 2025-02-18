#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RW612) && MG_ENABLE_DRIVER_RW612

struct mg_tcpip_driver_rw612_data {
  // 38.1.8 MII Speed Control Register (MSCR)
  // MDC clock frequency must not exceed 2.5 MHz and is calculated as follows:
  // MDC_freq = P_clock / ((mdc_cr + 1) * 2), where P_clock is the
  // peripheral bus clock.
  // IEEE802.3 clause 22 defines a minimum of 10 ns for the hold time on the
  // MDIO output. Depending on the host bus frequency, the setting may need
  // to be increased.
  int mdc_cr;
  int mdc_holdtime; // Valid values: [0-7]
  uint8_t phy_addr;
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 2
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 51
#endif

#ifndef MG_DRIVER_MDC_HOLDTIME
#define MG_DRIVER_MDC_HOLDTIME 3
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_rw612_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                       \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                    \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_rw612;                        \
    mif_.driver_data = &driver_data_;                             \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: rw612, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
