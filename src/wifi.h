#pragma once

#include "arch.h"
#include "log.h"
#include "str.h"

struct mg_wifi_scan_bss_data {
    struct mg_str SSID;
    char *BSSID;
    int16_t RSSI;
    uint8_t security;
#define MG_WIFI_SECURITY_OPEN 0
#define MG_WIFI_SECURITY_WEP  MG_BIT(0)
#define MG_WIFI_SECURITY_WPA  MG_BIT(1)
#define MG_WIFI_SECURITY_WPA2 MG_BIT(2)
#define MG_WIFI_SECURITY_WPA3 MG_BIT(3)
    uint8_t channel;
    unsigned band :2;
#define MG_WIFI_BAND_2G 0
#define MG_WIFI_BAND_5G 1
    unsigned has_n :1;
};


bool mg_wifi_scan(void);
bool mg_wifi_connect(char *ssid, char *pass);
bool mg_wifi_disconnect(void);
bool mg_wifi_ap_start(char *ssid, char *pass, unsigned int channel);
bool mg_wifi_ap_stop(void);
