// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
#pragma once

#include "mongoose.h"
#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_DASHBOARD 1
#define MQTT_KEEPALIVE_SEC 60
#define MQTT_SERVER_URL "mqtt://broker.hivemq.com:1883"
#define MQTT_ROOT_TOPIC "mg_mqtt_dashboard"

extern char *g_mqtt_server_url;
extern char *g_device_id;
extern char *g_root_topic;

void web_init(struct mg_mgr *mgr);
void web_free(void);

#ifdef __cplusplus
}
#endif
