#pragma once

struct mg_connection;

// User-supplied event handler. ev is enum mg_event; ev_data type depends on
// the event (see enum below). c->fn_data is the user pointer passed to
// mg_listen(), mg_connect(), mg_*_listen(), mg_*_connect().
typedef void (*mg_event_handler_t)(struct mg_connection *, int ev,
                                   void *ev_data);

// Fires ev on connection c, invoking c->fn and c->pfn with ev_data.
void mg_call(struct mg_connection *c, int ev, void *ev_data);

// Sets c->is_closing and fires MG_EV_ERROR with a printf-formatted message.
void mg_error(struct mg_connection *c, const char *fmt, ...);

// Event codes passed to mg_event_handler_t. Each entry shows the type of
// ev_data cast to use inside the handler, e.g.: char *msg = (char *) ev_data;
enum mg_event {
  // Event          Meaning                         ev_data type
  MG_EV_ERROR,      // Error                        char *error_message
  MG_EV_OPEN,       // Connection created           NULL
  MG_EV_POLL,       // mg_mgr_poll iteration        uint64_t *uptime_millis
  MG_EV_RESOLVE,    // Host name is resolved        NULL
  MG_EV_CONNECT,    // Connection established       NULL
  MG_EV_ACCEPT,     // Connection accepted          NULL
  MG_EV_TLS_HS,     // TLS handshake succeeded      NULL
  MG_EV_READ,       // Data received from socket    long *bytes_read
  MG_EV_WRITE,      // Data written to socket       long *bytes_written
  MG_EV_CLOSE,      // Connection closed            NULL
  MG_EV_HTTP_HDRS,  // HTTP headers                 struct mg_http_message *
  MG_EV_HTTP_MSG,   // Full HTTP request/response   struct mg_http_message *
  MG_EV_WS_OPEN,    // Websocket handshake done     struct mg_http_message *
  MG_EV_WS_MSG,     // Websocket msg, text or bin   struct mg_ws_message *
  MG_EV_WS_CTL,     // Websocket control msg        struct mg_ws_message *
  MG_EV_MQTT_CMD,   // MQTT low-level command       struct mg_mqtt_message *
  MG_EV_MQTT_MSG,   // MQTT PUBLISH received        struct mg_mqtt_message *
  MG_EV_MQTT_OPEN,  // MQTT CONNACK received        int *connack_status_code
  MG_EV_SNTP_TIME,  // SNTP time received           uint64_t *epoch_millis
  MG_EV_WAKEUP,     // mg_wakeup() data received    struct mg_str *data
  MG_EV_MDNS_REQ,   // mDNS request                 struct mg_mdns_req *
  MG_EV_MDNS_RESP,  // mDNS response                struct mg_mdns_resp *
  MG_EV_MODBUS_REQ, // Modbus TCP request            struct mg_modbus_req *
  MG_EV_USER        // Starting ID for user events
};
