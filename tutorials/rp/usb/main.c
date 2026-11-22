// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hardware/clocks.h"
#include "mongoose.h"
#include "pico/stdlib.h"

#define LED1 PICO_DEFAULT_LED_PIN  // User LED, active low
//#define LED1 16  // User LED, active low

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (mg_timer_expired(&blink_timer, 500, mg_millis())) {
    gpio_xor_mask(1u << LED1);
  }
}

// Match MG24's heap-extent measurements. The Pico SDK's newlib _sbrk(0)
// returns the current heap break without allocating. Its linker reserves
// stacks separately, so free space ends at __HeapLimit, not the current SP.
// These figures exclude globals/stacks and reusable blocks inside malloc.
extern void *_sbrk(int incr);
extern char __end__, __HeapLimit;

static size_t hal_ram_used(void) {
  return (size_t) ((uintptr_t) _sbrk(0) - (uintptr_t) &__end__);
}

static size_t hal_ram_free(void) {
  return (size_t) ((uintptr_t) &__HeapLimit - (uintptr_t) _sbrk(0));
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
  gpio_init(LED1);
  gpio_put(LED1, 1);
  gpio_set_dir(LED1, GPIO_OUT);

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);  // Also starts the USB driver, see mongoose_config.h
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);
  mg_http_listen(&mgr, "https://0.0.0.0", http_ev_handler, NULL);

  MG_INFO(("Initialised. CPU clock: %lu MHz", clock_get_hz(clk_sys) / 1000000));
  MG_INFO(("RAM used: %zu, free: %zu", hal_ram_used(), hal_ram_free()));
  MG_INFO(("USB Ethernet + serial"));

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    blink_task();
  }

  return 0;
}
