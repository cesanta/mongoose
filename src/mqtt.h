#pragma once

#include "net.h"
#include "str.h"

struct mg_mqtt_opts {
  struct mg_str client_id;
  struct mg_str will_topic;
  struct mg_str will_message;
  uint8_t qos;         // Quality of service
  bool will_retain;    // Retain last will
  bool clean;          // Use clean session, 0 or 1
  uint16_t keepalive;  // Keep-alive timer in seconds
};

struct mg_mqtt_message {
  struct mg_str topic;
  struct mg_str data;
};

struct mg_connection *mg_mqtt_connect(struct mg_mgr *, const char *url,
                                      struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data);
void mg_mqtt_pub(struct mg_connection *, struct mg_str *topic,
                 struct mg_str *data);
void mg_mqtt_sub(struct mg_connection *, struct mg_str *topic);
int mg_mqtt_parse(const unsigned char *buf, int len, struct mg_mqtt_message *m);
