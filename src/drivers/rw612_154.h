#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RW612_154) && \
    MG_ENABLE_DRIVER_RW612_154

// NXP RW612 IEEE 802.15.4 radio, as an Ethernet interface: Ethernet frames
// are split into radio frames, see mg_l2_802154_tx(). The radio runs on
// CPU2: NXP BLE/802.15.4 combo firmware, an OpenThread radio co-processor
// (RCP). The firmware must be loaded and running before mg_tcpip_init(), see
// the FRDM-RW612 eth_radio_bridge tutorial. The driver talks to it over the
// CPU2 <-> CPU3 IMU mailbox: IMUMC messages carrying HDLC framed Spinel.
// driver_data: struct mg_l2_802154_data, channel and PAN ID set

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(mgr)                                       \
  do {                                                                  \
    static struct mg_l2_802154_data driver_data_;                     \
    static struct mg_tcpip_if mif_;                                     \
    driver_data_.channel = MG_L2_802154_CHANNEL;                      \
    driver_data_.pan_id = MG_L2_802154_PAN_ID;                        \
    mif_.driver = &mg_tcpip_driver_rw612_154;                           \
    mif_.driver_data = &driver_data_;                                   \
    MG_SET_MAC_ADDRESS(mif_.mac);                                       \
    mg_tcpip_init(mgr, &mif_);                                          \
    MG_INFO(("Driver: rw612_154, channel %d", MG_L2_802154_CHANNEL)); \
  } while (0)
#endif

#endif
