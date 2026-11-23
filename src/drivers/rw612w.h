// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RW612W) && \
    MG_ENABLE_DRIVER_RW612W

extern struct mg_tcpip_driver mg_tcpip_driver_rw612w;

#define MG_TCPIP_DRIVER_RW612W_INIT(mgr)                          \
  do {                                                            \
    static struct mg_tcpip_if mif_;                               \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_rw612w;                        \
    mg_tcpip_init((mgr), &mif_);                                  \
    MG_INFO(("Driver: rw612w, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(x) MG_TCPIP_DRIVER_RW612W_INIT(x)
#endif

#endif
