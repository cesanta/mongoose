#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RA) && MG_ENABLE_DRIVER_RA

struct mg_tcpip_driver_ra_data {
  // MDC clock "divider". MDC clock is software generated,
  uint32_t clock;    // core clock frequency in Hz
  uint16_t irqno;    // IRQn, R_ICU->IELSR[irqno]
  uint8_t phy_addr;  // PHY address
};

#undef MG_ENABLE_TCPIP_DRIVER_INIT
#define MG_ENABLE_TCPIP_DRIVER_INIT 0  // TODO(): needs SystemCoreClock
#if 0
#ifndef MG_MAC_ADDRESS
#define MG_MAC_ADDRESS MG_MAC_ADDRESS_RANDOM
#endif

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 1
#endif

#ifndef MG_DRIVER_RA_CLOCK
#define MG_DRIVER_RA_CLOCK
#endif

#ifndef MG_DRIVER_RA_IRQNO
#define MG_DRIVER_RA_IRQNO 0
#endif

#define MG_TCPIP_DRIVER_DATA                                \
  static struct mg_tcpip_driver_ra_data driver_data = { \
      .clock = MG_DRIVER_RA_CLOCK,                            \
      .irqno = MG_DRIVER_RA_CLOCK,                            \
      .phy_addr = MG_TCPIP_PHY_ADDR,                        \
  };

#define MG_TCPIP_DRIVER_CODE &mg_tcpip_driver_ra
#define MG_TCPIP_DRIVER_NAME "ra"
#endif
#endif
