// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "hal.h"

#ifndef UART_DEBUG
#define UART_DEBUG USART3
#endif

#define LED1 PIN(0, 12)  // Green
#define LED2 PIN(0, 0)   // Blue
#define LED3 PIN(0, 1)   // Red

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (hal_timer_expired(&blink_timer, 100, hal_get_tick())) {
    // hal_gpio_toggle(LED1);
  }
}

uint64_t mg_millis(void) {
  return hal_get_tick();
}

#if MG_ENABLE_CUSTOM_RANDOM
bool mg_random(void *buf, size_t len) {
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = hal_rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
}
#endif

// Self-signed P-256 certificate and key for the HTTPS listener. Created by:
//   openssl ecparam -genkey -name prime256v1 -noout -out key.pem
//   openssl req -x509 -key key.pem -out cert.pem -days 3650 -subj /CN=mg
#define TLS_CERT                                                       \
  "-----BEGIN CERTIFICATE-----\n"                                      \
  "MIIBgjCCASegAwIBAgIUIJOq77Qn7lHbH6WseMemeqorR50wCgYIKoZIzj0EAwIw\n" \
  "FjEUMBIGA1UEAwwLbnVjbGVvLWg3MjMwHhcNMjYxMDEyMTUwMjE5WhcNMzYxMDA5\n" \
  "MTUwMjE5WjAWMRQwEgYDVQQDDAtudWNsZW8taDcyMzBZMBMGByqGSM49AgEGCCqG\n" \
  "SM49AwEHA0IABB6IQJNmzrsdQL+2BnZYS7GYOaj7sz8f6cjBmz3n8D9Em19XodlU\n" \
  "uy4ybIvyhr766lL0ERoTmMUivb/lGmzx9C+jUzBRMB0GA1UdDgQWBBQqxMuc4gj7\n" \
  "MTMzreWlRMNI3QMqTzAfBgNVHSMEGDAWgBQqxMuc4gj7MTMzreWlRMNI3QMqTzAP\n" \
  "BgNVHRMBAf8EBTADAQH/MAoGCCqGSM49BAMCA0kAMEYCIQDY4xaJpuxq6b3rHK3b\n" \
  "lOIQyfkQX+orCRqQUkseplqpqwIhAKswZFNn+2zUNRRNY8+cHmdIwmdpsmxtoWYP\n" \
  "BCjX++LI\n"                                                         \
  "-----END CERTIFICATE-----\n"

#define TLS_KEY                                                        \
  "-----BEGIN EC PRIVATE KEY-----\n"                                   \
  "MHcCAQEEIJ/GBl0TlD52q9PMg1KWaNIdXqp8b/HyIsABmBpqC+O1oAoGCCqGSM49\n" \
  "AwEHoUQDQgAEHohAk2bOux1Av7YGdlhLsZg5qPuzPx/pyMGbPefwP0SbX1eh2VS7\n" \
  "LjJsi/KGvvrqUvQRGhOYxSK9v+UabPH0Lw==\n"                             \
  "-----END EC PRIVATE KEY-----\n"

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ACCEPT && c->is_tls) {  // Accepted on the https:// listener
    struct mg_tls_opts opts = {.cert = mg_str(TLS_CERT),
                               .key = mg_str(TLS_KEY)};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), hal_get_tick());
    } else if (mg_match(hm->uri, mg_str("/api/crash"), NULL)) {
      __asm volatile("udf #0");  // Trigger HardFault
    } else {
      mg_http_reply(c, 200, "",
                    "Hi from Mongoose!\n"
                    "tick:     %llu\n"
                    "ram used: %zu\n"
                    "ram free: %zu\n",
                    hal_get_tick(), hal_ram_used(), hal_ram_free());
    }
  }
}

// Our address on the radio side, and the /64 prefix we advertise there.
// fd12:3456:789a:1::1, a unique local address, RFC 4193
#define RADIO_IP6 \
  "\xfd\x12\x34\x56\x78\x9a\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01"

// NAT64 prefix advertised to radio nodes: 64:ff9b::/96, RFC 6052. Nodes reach
// IPv4 hosts as 64:ff9b::a.b.c.d, we translate to IPv4 on Ethernet
#define NAT64_PREFIX "\x00\x64\xff\x9b\x00\x00\x00\x00\x00\x00\x00\x00"

int main(void) {
  hal_clock_init();
  hal_rng_init();
  hal_gpio_output(LED1);
  hal_gpio_write(LED1, true);
  hal_gpio_output(LED2);
  hal_gpio_write(LED2, true);
  hal_gpio_output(LED3);
  hal_gpio_write(LED3, true);
  hal_uart_init(UART_DEBUG, 115200);
  mg_log_set_fn(log_fn, UART_DEBUG);

  if (hal_radio_err) {
    MG_ERROR(("Radio firmware failed to start, step %d", hal_radio_err));
  }

  // Two interfaces, each on its own manager: Ethernet and 802.15.4
  static struct mg_tcpip_driver_rw612_data eth_data = {
      .mdc_cr = MG_DRIVER_MDC_CR, .phy_addr = MG_TCPIP_PHY_ADDR};
  static struct mg_tcpip_if eth = {.driver = &mg_tcpip_driver_rw612,
                                   .driver_data = &eth_data};
  static struct mg_tcpip_if radio = {.driver = &mg_tcpip_driver_rw612_154,
                                     .l2type = MG_TCPIP_L2_LOWPAN};
  struct mg_mgr mgr_eth, mgr_radio;
  uint8_t uid[FSL_OCOTP_UID_LENGTH], digest[32];
  uint32_t uid_len = sizeof(uid);

  // Stable addresses, made from the chip unique ID: neighbours remember
  // them, e.g. radio nodes keep us as their default router
  memset(uid, 0, sizeof(uid));
  OCOTP_ReadUniqueID(uid, &uid_len);
  mg_sha256(digest, uid, sizeof(uid));
  memcpy(eth.mac, digest, 6);
  memcpy(radio.mac, digest + 6, 8);
  eth.mac[0] = radio.mac[0] = 0x02;  // Locally administered, unicast
  hal_ethernet_init();
  mg_mgr_init(&mgr_eth);
  mg_tcpip_init(&mgr_eth, &eth);
  radio.l2data.lowpan.channel = 15;
  radio.l2data.lowpan.pan_id = 0xabcd;
  memcpy(radio.ip6, RADIO_IP6, sizeof(radio.ip6));  // Static: we are router
  memcpy(radio.prefix, RADIO_IP6, sizeof(radio.prefix));
  radio.prefix_len = 64;
  memcpy(radio.nat64, NAT64_PREFIX, sizeof(radio.nat64));  // We do NAT64
  mg_mgr_init(&mgr_radio);
  mg_tcpip_init(&mgr_radio, &radio);
  mgr_radio.use_dns6 = true;  // IPv6 only: no IPv4 over 6LoWPAN

  // Route IPv6 between the two: radio nodes get addresses from RADIO_IP6/64,
  // the Ethernet LAN gets a route to that prefix via us
  mg_tcpip_route(&eth, &radio);

  // Minimal web server on both interfaces
  mg_http_listen(&mgr_eth, "http://0.0.0.0:80", http_ev_handler, NULL);
  mg_http_listen(&mgr_eth, "https://0.0.0.0:443", http_ev_handler, NULL);
  mg_http_listen(&mgr_radio, "http://[::]:80", http_ev_handler, NULL);
  mg_http_listen(&mgr_radio, "https://[::]:443", http_ev_handler, NULL);

  MG_INFO(("Initialised. CPU clock: %lu MHz", SystemCoreClock / 1000000));
  MG_INFO(("RAM used: %zu, free: %zu", hal_ram_used(), hal_ram_free()));
  MG_INFO(("Crash record: %M", mg_print_crash_record));

  for (;;) {
    mg_mgr_poll(&mgr_eth, 0);
    mg_mgr_poll(&mgr_radio, 0);
    blink_task();
  }

  return 0;
}
