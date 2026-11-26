#pragma once

// nRF52840 USB device controller, see usb.h: CDC-NCM Ethernet and a CDC-ACM
// serial log console. No TinyUSB, polled. Needs the nRF52840 device headers
// from nrfx/mdk on the include path. Example: tutorials/nordic/xiao-nrf52840

#if (MG_ENABLE_TCPIP || !MG_ENABLE_USB_ETHERNET) && \
    MG_ENABLE_DRIVER_USB_NRF52 &&                   \
    (MG_ENABLE_USB_ETHERNET || MG_ENABLE_USB_SERIAL)

// The controller. Without USB Ethernet, run the serial console from main():
// mg_usb_init(NULL, &mg_usb_hw_nrf52) once, mg_usb_poll(NULL, false) in
// the main loop
extern const struct mg_usb_hw mg_usb_hw_nrf52;

#endif

#if MG_ENABLE_TCPIP && MG_ENABLE_DRIVER_USB_NRF52 && MG_ENABLE_USB_ETHERNET

extern struct mg_tcpip_driver mg_tcpip_driver_usb_nrf52;

// Set MG_TCPIP_IP to give the device a static address and serve DHCP to the
// USB host. Leave it unset to get an address over DHCP, e.g. from macOS
// Internet Sharing. The device MAC comes from MG_SET_MAC_ADDRESS, random if
// it is not set; the host end of the link gets a MAC derived from it
#define MG_TCPIP_DRIVER_USB_NRF52_INIT(mgr)                          \
  do {                                                               \
    static struct mg_tcpip_if mif_;                                  \
    mif_.ip = MG_TCPIP_IP;                                           \
    mif_.mask = MG_TCPIP_MASK;                                       \
    mif_.gw = MG_TCPIP_GW;                                           \
    mif_.enable_dhcp_server = mif_.ip != 0;                          \
    mif_.driver = &mg_tcpip_driver_usb_nrf52;                        \
    mif_.recv_queue.size = MG_USB_QUEUE_SIZE;                        \
    MG_SET_MAC_ADDRESS(mif_.mac);                                    \
    mg_tcpip_init(mgr, &mif_);                                       \
    MG_INFO(("Driver: usb_nrf52, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

// Like nrf52.h: both nRF52 drivers can be enabled at once, e.g. in a bridge
#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(mgr) MG_TCPIP_DRIVER_USB_NRF52_INIT(mgr)
#endif

#endif
