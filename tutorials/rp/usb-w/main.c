// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hardware/clocks.h"
#include "mongoose.h"
#include "pico/stdlib.h"

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  static bool on;  // Kept here: reading the CYW43 LED pin back is unreliable
  if (mg_timer_expired(&blink_timer, 500, mg_millis())) {
    on = !on;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
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

// WiFi settings, edited in the dashboard, see dashboard.c
char g_wifi_ssid[33];             // Network name, up to 32 bytes
char g_wifi_pass[64];             // WPA2 passphrase, up to 63 characters
bool g_wifi_enable;               // Connect, and poll s_wifi_mgr, if true
bool g_wifi_changed;              // Settings saved: reconnect in main()
static struct mg_mgr s_wifi_mgr;  // WiFi network, polled while enabled

// WiFi IP address, 0 if WiFi is disabled or has no address yet
uint32_t wifi_ip(void) {
  struct mg_tcpip_if *ifp = s_wifi_mgr.ifp;
  bool up = g_wifi_enable && ifp != NULL && ifp->state >= MG_TCPIP_STATE_IP;
  return up ? ifp->ip : 0;
}

// HTTP server on the WiFi interface: one plain text page
static void wifi_http_fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    (void) ev_data;
    mg_http_reply(c, 200, "Content-Type: text/plain\r\n",
                  "Hi from Mongoose over WiFi, uptime %llu ms\n", mg_millis());
  }
}

int main(void) {
  struct mg_mgr mgr;  // USB network, always polled
  mg_mgr_init(&mgr);
  MG_TCPIP_DRIVER_USB_RP_INIT(&mgr);  // Static IP, DHCP server for the host
  mg_dash_init(&mgr);                 // WiFi settings dashboard, see dashboard.c

  // WiFi interface: a DHCP client. The driver starts the CYW43 chip, so the
  // LED works from here on, and reads the MAC from OTP. It connects when
  // WiFi is enabled in the dashboard
  static struct mg_tcpip_driver_pico_w_data wifi_data;
  static struct mg_tcpip_if wifi_if;
  wifi_if.driver = &mg_tcpip_driver_pico_w;
  wifi_if.driver_data = &wifi_data;
  wifi_if.recv_queue.size = 8192;
  wifi_if.mac[0] = 2;  // Non-zero: keep mg_tcpip_init() from making one up
  mg_mgr_init(&s_wifi_mgr);
  mg_tcpip_init(&s_wifi_mgr, &wifi_if);
  mg_http_listen(&s_wifi_mgr, "http://0.0.0.0:80", wifi_http_fn, NULL);

  MG_INFO(("Initialised. CPU clock: %lu MHz", clock_get_hz(clk_sys) / 1000000));
  MG_INFO(("RAM used: %zu, free: %zu", hal_ram_used(), hal_ram_free()));
  MG_INFO(("USB Ethernet + serial + WiFi"));

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    mg_dash_poll(&mgr);
    if (g_wifi_changed) {  // Settings saved in the dashboard: reconnect
      g_wifi_changed = false;
      mg_wifi_disconnect();
      if (g_wifi_enable && g_wifi_ssid[0] != '\0') {
        struct mg_wifi_data wifi;
        memset(&wifi, 0, sizeof(wifi));
        wifi.ssid = g_wifi_ssid, wifi.pass = g_wifi_pass;
        mg_wifi_connect(&wifi);
      }
    }
    if (g_wifi_enable) mg_mgr_poll(&s_wifi_mgr, 0);
    blink_task();
  }

  return 0;
}
