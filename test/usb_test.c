#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0
#define MG_ENABLE_ASSERT 1
#define MG_ENABLE_DRIVER_USB_DWC2 1
#define MG_ENABLE_USB_ETHERNET 1
#ifndef MG_ENABLE_USB_SERIAL
#define MG_ENABLE_USB_SERIAL 1
#endif

#include "mongoose.h"

// Exercise the USB protocol through a simulated controller, without MMIO
static struct mg_tcpip_if s_if;
static bool s_pending;
static enum mg_usb_ev s_ev;
static enum mg_usb_ep s_ep;
static uint8_t s_request[8], s_packet[64];
static size_t s_length, s_packet_length;
static int s_result;
static unsigned s_frames, s_notifications;

static void usb_init(char serial[17]) {
  memcpy(serial, "0123456789ABCDEF", 17);
}

static void usb_poll(int (*fn)(enum mg_usb_ev, enum mg_usb_ep, void *, size_t)) {
  if (s_pending) {
    s_pending = false;
    s_result = fn(s_ev, s_ep, s_request, s_length);
  }
}

static void usb_in(enum mg_usb_ep ep, const void *buf, size_t len) {
  if (ep == MG_USB_EP_ETH_DATA) s_frames++;
  if (ep == MG_USB_EP_ETH_EV) {
    assert(len <= sizeof(s_packet));
    memcpy(s_packet, buf, len);
    s_packet_length = len;
    s_notifications++;
  }
}

static bool event(enum mg_usb_ev ev, enum mg_usb_ep ep, size_t len) {
  s_ev = ev, s_ep = ep, s_length = len, s_pending = true;
  return mg_usb_poll(&s_if, true);
}

static bool request(uint8_t type, uint8_t code, uint16_t value,
                    uint16_t index) {
  s_request[0] = type, s_request[1] = code;
  MG_STORE_LE16(s_request + 2, value);
  MG_STORE_LE16(s_request + 4, index);
  MG_STORE_LE16(s_request + 6, 0);
  bool up = event(MG_USB_EV_SETUP, MG_USB_EP_CTL, sizeof(s_request));
  assert(s_result == 0);
  return up;
}

int main(void) {
  static const struct mg_usb_hw hw = {
      .init = usb_init, .poll = usb_poll, .in = usb_in};
  const uint8_t frame[14] = {0};
  const uint16_t control = MG_USB_IF_ETH_DATA - 1;

  assert(mg_usb_init(&s_if, &hw));
  assert(!event(MG_USB_EV_RESET, MG_USB_EP_CTL, 0));
  assert(!request(0, 9, 1, 0));  // SET_CONFIGURATION
  assert(mg_usb_tx(frame, sizeof(frame), &s_if) == 0);

  // Windows opens NCM without sending SET_ETHERNET_PACKET_FILTER
  assert(request(1, 11, 1, MG_USB_IF_ETH_DATA));
  assert(mg_usb_tx(frame, sizeof(frame), &s_if) == sizeof(frame));
  assert(s_frames == 1);
  assert(s_notifications == 1 && s_packet_length == 16);
  assert(s_packet[1] == 0x2a && MG_LOAD_LE16(s_packet + 4) == control);
  assert(event(MG_USB_EV_IN, MG_USB_EP_ETH_EV, 0));
  assert(s_notifications == 2 && s_packet_length == 8);
  assert(s_packet[1] == 0 && MG_LOAD_LE16(s_packet + 2) == 1);
  assert(MG_LOAD_LE16(s_packet + 4) == control);
  assert(event(MG_USB_EV_IN, MG_USB_EP_ETH_EV, 0));
  assert(event(MG_USB_EV_IN, MG_USB_EP_ETH_DATA, 0));

  // Linux can still set a filter; an explicit zero suppresses traffic
  assert(!request(0x21, 0x43, 0, control));
  assert(mg_usb_tx(frame, sizeof(frame), &s_if) == 0);
  assert(request(0x21, 0x43, 0x0c, control));
  assert(mg_usb_tx(frame, sizeof(frame), &s_if) == sizeof(frame));
  assert(s_frames == 2);
  assert(event(MG_USB_EV_IN, MG_USB_EP_ETH_DATA, 0));

  // Closing/reopening restores the default, including after a zero filter
  assert(!request(0x21, 0x43, 0, control));
  assert(!request(1, 11, 0, MG_USB_IF_ETH_DATA));
  assert(s_notifications == 3 && s_packet_length == 8);
  assert(s_packet[1] == 0 && MG_LOAD_LE16(s_packet + 2) == 0);
  assert(MG_LOAD_LE16(s_packet + 4) == control);
  assert(!event(MG_USB_EV_IN, MG_USB_EP_ETH_EV, 0));
  assert(mg_usb_tx(frame, sizeof(frame), &s_if) == 0);
  assert(request(1, 11, 1, MG_USB_IF_ETH_DATA));
  assert(mg_usb_tx(frame, sizeof(frame), &s_if) == sizeof(frame));
  assert(s_frames == 3);

  assert(!event(MG_USB_EV_RESET, MG_USB_EP_CTL, 0));
  assert(mg_usb_tx(frame, sizeof(frame), &s_if) == 0);
  puts("USB NCM tests passed");
  return 0;
}
