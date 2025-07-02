#pragma once

#if MG_ENABLE_TCPIP && \
  (defined(MG_ENABLE_DRIVER_RA6) && MG_ENABLE_DRIVER_RA6) || \
  (defined(MG_ENABLE_DRIVER_RA8) && MG_ENABLE_DRIVER_RA8)

struct mg_tcpip_driver_ra_data {
  // MDC clock "divider". MDC clock is software generated,
  uint32_t clock;    // core clock frequency in Hz
  uint16_t irqno;    // IRQn, R_ICU->IELSR[irqno]
  uint8_t phy_addr;  // PHY address
};

#ifndef MG_DRIVER_CLK_FREQ
#define MG_DRIVER_CLK_FREQ 100000000UL
#endif

#ifndef MG_DRIVER_IRQ_NO
#define MG_DRIVER_IRQ_NO 0
#endif

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                             \
  do {                                                        \
    static struct mg_tcpip_driver_ra_data driver_data_;       \
    static struct mg_tcpip_if mif_;                           \
    driver_data_.clock = MG_DRIVER_CLK_FREQ;                  \
    driver_data_.irqno = MG_DRIVER_IRQ_NO;                    \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                \
    mif_.ip = MG_TCPIP_IP;                                    \
    mif_.mask = MG_TCPIP_MASK;                                \
    mif_.gw = MG_TCPIP_GW;                                    \
    mif_.driver = &mg_tcpip_driver_ra;                        \
    mif_.driver_data = &driver_data_;                         \
    MG_SET_MAC_ADDRESS(mif_.mac);                             \
    mg_tcpip_init(mgr, &mif_);                                \
    MG_INFO(("Driver: ra, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
