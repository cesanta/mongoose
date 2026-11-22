#include "drivers/usb.h"
#include "drivers/usb_nrf52.h"
#include "net_builtin.h"

#if MG_ENABLE_USB && MG_ENABLE_DRIVER_USB_NRF52
#include <nrf.h>

// nRF52840 USB device controller, polled, full speed. Endpoint buffers are
// inside the controller: EasyDMA moves each packet between them and RAM,
// one transfer at a time, so longer IN transfers go out 64 bytes at a time.
// EasyDMA reads RAM only, thus constant data is copied to RAM first. The
// controller handles SET_ADDRESS by itself. Errata workarounds follow
// Nordic's nrfx_usbd.c: 171, 187 when enabling, 199 around each DMA

#define ERRATA_199 (*(volatile uint32_t *) 0x40027c1c)

struct nrf_in {         // IN transfer on an endpoint
  const uint8_t *data;  // Bytes still to send, in RAM
  size_t left;          // How many
  unsigned packets;     // Packets still to send
  bool busy;            // A packet is waiting for the host
};

static struct nrf_in s_in[5];  // Index: endpoint number
static struct mg_usb s_usb;
static uint8_t s_req[16];        // SETUP packet, then its OUT data stage
static uint32_t s_ep0[256 / 4];  // Control IN reply, OUT data stage
static char s_serial[17];        // Serial number: device ID in hex
#if MG_ENABLE_USB_ETHERNET
static uint32_t s_note[16 / 4];  // Link notification
static uint32_t s_rx[MG_USB_NTB_SIZE / 4], s_tx[MG_USB_NTB_SIZE / 4];
static size_t s_rxlen;  // Received part of the OUT transfer block
static bool s_discard;  // OUT transfer block too long: drop it
#endif
#if MG_ENABLE_USB_SERIAL
static uint32_t s_log[64 / 4];  // Log output in flight
#endif

// Write an undocumented register the way Nordic's errata workarounds do
static void errata(uint32_t addr, uint32_t value) {
  volatile uint32_t *unlock = (volatile uint32_t *) 0x4006ec00;
  bool locked = *unlock == 0;
  if (locked) *unlock = 0x9375;
  *(volatile uint32_t *) addr = value;
  if (locked) *unlock = 0x9375;
}

// Run one EasyDMA transfer and wait for its end event
static void dma(volatile uint32_t *task, volatile uint32_t *end) {
  unsigned i;
  *end = 0;
  ERRATA_199 = 0x82;
  *task = 1;
  for (i = 0; *end == 0 && i < 100000; i++) (void) 0;  // Microseconds
  *end = 0;
  ERRATA_199 = 0;
}

// Send the next packet of the IN transfer on ep. False if it is over
static bool in_next(unsigned ep) {
  struct nrf_in *e = &s_in[ep];
  size_t n = e->left > 64 ? 64 : e->left;
  if (e->packets == 0) return false;
  NRF_USBD->EPIN[ep].PTR = (uint32_t) (uintptr_t) e->data;
  NRF_USBD->EPIN[ep].MAXCNT = (uint32_t) n;
  dma(&NRF_USBD->TASKS_STARTEPIN[ep], &NRF_USBD->EVENTS_ENDEPIN[ep]);
  e->data += n, e->left -= n, e->packets--;
  return true;
}

// Start an IN transfer from RAM. zlp: end a multiple of 64 bytes with an
// empty packet
static void in_start(unsigned ep, const void *data, size_t len, bool zlp) {
  struct nrf_in *e = &s_in[ep];
  e->data = (const uint8_t *) data, e->left = len;
  e->packets = (unsigned) (len / 64 + (len % 64 || len == 0 || zlp));
  e->busy = in_next(ep);
}

// Data toggle of endpoint address ep back to DATA0
static void dtoggle(unsigned ep) {
  NRF_USBD->DTOGGLE = ep;  // Select the endpoint, then set its toggle
  NRF_USBD->DTOGGLE = (USBD_DTOGGLE_VALUE_Data0 << USBD_DTOGGLE_VALUE_Pos) | ep;
}

#if MG_ENABLE_USB_ETHERNET
// Ethernet data endpoints idle, ready to receive
static void data_reset(void) {
  dtoggle(0x02), dtoggle(0x82);
  memset(&s_in[2], 0, sizeof(s_in[2]));
  s_rxlen = 0, s_discard = false;
  NRF_USBD->SIZE.EPOUT[2] = 0;  // Drop any received packet, accept the next
}

// A packet on the Ethernet OUT endpoint: add it to the transfer block. The
// endpoint accepts the next packet once this one is moved or dropped
static void rx(void) {
  size_t n = NRF_USBD->SIZE.EPOUT[2];
  if (n > 0 && !s_discard && s_rxlen + n <= sizeof(s_rx)) {
    uint8_t *p = (uint8_t *) s_rx + s_rxlen;
    NRF_USBD->EPOUT[2].PTR = (uint32_t) (uintptr_t) p;
    NRF_USBD->EPOUT[2].MAXCNT = (uint32_t) n;
    dma(&NRF_USBD->TASKS_STARTEPOUT[2], &NRF_USBD->EVENTS_ENDEPOUT[2]);
    s_rxlen += n;
  } else {
    s_discard = s_discard || n > 0;  // An empty packet only ends the block
    NRF_USBD->SIZE.EPOUT[2] = 0;
  }
  // At the advertised maximum OUT size, the host need not send a ZLP.
  // Other transfers end with a short packet (including a ZLP)
  if (n < 64 || s_rxlen == sizeof(s_rx)) {
    if (!s_discard) mg_usb_decode(&s_usb, (uint8_t *) s_rx, s_rxlen);
    s_rxlen = 0, s_discard = false;
  }
}
#endif

// Endpoints 1-4 on and idle if configured, else off
static void endpoints_reset(void) {
  memset(s_in, 0, sizeof(s_in));
  NRF_USBD->EPINEN = 1, NRF_USBD->EPOUTEN = 1;
  if (!s_usb.configured) return;
#if MG_ENABLE_USB_ETHERNET
  NRF_USBD->EPINEN |= MG_BIT(1) | MG_BIT(2), NRF_USBD->EPOUTEN |= MG_BIT(2);
  dtoggle(0x81);
  data_reset();
#endif
#if MG_ENABLE_USB_SERIAL
  NRF_USBD->EPINEN |= MG_BIT(3) | MG_BIT(4), NRF_USBD->EPOUTEN |= MG_BIT(4);
  dtoggle(0x83), dtoggle(0x04), dtoggle(0x84);
  NRF_USBD->SIZE.EPOUT[4] = 0;
#endif
}

// Run a complete control request: SETUP in s_req, then any OUT data
static void request(void) {
  const void *reply;
  unsigned wlen = MG_LOAD_LE16(s_req + 6);
  int n = mg_usb_setup(&s_usb, s_req, &reply);
  if (n < 0) {
    NRF_USBD->TASKS_EP0STALL = 1;
    return;
  }
  switch ((s_req[0] << 8) | s_req[1]) {
    case 0x0009:  // SET_CONFIGURATION
      endpoints_reset();
      break;
#if MG_ENABLE_USB_ETHERNET
    case 0x010b:  // SET_INTERFACE
      if (s_req[4] == 1) data_reset();
      break;
#endif
    case 0x0201:  // CLEAR_FEATURE(ENDPOINT_HALT)
      dtoggle(s_req[4]);
      NRF_USBD->EPSTALL =
          (USBD_EPSTALL_STALL_UnStall << USBD_EPSTALL_STALL_Pos) | s_req[4];
      if (!(s_req[4] & 0x80)) NRF_USBD->SIZE.EPOUT[s_req[4] & 15] = 0;
      break;
  }
  if ((s_req[0] & 0x80) && wlen) {  // IN data stage. Largest reply: 160
    memcpy(s_ep0, reply, (size_t) n);
    in_start(0, s_ep0, (size_t) n, n < (int) wlen);
  } else {
    NRF_USBD->TASKS_EP0STATUS = 1;
  }
}

// A SETUP packet: run it, or first receive its OUT data stage
static void setup(void) {
  const volatile uint32_t *regs = &NRF_USBD->BMREQUESTTYPE;
  unsigned i, wlen;
  for (i = 0; i < 8; i++) s_req[i] = (uint8_t) regs[i];
  wlen = MG_LOAD_LE16(s_req + 6);
  memset(&s_in[0], 0, sizeof(s_in[0]));  // Cancels the previous transfer
  if (s_req[0] == 0 && s_req[1] == 5) return;  // SET_ADDRESS: hardware's
  if ((s_req[0] & 0x80) || wlen == 0) {
    request();
  } else if (wlen > sizeof(s_req) - 8) {
    NRF_USBD->TASKS_EP0STALL = 1;
  } else {
    NRF_USBD->TASKS_EP0RCVOUT = 1;  // Accept the OUT data stage
  }
}

// A control data stage packet: an IN one acknowledged, or an OUT one arrived
static void ep0_data(void) {
  size_t n = NRF_USBD->SIZE.EPOUT[0];
  if (s_req[0] & 0x80) {
    if (!in_next(0)) NRF_USBD->TASKS_EP0STATUS = 1;
  } else if (n != MG_LOAD_LE16(s_req + 6)) {
    NRF_USBD->TASKS_EP0STALL = 1;
  } else {
    NRF_USBD->EPOUT[0].PTR = (uint32_t) (uintptr_t) s_ep0;
    NRF_USBD->EPOUT[0].MAXCNT = (uint32_t) n;
    dma(&NRF_USBD->TASKS_STARTEPOUT[0], &NRF_USBD->EVENTS_ENDEPOUT[0]);
    memcpy(s_req + 8, s_ep0, n);
    request();
  }
}

// Follow USB power: enable the controller when VBUS appears, attach to the
// bus once both are ready, and detach when VBUS goes. True while attached
static bool attach(void) {
  uint32_t power = NRF_POWER->USBREGSTATUS;
  if (!(power & POWER_USBREGSTATUS_VBUSDETECT_Msk)) {
    if (NRF_USBD->ENABLE) {
      NRF_USBD->USBPULLUP = 0, NRF_USBD->ENABLE = 0;
      mg_usb_reset(&s_usb);
    }
    return false;
  }
  if (!NRF_USBD->ENABLE) {
    errata(0x4006ed14, 3);     // Errata 187
    errata(0x4006ec14, 0xc0);  // Errata 171
    NRF_USBD->EVENTCAUSE = USBD_EVENTCAUSE_READY_Msk;
    NRF_USBD->ENABLE = 1;
  }
  if (NRF_USBD->USBPULLUP) return true;
  if (!(NRF_USBD->EVENTCAUSE & USBD_EVENTCAUSE_READY_Msk) ||
      !(power & POWER_USBREGSTATUS_OUTPUTRDY_Msk))
    return false;
  NRF_USBD->EVENTCAUSE = USBD_EVENTCAUSE_READY_Msk;
  errata(0x4006ec14, 0);
  errata(0x4006ed14, 0);
  NRF_USBD->USBPULLUP = 1;
  return true;
}

void mg_usb_init(void) {
  static bool started;
  uint32_t id[2], xtal = CLOCK_HFCLKSTAT_SRC_Msk | CLOCK_HFCLKSTAT_STATE_Msk;
  if (started) return;
  started = true;
  id[0] = NRF_FICR->DEVICEID[0], id[1] = NRF_FICR->DEVICEID[1];
  mg_snprintf(s_serial, sizeof(s_serial), "%M", mg_print_hex, (int) sizeof(id),
              (uint8_t *) id);
  s_usb.serial = s_serial;
  mg_usb_reset(&s_usb);
#if MG_ENABLE_USB_SERIAL
  mg_log_set_fn(mg_usb_log, &s_usb);  // Log to the USB serial port
#endif
  if ((NRF_CLOCK->HFCLKSTAT & xtal) != xtal) {  // USB needs the crystal
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while ((NRF_CLOCK->HFCLKSTAT & xtal) != xtal) (void) 0;
  }
  // The controller itself starts in mg_usb_poll(), once USB power is there
}

void mg_usb_poll(void) {
  mg_usb_init();
#if MG_ENABLE_USB_SERIAL
  if (s_usb.reboot && mg_millis() > s_usb.reboot) {
    NRF_POWER->GPREGRET = 0x57;  // Adafruit bootloader: stay in DFU mode
    NVIC_SystemReset();
  }
#endif
  if (!attach()) return;
  if (NRF_USBD->EVENTS_USBRESET) {
    NRF_USBD->EVENTS_USBRESET = 0;
    NRF_USBD->EVENTS_EP0DATADONE = NRF_USBD->EVENTS_EPDATA = 0;
    NRF_USBD->EPDATASTATUS = NRF_USBD->EPDATASTATUS;
    mg_usb_reset(&s_usb);
    endpoints_reset();
    return;
  }
  if (NRF_USBD->EVENTS_EP0DATADONE) {  // Before a new SETUP replaces s_req
    NRF_USBD->EVENTS_EP0DATADONE = 0;
    ep0_data();
  }
  if (NRF_USBD->EVENTS_EP0SETUP) {
    NRF_USBD->EVENTS_EP0SETUP = 0;
    setup();
  }
  if (NRF_USBD->EVENTS_EPDATA) {  // Bit n: IN n acknowledged, 16+n: OUT n
    uint32_t done = NRF_USBD->EPDATASTATUS;
    unsigned i;
    NRF_USBD->EVENTS_EPDATA = 0;
    NRF_USBD->EPDATASTATUS = done;
    for (i = 1; i < 5; i++) {
      if (done & MG_BIT(i)) s_in[i].busy = in_next(i);
    }
#if MG_ENABLE_USB_ETHERNET
    if (done & MG_BIT(18)) rx();
#endif
#if MG_ENABLE_USB_SERIAL
    if (done & MG_BIT(20)) NRF_USBD->SIZE.EPOUT[4] = 0;  // Serial: ignored
#endif
  }
#if MG_ENABLE_USB_ETHERNET
  {
    const void *msg;
    size_t n;
    if (!s_in[1].busy && (n = mg_usb_notify(&s_usb, &msg)) > 0) {
      memcpy(s_note, msg, n);
      in_start(1, s_note, n, false);
    }
  }
#endif
#if MG_ENABLE_USB_SERIAL
  if (s_usb.configured && s_usb.dtr && !s_in[4].busy) {
    size_t n = mg_usb_log_read(&s_usb, s_log, sizeof(s_log));
    if (n > 0) in_start(4, s_log, n, true);
  }
#endif
}

#if MG_ENABLE_USB_ETHERNET
static bool mg_tcpip_driver_usb_nrf52_init(struct mg_tcpip_if *ifp) {
  s_usb.ifp = ifp;  // mg_tcpip_init() has started USB already
  return true;
}

static size_t mg_tcpip_driver_usb_nrf52_tx(const void *buf, size_t len,
                                           struct mg_tcpip_if *ifp) {
  // One frame can be in flight. Keep USB moving while waiting, but bound
  // the wait if the host stops reading or the cable is gone
  uint64_t until = mg_millis() + 20;
  size_t n = 0;
  while (s_in[2].busy && mg_millis() < until) mg_usb_poll();
  if (!s_in[2].busy) n = mg_usb_encode(&s_usb, (uint8_t *) s_tx, buf, len);
  if (n) in_start(2, s_tx, n, n < s_usb.input_size);
  (void) ifp;
  return n ? len : 0;
}

// USB itself runs from mg_mgr_poll(). Report the link: Ethernet open
static bool mg_tcpip_driver_usb_nrf52_poll(struct mg_tcpip_if *ifp, bool s1) {
  (void) ifp;
  return s1 && s_usb.alt && s_usb.filter;
}

struct mg_tcpip_driver mg_tcpip_driver_usb_nrf52 = {
    mg_tcpip_driver_usb_nrf52_init, mg_tcpip_driver_usb_nrf52_tx, NULL,
    mg_tcpip_driver_usb_nrf52_poll};
#endif

#endif
