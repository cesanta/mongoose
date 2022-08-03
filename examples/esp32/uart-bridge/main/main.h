#pragma once

#include "mongoose.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_spiffs.h"
#include "freertos/FreeRTOS.h"

#define FS_ROOT "/spiffs"
#define WIFI_FILE FS_ROOT "/wifi.json"
#define UART_NO 1

void uart_bridge_fn(struct mg_connection *, int, void *, void *);
int uart_read(void *buf, size_t len);
bool wifi_init(const char *ssid, const char *pass);
void cli(uint8_t ch);
void cli_init(void);
