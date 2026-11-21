#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_W5500) && \
    MG_ENABLE_DRIVER_W5500

extern struct mg_tcpip_driver mg_tcpip_driver_w5500;

// Initialises W5500 on mgr. The application defines
// struct mg_tcpip_spi mg_spi. Example: tutorials/rp/w5500-evb-pico
#define MG_TCPIP_DRIVER_W5500_INIT(mgr)                          \
  do {                                                           \
    static struct mg_tcpip_if mif_;                              \
    mif_.ip = MG_TCPIP_IP;                                       \
    mif_.mask = MG_TCPIP_MASK;                                   \
    mif_.gw = MG_TCPIP_GW;                                       \
    mif_.driver = &mg_tcpip_driver_w5500;                        \
    mif_.driver_data = &mg_spi;                                  \
    MG_SET_MAC_ADDRESS(mif_.mac);                                \
    mg_tcpip_init((mgr), &mif_);                                 \
    MG_INFO(("Driver: w5500, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(x) MG_TCPIP_DRIVER_W5500_INIT(x)
#endif

#endif
