// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// USB to BLE bridge. The USB Ethernet interface is the LAN, the radio
// carries the mesh over BLE 2 Mbit/s frames. The bridge runs the mesh: it
// admits nodes and gives each one a turn to send

#include "hal.h"
#include "mongoose.h"

static uint64_t s_frames_up, s_frames_down;  // Up: radio to USB

// LAN side: count Ethernet frames forwarded from the radio to the USB host
static size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  size_t sent = mg_tcpip_driver_usb_nrf52.tx(buf, len, ifp);
  if (sent != 0) s_frames_up++;
  return sent;
}

// Mesh side: count Ethernet frames from the USB host queued for the radio
static size_t radio_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  size_t queued = mg_tcpip_driver_nrf52.tx(buf, len, ifp);
  if (queued != 0) s_frames_down++;
  return queued;
}

int main(void) {
  static struct mg_tcpip_if usb, radio;
  static struct mg_tcpip_driver usb_driver;
  static char usb_queue[8192];
  static struct mg_tcpip_driver radio_driver;
  uint64_t blink_timer = 0, stats_timer = 0;

  hal_system_init();
  hal_rng_init();
  hal_gpio_write(LED, true);  // Off
  hal_gpio_output(LED);
  mg_usb_init(NULL, &mg_usb_hw_nrf52);  // USB first: the log goes there

  radio_driver = mg_tcpip_driver_nrf52;
  radio_driver.tx = radio_tx;
  radio.driver = &radio_driver;
  MG_SET_MAC_ADDRESS(radio.mac);
  if (!radio.driver->init(&radio)) return 1;

  usb_driver = mg_tcpip_driver_usb_nrf52;
  usb_driver.tx = usb_tx;
  usb.driver = &usb_driver;
  memcpy(usb.mac, radio.mac, sizeof(usb.mac));
  mg_queue_init(&usb.recv_queue, usb_queue, sizeof(usb_queue));
  if (!usb.driver->init(&usb)) return 1;

  mg_radio_bridge(&radio, &usb);  // Forward between radio and USB

  MG_INFO(("USB to BLE bridge, channel %d, MAC %M", MG_RADIO_CHANNEL,
           mg_print_mac, radio.mac));
  for (;;) {
    uint64_t now = mg_millis();
    usb.driver->poll(&usb, false);
    radio.driver->poll(&radio, false);
    if (mg_timer_expired(&blink_timer, 500, now)) hal_gpio_toggle(LED);
    if (mg_timer_expired(&stats_timer, 5000, now)) {
      MG_INFO(("Ethernet frames up/down: %llu/%llu, dropped USB/radio: %u/%u",
               s_frames_up, s_frames_down, (unsigned) usb.ndrop,
               (unsigned) radio.ndrop));
    }
  }

  return 0;
}
