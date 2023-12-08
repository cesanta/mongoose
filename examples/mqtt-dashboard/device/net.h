// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
#pragma once

#include "mongoose.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_DASHBOARD
#define MQTT_DASHBOARD 1
#endif

#define DEVICE_ID_LEN 10
#define MQTT_KEEP_ALIVE_INTERVAL 60
#define MQTT_SERVER_URL "mqtt://broker.hivemq.com:1883"
#define MQTT_ROOT_TOPIC "topic_mg_device"

extern char *g_url;
extern char *g_device_id;
extern char *g_root_topic;

void web_init(struct mg_mgr *mgr);
void web_destroy();

void hal_gpio_write(int pin, bool status);
bool hal_gpio_read(int pin);
int hal_led_pin(void);

#ifdef __cplusplus
}
#endif
