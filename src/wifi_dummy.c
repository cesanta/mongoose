#include "wifi.h"

#if (!defined(MG_ENABLE_DRIVER_PICO_W) || !MG_ENABLE_DRIVER_PICO_W) && \
    (!defined(MG_ENABLE_DRIVER_CYW) || !MG_ENABLE_DRIVER_CYW) && \
    (!defined(MG_ENABLE_DRIVER_CYW_SDIO) || !MG_ENABLE_DRIVER_CYW_SDIO) && \
    (!defined(MG_ENABLE_DRIVER_NXP_WIFI) || !MG_ENABLE_DRIVER_NXP_WIFI)


bool mg_wifi_scan(void) {
  MG_ERROR(("No Wi-Fi driver enabled"));
  return false;
}

bool mg_wifi_connect(struct mg_wifi_data *wifi) {
  (void) wifi;
  return mg_wifi_scan();
}

bool mg_wifi_disconnect(void) {
  return mg_wifi_scan();
}

bool mg_wifi_ap_start(struct mg_wifi_data *wifi) {
  (void) wifi;
  return mg_wifi_scan();
}

bool mg_wifi_ap_stop(void) {
  return mg_wifi_scan();
}

#endif
