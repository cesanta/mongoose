#include "net_builtin.h"
#include "drivers/usb.h"
#include "drivers/usb_dwc2.h"
#include "printf.h"
#include "util.h"

#if (MG_ENABLE_TCPIP || !MG_ENABLE_USB_ETHERNET) && \
    MG_ENABLE_DRIVER_USB_DWC2 &&                    \
    (MG_ENABLE_USB_ETHERNET || MG_ENABLE_USB_SERIAL)

// DWC2 device controller, polled: no interrupt handler, no DMA. Received
// packets come through one shared RX FIFO, each IN endpoint has a TX FIFO of
// its own. Register layout: STM32 reference manuals, OTG chapter.

#ifndef MG_USB_DWC2_BASE
#error "MG_ENABLE_DRIVER_USB_DWC2 needs MG_USB_DWC2_BASE, the core's address"
#endif
#ifndef MG_USB_DWC2_ULPI
#define MG_USB_DWC2_ULPI 0
#endif
#ifndef MG_USB_DWC2_UTMI
#define MG_USB_DWC2_UTMI 0
#endif
#if MG_USB_DWC2_ULPI && MG_USB_DWC2_UTMI
#error "MG_USB_DWC2_ULPI and MG_USB_DWC2_UTMI: choose one PHY"
#endif
#define MGI_DWC2_HS_PHY (MG_USB_DWC2_ULPI || MG_USB_DWC2_UTMI)  // At full speed

// Register at offset off
#define MGI_DWC2(off) \
  (*(volatile uint32_t *) ((uintptr_t) (MG_USB_DWC2_BASE) + (off)))
#define MGI_DWC2_GOTGCTL MGI_DWC2(0x000)
#define MGI_DWC2_GUSBCFG MGI_DWC2(0x00c)
#define MGI_DWC2_GRSTCTL MGI_DWC2(0x010)
#define MGI_DWC2_GINTSTS MGI_DWC2(0x014)
#define MGI_DWC2_GRXSTSP MGI_DWC2(0x020)
#define MGI_DWC2_GRXFSIZ MGI_DWC2(0x024)
#define MGI_DWC2_DIEPTXF0 MGI_DWC2(0x028)
#define MGI_DWC2_GCCFG MGI_DWC2(0x038)
#define MGI_DWC2_GSNPSID MGI_DWC2(0x040)
#define MGI_DWC2_DIEPTXF(n) MGI_DWC2(0x100 + 4 * (n))  // n: 1..
#define MGI_DWC2_DCFG MGI_DWC2(0x800)
#define MGI_DWC2_DCTL MGI_DWC2(0x804)
#define MGI_DWC2_DIEPMSK MGI_DWC2(0x810)
#define MGI_DWC2_DOEPMSK MGI_DWC2(0x814)
#define MGI_DWC2_DAINT MGI_DWC2(0x818)
#define MGI_DWC2_DAINTMSK MGI_DWC2(0x81c)
#define MGI_DWC2_DIEPCTL(n) MGI_DWC2(0x900 + 0x20 * (n))
#define MGI_DWC2_DIEPINT(n) MGI_DWC2(0x908 + 0x20 * (n))
#define MGI_DWC2_DIEPTSIZ(n) MGI_DWC2(0x910 + 0x20 * (n))
#define MGI_DWC2_DOEPCTL(n) MGI_DWC2(0xb00 + 0x20 * (n))
#define MGI_DWC2_DOEPINT(n) MGI_DWC2(0xb08 + 0x20 * (n))
#define MGI_DWC2_DOEPTSIZ(n) MGI_DWC2(0xb10 + 0x20 * (n))
#define MGI_DWC2_PCGCCTL MGI_DWC2(0xe00)
#define MGI_DWC2_FIFO(n) MGI_DWC2(0x1000 * ((n) + 1))

// Endpoint control bits, DIEPCTL and DOEPCTL
#define MGI_DWC2_USBAEP MG_BIT(15)  // Active
#define MGI_DWC2_STALL MG_BIT(21)
#define MGI_DWC2_CNAK MG_BIT(26)
#define MGI_DWC2_SNAK MG_BIT(27)
#define MGI_DWC2_SD0PID MG_BIT(28)  // Next packet is DATA0
#define MGI_DWC2_EPDIS MG_BIT(30)
#define MGI_DWC2_EPENA MG_BIT(31)

// FIFOs, in 32-bit words: RX first, then a packet's room per IN endpoint.
// 208 words in all, so it fits a full-speed core's 320
#define MGI_DWC2_RX_WORDS 128

// usb.c's event handler, see mg_usb_hw::poll()
#define MGI_DWC2_FN int (*fn)(enum mg_usb_ev, enum mg_usb_ep, void *, size_t)

static uint32_t s_mgi_dwc2_pkt[64 / 4];  // A received packet
static uint8_t s_mgi_dwc2_req[16];       // SETUP packet, then its OUT data
static size_t s_mgi_dwc2_req_len;        // OUT data received so far
static bool s_mgi_dwc2_req_out;          // Waiting for that OUT data stage

// Wait until the mask bits of reg are set, or clear. Bounded
static void mgi_dwc2_wait(volatile uint32_t *reg, uint32_t mask, bool set) {
  unsigned i;
  for (i = 0; ((*reg & mask) != 0) != set && i < 1000000; i++) (void) 0;
}

// Flush the TX FIFO of endpoint n, 0x10: all of them
static void mgi_dwc2_flush_tx(unsigned n) {
  MGI_DWC2_GRSTCTL = MG_BIT(5) | (n << 6);  // TXFFLSH, TXFNUM
  mgi_dwc2_wait(&MGI_DWC2_GRSTCTL, MG_BIT(5), false);
}

// mg_usb_hw::in(): load one packet, then let the endpoint send it
static void mgi_dwc2_in(enum mg_usb_ep ep, const void *buf, size_t len) {
  const uint8_t *p = (const uint8_t *) buf;
  size_t i, k;
  MGI_DWC2_DIEPTSIZ(ep) = MG_BIT(19) | (uint32_t) len;  // One packet
  MGI_DWC2_DIEPCTL(ep) |= MGI_DWC2_CNAK | MGI_DWC2_EPENA;
  for (i = 0; i < len; i += 4) {
    uint32_t w = 0;
    for (k = 0; k < 4 && i + k < len; k++) w |= (uint32_t) p[i + k] << (8 * k);
    MGI_DWC2_FIFO(ep) = w;
  }
}

// Stop an IN transfer on endpoint ep, and empty its TX FIFO
static void mgi_dwc2_in_stop(unsigned ep) {
  if (MGI_DWC2_DIEPCTL(ep) & MGI_DWC2_EPENA) {
    MGI_DWC2_DIEPCTL(ep) |= MGI_DWC2_SNAK;
    mgi_dwc2_wait(&MGI_DWC2_DIEPINT(ep), MG_BIT(6), true);  // INEPNE
    MGI_DWC2_DIEPCTL(ep) |= MGI_DWC2_EPDIS;
    mgi_dwc2_wait(&MGI_DWC2_DIEPINT(ep), MG_BIT(1), true);  // EPDISD
    MGI_DWC2_DIEPINT(ep) = MG_BIT(1);
  }
  mgi_dwc2_flush_tx(ep);
}

// Let OUT endpoint ep take one packet. Endpoint 0 also takes SETUPs
static void mgi_dwc2_arm(unsigned ep) {
  MGI_DWC2_DOEPTSIZ(ep) = (ep == 0 ? 3UL << 29 : 0) | MG_BIT(19) | 64;
  MGI_DWC2_DOEPCTL(ep) |= MGI_DWC2_CNAK | MGI_DWC2_EPENA;
}

// Endpoint ep active, idle, DATA0 next. type: 2 bulk, 3 interrupt. out:
// also its OUT direction, ready to receive
static void mgi_dwc2_open(unsigned ep, uint32_t type, bool out) {
  uint32_t ctl = 64 | MGI_DWC2_USBAEP | (type << 18) | MGI_DWC2_SD0PID;
  mgi_dwc2_in_stop(ep);
  MGI_DWC2_DIEPCTL(ep) = ctl | (ep << 22) | MGI_DWC2_SNAK;  // Own TX FIFO
  if (out) {
    MGI_DWC2_DOEPCTL(ep) = ctl;
    mgi_dwc2_arm(ep);
  }
}

// Stall endpoint 0, until the next SETUP
static void mgi_dwc2_stall(void) {
  MGI_DWC2_DIEPCTL(0) |= MGI_DWC2_STALL;
  MGI_DWC2_DOEPCTL(0) |= MGI_DWC2_STALL;
}

// USB reset: address 0, endpoints off, FIFOs laid out again
static void mgi_dwc2_bus_reset(void) {
  unsigned ep;
  for (ep = 0; ep < 5; ep++) {
    MGI_DWC2_DOEPCTL(ep) |= MGI_DWC2_SNAK;
    if (ep > 0) MGI_DWC2_DOEPCTL(ep) &= ~MGI_DWC2_USBAEP;
    if (ep > 0) MGI_DWC2_DIEPCTL(ep) &= ~MGI_DWC2_USBAEP;
    mgi_dwc2_in_stop(ep);
  }
  MGI_DWC2_DAINTMSK = 0x1f;           // IN endpoints 0-4: transfer complete
  MGI_DWC2_DIEPMSK = MG_BIT(0);       // XFRC
  MGI_DWC2_DOEPMSK = MG_BIT(3) | 1U;  // STUP, XFRC
  MGI_DWC2_GRSTCTL = MG_BIT(4);       // RXFFLSH
  mgi_dwc2_wait(&MGI_DWC2_GRSTCTL, MG_BIT(4), false);
  MGI_DWC2_GRXFSIZ = MGI_DWC2_RX_WORDS;
  MGI_DWC2_DIEPTXF0 = (16UL << 16) | MGI_DWC2_RX_WORDS;
  for (ep = 1; ep < 5; ep++) {
    MGI_DWC2_DIEPTXF(ep) = (16UL << 16) | (MGI_DWC2_RX_WORDS + 16 * ep);
  }
  MGI_DWC2_DCFG &= ~(0x7fUL << 4);  // Address 0
  MGI_DWC2_DIEPCTL(0) &= ~3UL;      // Endpoint 0: 64-byte packets
  MGI_DWC2_DOEPCTL(0) &= ~3UL;
  MGI_DWC2_DOEPTSIZ(0) = 3UL << 29;  // Up to 3 SETUPs in a row
  s_mgi_dwc2_req_out = false;
}

// A complete control request: usb.c answers it, then the controller follows
static void mgi_dwc2_request(MGI_DWC2_FN) {
  uint8_t *r = s_mgi_dwc2_req;
  size_t out = (r[0] & 0x80) ? 0 : MG_LOAD_LE16(r + 6);  // OUT data stage
  int res = fn(MG_USB_EV_SETUP, MG_USB_EP_CTL, r, 8 + out);
  if (res < 0) {
    mgi_dwc2_stall();
    return;
  }
  switch ((r[0] << 8) | r[1]) {
    case 0x0005:  // SET_ADDRESS: the new address goes before the status stage
      MGI_DWC2_DCFG =
          (MGI_DWC2_DCFG & ~(0x7fUL << 4)) | ((uint32_t) (r[2] & 0x7f) << 4);
      break;
    case 0x0009:  // SET_CONFIGURATION
      if (r[2] != 1) break;
#if MG_ENABLE_USB_ETHERNET
      mgi_dwc2_open(MG_USB_EP_ETH_EV, 3, false);
      mgi_dwc2_open(MG_USB_EP_ETH_DATA, 2, true);
#endif
#if MG_ENABLE_USB_SERIAL
      mgi_dwc2_open(MG_USB_EP_SER_EV, 3, false);
      mgi_dwc2_open(MG_USB_EP_SER_DATA, 2, true);
#endif
      break;
#if MG_ENABLE_USB_ETHERNET
    case 0x010b:  // SET_INTERFACE: Ethernet data endpoints back to DATA0
      if (r[4] == MG_USB_IF_ETH_DATA) {
        mgi_dwc2_open(MG_USB_EP_ETH_DATA, 2, r[2] == 1);
      }
      break;
#endif
    case 0x0201:  // CLEAR_FEATURE(ENDPOINT_HALT)
      if (r[4] & 0x80) {
        MGI_DWC2_DIEPCTL(r[4] & 15) =
            (MGI_DWC2_DIEPCTL(r[4] & 15) & ~MGI_DWC2_STALL) | MGI_DWC2_SD0PID;
      } else {
        MGI_DWC2_DOEPCTL(r[4] & 15) =
            (MGI_DWC2_DOEPCTL(r[4] & 15) & ~MGI_DWC2_STALL) | MGI_DWC2_SD0PID;
      }
      break;
  }
  if (res == 0) mgi_dwc2_in(MG_USB_EP_CTL, NULL, 0);  // Status stage
}

// A SETUP packet: run it, or first receive its OUT data stage
static void mgi_dwc2_setup(MGI_DWC2_FN) {
  uint8_t *r = s_mgi_dwc2_req;
  unsigned wlen = MG_LOAD_LE16(r + 6);
  mgi_dwc2_in_stop(0);  // A SETUP ends the previous control transfer
  s_mgi_dwc2_req_out = false;
  if ((r[0] & 0x80) || wlen == 0) {
    mgi_dwc2_request(fn);
  } else if (wlen > sizeof(s_mgi_dwc2_req) - 8) {
    mgi_dwc2_stall();
  } else {
    s_mgi_dwc2_req_out = true, s_mgi_dwc2_req_len = 0;
    mgi_dwc2_arm(0);
  }
}

// One entry of the RX FIFO: a SETUP, OUT data, or the end of either
static void mgi_dwc2_rx(MGI_DWC2_FN) {
  uint32_t st = MGI_DWC2_GRXSTSP, i;
  unsigned ep = st & 15, kind = (st >> 17) & 15;
  size_t n = (st >> 4) & 0x7ff;
  uint8_t *r = s_mgi_dwc2_req;
  for (i = 0; i < (n + 3) / 4; i++) {  // Take the packet off the FIFO
    uint32_t w = MGI_DWC2_FIFO(0);
    if (i < sizeof(s_mgi_dwc2_pkt) / 4) s_mgi_dwc2_pkt[i] = w;
  }
  if (n > sizeof(s_mgi_dwc2_pkt)) n = sizeof(s_mgi_dwc2_pkt);
  if (kind == 6) {  // SETUP packet
    memcpy(r, s_mgi_dwc2_pkt, 8);
  } else if (kind == 4) {  // SETUP stage done
    MGI_DWC2_DOEPTSIZ(0) |= 3UL << 29;
    MGI_DWC2_DOEPINT(0) = MG_BIT(3);  // STUP
    mgi_dwc2_setup(fn);
  } else if (kind == 2 && ep == 0 && s_mgi_dwc2_req_out) {  // Control OUT
    size_t room = sizeof(s_mgi_dwc2_req) - 8 - s_mgi_dwc2_req_len;
    memcpy(r + 8 + s_mgi_dwc2_req_len, s_mgi_dwc2_pkt, n < room ? n : room);
    s_mgi_dwc2_req_len += n;
#if MG_ENABLE_USB_ETHERNET
  } else if (kind == 2 && ep == MG_USB_EP_ETH_DATA) {
    fn(MG_USB_EV_OUT, MG_USB_EP_ETH_DATA, s_mgi_dwc2_pkt, n);
#endif
  } else if (kind == 3 && ep == 0 && s_mgi_dwc2_req_out) {  // OUT stage done
    s_mgi_dwc2_req_out = false;
    if (s_mgi_dwc2_req_len != MG_LOAD_LE16(r + 6)) {
      mgi_dwc2_stall();
    } else {
      mgi_dwc2_request(fn);
    }
  } else if (kind == 3 && ep != 0) {  // An OUT packet taken: take the next
    mgi_dwc2_arm(ep);                 // Serial input is dropped
  }
}

// mg_usb_hw::poll()
static void mgi_dwc2_poll(MGI_DWC2_FN) {
  uint32_t sts = MGI_DWC2_GINTSTS;
  if (sts & MG_BIT(12)) {  // USBRST
    MGI_DWC2_GINTSTS = MG_BIT(12);
    mgi_dwc2_bus_reset();
    fn(MG_USB_EV_RESET, MG_USB_EP_CTL, NULL, 0);
  }
  if (sts & MG_BIT(13)) MGI_DWC2_GINTSTS = MG_BIT(13);   // ENUMDNE
  while (MGI_DWC2_GINTSTS & MG_BIT(4)) mgi_dwc2_rx(fn);  // RXFLVL
  if (sts & MG_BIT(18)) {                                // IEPINT
    uint32_t daint = MGI_DWC2_DAINT;
    unsigned ep;
    for (ep = 0; ep < 5; ep++) {
      uint32_t ints;
      if (!(daint & MG_BIT(ep))) continue;
      ints = MGI_DWC2_DIEPINT(ep);
      MGI_DWC2_DIEPINT(ep) = ints;
      if (!(ints & 1U)) continue;  // XFRC: the packet went out
      if (!fn(MG_USB_EV_IN, (enum mg_usb_ep) ep, NULL, 0) && ep == 0 &&
          (s_mgi_dwc2_req[0] & 0x80) && MG_LOAD_LE16(s_mgi_dwc2_req + 6)) {
        mgi_dwc2_arm(0);  // IN data stage over: the host's OUT status stage
      }
    }
  }
}

// Core soft reset. From core v4.20a, the reset done bit says when it is over
static void mgi_dwc2_core_reset(void) {
  bool done_bit = (MGI_DWC2_GSNPSID & 0xffff) >= 0x420a;
  mgi_dwc2_wait(&MGI_DWC2_GRSTCTL, MG_BIT(31), true);  // AHBIDL
  MGI_DWC2_GRSTCTL |= 1U;                              // CSRST
  if (done_bit) {
    mgi_dwc2_wait(&MGI_DWC2_GRSTCTL, MG_BIT(29), true);
    MGI_DWC2_GRSTCTL = (MGI_DWC2_GRSTCTL & ~1U) | MG_BIT(29);
  } else {
    mgi_dwc2_wait(&MGI_DWC2_GRSTCTL, 1U, false);
  }
  mgi_dwc2_wait(&MGI_DWC2_GRSTCTL, MG_BIT(31), true);
}

// mg_usb_hw::init(): the serial number, the core as a full-speed device,
// then attach
static void mgi_dwc2_start(char serial[17]) {
#if defined(UID_BASE)  // STM32: from the 96-bit unique ID
  const volatile uint32_t *uid = (const volatile uint32_t *) UID_BASE;
  uint32_t id[2] = {uid[0] ^ uid[2], uid[1]};
  mg_snprintf(serial, 17, "%M", mg_print_hex, (int) sizeof(id), (uint8_t *) id);
#else
  mg_snprintf(serial, 17, "%s", "0000000000000000");
#endif
#if MG_USB_DWC2_ULPI  // 8-bit single data rate, internal VBUS indicator
  MGI_DWC2_GUSBCFG =
      (MGI_DWC2_GUSBCFG & ~(MG_BIT(3) | MG_BIT(6) | MG_BIT(7) | MG_BIT(17) |
                            MG_BIT(19) | MG_BIT(20) | MG_BIT(21))) |
      MG_BIT(4);  // ULPI
#elif MG_USB_DWC2_UTMI
  // The core's own high-speed PHY: UTMI, not ULPI or full-speed; no TSDPS
  MGI_DWC2_GUSBCFG &= ~(MG_BIT(4) | MG_BIT(6) | MG_BIT(22));
#else
  MGI_DWC2_GUSBCFG |= MG_BIT(6);  // The core's own full-speed PHY
#endif
#ifdef MG_USB_DWC2_GCCFG
  MGI_DWC2_GCCFG = MG_USB_DWC2_GCCFG;
#endif
  mgi_dwc2_core_reset();
#ifdef MG_USB_DWC2_GCCFG
  MGI_DWC2_GCCFG = MG_USB_DWC2_GCCFG;
#endif
  // Turnaround time: 9 for a high-speed PHY, 6 for a fast AHB otherwise.
  // Timeout calibration: the maximum. Then device mode, which takes a while
  MGI_DWC2_GUSBCFG = (MGI_DWC2_GUSBCFG & ~((0xfUL << 10) | 7U | MG_BIT(29))) |
                     ((MGI_DWC2_HS_PHY ? 9UL : 6UL) << 10) | 7U | MG_BIT(30);
  mgi_dwc2_wait(&MGI_DWC2_GINTSTS, 1U, false);  // CMOD: device mode
  MGI_DWC2_PCGCCTL = 0;                         // PHY clock on
  // Full speed, on a high-speed PHY (DSPD 1) or the core's own full-speed
  // one (DSPD 3). Stall a non-empty status stage. XCVRDLY: some ULPI PHYs
  // need it to chirp
  MGI_DWC2_DCFG = (MGI_DWC2_DCFG & ~3U) | MG_BIT(2) |
                  (MGI_DWC2_HS_PHY ? 1U : 3U) |
                  (MG_USB_DWC2_ULPI ? MG_BIT(14) : 0U);
  MGI_DWC2_DCTL |= MG_BIT(1);  // SDIS: detached while setting up
  // No VBUS sensing: B-session valid, always
  MGI_DWC2_GOTGCTL = (MGI_DWC2_GOTGCTL & ~MG_BIT(4)) | MG_BIT(6) | MG_BIT(7);
  MGI_DWC2_GINTSTS = 0xffffffff;
  mgi_dwc2_flush_tx(0x10);
  mgi_dwc2_bus_reset();
  MGI_DWC2_DCTL &= ~MG_BIT(1);  // Attach
}

const struct mg_usb_hw mg_usb_hw_dwc2 = {mgi_dwc2_start, mgi_dwc2_poll,
                                         mgi_dwc2_in};

#if MG_ENABLE_USB_ETHERNET
static bool mgi_dwc2_init(struct mg_tcpip_if *ifp) {
  return mg_usb_init(ifp, &mg_usb_hw_dwc2);
}

struct mg_tcpip_driver mg_tcpip_driver_usb_dwc2 = {mgi_dwc2_init, mg_usb_tx,
                                                   NULL, mg_usb_poll};
#endif

#endif
