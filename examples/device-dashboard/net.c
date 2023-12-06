// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "net.h"

char *g_url = MQTT_SERVER_URL;
char *g_device_id;
char *g_root_topic;
static uint8_t s_qos = 1;                 // MQTT QoS
static struct mg_connection *s_conn;  // Client connection
static struct mg_rpc *s_rpc_head = NULL;

struct device_config {
  bool led_status;
  uint8_t led_pin;
  uint8_t brightness;
  uint8_t log_level;
};

static struct device_config s_device_config;

// This is for newlib and TLS (mbedTLS)
uint64_t mg_now(void) {
  return mg_millis();
}

static void generate_device_id(void) {
  char tmp[DEVICE_ID_LEN + 1];
  tmp[DEVICE_ID_LEN] = '\0';
  mg_random_str(tmp, DEVICE_ID_LEN);
  g_device_id = strdup(tmp);
}

static size_t print_fw_status(void (*out)(char, void *), void *ptr,
                              va_list *ap) {
  int fw = va_arg(*ap, int);
  return mg_xprintf(out, ptr, "{%m:%d,%m:%c%lx%c,%m:%u,%m:%u}",
                    MG_ESC("status"), mg_ota_status(fw), MG_ESC("crc32"), '"',
                    mg_ota_crc32(fw), '"', MG_ESC("size"), mg_ota_size(fw),
                    MG_ESC("timestamp"), mg_ota_timestamp(fw));
}

static void publish_status(struct mg_connection *c) {
  int status_topic_len = 50;
  char *status_topic = calloc(status_topic_len, sizeof(char));
  if (!status_topic) {
    MG_ERROR(("Out of memory"));
    return;
  }
  mg_snprintf(status_topic, status_topic_len, "%s/%s/status", g_root_topic,
              g_device_id);
  struct mg_str pubt = mg_str(status_topic);
  struct mg_mqtt_opts pub_opts;
  memset(&pub_opts, 0, sizeof(pub_opts));
  pub_opts.topic = pubt;
  int json_len = 400;
  char *device_status_json;
  device_status_json = calloc(json_len, sizeof(char));
  if (!device_status_json) {
    MG_ERROR(("Out of memory"));
    return;
  }
  s_device_config.led_status = gpio_read(s_device_config.led_pin);
  mg_snprintf(device_status_json, json_len,
              "{%m:%m,%m:{%m:%m,%m:%s,%m:%hhu,%m:%hhu,%m:%hhu,%m:%M,%m:%M}}",
              MG_ESC("method"), MG_ESC("status.notify"), MG_ESC("params"),
              MG_ESC("status"), MG_ESC("online"), MG_ESC("led_status"),
              s_device_config.led_status ? "true" : "false", MG_ESC("led_pin"),
              s_device_config.led_pin, MG_ESC("brightness"),
              s_device_config.brightness, MG_ESC(("log_level")),
              s_device_config.log_level, MG_ESC(("crnt_fw")), print_fw_status,
              MG_FIRMWARE_CURRENT, MG_ESC(("prev_fw")), print_fw_status,
              MG_FIRMWARE_PREVIOUS);
  struct mg_str data = mg_str(device_status_json);
  pub_opts.message = data;
  pub_opts.qos = s_qos, pub_opts.retain = true;
  mg_mqtt_pub(c, &pub_opts);
  MG_INFO(("%lu PUBLISHED %.*s -> %.*s", c->id, (int) data.len, data.ptr,
           (int) pubt.len, pubt.ptr));
  free(device_status_json);
  free(status_topic);
}

static void publish_response(struct mg_connection *c, char *buf, size_t len) {
  int tx_topic_len = 50;
  char *tx_topic = calloc(tx_topic_len, sizeof(char));
  if (!tx_topic) {
    MG_ERROR(("Out of memory"));
    return;
  }
  mg_snprintf(tx_topic, tx_topic_len, "%s/%s/tx", g_root_topic, g_device_id);
  struct mg_str pubt = mg_str(tx_topic);
  struct mg_mqtt_opts pub_opts;
  memset(&pub_opts, 0, sizeof(pub_opts));
  pub_opts.topic = pubt;
  struct mg_str data = mg_str_n(buf, len);
  pub_opts.message = data;
  pub_opts.qos = s_qos;
  mg_mqtt_pub(c, &pub_opts);
  MG_INFO(("%lu PUBLISHED %.*s -> %.*s", c->id, (int) data.len, data.ptr,
           (int) pubt.len, pubt.ptr));
  free(tx_topic);
}

static void subscribe(struct mg_connection *c) {
  int rx_topic_len = 50;
  char *rx_topic = calloc(rx_topic_len, sizeof(char));
  if (!rx_topic) {
    MG_ERROR(("Out of memory"));
    return;
  }
  mg_snprintf(rx_topic, rx_topic_len, "%s/%s/rx", g_root_topic, g_device_id);
  struct mg_str subt = mg_str(rx_topic);
  struct mg_mqtt_opts sub_opts;
  memset(&sub_opts, 0, sizeof(sub_opts));
  sub_opts.topic = subt;
  sub_opts.qos = s_qos;
  mg_mqtt_sub(c, &sub_opts);
  MG_INFO(("%lu SUBSCRIBED to %.*s", c->id, (int) subt.len, subt.ptr));
  free(rx_topic);
}

static void rpc_config_set(struct mg_rpc_req *r) {
  bool tmp_status, ok;
  int8_t tmp_brightness, tmp_level, tmp_pin;

  ok = mg_json_get_bool(r->frame, "$.params.led_status", &tmp_status);
  if (ok) s_device_config.led_status = tmp_status;

  tmp_brightness = (int8_t) mg_json_get_long(r->frame, "$.params.brightness", -1);
  if (tmp_brightness >= 0) s_device_config.brightness = tmp_brightness;

  tmp_level = (int8_t) mg_json_get_long(r->frame, "$.params.log_level", -1);
  if (tmp_level >= 0) {
    s_device_config.log_level = tmp_level;
    mg_log_set(s_device_config.log_level);
  }

  tmp_pin = (int8_t) mg_json_get_long(r->frame, "$.params.led_pin", -1);
  if (tmp_pin > 0) s_device_config.led_pin = tmp_pin;

  if (tmp_pin > 0 && ok) {
    gpio_write(s_device_config.led_pin, s_device_config.led_status);
  }

  mg_rpc_ok(r, "%m", MG_ESC("ok"));
}

static void rpc_ota_commit(struct mg_rpc_req *r) {
  if (mg_ota_commit()) {
    mg_rpc_ok(r, "%m", MG_ESC("ok"));
  } else {
    mg_rpc_err(r, 1, "Failed to commit the firmware");
  }
}

static void rpc_device_reset(struct mg_rpc_req *r) {
  mg_rpc_ok(r, "%m", MG_ESC("ok"));
  mg_timer_add(s_conn->mgr, 500, 0, (void (*)(void *)) mg_device_reset, NULL);
}

static void rpc_ota_rollback(struct mg_rpc_req *r) {
  if (mg_ota_rollback()) {
    mg_rpc_ok(r, "%m", MG_ESC("ok"));
  } else {
    mg_rpc_err(r, 1, "Failed to rollback to the previous firmware");
  }
}

static void rpc_ota_upload(struct mg_rpc_req *r) {
  long ofs = mg_json_get_long(r->frame, "$.params.offset", -1);
  long tot = mg_json_get_long(r->frame, "$.params.total", -1);
  int len;
  char *file_chunk = mg_json_get_b64(r->frame, "$.params.chunk", &len);
  if (!file_chunk) {
    mg_rpc_err(r, 1, "Error processing the binary chunk.");
    return;
  }
  struct mg_str data = mg_str_n(file_chunk, len);
  if (ofs < 0 || tot < 0) {
    mg_rpc_err(r, 1, "offset and total not set");
  } else if (ofs == 0 && mg_ota_begin((size_t) tot) == false) {
    mg_rpc_err(r, 1, "mg_ota_begin(%ld) failed\n", tot);
  } else if (data.len > 0 && mg_ota_write(data.ptr, data.len) == false) {
    mg_rpc_err(r, 1, "mg_ota_write(%lu) @%ld failed\n", data.len, ofs);
    mg_ota_end();
  } else if (data.len == 0 && mg_ota_end() == false) {
    mg_rpc_err(r, 1, "mg_ota_end() failed\n", tot);
  } else {
    mg_rpc_ok(r, "%m", MG_ESC("ok"));
    if (data.len == 0) {
      // Successful mg_ota_end() called, schedule device reboot
      mg_timer_add(s_conn->mgr, 500, 0, (void (*)(void *)) mg_device_reset,
                   NULL);
    }
  }
  free(file_chunk);
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    MG_INFO(("%lu CREATED", c->id));
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    MG_INFO(("Device ID is connected %s", g_device_id));
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    MG_INFO(("%lu CONNECTED to %s", c->id, g_url));
    subscribe(c);
    publish_status(c);
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->data.len < 1024) {
      MG_INFO(("%lu RECEIVED %.*s <- %.*s", c->id, (int) mm->data.len,
               mm->data.ptr, (int) mm->topic.len, mm->topic.ptr));
    }
    struct mg_iobuf io = {0, 0, 0, 512};
    struct mg_rpc_req r = {&s_rpc_head, 0, mg_pfn_iobuf, &io, 0, mm->data};
    mg_rpc_process(&r);
    if (io.buf) {
      publish_response(c, (char *) io.buf, io.len);
      publish_status(c);
    }
    mg_iobuf_free(&io);
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("%lu CLOSED", c->id));
    s_conn = NULL;  // Mark that we're closed
  }
  (void) fn_data;
}

// Timer function - recreate client connection if it is closed
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  char status_topic[50];
  memset(status_topic, 0, sizeof(status_topic));
  mg_snprintf(status_topic, sizeof(status_topic), "%s/%s/status", g_root_topic,
              g_device_id);
  char msg[200];
  memset(msg, 0, sizeof(msg));
  mg_snprintf(msg, sizeof(msg), "{%m:%m,%m:{%m:%m}}", MG_ESC("method"),
              MG_ESC("status.notify"), MG_ESC("params"), MG_ESC("status"),
              MG_ESC("offline"));

  struct mg_mqtt_opts opts = {.clean = true,
                              .qos = s_qos,
                              .topic = mg_str(status_topic),
                              .version = 4,
                              .keepalive = KEEP_ALIVE_INTERVAL,
                              .retain = true,
                              .message = mg_str(msg)};
  if (s_conn == NULL) s_conn = mg_mqtt_connect(mgr, g_url, &opts, fn, NULL);
}

static void timer_keepalive(void *arg) {
  mg_mqtt_send_header(s_conn, MQTT_CMD_PINGREQ, 0, 0);
  (void) arg;
}

void web_init(struct mg_mgr *mgr) {
  int pingreq_interval_ms = KEEP_ALIVE_INTERVAL * 1000 - 500;
  if (!g_device_id) generate_device_id();
  if (!g_root_topic) g_root_topic = DEFAULT_ROOT_TOPIC;
  s_device_config.log_level = (uint8_t) mg_log_level;

  // Configure JSON-RPC functions we're going to handle
  mg_rpc_add(&s_rpc_head, mg_str("config.set"), rpc_config_set, NULL);
  mg_rpc_add(&s_rpc_head, mg_str("ota.commit"), rpc_ota_commit, NULL);
  mg_rpc_add(&s_rpc_head, mg_str("device.reset"), rpc_device_reset, NULL);
  mg_rpc_add(&s_rpc_head, mg_str("ota.rollback"), rpc_ota_rollback, NULL);
  mg_rpc_add(&s_rpc_head, mg_str("ota.upload"), rpc_ota_upload, NULL);

  mg_timer_add(mgr, 3000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, mgr);
  mg_timer_add(mgr, pingreq_interval_ms, MG_TIMER_REPEAT, timer_keepalive,
               mgr);
}

void web_destroy() {
  mg_rpc_del(&s_rpc_head, NULL);    // Deallocate RPC handlers
  free(g_device_id);
}
