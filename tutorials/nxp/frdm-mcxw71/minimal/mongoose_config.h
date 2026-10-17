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

// Stable EUI-64, so the IPv6 addresses stay the same across reboots: a hash
// of the 128-bit chip unique ID, MSCM UID, locally administered
#define MG_SET_MAC_ADDRESS(mac)                                             \
  do {                                                                      \
    uint8_t b_[32];                                                         \
    mg_sha256(b_, (uint8_t *) MSCM->UID, sizeof(MSCM->UID));                \
    (mac)[0] = 2, (mac)[1] = b_[1], (mac)[2] = b_[2], (mac)[3] = b_[3];     \
    (mac)[4] = b_[4], (mac)[5] = b_[5], (mac)[6] = b_[6], (mac)[7] = b_[7]; \
  } while (0)
