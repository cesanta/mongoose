// Copyright (c) 2025 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "hal.h"


#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"  // SET THIS!
#define WIFI_PASS "YOUR_WIFI_PASSWORD"      // SET THIS!


static void hwspecific_sdio_init(void) {
  gpio_output(PWRPIN);
  gpio_write(PWRPIN, 0);
  mg_delayms(100);
  sdmmc_init();
  mg_delayms(1);
  gpio_write(PWRPIN, 1);
  mg_delayms(50);
}

void hwspecific_sdio_config(void *sdio, uint8_t cfg) {
  (void) sdio;
  if (cfg == 0) return sdmmc_set_ds();
  if (cfg == 1) return sdmmc_set_hs();
}
bool hwspecific_sdio_txn(void *sdio, uint8_t cmd, uint32_t arg, uint32_t *resp) {
  (void) sdio;
  return sdmmc_transact(cmd, arg, resp);
}
bool hwspecific_sdio_xfr(void *sdio, bool write, uint32_t arg, uint16_t blksz, uint32_t *data, uint32_t len, uint32_t *resp){
  (void) sdio;
  return sdmmc_transfer(write, arg, blksz, data, len, resp);
}

static const struct mg_tcpip_sdio sdio = {NULL, hwspecific_sdio_config, hwspecific_sdio_txn, hwspecific_sdio_xfr};

// clang-format off
typedef enum {RESOURCE_IN_MEMORY,RESOURCE_IN_EXTERNAL_STORAGE} resource_location_t;
typedef struct {resource_location_t l;unsigned long sz;union{struct {unsigned long o;const char *f;}fs;struct {const char *m;}mem;}val;} resource_hnd_t;
#include "mbed/targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD/resources/firmware/COMPONENT_4343W_FS/4343WA1_bin.c"
#include "mbed/targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD/resources/firmware/COMPONENT_4343W_FS/4343WA1_clm_blob.c"
#include "mbed/targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD/resources/nvram/wifi_nvram_image.h"
// clang-format on
static const struct mg_tcpip_driver_cyw_firmware fw = {
  (const uint8_t *)wifi_firmware_image_data, (size_t)sizeof(wifi_firmware_image_data),
  (const uint8_t *)wifi_nvram_image, (size_t)sizeof(wifi_nvram_image),
  (const uint8_t *)wifi_firmware_clm_blob_image_data, (size_t)sizeof(wifi_firmware_clm_blob_image_data)};

// mif user states
enum {AP, SCANNING, STOPPING_AP, CONNECTING, READY};
static unsigned int state;


static void mif_fn(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  if (ev == MG_TCPIP_EV_ST_CHG) {
    MG_INFO(("State change: %u", *(uint8_t *) ev_data));
  }
  switch(state) {
    case AP: // we are in AP mode, wait for a user connection to trigger a scan or a connection to a network
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_READY) {
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
        struct mg_wifi_data *wifi = &((struct mg_tcpip_driver_cyw_data *) ifp->driver_data)->wifi;
        wifi->apmode = false;

        // simulate user selection of a network (2/2: actual connect)
        bool res = mg_wifi_connect(wifi);
        MG_INFO(("Manually connecting: %s", res ? "OK":"FAIL"));
        if (res) {
          state = CONNECTING;
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
        struct mg_wifi_data *wifi = &((struct mg_tcpip_driver_cyw_data *) ifp->driver_data)->wifi;
        bool res = mg_wifi_ap_start(wifi);
        MG_INFO(("Disconnected"));
        MG_INFO(("Manually starting AP: %s", res ? "OK":"FAIL"));
        if (res) {
          state = AP;
          wifi->apmode = true;
        }
      }
      break;
  }
}


static struct mg_tcpip_driver_cyw_data d = {
  {WIFI_SSID, WIFI_PASS, "mongoose", "mongoose", 0, 0, 0, 0, 10, true}, (void *)&sdio, (struct mg_tcpip_driver_cyw_firmware *)&fw, false};

int main(void) {
  hal_init();

  hwspecific_sdio_init();

  d.wifi.apip = MG_IPV4(192, 168, 169, 1),
  d.wifi.apmask = MG_IPV4(255, 255, 255, 0),
  state = d.wifi.apmode ? AP : CONNECTING;

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
