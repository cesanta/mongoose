// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
#pragma once

#include "mongoose.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_DASHBOARD 1
#define MQTT_KEEPALIVE_SEC 60
#define MQTT_SERVER_URL "mqtt://broker.hivemq.com:1883"
#define MQTT_ROOT_TOPIC "mg_mqtt_dashboard"
#define NUM_PINS 30

extern char *g_url;
extern char *g_device_id;
extern char *g_root_topic;

void web_init(struct mg_mgr *mgr);
void web_free(void);

bool hal_gpio_write(int pin, bool status);
bool hal_gpio_read(int pin);

#ifdef __cplusplus
}
#endif
