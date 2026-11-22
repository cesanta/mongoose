#include "drivers/usb.h"
#include "drivers/usb_rp.h"
#include "net_builtin.h"

#if MG_ENABLE_USB && MG_ENABLE_DRIVER_USB_RP

// RP2040 / RP2350 USB device controller: single core, polled, full speed,
// 64-byte single-buffered endpoints in USB DPRAM. Longer IN transfers go
// out one packet at a time, the next one when the previous completes

struct rp_ep {
  uint8_t *buf;         // Endpoint buffer in USB DPRAM
  io_rw_32 *ctrl;       // Buffer control register
  const uint8_t *data;  // IN transfer: bytes still to send
  size_t left;          // IN transfer: how many
  unsigned packets;     // IN transfer: packets still to send
  uint8_t pid;          // Next DATA PID, 0 or 1
  bool busy;            // Buffer handed to the controller
};

static struct rp_ep s_ep[5][2];  // Index: endpoint number, OUT=0 IN=1
static struct mg_usb s_usb;
static uint8_t s_req[16];  // SETUP packet, then its OUT data stage
static bool s_req_out;     // Waiting for that OUT data stage
static uint8_t s_address;  // Address to apply after the status stage
static char s_serial[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
#if MG_ENABLE_USB_ETHERNET
static uint8_t s_rx[MG_USB_NTB_SIZE], s_tx[MG_USB_NTB_SIZE];
static size_t s_rxlen;  // Received part of the OUT transfer block
static bool s_discard;  // OUT transfer block too long: drop it
#endif
#if MG_ENABLE_USB_SERIAL
static char s_log[64];  // Log output in flight
#endif

// Hand one packet buffer to the controller. OUT: accept up to len bytes
static void packet(unsigned ep, bool in, const void *data, unsigned len) {
  struct rp_ep *e = &s_ep[ep][in];
  uint32_t v = len | (e->pid ? USB_BUF_CTRL_DATA1_PID : 0) |
               (in ? USB_BUF_CTRL_FULL : 0);
  if (in && len) memcpy(e->buf, data, len);
  e->pid ^= 1;
  e->busy = true;
  *e->ctrl = v;
  // RP2040 datasheet: allow at least 12 cycles between writing buffer
  // metadata and handing ownership to the controller through AVAIL
  busy_wait_at_least_cycles(12);
  *e->ctrl = v | USB_BUF_CTRL_AVAIL;
}

// Send the next packet of the IN transfer on ep. False if it is over
static bool in_next(unsigned ep) {
  struct rp_ep *e = &s_ep[ep][1];
  unsigned n = e->left > 64 ? 64 : (unsigned) e->left;
  if (e->packets == 0) return false;
  packet(ep, true, e->data, n);
  e->data += n, e->left -= n, e->packets--;
  return true;
}

// Start an IN transfer. zlp: end a multiple of 64 bytes with an empty packet
static void in_start(unsigned ep, const void *data, size_t len, bool zlp) {
  struct rp_ep *e = &s_ep[ep][1];
  e->data = (const uint8_t *) data, e->left = len;
  e->packets = (unsigned) (len / 64 + (len % 64 || len == 0 || zlp));
  in_next(ep);
}

// Set up endpoint ep, direction in, idle. Endpoints 1 and 3 are interrupt,
// 2 and 4 bulk, each with a 64-byte DPRAM buffer of its own
static void ep_init(unsigned ep, bool in) {
  struct rp_ep *e = &s_ep[ep][in];
  e->ctrl = in ? &usb_dpram->ep_buf_ctrl[ep].in
               : &usb_dpram->ep_buf_ctrl[ep].out;
  e->buf = ep ? &usb_dpram->epx_data[(ep * 2 + in) * 64] : usb_dpram->ep0_buf_a;
  e->pid = 0, e->busy = false, e->packets = 0;
  *e->ctrl = 0;
  if (ep) {
    io_rw_32 *ctrl = in ? &usb_dpram->ep_ctrl[ep - 1].in
                        : &usb_dpram->ep_ctrl[ep - 1].out;
    *ctrl = EP_CTRL_ENABLE_BITS | EP_CTRL_INTERRUPT_PER_BUFFER |
            ((ep & 1 ? 3U : 2U) << EP_CTRL_BUFFER_TYPE_LSB) |
            (uint32_t) ((uintptr_t) e->buf - (uintptr_t) usb_dpram);
  }
}

#if MG_ENABLE_USB_ETHERNET
// Ethernet data endpoints idle, then armed to receive if Ethernet is open
static void data_reset(void) {
  ep_init(2, false), ep_init(2, true);
  s_rxlen = 0, s_discard = false;
  if (s_usb.alt) packet(2, false, NULL, 64);
}

// A packet on the Ethernet OUT endpoint: add it to the transfer block
static void rx(const uint8_t *buf, unsigned n) {
  if (s_rxlen + n <= sizeof(s_rx) && !s_discard) {
    memcpy(s_rx + s_rxlen, buf, n);
    s_rxlen += n;
  } else {
    s_discard = true;
  }
  // At the advertised maximum OUT size, the host need not send a ZLP.
  // Other transfers end with a short packet (including a ZLP)
  if (n < 64 || s_rxlen == sizeof(s_rx)) {
    if (!s_discard) mg_usb_decode(&s_usb, s_rx, s_rxlen);
    s_rxlen = 0;
    s_discard = false;
  }
  if (s_usb.alt) packet(2, false, NULL, 64);
}
#endif

// All endpoints idle, then the serial port armed to receive if configured
static void endpoints_reset(void) {
  static const uint8_t eps[] = {
      0x00, 0x80,
#if MG_ENABLE_USB_ETHERNET
      0x81,
#endif
#if MG_ENABLE_USB_SERIAL
      0x83, 0x04, 0x84,
#endif
  };
  size_t i;
  for (i = 0; i < sizeof(eps); i++) ep_init(eps[i] & 15U, eps[i] >> 7);
#if MG_ENABLE_USB_ETHERNET
  data_reset();
#endif
#if MG_ENABLE_USB_SERIAL
  if (s_usb.configured) packet(4, false, NULL, 64);
#endif
}

// Stall endpoint 0 until the next SETUP
static void stall(void) {
  usb_hw->ep_stall_arm =
      USB_EP_STALL_ARM_EP0_IN_BITS | USB_EP_STALL_ARM_EP0_OUT_BITS;
  *s_ep[0][1].ctrl = USB_BUF_CTRL_STALL;
  *s_ep[0][0].ctrl = USB_BUF_CTRL_STALL;
}

// Run a complete control request: SETUP in s_req, then any OUT data
static void request(void) {
  const void *reply;
  int n = mg_usb_setup(&s_usb, s_req, &reply);
  if (n < 0) {
    stall();
    return;
  }
  switch ((s_req[0] << 8) | s_req[1]) {
    case 0x0005:  // SET_ADDRESS, applied after the status stage
      s_address = s_req[2];
      break;
    case 0x0009:  // SET_CONFIGURATION
      endpoints_reset();
      break;
#if MG_ENABLE_USB_ETHERNET
    case 0x010b:  // SET_INTERFACE
      if (s_req[4] == 1) data_reset();
      break;
#endif
    case 0x0201: {  // CLEAR_FEATURE(ENDPOINT_HALT)
      struct rp_ep *e = &s_ep[s_req[4] & 15][s_req[4] >> 7];
      *e->ctrl &= ~USB_BUF_CTRL_STALL, e->pid = 0;
      break;
    }
  }
  // IN data stage, or the status stage, both starting with DATA1
  s_ep[0][1].pid = 1;
  in_start(0, reply, (size_t) n, n < (int) MG_LOAD_LE16(s_req + 6));
}

// A SETUP packet: run it, or first receive its OUT data stage
static void setup(void) {
  unsigned wlen;
  memcpy(s_req, (const void *) usb_dpram->setup_packet, 8);
  wlen = MG_LOAD_LE16(s_req + 6);
  // A SETUP cancels the preceding control transfer. OUT stages use DATA1
  ep_init(0, false), ep_init(0, true);
  s_ep[0][0].pid = 1;
  s_address = 0, s_req_out = false;
  if ((s_req[0] & 0x80) || wlen == 0) {
    request();
  } else if (wlen > sizeof(s_req) - 8) {
    stall();
  } else {
    s_req_out = true;
    packet(0, false, NULL, 64);
  }
}

// A buffer the controller has finished with
static void complete(unsigned ep, bool in) {
  struct rp_ep *e = &s_ep[ep][in];
  unsigned n = *e->ctrl & USB_BUF_CTRL_LEN_MASK;
  e->busy = false;
  if (in && in_next(ep)) return;
  if (ep == 0 && in) {  // Control IN data or status stage sent
    if (s_address) {
      usb_hw->dev_addr_ctrl = s_address, s_address = 0;
    } else if ((s_req[0] & 0x80) && MG_LOAD_LE16(s_req + 6)) {
      packet(0, false, NULL, 64);  // Host's OUT status stage
    }
  } else if (ep == 0 && !in && s_req_out) {  // Control OUT data stage
    s_req_out = false;
    if (n != MG_LOAD_LE16(s_req + 6)) {
      stall();
    } else {
      memcpy(s_req + 8, e->buf, n);
      request();
    }
#if MG_ENABLE_USB_ETHERNET
  } else if (ep == 2 && !in) {
    rx(e->buf, n);
#endif
#if MG_ENABLE_USB_SERIAL
  } else if (ep == 4 && !in) {
    packet(4, false, NULL, 64);  // Serial input is discarded: log console
#endif
  }
}

void mg_usb_init(void) {
  static bool started;
  if (started) return;
  started = true;
  pico_get_unique_board_id_string(s_serial, sizeof(s_serial));
  s_usb.serial = s_serial;
  mg_usb_reset(&s_usb);
#if MG_ENABLE_USB_SERIAL
  mg_log_set_fn(mg_usb_log, &s_usb);  // Log to the USB serial port
#endif
  reset_block(RESETS_RESET_USBCTRL_BITS);
  unreset_block_wait(RESETS_RESET_USBCTRL_BITS);
  memset(usb_dpram, 0, sizeof(*usb_dpram));
  usb_hw->muxing = USB_USB_MUXING_TO_PHY_BITS | USB_USB_MUXING_SOFTCON_BITS;
  usb_hw->pwr =
      USB_USB_PWR_VBUS_DETECT_BITS | USB_USB_PWR_VBUS_DETECT_OVERRIDE_EN_BITS;
  usb_hw->main_ctrl = USB_MAIN_CTRL_CONTROLLER_EN_BITS;
  usb_hw->sie_ctrl = USB_SIE_CTRL_EP0_INT_1BUF_BITS;
  usb_hw->inte = 0;  // Poll raw status instead of an interrupt handler
  endpoints_reset();
  hw_set_bits(&usb_hw->sie_ctrl, USB_SIE_CTRL_PULLUP_EN_BITS);
}

void mg_usb_poll(void) {
  uint32_t status, done;
  unsigned i;
  mg_usb_init();
  status = usb_hw->intr;
#if MG_ENABLE_USB_SERIAL
  if (s_usb.reboot && mg_millis() > s_usb.reboot) reset_usb_boot(0, 0);
#endif
  if (status & USB_INTR_BUS_RESET_BITS) {
    usb_hw->sie_status = USB_SIE_STATUS_BUS_RESET_BITS;
    usb_hw->dev_addr_ctrl = 0;
    usb_hw->buf_status = 0xffffffff;
    mg_usb_reset(&s_usb);
    endpoints_reset();
    return;
  }
  if (status & USB_INTR_SETUP_REQ_BITS) {
    // Discard stale EP0 completions from an interrupted control transfer
    usb_hw->buf_status = 3;
    usb_hw->sie_status = USB_SIE_STATUS_SETUP_REC_BITS;
    setup();
  }
  done = usb_hw->buf_status;
  for (i = 0; i < 10; i++) {  // Bits: EP0 IN, EP0 OUT, EP1 IN, ...
    if (!(done & (1U << i))) continue;
    usb_hw->buf_status = 1U << i;
    if (s_ep[i / 2][!(i & 1)].ctrl) complete(i / 2, !(i & 1));
  }
#if MG_ENABLE_USB_ETHERNET
  if (!s_ep[1][1].busy) {
    const void *msg;
    size_t n = mg_usb_notify(&s_usb, &msg);
    if (n) in_start(1, msg, n, false);
  }
#endif
#if MG_ENABLE_USB_SERIAL
  if (s_usb.configured && s_usb.dtr && !s_ep[4][1].busy) {
    size_t n = mg_usb_log_read(&s_usb, s_log, sizeof(s_log));
    if (n) in_start(4, s_log, n, true);
  }
#endif
}

#if MG_ENABLE_USB_ETHERNET
static bool mg_tcpip_driver_usb_rp_init(struct mg_tcpip_if *ifp) {
  s_usb.ifp = ifp;  // mg_tcpip_init() has started USB already
  return true;
}

static size_t mg_tcpip_driver_usb_rp_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  // One frame can be in flight. Keep USB moving while waiting, but bound
  // the wait if the host stops reading or the cable is gone
  uint64_t until = mg_millis() + 20;
  size_t n = 0;
  while (s_ep[2][1].busy && mg_millis() < until) mg_usb_poll();
  if (!s_ep[2][1].busy) n = mg_usb_encode(&s_usb, s_tx, buf, len);
  if (n) in_start(2, s_tx, n, n < s_usb.input_size);
  (void) ifp;
  return n ? len : 0;
}

// USB itself runs from mg_mgr_poll(). Report the link: Ethernet open
static bool mg_tcpip_driver_usb_rp_poll(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp;
  return s1 && s_usb.alt && s_usb.filter;
}

struct mg_tcpip_driver mg_tcpip_driver_usb_rp = {
    mg_tcpip_driver_usb_rp_init, mg_tcpip_driver_usb_rp_tx, NULL,
    mg_tcpip_driver_usb_rp_poll};
#endif

#endif
