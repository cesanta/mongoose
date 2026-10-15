// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "pico/unique_id.h"

#include "mongoose.h"

enum { SPI_CS = 17, SPI_CLK = 18, SPI_TX = 19, SPI_RX = 16 };  // Pins
// enum { SPI_CS = 5, SPI_CLK = 2, SPI_TX = 3, SPI_RX = 4 };  // Pins

static void spi_begin(void *spi) {
  gpio_put(SPI_CS, 0);
}

static void spi_end(void *spi) {
  gpio_put(SPI_CS, 1);
}

static void spi_txn(void *spi, uint8_t *tx, uint8_t *rx, size_t len) {
  if (tx != NULL && rx != NULL) {
    spi_write_read_blocking(spi0, tx, rx, len);
  } else if (tx != NULL) {
    spi_write_blocking(spi0, tx, len);
  } else {
    spi_read_blocking(spi0, 0xFF, rx, len);
  }
}

static void ethernet_init(void) {
  // Init SPI pins
  spi_init(spi0, 500 * 1000);
  gpio_set_function(SPI_RX, GPIO_FUNC_SPI);   // MISO
  gpio_set_function(SPI_TX, GPIO_FUNC_SPI);   // MOSI
  gpio_set_function(SPI_CLK, GPIO_FUNC_SPI);  // CLK
  gpio_init(SPI_CS);                          // CS
  gpio_set_dir(SPI_CS, GPIO_OUT);             // Set CS to output
  gpio_put(SPI_CS, 1);                        // And drive CS high (inactive)
}

// Construct MAC address from the unique board ID
static void generate_mac_from_board_id(unsigned char *mac) {
  pico_unique_board_id_t board_id;
  pico_get_unique_board_id(&board_id);
  mac[0] = 2;
  memcpy(&mac[1], &board_id.id[3], 5);
}

static void blink_task(void) {
  static uint64_t timer;
  if (mg_timer_expired(&timer, 500, mg_now())) {
    gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
  }
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
  } else if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    uint64_t tick = to_ms_since_boot(get_absolute_time());
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), tick);
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n", tick);
    }
  }
}

static void log_fn(char ch, void *param) {
  (void) param;
  putchar(ch);
}

// In an RTOS environment, give this task 8 KB of stack space.
static void run_mongoose(void) {
  struct mg_mgr mgr;
  struct mg_tcpip_spi spi = {NULL, spi_begin, spi_end, spi_txn};
  struct mg_tcpip_if mif = {
      .driver = &mg_tcpip_driver_w5500,
      .driver_data = &spi,
  };
  generate_mac_from_board_id(mif.mac);
  mg_log_set_fn(log_fn, NULL);
  mg_mgr_init(&mgr);
  mg_tcpip_init(&mgr, &mif);
  MG_INFO(("Driver: W5500, MAC: %M", mg_print_mac, mif.mac));

  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);
  mg_http_listen(&mgr, "https://0.0.0.0", http_ev_handler, NULL);

  for (;;) {
    mg_mgr_poll(&mgr, 1);
    blink_task();
  }
}

int main(void) {
  stdio_init_all();
  gpio_init(PICO_DEFAULT_LED_PIN);               // Initialise GPIO
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);  // Set as output
  gpio_put(PICO_DEFAULT_LED_PIN, 1);
  ethernet_init();
  MG_INFO(("Hardware initialised, starting firmware..."));

  run_mongoose();
  return 0;
}
