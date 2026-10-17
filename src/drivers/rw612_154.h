#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RW612_154) && \
    MG_ENABLE_DRIVER_RW612_154

// NXP RW612 IEEE 802.15.4 radio. The radio runs on CPU2: NXP BLE/802.15.4
// combo firmware, an OpenThread radio co-processor (RCP). The firmware must
// be loaded and running before mg_tcpip_init(), see the FRDM-RW612 lowpan
// tutorial. The driver talks to it over the CPU2 <-> CPU3 IMU mailbox:
// IMUMC messages carrying HDLC framed Spinel. Frames passed to tx() and
// returned by rx() are raw 802.15.4 frames: MAC header and payload, no FCS.
// Use with MG_TCPIP_L2_LOWPAN: channel, PAN ID and addresses are taken from
// ifp->l2data.lowpan and ifp->mac. No driver_data

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(mgr)                                   \
  do {                                                              \
    static struct mg_tcpip_if mif_;                                 \
    mif_.l2type = MG_TCPIP_L2_LOWPAN;                               \
    mif_.driver = &mg_tcpip_driver_rw612_154;                       \
    mif_.l2data.lowpan.channel = 15;                                \
    mif_.l2data.lowpan.pan_id = 0xabcd;                             \
    MG_SET_MAC_ADDRESS(mif_.mac); /* EUI-64 */                      \
    mg_tcpip_init(mgr, &mif_);                                      \
    (mgr)->use_dns6 = true; /* IPv6 only: no IPv4 over 6LoWPAN */   \
    MG_INFO(("Driver: rw612_154, 6LoWPAN"));                        \
  } while (0)
#endif

#endif
