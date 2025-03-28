// Copyright (c) 2025 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "hal.h"


#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"  // SET THIS!
#define WIFI_PASS "YOUR_WIFI_PASSWORD"      // SET THIS!


static volatile uint64_t s_ticks;  // Milliseconds since boot
void SysTick_Handler(void) {       // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

bool mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
}


static void spi_write(uint8_t *data, size_t len) {
  while (len--) spi_write_byte(CYWSPI, *data++);
  spi_done(CYWSPI);
}

// Read data block from SPI interface
static void spi_read(uint8_t *data, size_t len) {
  spi_rx(CYWSPI);
  while (len--) *data++ = spi_read_byte(CYWSPI);
  spi_rxdone(CYWSPI);
}

static void hwspecific_spi_begin(void *arg) {
  spi_select(CYWSPI, true);
  (void) arg;
}

// either write or read, not both
static void hwspecific_spi_txn(void *arg, uint8_t *txdata, uint8_t *rxdata, size_t len) {
  if (txdata != NULL) spi_write(txdata, len);
  if (rxdata != NULL) spi_read(rxdata, len);
  (void) arg;
}

static void hwspecific_spi_end(void *arg) {
  spi_select(CYWSPI, false);
  (void) arg;
}

static void hwspecific_spi_init(void) {
  gpio_output(PWRPIN);
  gpio_write(PWRPIN, 0);
  spi_init(CYWSPI, 5);  // /64: 1.7MHz SPI1. Inits with CS low in order not to apply voltage to the radio chip
  mg_delayms(100);
  spi_select(CYWSPI, false); // de-select before applying power
  gpio_write(PWRPIN, 1);
  mg_delayms(50);
}

static const struct mg_tcpip_spi_ spi = {NULL, hwspecific_spi_begin, hwspecific_spi_end, hwspecific_spi_txn};

#ifndef CYW43_RESOURCE_ATTRIBUTE
#define CYW43_RESOURCE_ATTRIBUTE
#endif
#include "pico-sdk/lib/cyw43-driver/firmware/w43439A0_7_95_49_00_combined.h"
#include "pico-sdk/lib/cyw43-driver/firmware/wifi_nvram_43439.h"
static const struct mg_tcpip_driver_cyw_firmware fw = {
  (const uint8_t *)w43439A0_7_95_49_00_combined, (size_t)CYW43_WIFI_FW_LEN,
  (const uint8_t *)wifi_nvram_4343, (size_t)sizeof(wifi_nvram_4343),
  (const uint8_t *)(w43439A0_7_95_49_00_combined + sizeof(w43439A0_7_95_49_00_combined) - CYW43_CLM_LEN), (size_t)CYW43_CLM_LEN};

// mif user states
enum {AP, SCANNING, STOPPING_AP, CONNECTING, READY};
static unsigned int state;
static uint32_t s_ip, s_mask;


static void mif_fn(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  // TODO(): should we include this inside ifp ? add an fn_data ?
  
  if (ev == MG_TCPIP_EV_DRIVER) {
    spi_setup(CYWSPI, 3);  // /16: 3.4MHz SPI3, 6.75MHz SPI1 (< 50MHz)
  }
  if (ev == MG_TCPIP_EV_ST_CHG) {
    MG_INFO(("State change: %u", *(uint8_t *) ev_data));
  }
  switch(state) {
    case AP: // we are in AP mode, wait for a user connection to trigger a scan or a connection to a network
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_UP) {
        MG_INFO(("Access Point started"));
        s_ip = ifp->ip, ifp->ip = MG_IPV4(192, 168, 169, 1);
        s_mask = ifp->mask, ifp->mask = MG_IPV4(255, 255, 255, 0);
        ifp->enable_dhcp_client = false;
        ifp->enable_dhcp_server = true;
      } else if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_READY) {
        MG_INFO(("Access Point READY !"));

        // simulate user request to scan for networks
        bool res = mg_wifi_scan();
        MG_INFO(("Starting scan: %s", res ? "OK":"FAIL"));
        if (res) state = SCANNING;
      }
      break;
    case SCANNING:
      if (ev == MG_TCPIP_EV_WIFI_SCAN_RESULT) {
        struct mg_wifi_scan_bss_data *bss = (struct mg_wifi_scan_bss_data *) ev_data;
        MG_INFO(("BSS: %.*s (%u) (%M) %d dBm %u", bss->SSID.len, bss->SSID.buf, bss->channel, mg_print_mac, bss->BSSID, (int) bss->RSSI, bss->security));
      } else if (ev == MG_TCPIP_EV_WIFI_SCAN_END) {
        struct mg_tcpip_driver_cyw_data *d = (struct mg_tcpip_driver_cyw_data *) ifp->driver_data;
        MG_INFO(("Wi-Fi scan finished"));

        // simulate user selection of a network (1/2: stop AP)
        bool res = mg_wifi_ap_stop();
        MG_INFO(("Manually stopping AP: %s", res ? "OK":"FAIL"));
        if (res) state = STOPPING_AP;
        // else we have a hw/fw problem
      }
      break;
    case STOPPING_AP:
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_DOWN) {
        struct mg_tcpip_driver_cyw_data *d = (struct mg_tcpip_driver_cyw_data *) ifp->driver_data;
        d->apmode = false;

        // simulate user selection of a network (2/2: actual connect)
        bool res = mg_wifi_connect(d->ssid, d->pass);
        MG_INFO(("Manually connecting: %s", res ? "OK":"FAIL"));
        if (res) {
          state = CONNECTING;
          ifp->ip = s_ip;
          ifp->mask = s_mask;
          if (ifp->ip == 0) ifp->enable_dhcp_client = true;
          ifp->enable_dhcp_server = false;
        } // else manually start AP as below
      }
      break;
    case CONNECTING:
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_READY) {
        MG_INFO(("READY!"));
        state = READY;

        // simulate user code disconnection and go back to AP mode (1/2: disconnect)
        bool res = mg_wifi_disconnect();
        MG_INFO(("Manually disconnecting: %s", res ? "OK":"FAIL"));
      } else if (ev == MG_TCPIP_EV_WIFI_CONNECT_ERR) {
        MG_ERROR(("Wi-Fi connect failed"));
        // manually start AP as below
      }
      break;
    case READY:
      // go back to AP mode after a disconnection (simulation 2/2), you could retry
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_DOWN) {
        struct mg_tcpip_driver_cyw_data *d = (struct mg_tcpip_driver_cyw_data *) ifp->driver_data;
        bool res = mg_wifi_ap_start(d->apssid, d->appass, d->apchannel);
        MG_INFO(("Disconnected"));
        MG_INFO(("Manually starting AP: %s", res ? "OK":"FAIL"));
        if (res) {
          state = AP;
          d->apmode = true;
        }
      }
      break;
  }
}


static struct mg_tcpip_driver_cyw_data d = {
  (struct mg_tcpip_spi_ *)&spi, (struct mg_tcpip_driver_cyw_firmware *)&fw, WIFI_SSID, WIFI_PASS, "mongoose", "mongoose", 0, 0, 10, true, false};

int main(void) {
  uart_init(UART_DEBUG, 115200);  // Initialise debug printf

  hwspecific_spi_init();

  state = d.apmode ? AP : CONNECTING;

  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  // Initialise Mongoose network stack
  // Either set use_dhcp or enter a static config.
  // For static configuration, specify IP/mask/GW in network byte order
  struct mg_tcpip_if mif = {
      .ip = 0,
      .driver = (struct mg_tcpip_driver *)&mg_tcpip_driver_cyw,
      .driver_data = (struct mg_tcpip_driver_cyw_data*)&d,
      .fn = mif_fn,
//      .recv_queue.size = 8192
  };

  mg_tcpip_init(&mgr, &mif);
  MG_INFO(("Init done, starting main loop"));

  MG_INFO(("Initialising application..."));

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
