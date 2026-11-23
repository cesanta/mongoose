// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_NRF52) && MG_ENABLE_DRIVER_NRF52

extern struct mg_tcpip_driver mg_tcpip_driver_nrf52;

#define MG_TCPIP_DRIVER_NRF52_INIT(mgr)                          \
  do {                                                           \
    static struct mg_tcpip_if mif_;                              \
    MG_SET_MAC_ADDRESS(mif_.mac);                                \
    mif_.ip = MG_TCPIP_IP;                                       \
    mif_.mask = MG_TCPIP_MASK;                                   \
    mif_.gw = MG_TCPIP_GW;                                       \
    mif_.driver = &mg_tcpip_driver_nrf52;                        \
    mg_tcpip_init((mgr), &mif_);                                 \
    MG_INFO(("Driver: nrf52, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(x) MG_TCPIP_DRIVER_NRF52_INIT(x)
#endif

#endif
