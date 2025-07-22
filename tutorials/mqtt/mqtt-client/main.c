// Copyright (c) 2023-2025 Cesanta Software Limited
// All rights reserved
//
// Example MQTT client. It performs the following steps:
//    1. Connects to the MQTT server specified by `s_url` variable
//    2. When connected, subscribes to the topic `s_sub_topic`
//    3. When it receives a message, echoes it back to `s_pub_topic`
//    4. Timer-based reconnection logic revives the connection when it is down
//    5. Ping server periodically. When disconnected, a last will is published
//
// To enable SSL/TLS, see https://mongoose.ws/tutorials/tls/#how-to-build

#include "mongoose.h"

static const char *s_url = "mqtt://broker.hivemq.com:1883";
static const char *s_sub_topic = "mg/123/rx";  // Subscribe topic
static const char *s_pub_topic = "mg/123/tx";  // Publish topic
static int s_qos = 1;                          // MQTT QoS
static struct mg_connection *s_mqtt_conn;      // Client connection

static void subscribe(struct mg_connection *c, const char *topic) {
  struct mg_mqtt_opts opts = {};
  memset(&opts, 0, sizeof(opts));
  opts.topic = mg_str(topic);
  opts.qos = s_qos;
  mg_mqtt_sub(c, &opts);
  MG_INFO(("%lu SUBSCRIBED to %s", c->id, topic));
}

static void publish(struct mg_connection *c, const char *topic,
                    const char *message) {
  struct mg_mqtt_opts opts = {};
  memset(&opts, 0, sizeof(opts));
  opts.topic = mg_str(topic);
  opts.message = mg_str(message);
  opts.qos = s_qos;
  mg_mqtt_pub(c, &opts);
  MG_INFO(("%lu PUBLISHED %s -> %s", c->id, topic, message));
}

static void mqtt_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    MG_INFO(("%lu CREATED", c->id));
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    if (c->is_tls) {
      struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
                                 .name = mg_url_host(s_url)};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    MG_INFO(("%lu CONNECTED to %s", c->id, s_url));
    subscribe(c, s_sub_topic);
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    char response[100];
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    mg_snprintf(response, sizeof(response), "Got %.*s -> %.*s", mm->topic.len,
                mm->topic.buf, mm->data.len, mm->data.buf);
    publish(c, s_pub_topic, response);
  } else if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->cmd == MQTT_CMD_PINGREQ) mg_mqtt_pong(c);
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("%lu CLOSED", c->id));
    s_mqtt_conn = NULL;  // Mark that we're closed
  }
}

// Timer function - recreate client connection if it is closed
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  struct mg_mqtt_opts opts = {.clean = true,
                              .qos = s_qos,
                              .topic = mg_str(s_pub_topic),
                              .keepalive = 5,
                              .version = 4,
                              .message = mg_str("bye")};
  if (s_mqtt_conn == NULL) {
    s_mqtt_conn = mg_mqtt_connect(mgr, s_url, &opts, mqtt_ev_handler, NULL);
  } else {
    mg_mqtt_ping(s_mqtt_conn);
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  int i;

  // Parse command-line flags
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-u") == 0 && argv[i + 1] != NULL) {
      s_url = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0 && argv[i + 1] != NULL) {
      s_pub_topic = argv[++i];
    } else if (strcmp(argv[i], "-s") == 0 && argv[i + 1] != NULL) {
      s_sub_topic = argv[++i];
    } else if (strcmp(argv[i], "-v") == 0 && argv[i + 1] != NULL) {
      mg_log_set(atoi(argv[++i]));
    } else {
      MG_ERROR(("Unknown option: %s. Usage:", argv[i]));
      MG_ERROR(
          ("%s [-u mqtts://SERVER:PORT] [-p PUB_TOPIC] [-s SUB_TOPIC] "
           "[-v DEBUG_LEVEL]",
           argv[0], argv[i]));
      return 1;
    }
  }

  mg_mgr_init(&mgr);
  mg_timer_add(&mgr, 3000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
