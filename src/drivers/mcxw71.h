#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_MCXW71) && \
    MG_ENABLE_DRIVER_MCXW71

// NXP MCXW71 IEEE 802.15.4 radio, ZLL link layer. Frames passed to tx() and
// returned by rx() are raw 802.15.4 frames: MAC header and payload, no FCS.
// The transceiver (XCVR) must be set up for 802.15.4 before mg_tcpip_init(),
// see hal_radio_init() in the FRDM-MCXW71 tutorial.
// Use with MG_TCPIP_L2_LOWPAN: channel, PAN ID and addresses are taken from
// ifp->l2data.lowpan and ifp->mac. No driver_data

#define MG_TCPIP_DRIVER_INIT(mgr)                                   \
  do {                                                              \
    static struct mg_tcpip_if mif_;                                 \
    mif_.l2type = MG_TCPIP_L2_LOWPAN;                               \
    mif_.driver = &mg_tcpip_driver_mcxw71;                          \
    mif_.l2data.lowpan.channel = 15;                                \
    mif_.l2data.lowpan.pan_id = 0xabcd;                             \
    MG_SET_MAC_ADDRESS(mif_.mac); /* EUI-64 */                      \
    mg_tcpip_init(mgr, &mif_);                                      \
    (mgr)->use_dns6 = true; /* IPv6 only: no IPv4 over 6LoWPAN */   \
    MG_INFO(("Driver: mcxw71, 6LoWPAN"));                           \
  } while (0)

#endif
