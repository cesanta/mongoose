// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example MQTT client. It performs the following steps:
//    1. Connects to the MQTT server specified by `s_url` variable
//    2. When connected, subscribes to the topic `s_topic`
//    3. Publishes message `hello` to the `s_topic`
//    4. Receives that message back from the subscribed topic and exits
//
// To enable SSL/TLS for this client, build it like this:
//    make MBEDTLS_DIR=/path/to/your/mbedtls/installation

#include "mongoose.h"

static const char *s_url = "ws://broker.hivemq.com:8000/mqtt";
static const char *s_topic = "mg/test";

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%p %s", c->fd, (char *) ev_data));
  } else if (ev == MG_EV_CONNECT) {
    // If target URL is SSL/TLS, command client connection to use TLS
    if (mg_url_is_ssl(s_url)) {
      struct mg_tls_opts opts = {.ca = "ca.pem"};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_WS_OPEN) {
    // WS connection established. Perform MQTT login
    MG_INFO(("Connected to WS. Logging in to MQTT..."));
    struct mg_mqtt_opts opts = {.will_qos = 1,
                                .will_topic = mg_str(s_topic),
                                .will_message = mg_str("goodbye")};
    size_t len = c->send.len;
    mg_mqtt_login(c, &opts);
    mg_ws_wrap(c, c->send.len - len, WEBSOCKET_OP_BINARY);
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_mqtt_message mm;
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    MG_INFO(("GOT %d bytes WS msg", (int) wm->data.len));
    while ((mg_mqtt_parse((uint8_t *) wm->data.ptr, wm->data.len, &mm)) == 0) {
      switch (mm.cmd) {
        case MQTT_CMD_CONNACK:
          mg_call(c, MG_EV_MQTT_OPEN, &mm.ack);
          if (mm.ack == 0) {
            struct mg_str topic = mg_str(s_topic), data = mg_str("hello");
            size_t len = c->send.len;
            MG_INFO(("CONNECTED to %s", s_url));
            mg_mqtt_sub(c, topic, 1);
            mg_ws_wrap(c, len, WEBSOCKET_OP_BINARY);
            MG_INFO(("SUBSCRIBED to %.*s", (int) topic.len, topic.ptr));
            mg_mqtt_pub(c, topic, data, 1, false);
            MG_INFO(("PUBSLISHED %.*s -> %.*s", (int) data.len, data.ptr,
                     (int) topic.len, topic.ptr));
            len = mg_ws_wrap(c, c->send.len - len, WEBSOCKET_OP_BINARY);
          } else {
            MG_ERROR(("%lu MQTT auth failed, code %d", c->id, mm.ack));
            c->is_closing = 1;
          }
          break;
        case MQTT_CMD_PUBLISH: {
          MG_DEBUG(("%lu [%.*s] -> [%.*s]", c->id, (int) mm.topic.len,
                    mm.topic.ptr, (int) mm.data.len, mm.data.ptr));
          MG_INFO(("RECEIVED %.*s <- %.*s", (int) mm.data.len, mm.data.ptr,
                   (int) mm.topic.len, mm.topic.ptr));
          c->is_closing = 1;
          break;
        }
      }
      wm->data.ptr += mm.dgram.len;
      wm->data.len -= mm.dgram.len;
    }
  }

  if (ev == MG_EV_ERROR || ev == MG_EV_CLOSE) {
    *(bool *) fn_data = true;  // Signal that we're done
  }
}

int main(void) {
  struct mg_mgr mgr;  // Event manager
  bool done = false;  // Event handler flips it to true when done
  mg_mgr_init(&mgr);  // Initialise event manager
  mg_ws_connect(&mgr, s_url, fn, &done, NULL);    // Create client connection
  while (done == false) mg_mgr_poll(&mgr, 1000);  // Event loop
  mg_mgr_free(&mgr);                              // Finished, cleanup
  return 0;
}
