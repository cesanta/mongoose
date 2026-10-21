#pragma once
#include "MCXW716C.h"

// See https://mongoose.ws/docs/getting-started/build-environment/
#define MG_ARCH MG_ARCH_ARMGCC
#define MG_TLS MG_TLS_BUILTIN
#define MG_OTA MG_OTA_MCXW71

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_MCXW71 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_TCPIP_TCPRTX 1  // Link can be lossy, enable retransmit

// Stable MAC, so the DHCP address stays the same across reboots: a hash of
// the 128-bit chip unique ID, MSCM UID, locally administered
#define MG_SET_MAC_ADDRESS(mac)                                         \
  do {                                                                  \
    uint8_t b_[32];                                                     \
    mg_sha256(b_, (uint8_t *) MSCM->UID, sizeof(MSCM->UID));            \
    (mac)[0] = 2, (mac)[1] = b_[1], (mac)[2] = b_[2], (mac)[3] = b_[3]; \
    (mac)[4] = b_[4], (mac)[5] = b_[5];                                 \
  } while (0)
