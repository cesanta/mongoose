// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"

#define LED_RADIO PIN(0, 12)  // Green, active low
#define LED_LAN PIN(0, 0)     // Blue, active low
#define LED_RED PIN(0, 1)     // Red, active low

static uint64_t s_radio_led_until;

// Cumulative totals. Up: radio -> LAN; down: LAN -> radio queue.
static uint64_t s_frames_up, s_frames_down, s_bytes_up, s_bytes_down;
static uint64_t s_radio_sent, s_radio_received;
static size_t (*s_radio_tx)(struct mg_tcpip_if *, const void *, size_t);

static size_t radio_tx(struct mg_tcpip_if *ifp, const void *buf, size_t len) {
  size_t sent = s_radio_tx(ifp, buf, len);
  if (sent != 0) s_radio_sent++;
  return sent;
}

static size_t eth_up(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  size_t sent = mg_tcpip_driver_rw612.tx(buf, len, ifp);
  if (sent != 0) s_frames_up++, s_bytes_up += sent;
  return sent;
}

static size_t eth_down(struct mg_mesh_node *node, const void *buf, size_t len) {
  size_t queued = mg_mesh_default.tx(node, buf, len);
  if (queued != 0) s_frames_down++, s_bytes_down += queued;
  return queued;
}

static void radio_rx(struct mg_mesh_node *node, void *buf, size_t len) {
  s_radio_received++;
  s_radio_led_until = mg_millis() + 2;
  mg_mesh_default.rx(node, buf, len);
}

static void log_fn(char ch, void *arg) {
  hal_uart_write_buf(arg, &ch, 1);
}

uint64_t mg_millis(void) {
  return hal_get_tick();
}

int main(void) {
  static struct mg_tcpip_driver_rw612_data eth_data;
  static struct mg_tcpip_if eth;
  static struct mg_tcpip_driver eth_driver;
  static char eth_queue[8192];
  static struct mg_tcpip_driver_rw612w_data radio_data;
  static struct mg_tcpip_if radio;
  static struct mg_mesh mesh;
  uint32_t lan_received = 0;
  uint64_t lan_led_until = 0;
  uint64_t link_timer = 0, stats_timer = 0;

  hal_clock_init();
  hal_rng_init();
  hal_gpio_output(LED_RADIO);
  hal_gpio_output(LED_LAN);
  hal_gpio_output(LED_RED);
  hal_gpio_write(LED_RADIO, true);
  hal_gpio_write(LED_LAN, true);
  hal_gpio_write(LED_RED, true);
  hal_uart_init(USART3, 115200, PIN(0, 26), PIN(0, 24));
  mg_log_set_fn(log_fn, USART3);
  hal_ethernet_init();

  mesh = mg_mesh_default;
  mesh.tx = eth_down;
  mesh.rx = radio_rx;  // Observe radio reception without modifying the driver.
  radio_data.strategy = &mesh;
  radio.driver = &mg_tcpip_driver_rw612w;
  radio.driver_data = &radio_data;
  if (!radio.driver->init(&radio)) return 1;
  s_radio_tx = radio_data.mesh->tx;
  radio_data.mesh->tx = radio_tx;

  eth_data.mdc_cr = MG_DRIVER_MDC_CR;
  eth_data.mdc_holdtime = MG_DRIVER_MDC_HOLDTIME;
  eth_data.phy_addr = MG_TCPIP_PHY_ADDR;
  eth_driver = mg_tcpip_driver_rw612;
  eth_driver.tx = eth_up;
  eth.driver = &eth_driver;
  eth.driver_data = &eth_data;
  memcpy(eth.mac, radio.mac, 6);
  mg_queue_init(&eth.recv_queue, eth_queue, sizeof(eth_queue));
  if (!eth.driver->init(&eth)) return 1;

  ENET->RCR |= ENET_RCR_PROM_MASK;  // Receive frames addressed to radio nodes.
  radio_data.mesh->lan = &eth;
  radio_data.mesh->next_tick = 0;

  MG_INFO(("Ethernet/radio bridge, channel %u", MG_802154_CHANNEL));
  stats_timer = mg_millis() + 5000;
  for (;;) {
    uint64_t now = mg_millis();
    if (mg_timer_expired(&link_timer, 100, mg_millis())) {
      eth.state = eth.driver->poll(&eth, true) ? MG_TCPIP_STATE_UP
                                               : MG_TCPIP_STATE_DOWN;
    }
    radio.driver->poll(&radio, false);
    if (mg_timer_expired(&stats_timer, 5000, now)) {
      MG_INFO(
          ("ETH frames up/down %llu/%llu, bytes up/down "
           "%llu/%llu, radio frames sent/rcvd %llu/%llu",
           s_frames_up, s_frames_down, s_bytes_up, s_bytes_down, s_radio_sent,
           s_radio_received));
    }
    if (eth.nrecv != lan_received) {
      lan_received = eth.nrecv;
      lan_led_until = now + 2;
    }
    hal_gpio_write(LED_RADIO, now >= s_radio_led_until);
    hal_gpio_write(LED_LAN, now >= lan_led_until);
  }
  return 0;
}
