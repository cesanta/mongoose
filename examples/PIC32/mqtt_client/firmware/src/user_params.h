/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef _USER_PARAMS_H
#define _USER_PARAMS_H

#define NET_SSID "my_network"
#define NET_PWD "my_password"
#define NET_SECURITY SL_SEC_TYPE_WPA_WPA2

#define MQTT_BROKER_ADDRESS "192.168.1.108:1883"
#define MQTT_USER_NAME NULL
#define MQTT_USER_PWD NULL

#define CS_PORT PORT_CHANNEL_D
#define CS_PIN PORTS_BIT_POS_14

#define HIB_PORT PORT_CHANNEL_D
#define HIB_PIN PORTS_BIT_POS_4

#endif /* _USER_PARAMS_H */
