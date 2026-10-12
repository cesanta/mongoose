// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "esp_heap_caps.h"
#include "esp_system.h"
#include "mongoose.h"

#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"  // SET THIS!
#define WIFI_PASS "YOUR_WIFI_PASSWORD"      // SET THIS!

#ifndef HTTP_URL
#define HTTP_URL "http://0.0.0.0:80"
#endif

#ifndef HTTPS_URL
#define HTTPS_URL "https://0.0.0.0:443"
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
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    size_t total = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    size_t ram_free = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), mg_millis());
    } else if (mg_match(hm->uri, mg_str("/api/crash"), NULL)) {
      *(volatile uint32_t *) 0 = 0;  // Store to address 0: CPU exception
    } else {
      mg_http_reply(c, 200, "",
                    "Hi from Mongoose!\n"
                    "tick:     %llu\n"
                    "ram used: %zu\n"
                    "ram free: %zu\n"
                    "ram min:  %zu\n",  // Lowest free heap since boot
                    mg_millis(), total - ram_free, ram_free,
                    heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT));
    }
  }
}

extern void wifi_init(const char *ssid, const char *pass);

void app_main(void) {
  MG_HEALTH_INIT();
  wifi_init(WIFI_SSID, WIFI_PASS);

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, HTTP_URL, http_ev_handler, NULL);
  mg_http_listen(&mgr, HTTPS_URL, http_ev_handler, NULL);

  MG_INFO(("Initialised. CPU clock: %lu MHz",
            (unsigned long) esp_rom_get_cpu_ticks_per_us()));
  MG_INFO(("RAM used: %zu, free: %zu",
            heap_caps_get_total_size(MALLOC_CAP_DEFAULT) -
                heap_caps_get_free_size(MALLOC_CAP_DEFAULT),
            heap_caps_get_free_size(MALLOC_CAP_DEFAULT)));
  MG_INFO(("Crash record: %M", mg_print_crash_record));

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
}
