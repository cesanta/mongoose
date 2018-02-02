/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_ESP8266_ESP_SSL_KRYPTON_H_
#define CS_COMMON_PLATFORMS_ESP8266_ESP_SSL_KRYPTON_H_

#include "krypton/krypton.h"

struct mg_connection;

void mg_lwip_ssl_do_hs(struct mg_connection *nc);
void mg_lwip_ssl_send(struct mg_connection *nc);
void mg_lwip_ssl_recv(struct mg_connection *nc);

#endif /* CS_COMMON_PLATFORMS_ESP8266_ESP_SSL_KRYPTON_H_ */
