#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_CMSIS) && MG_ENABLE_DRIVER_CMSIS

#include "Driver_ETH_MAC.h"  // keep this include
#include "Driver_ETH_PHY.h"  // keep this include

extern struct mg_tcpip_driver mg_tcpip_driver_cmsis;

#define MG_TCPIP_DRIVER_CMSIS_INIT(mgr)                          \
  do {                                                           \
    static struct mg_tcpip_if mif_;                               \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_cmsis;                          \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mg_tcpip_init((mgr), &mif_);                                 \
    MG_INFO(("Driver: cmsis, MAC: %M", mg_print_mac, mif_.mac));    \
  } while (0)

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(x) MG_TCPIP_DRIVER_CMSIS_INIT(x)
#endif

#endif
