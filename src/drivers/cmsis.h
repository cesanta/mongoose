#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_CMSIS) && MG_ENABLE_DRIVER_CMSIS

#include "Driver_ETH_MAC.h"  // keep this include
#include "Driver_ETH_PHY.h"  // keep this include

#ifndef MG_MAC_ADDRESS
#define MG_MAC_ADDRESS MG_MAC_ADDRESS_RANDOM
#endif

#define MG_TCPIP_DRIVER_DATA int driver_data;

#define MG_TCPIP_DRIVER_CODE &mg_tcpip_driver_cmsis
#define MG_TCPIP_DRIVER_NAME "cmsis"

#endif
