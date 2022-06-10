// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example MQTT client. It performs the following steps:
//    1. Connects to the MQTT server specified by `s_url` variable
//    2. When connected, subscribes to the topic `s_sub_topic`
//    3. Publishes message `hello` to the `s_pub_topic`
//    4. Receives that message back from the subscribed topic and closes
//    5. Timer-based reconnection logic revives the connection when it is down
//
// To enable SSL/TLS for this client, build it like this:
//    make MBEDTLS_DIR=/path/to/your/mbedtls/installation

#include "mongoose.h"

static const char *s_url = "mqtt://broker.hivemq.com:1883";
static const char *s_sub_topic = "mg/+/test";
static const char *s_pub_topic = "mg/clnt/test";
static int s_qos = 1;
static struct mg_connection *s_conn;

// Handle interrupts, like Ctrl-C
static int s_signo;
static void signal_handler(int signo) {
  s_signo = signo;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    MG_INFO(("CREATED"));
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%p %s", c->fd, (char *) ev_data));
  } else if (ev == MG_EV_CONNECT) {
    // If target URL is SSL/TLS, command client connection to use TLS
    if (mg_url_is_ssl(s_url)) {
      struct mg_tls_opts opts = {.ca = "ca.pem"};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    struct mg_str subt = mg_str(s_sub_topic);
    struct mg_str pubt = mg_str(s_pub_topic), data = mg_str("hello");
    MG_INFO(("CONNECTED to %s", s_url));
    mg_mqtt_sub(c, subt, s_qos);
    MG_INFO(("SUBSCRIBED to %.*s", (int) subt.len, subt.ptr));

    mg_mqtt_pub(c, pubt, data, s_qos, false);
    MG_INFO(("PUBLISHED %.*s -> %.*s", (int) data.len, data.ptr,
             (int) pubt.len, pubt.ptr));
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_INFO(("RECEIVED %.*s <- %.*s", (int) mm->data.len, mm->data.ptr,
             (int) mm->topic.len, mm->topic.ptr));
    c->is_closing = 1;
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("CLOSED"));
    s_conn = NULL;  // Mark that we're closed
  }
  (void) fn_data;
}

// Timer function - recreate client connection if it is closed
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  struct mg_mqtt_opts opts = {.clean = true,
                              .will_qos = s_qos,
                              .will_topic = mg_str(s_pub_topic),
                              .will_message = mg_str("goodbye")};
  if (s_conn == NULL) s_conn = mg_mqtt_connect(mgr, s_url, &opts, fn, NULL);
}

int main(void) {
  struct mg_mgr mgr;
  int topts = MG_TIMER_REPEAT | MG_TIMER_RUN_NOW;

  signal(SIGINT, signal_handler);   // Setup signal handlers - exist event
  signal(SIGTERM, signal_handler);  // manager loop on SIGINT and SIGTERM

  mg_mgr_init(&mgr);                                // Init event manager
  mg_timer_add(&mgr, 3000, topts, timer_fn, &mgr);  // Init timer
  while (s_signo == 0) mg_mgr_poll(&mgr, 1000);     // Event loop, 1s timeout
  mg_mgr_free(&mgr);                                // Finished, cleanup

  return 0;
}
