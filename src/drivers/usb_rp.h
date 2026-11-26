#pragma once

// RP2040 / RP2350 native USB controller, see usb.h: CDC-NCM Ethernet and
// a CDC-ACM serial log console. No TinyUSB, polled. Example: tutorials/rp/usb

#if (MG_ENABLE_TCPIP || !MG_ENABLE_USB_ETHERNET) && MG_ENABLE_DRIVER_USB_RP && \
    (MG_ENABLE_USB_ETHERNET || MG_ENABLE_USB_SERIAL)

// The controller. Without USB Ethernet, run the serial console from main():
// mg_usb_init(NULL, &mg_usb_hw_rp) once, mg_usb_poll(NULL, false) in
// the main loop
extern const struct mg_usb_hw mg_usb_hw_rp;

#endif

#if MG_ENABLE_TCPIP && MG_ENABLE_DRIVER_USB_RP && MG_ENABLE_USB_ETHERNET

extern struct mg_tcpip_driver mg_tcpip_driver_usb_rp;

// Set MG_TCPIP_IP to give the device a static address and serve DHCP to the
// USB host. Leave it unset to get an address over DHCP, e.g. from macOS
// Internet Sharing. The device MAC comes from MG_SET_MAC_ADDRESS, random if
// it is not set; the host end of the link gets a MAC derived from it
#define MG_TCPIP_DRIVER_USB_RP_INIT(mgr)                          \
  do {                                                            \
    static struct mg_tcpip_if mif_;                               \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.enable_dhcp_server = mif_.ip != 0;                       \
    mif_.driver = &mg_tcpip_driver_usb_rp;                        \
    mif_.recv_queue.size = MG_USB_QUEUE_SIZE;                     \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: usb_rp, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(mgr) MG_TCPIP_DRIVER_USB_RP_INIT(mgr)
#endif

#endif
