// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Example MQTT client. It performs the following steps:
//    1. Connects to the MQTT server specified by MQTT_SERVER_URL
//    2. When connected, subscribes to the topic MQTT_SUBSCRIBE_TOPIC
//    3. When it receives a message, echoes it back to MQTT_PUBLISH_TOPIC
//    4. Timer-based reconnection logic revives the connection when it is down
//    5. Ping server periodically. When disconnected, a last will is published
//    6. Implements "ota.update" for OTA updates, see https://mongoose.ws/mqtt/

#include "mongoose.h"

#define MQTT_SERVER_URL "mqtt://broker.hivemq.com:1883"
#define MQTT_PUBLISH_TOPIC "mg/123/tx"
#define MQTT_SUBSCRIBE_TOPIC "mg/123/rx"
#define MQTT_QOS 1
#define RECONNECT_PERIOD_MS 3000

static struct mg_connection *s_mqtt_conn;  // Client connection
static struct mg_rpc *s_rpc = NULL;        // List of registered RPC methods

static void subscribe(struct mg_connection *c, struct mg_str topic) {
  struct mg_mqtt_opts opts = {};
  memset(&opts, 0, sizeof(opts));
  opts.topic = topic;
  opts.qos = MQTT_QOS;
  mg_mqtt_sub(c, &opts);
  MG_DEBUG(("%lu SUBSCRIBED to %.*s", c->id, topic.len, topic.buf));
}

static void publish(struct mg_connection *c, struct mg_str topic,
                    struct mg_str message) {
  struct mg_mqtt_opts opts = {};
  memset(&opts, 0, sizeof(opts));
  opts.topic = topic;
  opts.message = message;
  opts.qos = MQTT_QOS;
  mg_mqtt_pub(c, &opts);
  MG_DEBUG(("%lu PUBLISHED %.*s -> %.*s", c->id, topic.len, topic.buf,
            message.len, message.buf));
}

static void rpc_ota_update(struct mg_rpc_req *r) {
  long ofs = mg_json_get_long(r->frame, "$.params.offset", -1);
  long tot = mg_json_get_long(r->frame, "$.params.total", -1);
  int len = 0;
  char *buf = mg_json_get_b64(r->frame, "$.params.chunk", &len);
  if (buf == NULL) {
    mg_rpc_err(r, 1, "%m", MG_ESC("Chunk decoding error"));
  } else if (ofs < 0 || tot < 0) {
    mg_rpc_err(r, 1, "%m", MG_ESC("offset and total not set"));
  } else if (ofs == 0 && mg_ota_begin((size_t) tot) == false) {
    mg_rpc_err(r, 1, "\"mg_ota_begin(%ld) failed\"", tot);
  } else if (len > 0 && mg_ota_write(buf, len) == false) {
    mg_rpc_err(r, 1, "\"mg_ota_write(%lu) @%ld failed\"", len, ofs);
    mg_ota_end();
  } else if (len == 0 && mg_ota_end() == false) {
    mg_rpc_err(r, 1, "\"mg_ota_end() failed\"", tot);
  } else {
    mg_rpc_ok(r, "%m", MG_ESC("ok"));
  }
  mg_free(buf);
}

static void mqtt_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    MG_DEBUG(("%lu CREATED, %s", c->id, ev_data));
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    if (c->is_tls) {
      struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
                                 .name = mg_url_host(MQTT_SERVER_URL)};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    MG_DEBUG(("%lu CONNECTED to %s", c->id, MQTT_SERVER_URL));
    subscribe(c, mg_str(MQTT_SUBSCRIBE_TOPIC));
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    char response[100];
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    mg_snprintf(response, sizeof(response), "Got %.*s -> %.*s", mm->topic.len,
                mm->topic.buf, mm->data.len, mm->data.buf);
    publish(c, mg_str(MQTT_PUBLISH_TOPIC), mg_str(response));
  } else if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->cmd == MQTT_CMD_PINGREQ) mg_mqtt_pong(c);
  } else if (ev == MG_EV_CLOSE) {
    MG_ERROR(("%lu CLOSED", c->id));
    s_mqtt_conn = NULL;  // Mark that we're closed
  }
}

void mg_mqtt_init(struct mg_mgr *mgr) {
  (void) mgr;
  if (!s_rpc) mg_rpc_add(&s_rpc, mg_str("ota.update"), rpc_ota_update, NULL);
}

void mg_mqtt_poll(struct mg_mgr *mgr) {
  static uint64_t timer = 1;  // 1 triggers expiration on first poll

  // Reconnect if connection is closed, and send MQTT PINGs to keep
  // the connection alive or to detect connection loss
  if (mg_timer_expired(&timer, RECONNECT_PERIOD_MS, mg_now())) {
    if (s_mqtt_conn == NULL) {
      struct mg_mqtt_opts opts = {.clean = true,
                                  .qos = MQTT_QOS,
                                  .topic = mg_str(MQTT_PUBLISH_TOPIC),
                                  .keepalive = 5,
                                  .version = 4,
                                  .message = mg_str("bye")};
      s_mqtt_conn =
          mg_mqtt_connect(mgr, MQTT_SERVER_URL, &opts, mqtt_ev_handler, NULL);
    } else {
      mg_mqtt_ping(s_mqtt_conn);
    }
  }
}
