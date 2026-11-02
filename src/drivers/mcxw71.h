// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_MCXW71) && \
    MG_ENABLE_DRIVER_MCXW71

struct mg_tcpip_driver_mcxw71_data {
  struct mg_mesh_node *mesh;       // Driver-owned runtime state
  const struct mg_mesh *strategy;  // NULL selects mg_mesh_default
};

extern struct mg_tcpip_driver mg_tcpip_driver_mcxw71;

#define MG_TCPIP_DRIVER_MCXW71_INIT(manager)                      \
  do {                                                            \
    static struct mg_tcpip_driver_mcxw71_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.mgr = (manager);                                         \
    mif_.driver = &mg_tcpip_driver_mcxw71;                        \
    mif_.driver_data = &driver_data_;                             \
    mg_tcpip_init((manager), &mif_);                              \
    MG_INFO(("Driver: mcxw71, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(x) MG_TCPIP_DRIVER_MCXW71_INIT(x)
#endif

#endif
