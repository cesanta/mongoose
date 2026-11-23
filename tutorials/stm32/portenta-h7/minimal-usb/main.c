// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Portenta H7: Mongoose over USB. One USB-C cable gives the board a CDC-NCM
// Ethernet interface and a CDC-ACM serial port that carries the log

#include "hal.h"
#include "mongoose.h"

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (mg_timer_expired(&blink_timer, 500, mg_millis())) hal_gpio_toggle(LED);
}

uint64_t mg_millis(void) {
  return hal_get_tick();
}

bool mg_random(void *buf, size_t len) {
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = hal_rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
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
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), mg_millis());
    } else {
      mg_http_reply(c, 200, "",
                    "Hi from Mongoose!\n"
                    "tick:     %llu\n"
                    "ram used: %zu\n"
                    "ram free: %zu\n",
                    mg_millis(), hal_ram_used(), hal_ram_free());
    }
  }
}

int main(void) {
  hal_rng_init();
  hal_gpio_output(LED);
  hal_gpio_write(LED, true);  // Off, the LEDs are active low
  hal_usb_init();

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);  // Also starts the USB driver, see mongoose_config.h
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);
  mg_http_listen(&mgr, "https://0.0.0.0", http_ev_handler, NULL);

  MG_INFO(("Initialised. CPU clock: %lu MHz", SystemCoreClock / 1000000));
  MG_INFO(("RAM used: %zu, free: %zu", hal_ram_used(), hal_ram_free()));
  MG_INFO(("USB Ethernet + serial"));

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    blink_task();
  }

  return 0;
}
