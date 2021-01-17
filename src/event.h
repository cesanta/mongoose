#pragma once

struct mg_connection;
typedef void (*mg_event_handler_t)(struct mg_connection *, int ev,
                                   void *ev_data, void *fn_data);
#if 0
struct mg_fn {
  struct mg_fn *next;
  mg_event_handler_t fn;
  void *fn_data;
};

void mg_fn_add(struct mg_connection *c, mg_event_handler_t fn, void *fn_data);
void mg_fn_del(struct mg_connection *c, mg_event_handler_t fn);
#endif

void mg_call(struct mg_connection *c, int ev, void *ev_data);
void mg_error(struct mg_connection *c, const char *fmt, ...);

enum {
  MG_EV_ERROR,      // Error                        char *error_message
  MG_EV_POLL,       // mg_mgr_poll iteration        unsigned long *millis
  MG_EV_RESOLVE,    // Host name is resolved        NULL
  MG_EV_CONNECT,    // Connection established       NULL
  MG_EV_ACCEPT,     // Connection accepted          NULL
  MG_EV_READ,       // Data received from socket    struct mg_str *
  MG_EV_WRITE,      // Data written to socket       int *num_bytes_written
  MG_EV_CLOSE,      // Connection closed            NULL
  MG_EV_HTTP_MSG,   // HTTP request/response        struct mg_http_message *
  MG_EV_WS_OPEN,    // Websocket handshake done     struct mg_http_message *
  MG_EV_WS_MSG,     // Websocket msg, text or bin   struct mg_ws_message *
  MG_EV_WS_CTL,     // Websocket control msg        struct mg_ws_message *
  MG_EV_MQTT_CMD,   // MQTT low-level command       struct mg_mqtt_message *
  MG_EV_MQTT_MSG,   // MQTT PUBLISH received        struct mg_mqtt_message *
  MG_EV_MQTT_OPEN,  // MQTT CONNACK received        int *connack_status_code
  MG_EV_SNTP_TIME,  // SNTP time received           struct timeval *
  MG_EV_USER,       // Starting ID for user events
};
