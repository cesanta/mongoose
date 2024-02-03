#pragma once

struct mg_connection;
typedef void (*mg_event_handler_t)(struct mg_connection *, int ev,
                                   void *ev_data);
void mg_call(struct mg_connection *c, int ev, void *ev_data);
void mg_error(struct mg_connection *c, const char *fmt, ...);

enum {
  MG_EV_ERROR      =  0, // Error                        char *error_message
  MG_EV_OPEN       =  1, // Connection created           NULL
  MG_EV_POLL       =  2, // mg_mgr_poll iteration        uint64_t *uptime_millis
  MG_EV_RESOLVE    =  3, // Host name is resolved        NULL
  MG_EV_CONNECT    =  4, // Connection established       NULL
  MG_EV_ACCEPT     =  5, // Connection accepted          NULL
  MG_EV_TLS_HS     =  6, // TLS handshake succeeded      NULL
  MG_EV_READ       =  7, // Data received from socket    long *bytes_read
  MG_EV_WRITE      =  8, // Data written to socket       long *bytes_written
  MG_EV_CLOSE      =  9, // Connection closed            NULL
  MG_EV_HTTP_MSG   = 10, // HTTP request/response        struct mg_http_message *
  MG_EV_WS_OPEN    = 11, // Websocket handshake done     struct mg_http_message *
  MG_EV_WS_MSG     = 12, // Websocket msg, text or bin   struct mg_ws_message *
  MG_EV_WS_CTL     = 13, // Websocket control msg        struct mg_ws_message *
  MG_EV_MQTT_CMD   = 14, // MQTT low-level command       struct mg_mqtt_message *
  MG_EV_MQTT_MSG   = 15, // MQTT PUBLISH received        struct mg_mqtt_message *
  MG_EV_MQTT_OPEN  = 16, // MQTT CONNACK received        int *connack_status_code
  MG_EV_SNTP_TIME  = 17, // SNTP time received           uint64_t *epoch_millis
  MG_EV_WAKEUP     = 18, // mg_wakeup() data received    struct mg_str *data
  MG_EV_USER       = 19  // Starting ID for user events
};
