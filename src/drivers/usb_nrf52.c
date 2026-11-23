#include "net_builtin.h"
#include "drivers/usb.h"
#include "drivers/usb_nrf52.h"
#include "printf.h"
#include "util.h"

#if (MG_ENABLE_TCPIP || !MG_ENABLE_USB_ETHERNET) && \
    MG_ENABLE_DRIVER_USB_NRF52 &&                   \
    (MG_ENABLE_USB_ETHERNET || MG_ENABLE_USB_SERIAL)
#include <nrf.h>

// nRF52840 USB device controller, polled, full speed. Endpoint buffers are
// inside the controller: EasyDMA moves each packet between them and RAM,
// and reads RAM only, so every packet goes through s_mgi_usb_nrf52_pkt. The
// controller handles SET_ADDRESS by itself. Errata workarounds follow
// Nordic's nrfx_usbd.c: 171, 187 when enabling, 199 around each DMA

#define MGI_USB_NRF52_ERRATA_199 (*(volatile uint32_t *) 0x40027c1c)

// usb.c's event handler, see mg_usb_hw::poll()
#define MGI_USB_NRF52_FN \
  int (*fn)(enum mg_usb_ev, enum mg_usb_ep, void *, size_t)

static uint32_t s_mgi_usb_nrf52_pkt[64 / 4];  // One packet in RAM
static uint8_t s_mgi_usb_nrf52_req[16];       // SETUP packet, then its OUT data

// Write an undocumented register the way Nordic's errata workarounds do
static void mgi_usb_nrf52_errata(uint32_t addr, uint32_t value) {
  volatile uint32_t *unlock = (volatile uint32_t *) 0x4006ec00;
  bool locked = *unlock == 0;
  if (locked) *unlock = 0x9375;
  *(volatile uint32_t *) addr = value;
  if (locked) *unlock = 0x9375;
}

// Run one EasyDMA transfer and wait for its end event
static void mgi_usb_nrf52_dma(volatile uint32_t *task, volatile uint32_t *end) {
  unsigned i;
  *end = 0;
  MGI_USB_NRF52_ERRATA_199 = 0x82;
  *task = 1;
  for (i = 0; *end == 0 && i < 100000; i++) (void) 0;  // Microseconds
  *end = 0;
  MGI_USB_NRF52_ERRATA_199 = 0;
}

// Data toggle of endpoint address ep back to DATA0
static void mgi_usb_nrf52_dtoggle(unsigned ep) {
  NRF_USBD->DTOGGLE = ep;  // Select the endpoint, then set its toggle
  NRF_USBD->DTOGGLE = (USBD_DTOGGLE_VALUE_Data0 << USBD_DTOGGLE_VALUE_Pos) | ep;
}

// mg_usb_hw::in(): copy the packet to RAM, then into the endpoint
static void mgi_usb_nrf52_in(enum mg_usb_ep ep, const void *buf, size_t len) {
  if (len > 0) memcpy(s_mgi_usb_nrf52_pkt, buf, len);
  NRF_USBD->EPIN[ep].PTR = (uint32_t) (uintptr_t) s_mgi_usb_nrf52_pkt;
  NRF_USBD->EPIN[ep].MAXCNT = (uint32_t) len;
  mgi_usb_nrf52_dma(&NRF_USBD->TASKS_STARTEPIN[ep],
                    &NRF_USBD->EVENTS_ENDEPIN[ep]);
}

// Move the packet on OUT endpoint ep to RAM, which also lets the endpoint
// accept the next one. Returns its length
static size_t mgi_usb_nrf52_out(unsigned ep) {
  size_t n = NRF_USBD->SIZE.EPOUT[ep];
  if (n == 0) {
    NRF_USBD->SIZE.EPOUT[ep] = 0;  // Nothing to move: accept the next
  } else {
    NRF_USBD->EPOUT[ep].PTR = (uint32_t) (uintptr_t) s_mgi_usb_nrf52_pkt;
    NRF_USBD->EPOUT[ep].MAXCNT = (uint32_t) n;
    mgi_usb_nrf52_dma(&NRF_USBD->TASKS_STARTEPOUT[ep],
                      &NRF_USBD->EVENTS_ENDEPOUT[ep]);
  }
  return n;
}

#if MG_ENABLE_USB_ETHERNET
// Ethernet data endpoints back to DATA0, OUT ready to receive
static void mgi_usb_nrf52_data_reset(void) {
  mgi_usb_nrf52_dtoggle(0x02), mgi_usb_nrf52_dtoggle(0x82);
  NRF_USBD->SIZE.EPOUT[2] = 0;
}
#endif

// Endpoints 1-4 on and idle if configured, else off
static void mgi_usb_nrf52_endpoints(bool on) {
  NRF_USBD->EPINEN = 1, NRF_USBD->EPOUTEN = 1;
  if (!on) return;
#if MG_ENABLE_USB_ETHERNET
  NRF_USBD->EPINEN |= MG_BIT(1) | MG_BIT(2), NRF_USBD->EPOUTEN |= MG_BIT(2);
  mgi_usb_nrf52_dtoggle(0x81);
  mgi_usb_nrf52_data_reset();
#endif
#if MG_ENABLE_USB_SERIAL
  NRF_USBD->EPINEN |= MG_BIT(3) | MG_BIT(4), NRF_USBD->EPOUTEN |= MG_BIT(4);
  mgi_usb_nrf52_dtoggle(0x83), mgi_usb_nrf52_dtoggle(0x04);
  mgi_usb_nrf52_dtoggle(0x84);
  NRF_USBD->SIZE.EPOUT[4] = 0;
#endif
}

// A complete control request: usb.c answers it, then the controller follows
static void mgi_usb_nrf52_request(MGI_USB_NRF52_FN) {
  uint8_t *r = s_mgi_usb_nrf52_req;
  size_t out = (r[0] & 0x80) ? 0 : MG_LOAD_LE16(r + 6);  // OUT data stage
  int res = fn(MG_USB_EV_SETUP, MG_USB_EP_CTL, r, 8 + out);
  if (res < 0) {
    NRF_USBD->TASKS_EP0STALL = 1;
    return;
  }
  switch ((r[0] << 8) | r[1]) {
    case 0x0009:  // SET_CONFIGURATION
      mgi_usb_nrf52_endpoints(r[2] == 1);
      break;
#if MG_ENABLE_USB_ETHERNET
    case 0x010b:  // SET_INTERFACE
      if (r[4] == MG_USB_IF_ETH_DATA) mgi_usb_nrf52_data_reset();
      break;
#endif
    case 0x0201:  // CLEAR_FEATURE(ENDPOINT_HALT)
      mgi_usb_nrf52_dtoggle(r[4]);
      NRF_USBD->EPSTALL =
          (USBD_EPSTALL_STALL_UnStall << USBD_EPSTALL_STALL_Pos) | r[4];
      if (!(r[4] & 0x80)) NRF_USBD->SIZE.EPOUT[r[4] & 15] = 0;
      break;
  }
  if (res == 0) NRF_USBD->TASKS_EP0STATUS = 1;  // No IN data stage
}

// A SETUP packet: run it, or first receive its OUT data stage
static void mgi_usb_nrf52_setup(MGI_USB_NRF52_FN) {
  const volatile uint32_t *regs = &NRF_USBD->BMREQUESTTYPE;
  uint8_t *r = s_mgi_usb_nrf52_req;
  unsigned i, wlen;
  for (i = 0; i < 8; i++) r[i] = (uint8_t) regs[i];
  wlen = MG_LOAD_LE16(r + 6);
  if (r[0] == 0 && r[1] == 5) return;  // SET_ADDRESS: the controller's
  if ((r[0] & 0x80) || wlen == 0) {
    mgi_usb_nrf52_request(fn);
  } else if (wlen > sizeof(s_mgi_usb_nrf52_req) - 8) {
    NRF_USBD->TASKS_EP0STALL = 1;
  } else {
    NRF_USBD->TASKS_EP0RCVOUT = 1;  // Accept the OUT data stage
  }
}

// A control data stage packet: an IN one acknowledged, or an OUT one arrived
static void mgi_usb_nrf52_ep0_data(MGI_USB_NRF52_FN) {
  uint8_t *r = s_mgi_usb_nrf52_req;
  size_t n = NRF_USBD->SIZE.EPOUT[0];
  if (r[0] & 0x80) {
    if (!fn(MG_USB_EV_IN, MG_USB_EP_CTL, NULL, 0)) {
      NRF_USBD->TASKS_EP0STATUS = 1;
    }
  } else if (n != MG_LOAD_LE16(r + 6)) {
    NRF_USBD->TASKS_EP0STALL = 1;
  } else {
    mgi_usb_nrf52_out(0);
    memcpy(r + 8, s_mgi_usb_nrf52_pkt, n);
    mgi_usb_nrf52_request(fn);
  }
}

// Follow USB power: enable the controller when VBUS appears, attach to the
// bus once both are ready, and detach when VBUS goes. True while attached
static bool mgi_usb_nrf52_attach(MGI_USB_NRF52_FN) {
  uint32_t power = NRF_POWER->USBREGSTATUS;
  if (!(power & POWER_USBREGSTATUS_VBUSDETECT_Msk)) {
    if (NRF_USBD->ENABLE) {
      NRF_USBD->USBPULLUP = 0, NRF_USBD->ENABLE = 0;
      fn(MG_USB_EV_RESET, MG_USB_EP_CTL, NULL, 0);
    }
    return false;
  }
  if (!NRF_USBD->ENABLE) {
    mgi_usb_nrf52_errata(0x4006ed14, 3);     // Errata 187
    mgi_usb_nrf52_errata(0x4006ec14, 0xc0);  // Errata 171
    NRF_USBD->EVENTCAUSE = USBD_EVENTCAUSE_READY_Msk;
    NRF_USBD->ENABLE = 1;
  }
  if (NRF_USBD->USBPULLUP) return true;
  if (!(NRF_USBD->EVENTCAUSE & USBD_EVENTCAUSE_READY_Msk) ||
      !(power & POWER_USBREGSTATUS_OUTPUTRDY_Msk))
    return false;
  NRF_USBD->EVENTCAUSE = USBD_EVENTCAUSE_READY_Msk;
  mgi_usb_nrf52_errata(0x4006ec14, 0);
  mgi_usb_nrf52_errata(0x4006ed14, 0);
  NRF_USBD->USBPULLUP = 1;
  return true;
}

// mg_usb_hw::poll()
static void mgi_usb_nrf52_poll(MGI_USB_NRF52_FN) {
  if (!mgi_usb_nrf52_attach(fn)) return;
  if (NRF_USBD->EVENTS_USBRESET) {
    NRF_USBD->EVENTS_USBRESET = 0;
    NRF_USBD->EVENTS_EP0DATADONE = NRF_USBD->EVENTS_EPDATA = 0;
    NRF_USBD->EPDATASTATUS = NRF_USBD->EPDATASTATUS;
    fn(MG_USB_EV_RESET, MG_USB_EP_CTL, NULL, 0);
    mgi_usb_nrf52_endpoints(false);
    return;
  }
  if (NRF_USBD->EVENTS_EP0DATADONE) {  // Before a new SETUP replaces req
    NRF_USBD->EVENTS_EP0DATADONE = 0;
    mgi_usb_nrf52_ep0_data(fn);
  }
  if (NRF_USBD->EVENTS_EP0SETUP) {
    NRF_USBD->EVENTS_EP0SETUP = 0;
    mgi_usb_nrf52_setup(fn);
  }
  if (NRF_USBD->EVENTS_EPDATA) {  // Bit n: IN n acknowledged, 16+n: OUT n
    uint32_t done = NRF_USBD->EPDATASTATUS;
    unsigned i;
    NRF_USBD->EVENTS_EPDATA = 0;
    NRF_USBD->EPDATASTATUS = done;
    for (i = 1; i < 5; i++) {
      if (done & MG_BIT(i)) fn(MG_USB_EV_IN, (enum mg_usb_ep) i, NULL, 0);
    }
#if MG_ENABLE_USB_ETHERNET
    if (done & MG_BIT(16 + MG_USB_EP_ETH_DATA)) {
      size_t n = mgi_usb_nrf52_out(MG_USB_EP_ETH_DATA);
      fn(MG_USB_EV_OUT, MG_USB_EP_ETH_DATA, s_mgi_usb_nrf52_pkt, n);
    }
#endif
#if MG_ENABLE_USB_SERIAL
    if (done & MG_BIT(16 + MG_USB_EP_SER_DATA)) {
      NRF_USBD->SIZE.EPOUT[MG_USB_EP_SER_DATA] = 0;  // Serial input: dropped
    }
#endif
  }
}

// mg_usb_hw::init(): the serial number, and the crystal that USB needs. The
// controller itself starts in poll, once USB power is there
static void mgi_usb_nrf52_start(char serial[17]) {
  uint32_t id[2], xtal = CLOCK_HFCLKSTAT_SRC_Msk | CLOCK_HFCLKSTAT_STATE_Msk;
  id[0] = NRF_FICR->DEVICEID[0], id[1] = NRF_FICR->DEVICEID[1];
  mg_snprintf(serial, 17, "%M", mg_print_hex, (int) sizeof(id), (uint8_t *) id);
  if ((NRF_CLOCK->HFCLKSTAT & xtal) != xtal) {
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while ((NRF_CLOCK->HFCLKSTAT & xtal) != xtal) (void) 0;
  }
}

const struct mg_usb_hw mg_usb_hw_nrf52 = {
    mgi_usb_nrf52_start, mgi_usb_nrf52_poll, mgi_usb_nrf52_in};

#if MG_ENABLE_USB_ETHERNET
static bool mgi_usb_nrf52_init(struct mg_tcpip_if *ifp) {
  return mg_usb_init(ifp, &mg_usb_hw_nrf52);
}

struct mg_tcpip_driver mg_tcpip_driver_usb_nrf52 = {
    mgi_usb_nrf52_init, mg_usb_tx, NULL, mg_usb_poll};
#endif

#endif
