// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// IPv4 over IEEE 802.15.4 on FRDM-MCXW71, Mongoose built-in TCP/IP stack.
// The radio carries Ethernet frames, channel 15, PAN ID 0xabcd. With the
// FRDM-RW612 eth_radio_bridge tutorial, gets an address from the LAN's DHCP
// server and is reachable from the LAN. Serves pages on ports 80 and 443

#include "hal.h"
#include "mongoose.h"

#define UART_DEBUG LPUART1  // MCU-Link USB serial port

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

uint64_t mg_millis(void) {
  return hal_get_tick();
}

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
  } else if (ev == MG_EV_HTTP_HDRS) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/ota"), NULL)) {
      mg_http_stream_body(c, hm, mg_http_ota_cb, NULL);
    }
  } else if (ev == MG_EV_HTTP_MSG) {
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

int main(void) {
  hal_clock_init();
  hal_uart_init(UART_DEBUG, 115200);
  mg_log_set_fn(log_fn, UART_DEBUG);

  if (!hal_radio_init()) MG_ERROR(("Radio init failed"));

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:80", http_ev_handler, NULL);
  mg_http_listen(&mgr, "https://0.0.0.0:443", http_ev_handler, NULL);
  mg_mqtt_init(&mgr);

  MG_INFO(("Initialised. CPU clock: %lu MHz", SystemCoreClock / 1000000));
  MG_INFO(("RAM used: %zu, free: %zu", hal_ram_used(), hal_ram_free()));
  MG_INFO(("Crash record: %M", mg_print_crash_record));

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    mg_mqtt_poll(&mgr);
  }

  return 0;
}
