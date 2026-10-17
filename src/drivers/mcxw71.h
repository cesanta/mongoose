#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_MCXW71) && \
    MG_ENABLE_DRIVER_MCXW71

// NXP MCXW71 IEEE 802.15.4 radio, ZLL link layer, as an Ethernet interface:
// Ethernet frames are split into radio frames, see mg_l2_802154_tx().
// The transceiver (XCVR) must be set up for 802.15.4 before mg_tcpip_init(),
// see hal_radio_init() in the FRDM-MCXW71 tutorial.
// driver_data: struct mg_l2_802154_data, channel and PAN ID set

#define MG_TCPIP_DRIVER_INIT(mgr)                                    \
  do {                                                               \
    static struct mg_l2_802154_data driver_data_;                  \
    static struct mg_tcpip_if mif_;                                  \
    driver_data_.channel = MG_L2_802154_CHANNEL;                   \
    driver_data_.pan_id = MG_L2_802154_PAN_ID;                     \
    mif_.driver = &mg_tcpip_driver_mcxw71;                           \
    mif_.driver_data = &driver_data_;                                \
    MG_SET_MAC_ADDRESS(mif_.mac);                                    \
    mg_tcpip_init(mgr, &mif_);                                       \
    MG_INFO(("Driver: mcxw71, channel %d", MG_L2_802154_CHANNEL)); \
  } while (0)

#endif
