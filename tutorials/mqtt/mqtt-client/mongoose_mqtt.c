// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

#define MQTT_SERVER_URL "mqtt://broker.hivemq.com:1883"
#define MQTT_CLIENT_ID "d3"
#define MQTT_USER MQTT_CLIENT_ID
#define MQTT_PASS ""
#define MQTT_PUBLISH_TOPIC "mg/" MQTT_CLIENT_ID "/tx"
#define MQTT_SUBSCRIBE_TOPIC "mg/" MQTT_CLIENT_ID "/rx"
#define MQTT_QOS 1
#define MQTT_RECONNECT_PERIOD_MS 3000

#define TLS_CA ""
#define TLS_KEY ""
#define TLS_CRT ""

static struct mg_connection *s_mqtt_conn;  // Client connection

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
      struct mg_tls_opts opts = {.ca = mg_str(TLS_CA),
                                 .cert = mg_str(TLS_CRT),
                                 .key = mg_str(TLS_KEY),
                                 .name = mg_url_host(MQTT_SERVER_URL)};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
  } else if (ev == MG_EV_MQTT_OPEN) {
    int status = *(int *) ev_data;
    MG_DEBUG(("%lu CONNECT status: %d", c->id, status));
    if (status == 0) {
      subscribe(c, mg_str(MQTT_SUBSCRIBE_TOPIC));
      if (mg_match(mg_str(MQTT_SERVER_URL), mg_str("#azure-devices.net"), 0)) {
        // This is Azure IoT Hub. Subscribe for DPS messages
        subscribe(c, mg_str("$iothub/methods/POST/#"));
      }
    }
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    struct mg_str caps[5];  // caps[0] = method name, caps[2] = request id
    if (mg_match(mm->topic, mg_str("$iothub/methods/POST/*/?$rid=*"), caps)) {
      // Azure direct method call. Construct a stub response, "{}"
      char topic[128];
      mg_snprintf(topic, sizeof(topic), "$iothub/methods/res/%d/?$rid=%.*s",
                  200, (int) caps[2].len, caps[2].buf);
      publish(c, mg_str(topic), mg_str("{}"));
    } else {
      char response[100];
      mg_snprintf(response, sizeof(response), "Got %.*s -> %.*s", mm->topic.len,
                  mm->topic.buf, mm->data.len, mm->data.buf);
      publish(c, mg_str(MQTT_PUBLISH_TOPIC), mg_str(response));
    }
  } else if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->cmd == MQTT_CMD_PINGREQ) mg_mqtt_pong(c);
  } else if (ev == MG_EV_CLOSE) {
    MG_ERROR(("%lu CLOSED", c->id));
    s_mqtt_conn = NULL;  // Mark that we're closed
  }
}

void mg_mqtt_init(struct mg_mgr *mgr) {
  mg_rpc_add(&mgr->rpcs, mg_str("ota.update"), rpc_ota_update, NULL);
}

void mg_mqtt_poll(struct mg_mgr *mgr) {
  static uint64_t timer = 1;  // 1 triggers expiration on first poll

  // Reconnect if connection is closed, and send MQTT PINGs to keep
  // the connection alive or to detect connection loss
  if (mg_timer_expired(&timer, MQTT_RECONNECT_PERIOD_MS, mg_now())) {
    if (s_mqtt_conn == NULL) {
      struct mg_mqtt_opts opts = {
          .clean = true,
          .client_id = mg_str(MQTT_CLIENT_ID),
          .user = mg_str(MQTT_USER),
          .pass = mg_str(MQTT_PASS),
          .qos = MQTT_QOS,
          .keepalive = 5,
          .version = 4,  // MQTT 3.1.1
          .topic = mg_str(MQTT_PUBLISH_TOPIC),
          .message = mg_str("bye"),
      };
      s_mqtt_conn =
          mg_mqtt_connect(mgr, MQTT_SERVER_URL, &opts, mqtt_ev_handler, NULL);
    } else {
      mg_mqtt_ping(s_mqtt_conn);
      // publish(s_mqtt_conn, mg_str(MQTT_PUBLISH_TOPIC), mg_str("hi"));
    }
  }
}
