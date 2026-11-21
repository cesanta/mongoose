// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include <driver/gpio.h>
#include <esp_heap_caps.h>

#include "mongoose.h"

void mg_mqtt_init(struct mg_mgr *mgr);  // From tutorials/mqtt/mqtt-client
void mg_mqtt_poll(struct mg_mgr *mgr);

// Seeed XIAO ESP32-C6
#define LED1 GPIO_NUM_15       // User LED, active low
#define RF_SW_PWR GPIO_NUM_3   // RF switch power, active low
#define RF_SW_SEL GPIO_NUM_14  // Antenna select: low built-in, high U.FL

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

// Power the XIAO RF switch and select the built-in antenna
static void antenna_init(void) {
  gpio_set_direction(RF_SW_PWR, GPIO_MODE_OUTPUT);
  gpio_set_level(RF_SW_PWR, 0);
  gpio_set_direction(RF_SW_SEL, GPIO_MODE_OUTPUT);
  gpio_set_level(RF_SW_SEL, 0);
}

// Toggle the user LED twice a second
static void blink_task(void) {
  static uint64_t blink_timer = 0;
  static int on = 0;
  if (mg_timer_expired(&blink_timer, 500, mg_millis())) {
    gpio_set_level(LED1, on ^= 1);
  }
}

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ACCEPT && c->is_tls) {  // Accepted on the https:// listener
    struct mg_tls_opts opts = {.cert = mg_str(TLS_CERT),
                               .key = mg_str(TLS_KEY)};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), mg_millis());
    } else if (mg_match(hm->uri, mg_str("/api/crash"), NULL)) {
      __asm volatile("unimp");  // Illegal instruction: ESP-IDF prints a panic
    } else {
      mg_http_reply(c, 200, "",
                    "Hi from Mongoose!\n"
                    "tick:     %llu\n"
                    "ram free: %zu\n"
                    "ram min:  %zu\n",
                    mg_millis(), heap_caps_get_free_size(MALLOC_CAP_DEFAULT),
                    heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT));
    }
  }
}

void app_main(void) {
  struct mg_mgr mgr;

  antenna_init();
  gpio_set_direction(LED1, GPIO_MODE_OUTPUT);

  mg_mgr_init(&mgr);  // Also starts the 802.15.4 driver
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);
  mg_http_listen(&mgr, "https://0.0.0.0", http_ev_handler, NULL);
  mg_mqtt_init(&mgr);

  MG_INFO(("Initialised. RAM free: %zu",
           heap_caps_get_free_size(MALLOC_CAP_DEFAULT)));

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    mg_mqtt_poll(&mgr);
    blink_task();
  }
}
