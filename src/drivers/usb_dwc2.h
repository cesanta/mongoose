#pragma once

// Synopsys DesignWare USB 2.0 OTG controller, "DWC2", as a device, see usb.h:
// CDC-NCM Ethernet and a CDC-ACM serial log console. Polled, no DMA, full
// speed. STM32 OTG_FS and OTG_HS (F2, F4, F7, H7, L4+, U5) are DWC2. Set in
// mongoose_config.h:
// - MG_USB_DWC2_BASE: the core's address, e.g. USB1_OTG_HS_PERIPH_BASE. The
//   application enables its clocks, power and pins
// - MG_USB_DWC2_ULPI 1: an external ULPI PHY, run at full speed. Default: the
//   core's own full-speed PHY
// - MG_USB_DWC2_UTMI 1: the core's own high-speed PHY (UTMI), run at full
//   speed, e.g. STM32U59x/U5Ax OTG_HS. The application powers and clocks it
// - MG_USB_DWC2_GCCFG: optional, written to the STM32 GCCFG register, e.g.
//   its PWRDWN bit to power the core's own full-speed PHY
// On STM32, include the device header in mongoose_config.h: the USB serial
// number is then the chip's unique ID. Examples:
// tutorials/stm32/portenta-h7/minimal-usb (ULPI),
// tutorials/stm32/nucleo-u5a5zj-q/minimal-usb (UTMI)

#if (MG_ENABLE_TCPIP || !MG_ENABLE_USB_ETHERNET) && \
    MG_ENABLE_DRIVER_USB_DWC2 &&                    \
    (MG_ENABLE_USB_ETHERNET || MG_ENABLE_USB_SERIAL)

// The controller. Without USB Ethernet, run the serial console from main():
// mg_usb_init(NULL, &mg_usb_hw_dwc2) once, mg_usb_poll(NULL, false) in
// the main loop
extern const struct mg_usb_hw mg_usb_hw_dwc2;

#endif

#if MG_ENABLE_TCPIP && MG_ENABLE_DRIVER_USB_DWC2 && MG_ENABLE_USB_ETHERNET

extern struct mg_tcpip_driver mg_tcpip_driver_usb_dwc2;

// Set MG_TCPIP_IP to give the device a static address and serve DHCP to the
// USB host. Leave it unset to get an address over DHCP, e.g. from macOS
// Internet Sharing. The device MAC comes from MG_SET_MAC_ADDRESS, random if
// it is not set; the host end of the link gets a MAC derived from it
#define MG_TCPIP_DRIVER_USB_DWC2_INIT(mgr)                          \
  do {                                                              \
    static struct mg_tcpip_if mif_;                                 \
    mif_.ip = MG_TCPIP_IP;                                          \
    mif_.mask = MG_TCPIP_MASK;                                      \
    mif_.gw = MG_TCPIP_GW;                                          \
    mif_.enable_dhcp_server = mif_.ip != 0;                         \
    mif_.driver = &mg_tcpip_driver_usb_dwc2;                        \
    mif_.recv_queue.size = MG_USB_QUEUE_SIZE;                       \
    MG_SET_MAC_ADDRESS(mif_.mac);                                   \
    mg_tcpip_init(mgr, &mif_);                                      \
    MG_INFO(("Driver: usb_dwc2, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(mgr) MG_TCPIP_DRIVER_USB_DWC2_INIT(mgr)
#endif

#endif
