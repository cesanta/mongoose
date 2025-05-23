#include "wifi.h"

#if (!defined(MG_ENABLE_DRIVER_PICO_W) || !MG_ENABLE_DRIVER_PICO_W) && \
    (!defined(MG_ENABLE_DRIVER_CYW) || !MG_ENABLE_DRIVER_CYW) && \
    (!defined(MG_ENABLE_DRIVER_CYW_SDIO) || !MG_ENABLE_DRIVER_CYW_SDIO)


bool mg_wifi_scan(void) {
  MG_ERROR(("No Wi-Fi driver enabled"));
  return false;
}

bool mg_wifi_connect(char *ssid, char *pass) {
  (void) ssid;
  (void) pass;
  return mg_wifi_scan();
}

bool mg_wifi_disconnect(void) {
  return mg_wifi_scan();
}

bool mg_wifi_ap_start(char *ssid, char *pass, unsigned int channel) {
  (void) ssid;
  (void) pass;
  (void) channel;
  return mg_wifi_scan();
}

bool mg_wifi_ap_stop(void) {
  return mg_wifi_scan();
}

#endif
