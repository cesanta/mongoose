#pragma once

// USB device: a CDC-NCM Ethernet interface, enabled by MG_ENABLE_USB_ETHERNET,
// and a CDC-ACM serial port for logs, enabled by MG_ENABLE_USB_SERIAL.

#ifndef MG_USB_NTB_SIZE
#define MG_USB_NTB_SIZE 2048  // Transfer block, each direction, >= 2048
#endif
#ifndef MG_USB_TX_SIZE
#define MG_USB_TX_SIZE 4096  // Ethernet frames waiting to go to the host
#endif
#ifndef MG_USB_QUEUE_SIZE
#define MG_USB_QUEUE_SIZE 4096  // Ethernet frames from the host
#endif
#ifndef MG_USB_LOG_SIZE
#define MG_USB_LOG_SIZE 1024  // Serial log ring
#endif

// Endpoints, numbered as in the configuration descriptor
enum mg_usb_ep {
  MG_USB_EP_CTL,       // control
  MG_USB_EP_ETH_EV,    // Ethernet link notifications, IN
  MG_USB_EP_ETH_DATA,  // Ethernet data, IN and OUT
  MG_USB_EP_SER_EV,    // serial notifications, IN, unused
  MG_USB_EP_SER_DATA,  // serial data, IN and OUT
};

// Interfaces: the serial port's control and data first, then Ethernet's. The
// Ethernet data interface has setting 1: Ethernet open
#define MG_USB_IF_ETH_DATA (2 * MG_ENABLE_USB_SERIAL + 1)

// Controller events, reported by mg_usb_hw::poll() to its fn. What fn returns
enum mg_usb_ev {
  MG_USB_EV_RESET,  // Bus reset or detach. Returns 0
  MG_USB_EV_SETUP,  // Control request, buf: the SETUP packet, then its OUT
                    // data. Returns -1: stall, 0: no IN data stage, send
                    // the status stage, 1: IN data stage started on ep 0
  MG_USB_EV_IN,     // An IN packet on ep went out. Returns 1: next packet
                    // sent, 0: transfer done
  MG_USB_EV_OUT     // An OUT packet arrived on ep, buf and len. Returns 0
};

// A USB device controller, implemented by its driver. Packets: up to 64 bytes
struct mg_usb_hw {
  void (*init)(char serial[17]);  // Start, write the serial number
  void (*poll)(int (*fn)(enum mg_usb_ev, enum mg_usb_ep, void *buf, size_t));
  void (*in)(enum mg_usb_ep, const void *buf, size_t len);  // Write IN packet
};

struct mg_tcpip_if;  // Without MG_ENABLE_TCPIP: serial console only

// Start the controller hw, and attach ifp: NULL for the serial console only.
// Call again to attach ifp later; the first call's hw stays in use
bool mg_usb_init(struct mg_tcpip_if *ifp, const struct mg_usb_hw *hw);

// Queue an Ethernet frame for the host. The controller driver's tx
size_t mg_usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp);

// Run the controller. The controller driver's poll; returns true when the
// host has the Ethernet interface open and s1 is true. Serial only: ifp NULL
bool mg_usb_poll(struct mg_tcpip_if *ifp, bool s1);
