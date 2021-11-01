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

static const char *s_url = "mqtt://broker.hivemq.com:1883";
static const char *s_topic = "mg/mq-clnt-test";
static int s_qos = 1;

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_ERROR) {
    // On error, log error message
    LOG(LL_ERROR, ("%p %s", c->fd, (char *) ev_data));
  } else if (ev == MG_EV_CONNECT) {
    // If target URL is SSL/TLS, command client connection to use TLS
    if (mg_url_is_ssl(s_url)) {
      struct mg_tls_opts opts = {.ca = "ca.pem"};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    struct mg_str topic = mg_str(s_topic), data = mg_str("hello");
    LOG(LL_INFO, ("CONNECTED to %s", s_url));
    mg_mqtt_sub(c, &topic, 1);
    LOG(LL_INFO, ("SUBSCRIBED to %.*s", (int) topic.len, topic.ptr));
    mg_mqtt_pub(c, &topic, &data, 1, false);
    LOG(LL_INFO, ("PUBSLISHED %.*s -> %.*s", (int) data.len, data.ptr,
                  (int) topic.len, topic.ptr));
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    LOG(LL_INFO, ("RECEIVED %.*s <- %.*s", (int) mm->data.len, mm->data.ptr,
                  (int) mm->topic.len, mm->topic.ptr));
  }

  if (ev == MG_EV_ERROR || ev == MG_EV_CLOSE || ev == MG_EV_MQTT_MSG) {
    *(bool *) fn_data = true;  // Signal that we're done
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_mqtt_opts opts = {.qos = s_qos,
                              .will_topic = mg_str(s_topic),
                              .will_message = mg_str("goodbye")};
  bool done = false;  // Event handler sets this to true

  mg_mgr_init(&mgr);                               // Initialise event manager
  mg_mqtt_connect(&mgr, s_url, &opts, fn, &done);  // Create client connection
  while (done == false) mg_mgr_poll(&mgr, 1000);   // Loop until done
  mg_mgr_free(&mgr);                               // Finished, cleanup

  return 0;
}
