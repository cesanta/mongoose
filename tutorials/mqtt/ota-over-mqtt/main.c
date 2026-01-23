// Copyright (c) 2013-2026 Cesanta Software Limited
// All rights reserved
//
// This example implements a JSON-RPC server that connects to the MQTT server
// and expects requests on MQTT_RX_TOPIC, and sends responses to the
// MQTT_TX_TOPIC. Only a single RPC method is implemented, "ota.update",
// which expects parameters {"total": XX, "offset": XX, "chunk": "..."}
// The rpc_ota_update() function handles those requests and calls Mongoose
// ota_begin(), ota_write(), ota_end() respectively.
//
// Visit https://mongoose.ws/mqtt/ web page to push the firmware binary.

#include "mongoose.h"

#define MQTT_SERVER_URL "mqtt://broker.hivemq.com:1883"
#define MQTT_RX_TOPIC "mg/123/rx"
#define MQTT_TX_TOPIC "mg/123/tx"

static uint8_t s_qos = 1;             // MQTT QoS
static struct mg_connection *s_conn;  // MQTT Client connection
static struct mg_rpc *s_rpc = NULL;   // List of registered RPC methods

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

static void subscribe(struct mg_connection *c, struct mg_str topic) {
  struct mg_mqtt_opts opts = {};
  memset(&opts, 0, sizeof(opts));
  opts.topic = topic;
  opts.qos = s_qos;
  mg_mqtt_sub(c, &opts);
  MG_INFO(("%lu SUBSCRIBED to %.*s", c->id, topic.len, topic.buf));
}

static void publish(struct mg_connection *c, struct mg_str topic,
                    struct mg_str message) {
  struct mg_mqtt_opts opts = {};
  memset(&opts, 0, sizeof(opts));
  opts.topic = topic;
  opts.message = message;
  opts.qos = s_qos;
  mg_mqtt_pub(c, &opts);
  MG_INFO(("%lu PUBLISHED %.*s -> %.*s", c->id, topic.len, topic.buf,
           message.len, message.buf));
}

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    MG_INFO(("%lu CREATED", c->id));
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    MG_INFO(("Connected"));
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    MG_INFO(("%lu CONNECTED", c->id));
    subscribe(c, mg_str(MQTT_RX_TOPIC));
  } else if (ev == MG_EV_MQTT_MSG) {
    // Treat this message as JSON-RPC: process an RPC request
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    struct mg_iobuf io = {0, 0, 0, 512};
    struct mg_rpc_req r = {&s_rpc, NULL, mg_pfn_iobuf,
                           &io,    NULL, {mm->data.buf, mm->data.len}};
    size_t clipped_len = mm->data.len > 512 ? 512 : mm->data.len;
    MG_INFO(("%lu RECEIVED %.*s <- %.*s", c->id, clipped_len, mm->data.buf,
             mm->topic.len, mm->topic.buf));
    mg_rpc_process(&r);
    if (io.buf != NULL && io.len > 0) {
      publish(c, mg_str(MQTT_TX_TOPIC), mg_str_n((char *) io.buf, io.len));
    }
    mg_iobuf_free(&io);
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("%lu CLOSED", c->id));
    s_conn = NULL;  // Mark that we're closed
  }
}

// SNTP timer function. Sync up time - may be needed for TLS on embedded
static void timer_sntp(void *param) {
  static uint64_t hourly_timer = 0;
  uint64_t t1 = mg_now(), t2 = mg_millis();
  if (t1 < t2 + 3600 || mg_timer_expired(&hourly_timer, 3600000, t2)) {
    mg_sntp_connect(param, "udp://time.google.com:123", NULL, NULL);
  }
}

static void timer_fn(void *arg) {
  if (s_conn == NULL) {
    struct mg_mgr *mgr = (struct mg_mgr *) arg;
    struct mg_mqtt_opts opts = {.clean = true,
                                .qos = s_qos,
                                .topic = mg_str(MQTT_TX_TOPIC),
                                .keepalive = 5,
                                .version = 4,
                                .message = mg_str("{\"method\":\"bye\"}")};
    s_conn = mg_mqtt_connect(mgr, MQTT_SERVER_URL, &opts, ev_handler, NULL);
  } else {
    mg_mqtt_ping(s_conn);
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_log_set(MG_LL_INFO);
  mg_mgr_init(&mgr);

  mg_timer_add(&mgr, 3000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);
  mg_timer_add(&mgr, 100, MG_TIMER_REPEAT, timer_sntp, &mgr);

  mg_rpc_add(&s_rpc, mg_str("ota.update"), rpc_ota_update, NULL);

  for (;;) mg_mgr_poll(&mgr, 100);
  mg_mgr_free(&mgr);

  return 0;
}
