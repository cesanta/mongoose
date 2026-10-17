// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Ethernet <-> IEEE 802.15.4 radio bridge. Radio frames carry Ethernet frames
// split into pieces, see mg_l2_802154_tx(). Radio nodes, e.g. the
// FRDM-MCXW71 minimal tutorial, are on the Ethernet LAN: they get IPv4
// addresses from its DHCP server. The bridge itself has no IP address.
// Everything from the radio goes to Ethernet. From Ethernet, only broadcasts
// and frames to radio nodes go to the radio, it is much slower

#include "hal.h"
#include "mongoose.h"

#define UART_DEBUG USART3

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

uint64_t mg_millis(void) {
  return hal_get_tick();
}

// MAC addresses of radio nodes, learned from frames they send
static uint8_t s_nodes[8][6];
static size_t s_next;

// Remember a radio node's MAC address, if new
static void learn(const uint8_t *mac) {
  size_t i, n = sizeof(s_nodes) / sizeof(s_nodes[0]);
  for (i = 0; i < n && memcmp(s_nodes[i], mac, 6) != 0; i++) (void) 0;
  if (i < n) return;
  memcpy(s_nodes[s_next], mac, 6);
  s_next = (s_next + 1) % n;
  MG_INFO(("Radio node %M", mg_print_mac, mac));
}

// Is an Ethernet frame for the radio: broadcast, or to a radio node
static bool for_radio(const uint8_t *dst) {
  size_t i, n = sizeof(s_nodes) / sizeof(s_nodes[0]);
  if (memcmp(dst, "\xff\xff\xff\xff\xff\xff", 6) == 0) return true;
  for (i = 0; i < n && memcmp(s_nodes[i], dst, 6) != 0; i++) (void) 0;
  return i < n;
}

int main(void) {
  static struct mg_tcpip_driver_rw612_data eth_data = {
      .mdc_cr = MG_DRIVER_MDC_CR, .phy_addr = MG_TCPIP_PHY_ADDR};
  static struct mg_l2_802154_data radio_data = {
      .channel = MG_L2_802154_CHANNEL, .pan_id = MG_L2_802154_PAN_ID};
  static struct mg_tcpip_if eth = {.driver = &mg_tcpip_driver_rw612,
                                   .driver_data = &eth_data};
  static struct mg_tcpip_if radio = {.driver = &mg_tcpip_driver_rw612_154,
                                     .driver_data = &radio_data};
  static char eth_queue[8192];  // Filled by the Ethernet IRQ handler
  static uint8_t frame[1540];
  uint8_t uid[FSL_OCOTP_UID_LENGTH], digest[32];
  uint32_t uid_len = sizeof(uid);
  uint64_t timer = 0;
  char *p;
  size_t n;

  hal_clock_init();
  hal_uart_init(UART_DEBUG, 115200);
  mg_log_set_fn(log_fn, UART_DEBUG);
  if (hal_radio_err) {
    MG_ERROR(("Radio firmware failed to start, step %d", hal_radio_err));
  }

  // MAC addresses made from the chip unique ID
  memset(uid, 0, sizeof(uid));
  OCOTP_ReadUniqueID(uid, &uid_len);
  mg_sha256(digest, uid, sizeof(uid));
  memcpy(eth.mac, digest, 6);
  memcpy(radio.mac, digest + 6, 6);
  eth.mac[0] = radio.mac[0] = 0x02;  // Locally administered, unicast

  hal_ethernet_init();
  mg_queue_init(&eth.recv_queue, eth_queue, sizeof(eth_queue));
  if (!eth.driver->init(&eth)) MG_ERROR(("Ethernet init failed"));
  if (!radio.driver->init(&radio)) MG_ERROR(("Radio init failed"));
  ENET->RCR |= ENET_RCR_PROM_MASK;  // Receive frames for radio nodes, too
  MG_INFO(("Bridge started, radio channel %d", radio_data.channel));

  for (;;) {
    if ((n = mg_queue_next(&eth.recv_queue, &p)) > 0) {  // Ethernet -> radio
      if (n >= 14 && for_radio((uint8_t *) p)) radio.driver->tx(p, n, &radio);
      mg_queue_del(&eth.recv_queue, n);
    }
    if ((n = radio.driver->rx(frame, sizeof(frame), &radio)) >= 14) {
      learn(frame + 6);  // Radio -> Ethernet
      eth.driver->tx(frame, n, &eth);
    }
    if (mg_timer_expired(&timer, 1000, mg_millis())) {  // Ethernet link
      bool up = eth.driver->poll(&eth, true);
      eth.state = up ? MG_TCPIP_STATE_UP : MG_TCPIP_STATE_DOWN;
    }
  }

  return 0;
}
