// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

static struct mg_dash s_dash;

// WiFi settings and logic are in main.c. This is the "wifi" object in
// dashboard.html
extern char g_wifi_ssid[33], g_wifi_pass[64];
extern bool g_wifi_enable, g_wifi_changed;
extern uint32_t wifi_ip(void);
static char s_wifi_status[40];  // Read-only, regenerated on every read

static bool wifi_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  (void) u;
  if (op == MG_DASH_READ) {
    uint32_t ip = wifi_ip();
    if (ip != 0) {
      mg_snprintf(s_wifi_status, sizeof(s_wifi_status), "enabled, IP: %M",
                  mg_print_ip4, &ip);
    } else {
      mg_snprintf(s_wifi_status, sizeof(s_wifi_status), "not connected");
    }
    return true;
  }
  // Called twice: before the fields are updated (permission check), and
  // after. main() reconnects once, later
  if (op == MG_DASH_WRITE) g_wifi_changed = true;
  return op == MG_DASH_WRITE;
}

static struct mg_field fields_wifi[] = {
    {"ssid", MG_VAL_STR, g_wifi_ssid, sizeof(g_wifi_ssid)},
    {"pass", MG_VAL_STR, g_wifi_pass, sizeof(g_wifi_pass)},
    {"enable", MG_VAL_BOOL, &g_wifi_enable, sizeof(g_wifi_enable)},
    {"status", MG_VAL_STR, s_wifi_status, 0},  // value_size 0: read-only
    {NULL, MG_VAL_INT, NULL, 0},
};
static struct mg_field_set set_wifi = {"wifi", fields_wifi, wifi_fn, NULL,
                                       NULL};

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

void mg_dash_init(struct mg_mgr *mgr) {
  s_dash.tls_cert = mg_str(TLS_CERT);  // HTTPS listener, see MG_HTTPS_ADDR
  s_dash.tls_key = mg_str(TLS_KEY);
  MG_DASH_ADD_FIELD_SET(&s_dash, &set_wifi);
  mg_mem_files = mg_packed_files;
  mg_http_listen(mgr, MG_HTTP_ADDR, mg_dash_ev_handler, &s_dash);
  mg_http_listen(mgr, MG_HTTPS_ADDR, mg_dash_ev_handler, &s_dash);
}

void mg_dash_poll(struct mg_mgr *mgr) {
  static uint32_t prev_ip;
  if (wifi_ip() != prev_ip) {  // Got or lost an address: update the status
    prev_ip = wifi_ip();
    mg_dash_send_change(mgr, &set_wifi);
  }
}
