#include "net_builtin.h"
#include "drivers/usb.h"
#include "drivers/usb_rp.h"
#include "util.h"

#if (MG_ENABLE_TCPIP || !MG_ENABLE_USB_ETHERNET) && MG_ENABLE_DRIVER_USB_RP && \
    (MG_ENABLE_USB_ETHERNET || MG_ENABLE_USB_SERIAL)

// RP2040 / RP2350 USB device controller: single core, polled, full speed,
// 64-byte single-buffered endpoints in USB DPRAM

// usb.c's event handler, see mg_usb_hw::poll()
#define MGI_USB_RP_FN int (*fn)(enum mg_usb_ev, enum mg_usb_ep, void *, size_t)

struct mgi_usb_rp_ep {
  uint8_t *buf;    // Endpoint buffer in USB DPRAM
  io_rw_32 *ctrl;  // Buffer control register
  uint8_t pid;     // Next DATA PID, 0 or 1
};

static struct mgi_usb_rp_ep s_mgi_usb_rp_ep[5][2];  // Endpoint, OUT=0 IN=1
static uint8_t s_mgi_usb_rp_req[16];  // SETUP packet, then its OUT data
static bool s_mgi_usb_rp_req_out;     // Waiting for that OUT data stage
static uint8_t s_mgi_usb_rp_address;  // Address to set after the status stage

// Hand one packet buffer to the controller. OUT: accept up to len bytes
static void mgi_usb_rp_packet(unsigned ep, bool in, const void *data,
                              unsigned len) {
  struct mgi_usb_rp_ep *e = &s_mgi_usb_rp_ep[ep][in];
  uint32_t v = len | (e->pid ? USB_BUF_CTRL_DATA1_PID : 0) |
               (in ? USB_BUF_CTRL_FULL : 0);
  if (in && len) memcpy(e->buf, data, len);
  e->pid ^= 1;
  *e->ctrl = v;
  // RP2040 datasheet: allow at least 12 cycles between writing buffer
  // metadata and handing ownership to the controller through AVAIL
  busy_wait_at_least_cycles(12);
  *e->ctrl = v | USB_BUF_CTRL_AVAIL;
}

// mg_usb_hw::in()
static void mgi_usb_rp_in(enum mg_usb_ep ep, const void *buf, size_t len) {
  mgi_usb_rp_packet(ep, true, buf, (unsigned) len);
}

// Set up endpoint ep, direction in, idle. Endpoints 1 and 3 are interrupt,
// 2 and 4 bulk, each with a 64-byte DPRAM buffer of its own
static void mgi_usb_rp_ep_init(unsigned ep, bool in) {
  struct mgi_usb_rp_ep *e = &s_mgi_usb_rp_ep[ep][in];
  e->ctrl =
      in ? &usb_dpram->ep_buf_ctrl[ep].in : &usb_dpram->ep_buf_ctrl[ep].out;
  e->buf = ep ? &usb_dpram->epx_data[(ep * 2 + in) * 64] : usb_dpram->ep0_buf_a;
  e->pid = 0;
  *e->ctrl = 0;
  if (ep) {
    io_rw_32 *ctrl =
        in ? &usb_dpram->ep_ctrl[ep - 1].in : &usb_dpram->ep_ctrl[ep - 1].out;
    *ctrl = EP_CTRL_ENABLE_BITS | EP_CTRL_INTERRUPT_PER_BUFFER |
            ((ep & 1 ? 3U : 2U) << EP_CTRL_BUFFER_TYPE_LSB) |
            (uint32_t) ((uintptr_t) e->buf - (uintptr_t) usb_dpram);
  }
}

#if MG_ENABLE_USB_ETHERNET
// Ethernet data endpoints idle, OUT armed to receive if Ethernet is open
static void mgi_usb_rp_data_reset(bool open) {
  mgi_usb_rp_ep_init(2, false), mgi_usb_rp_ep_init(2, true);
  if (open) mgi_usb_rp_packet(2, false, NULL, 64);
}
#endif

// Endpoints 1-4 idle. If configured, the serial port armed to receive
static void mgi_usb_rp_endpoints(bool on) {
#if MG_ENABLE_USB_ETHERNET
  mgi_usb_rp_ep_init(1, true);
  mgi_usb_rp_data_reset(false);
#endif
#if MG_ENABLE_USB_SERIAL
  mgi_usb_rp_ep_init(3, true);
  mgi_usb_rp_ep_init(4, false), mgi_usb_rp_ep_init(4, true);
  if (on) mgi_usb_rp_packet(4, false, NULL, 64);
#endif
  (void) on;
}

// Stall endpoint 0 until the next SETUP
static void mgi_usb_rp_stall(void) {
  usb_hw->ep_stall_arm =
      USB_EP_STALL_ARM_EP0_IN_BITS | USB_EP_STALL_ARM_EP0_OUT_BITS;
  *s_mgi_usb_rp_ep[0][1].ctrl = USB_BUF_CTRL_STALL;
  *s_mgi_usb_rp_ep[0][0].ctrl = USB_BUF_CTRL_STALL;
}

// A complete control request: usb.c answers it, then the controller follows
static void mgi_usb_rp_request(MGI_USB_RP_FN) {
  uint8_t *r = s_mgi_usb_rp_req;
  size_t out = (r[0] & 0x80) ? 0 : MG_LOAD_LE16(r + 6);  // OUT data stage
  int res = fn(MG_USB_EV_SETUP, MG_USB_EP_CTL, r, 8 + out);
  if (res < 0) {
    mgi_usb_rp_stall();
    return;
  }
  switch ((r[0] << 8) | r[1]) {
    case 0x0005:  // SET_ADDRESS, set after the status stage
      s_mgi_usb_rp_address = r[2];
      break;
    case 0x0009:  // SET_CONFIGURATION
      mgi_usb_rp_endpoints(r[2] == 1);
      break;
#if MG_ENABLE_USB_ETHERNET
    case 0x010b:  // SET_INTERFACE
      if (r[4] == MG_USB_IF_ETH_DATA) mgi_usb_rp_data_reset(r[2] == 1);
      break;
#endif
    case 0x0201: {  // CLEAR_FEATURE(ENDPOINT_HALT)
      struct mgi_usb_rp_ep *e = &s_mgi_usb_rp_ep[r[4] & 15][r[4] >> 7];
      *e->ctrl &= ~USB_BUF_CTRL_STALL, e->pid = 0;
      break;
    }
  }
  if (res == 0) mgi_usb_rp_packet(0, true, NULL, 0);  // Status stage, DATA1
}

// A SETUP packet: run it, or first receive its OUT data stage
static void mgi_usb_rp_setup(MGI_USB_RP_FN) {
  uint8_t *r = s_mgi_usb_rp_req;
  unsigned wlen;
  memcpy(r, (const void *) usb_dpram->setup_packet, 8);
  wlen = MG_LOAD_LE16(r + 6);
  // A SETUP cancels the preceding control transfer. Both its data and
  // status stages start with DATA1
  mgi_usb_rp_ep_init(0, false), mgi_usb_rp_ep_init(0, true);
  s_mgi_usb_rp_ep[0][0].pid = s_mgi_usb_rp_ep[0][1].pid = 1;
  s_mgi_usb_rp_address = 0, s_mgi_usb_rp_req_out = false;
  if ((r[0] & 0x80) || wlen == 0) {
    mgi_usb_rp_request(fn);
  } else if (wlen > sizeof(s_mgi_usb_rp_req) - 8) {
    mgi_usb_rp_stall();
  } else {
    s_mgi_usb_rp_req_out = true;
    mgi_usb_rp_packet(0, false, NULL, 64);
  }
}

// A buffer the controller has finished with
static void mgi_usb_rp_complete(unsigned ep, bool in, MGI_USB_RP_FN) {
  struct mgi_usb_rp_ep *e = &s_mgi_usb_rp_ep[ep][in];
  uint8_t *r = s_mgi_usb_rp_req;
  unsigned n = *e->ctrl & USB_BUF_CTRL_LEN_MASK;
  if (in && fn(MG_USB_EV_IN, (enum mg_usb_ep) ep, NULL, 0)) return;
  if (ep == 0 && in) {  // Control IN data or status stage sent
    if (s_mgi_usb_rp_address) {
      usb_hw->dev_addr_ctrl = s_mgi_usb_rp_address, s_mgi_usb_rp_address = 0;
    } else if ((r[0] & 0x80) && MG_LOAD_LE16(r + 6)) {
      mgi_usb_rp_packet(0, false, NULL, 64);  // Host's OUT status stage
    }
  } else if (ep == 0 && !in && s_mgi_usb_rp_req_out) {  // OUT data stage
    s_mgi_usb_rp_req_out = false;
    if (n != MG_LOAD_LE16(r + 6)) {
      mgi_usb_rp_stall();
    } else {
      memcpy(r + 8, e->buf, n);
      mgi_usb_rp_request(fn);
    }
#if MG_ENABLE_USB_ETHERNET
  } else if (ep == MG_USB_EP_ETH_DATA && !in) {
    fn(MG_USB_EV_OUT, MG_USB_EP_ETH_DATA, e->buf, n);
    mgi_usb_rp_packet(ep, false, NULL, 64);
#endif
#if MG_ENABLE_USB_SERIAL
  } else if (ep == MG_USB_EP_SER_DATA && !in) {
    mgi_usb_rp_packet(ep, false, NULL, 64);  // Serial input: dropped
#endif
  }
}

// mg_usb_hw::poll()
static void mgi_usb_rp_poll(MGI_USB_RP_FN) {
  uint32_t status = usb_hw->intr, done;
  unsigned i;
  if (status & USB_INTR_BUS_RESET_BITS) {
    usb_hw->sie_status = USB_SIE_STATUS_BUS_RESET_BITS;
    usb_hw->dev_addr_ctrl = 0;
    usb_hw->buf_status = 0xffffffff;
    fn(MG_USB_EV_RESET, MG_USB_EP_CTL, NULL, 0);
    mgi_usb_rp_ep_init(0, false), mgi_usb_rp_ep_init(0, true);
    mgi_usb_rp_endpoints(false);
    return;
  }
  if (status & USB_INTR_SETUP_REQ_BITS) {
    // Discard stale EP0 completions from an interrupted control transfer
    usb_hw->buf_status = 3;
    usb_hw->sie_status = USB_SIE_STATUS_SETUP_REC_BITS;
    mgi_usb_rp_setup(fn);
  }
  done = usb_hw->buf_status;
  for (i = 0; i < 10; i++) {  // Bits: EP0 IN, EP0 OUT, EP1 IN, ...
    if (!(done & (1U << i))) continue;
    usb_hw->buf_status = 1U << i;
    if (s_mgi_usb_rp_ep[i / 2][!(i & 1)].ctrl) {
      mgi_usb_rp_complete(i / 2, !(i & 1), fn);
    }
  }
}

// mg_usb_hw::init(): the serial number, then the controller, attached
static void mgi_usb_rp_start(char serial[17]) {
  pico_get_unique_board_id_string(serial, 17);
  reset_block(RESETS_RESET_USBCTRL_BITS);
  unreset_block_wait(RESETS_RESET_USBCTRL_BITS);
  memset(usb_dpram, 0, sizeof(*usb_dpram));
  usb_hw->muxing = USB_USB_MUXING_TO_PHY_BITS | USB_USB_MUXING_SOFTCON_BITS;
  usb_hw->pwr =
      USB_USB_PWR_VBUS_DETECT_BITS | USB_USB_PWR_VBUS_DETECT_OVERRIDE_EN_BITS;
  usb_hw->main_ctrl = USB_MAIN_CTRL_CONTROLLER_EN_BITS;
  usb_hw->sie_ctrl = USB_SIE_CTRL_EP0_INT_1BUF_BITS;
  usb_hw->inte = 0;  // Poll raw status instead of an interrupt handler
  mgi_usb_rp_ep_init(0, false), mgi_usb_rp_ep_init(0, true);
  mgi_usb_rp_endpoints(false);
  hw_set_bits(&usb_hw->sie_ctrl, USB_SIE_CTRL_PULLUP_EN_BITS);
}

const struct mg_usb_hw mg_usb_hw_rp = {mgi_usb_rp_start, mgi_usb_rp_poll,
                                       mgi_usb_rp_in};

#if MG_ENABLE_USB_ETHERNET
static bool mgi_usb_rp_init(struct mg_tcpip_if *ifp) {
  return mg_usb_init(ifp, &mg_usb_hw_rp);
}

struct mg_tcpip_driver mg_tcpip_driver_usb_rp = {mgi_usb_rp_init, mg_usb_tx,
                                                 NULL, mg_usb_poll};
#endif

#endif
