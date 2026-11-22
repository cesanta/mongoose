#pragma once

// USB device, independent of the controller: CDC-NCM Ethernet, enabled by
// MG_ENABLE_USB_ETHERNET, and a CDC-ACM serial port that carries the
// Mongoose log, enabled by MG_ENABLE_USB_SERIAL. The controller drivers
// are usb_rp.c, usb_samd21.c and usb_nrf52.c, enabled by
// MG_ENABLE_DRIVER_USB_*. Mongoose runs USB from mg_tcpip_init() and
// mg_mgr_poll(), so user code needs no USB calls

#if MG_ENABLE_TCPIP &&                                          \
    (MG_ENABLE_DRIVER_USB_RP || MG_ENABLE_DRIVER_USB_SAMD21 ||  \
     MG_ENABLE_DRIVER_USB_NRF52) &&                             \
    (MG_ENABLE_USB_ETHERNET || MG_ENABLE_USB_SERIAL)
#define MG_ENABLE_USB 1

// #if defined(MG_ENABLE_DRIVER_USB_SAMD21) && MG_ENABLE_DRIVER_USB_SAMD21
// #define MG_USB_DEFAULT_SIZE 2048  // SAMD21: 32k RAM
// #else
// #define MG_USB_DEFAULT_SIZE 4096
// #endif
#define MG_USB_DEFAULT_SIZE 2048
#ifndef MG_USB_NTB_SIZE  // Transfer block buffer, each direction, >= 2048
#define MG_USB_NTB_SIZE MG_USB_DEFAULT_SIZE
#endif
#ifndef MG_USB_LOG_SIZE  // Serial log ring buffer
#define MG_USB_LOG_SIZE (MG_USB_DEFAULT_SIZE / 2)
#endif
#ifndef MG_USB_QUEUE_SIZE  // TCP/IP stack receive queue
#define MG_USB_QUEUE_SIZE (MG_USB_DEFAULT_SIZE * 2)
#endif

// Endpoints: 1 interrupt IN, link notifications. 2 bulk IN/OUT, Ethernet.
// 3 interrupt IN, serial notifications (unused). 4 bulk IN/OUT, serial

struct mg_usb {
  const char *serial;  // USB serial number, set by the driver
  bool configured;     // Host has set the configuration
  uint8_t buf[66];     // Generated replies
#if MG_ENABLE_USB_ETHERNET
  struct mg_tcpip_if *ifp;    // TCP/IP interface, set by the driver
  bool alt;                   // Ethernet open: data interface setting 1
  uint16_t filter, sequence;  // Host packet filter, next IN block number
  uint32_t input_size;        // Largest IN block the host accepts
  uint8_t notify;             // Pending link notifications
#endif
#if MG_ENABLE_USB_SERIAL
  bool dtr;                   // Terminal open
  uint64_t reboot;            // mg_millis() to enter the bootloader, or 0
  uint8_t line[7];            // Serial line coding
  char log[MG_USB_LOG_SIZE];  // Serial log ring buffer
  unsigned head, tail;        // Log ring write and read counters
#endif
};

// Implemented by the enabled controller driver. Start the controller, once,
// and send the log to the serial port
void mg_usb_init(void);

// Implemented by the enabled controller driver. Run the controller. Calls
// mg_usb_init() first, so it works without a TCP/IP interface too
void mg_usb_poll(void);

// Unconfigured state, on start and after a bus reset. Keeps the log
void mg_usb_reset(struct mg_usb *u);

// Handle an 8-byte SETUP packet, followed by its OUT data stage if it has
// one. Returns -1 to stall, else the IN reply length, 0 for status only
int mg_usb_setup(struct mg_usb *u, const uint8_t *req, const void **reply);

#if MG_ENABLE_USB_ETHERNET
// Next link notification for endpoint 1. Returns its length, 0 if none
size_t mg_usb_notify(struct mg_usb *u, const void **msg);

// Wrap an Ethernet frame into a transfer block at ntb. Returns the block
// length, 0 if the link is down or the frame does not fit
size_t mg_usb_encode(struct mg_usb *u, uint8_t *ntb, const void *buf,
                     size_t len);

// Queue the Ethernet frames of a received transfer block to the TCP/IP stack
void mg_usb_decode(struct mg_usb *u, const uint8_t *ntb, size_t len);
#endif

#if MG_ENABLE_USB_SERIAL
// Log output function for mg_log_set_fn(mg_usb_log, u). Keeps the latest
// MG_USB_LOG_SIZE characters
void mg_usb_log(char ch, void *u);

// Take up to len logged characters into buf. Returns how many
size_t mg_usb_log_read(struct mg_usb *u, void *buf, size_t len);
#endif

#else
#define MG_ENABLE_USB 0
#define mg_usb_init()
#define mg_usb_poll()
#endif
