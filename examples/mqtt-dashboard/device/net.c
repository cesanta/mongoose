// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "net.h"

char *g_mqtt_server_url = MQTT_SERVER_URL;
char *g_root_topic = MQTT_ROOT_TOPIC;
char *g_device_id;

static uint8_t s_qos = 1;             // MQTT QoS
static struct mg_connection *s_conn;  // MQTT Client connection
static struct mg_rpc *s_rpc = NULL;   // List of registered RPC methods

#define MAX_PINS 20
struct device_config {
  int log_level;               // Device logging level, 0-4
  int pin_count;               // Number of pins to handle
  uint16_t pin_map[MAX_PINS];  // Pins to handle
  bool pin_state[MAX_PINS];    // State of the GPIO pins
};

static struct device_config s_device_config;

// Device ID generation function. Create an ID that is unique
// for a given device, and does not change between device restarts.
static void set_device_id(void) {
  char buf[15] = "";

#ifdef _WIN32
  unsigned long serial = 0;
  if (GetVolumeInformationA("c:\\", NULL, 0, &serial, NULL, NULL, NULL, 0)) {
    mg_snprintf(buf, sizeof(buf), "%lx", serial);
  }
#elif defined(__APPLE__)
  FILE *fp = popen(
      "ioreg -l | grep IOPlatformSerialNumber | cut -d'\"' -f4 | tr -d $'\n'",
      "r");
  if (fp != NULL) {
    fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
  }
#elif defined(__linux__)
  struct mg_str id = mg_file_read(&mg_fs_posix, "/etc/machine-id");
  if (id.ptr != NULL) {
    mg_snprintf(buf, sizeof(buf), "%s", id.ptr);
    free((void *) id.ptr);
  }
#endif

  if (buf[0] == '\0') mg_snprintf(buf, sizeof(buf), "%s", "MyDeviceID");

  buf[sizeof(buf) - 1] = '\0';
  g_device_id = strdup(buf);
}

static size_t print_fw_status(void (*out)(char, void *), void *ptr,
                              va_list *ap) {
  int fw = va_arg(*ap, int);
  return mg_xprintf(out, ptr, "{%m:%d,%m:%c%lx%c,%m:%u,%m:%u}",
                    MG_ESC("status"), mg_ota_status(fw), MG_ESC("crc32"), '"',
                    mg_ota_crc32(fw), '"', MG_ESC("size"), mg_ota_size(fw),
                    MG_ESC("timestamp"), mg_ota_timestamp(fw));
}

static size_t print_shorts(void (*out)(char, void *), void *ptr, va_list *ap) {
  uint16_t *array = va_arg(*ap, uint16_t *);
  int i, len = 0, num_elems = va_arg(*ap, int);
  for (i = 0; i < num_elems; i++) {
    len += mg_xprintf(out, ptr, "%s%hu", i ? "," : "", array[i]);
  }
  return len;
}

static size_t print_bools(void (*out)(char, void *), void *ptr, va_list *ap) {
  bool *array = va_arg(*ap, bool *);
  int i, len = 0, num_elems = va_arg(*ap, int);
  for (i = 0; i < num_elems; i++) {
    len += mg_xprintf(out, ptr, "%s%d", i ? "," : "", array[i]);
  }
  return len;
}

static void publish_status(struct mg_connection *c) {
  char topic[100];
  struct mg_mqtt_opts pub_opts;
  struct mg_iobuf io = {0, 0, 0, 512};

  // Print JSON notification into the io buffer
  mg_xprintf(mg_pfn_iobuf, &io,
             "{%m:%m,%m:{%m:%m,%m:%d,%m:%d,%m:[%M],%m:[%M],%m:%M,%m:%M}}",  //
             MG_ESC("method"), MG_ESC("status.notify"), MG_ESC("params"),   //
             MG_ESC("status"), MG_ESC("online"),                            //
             MG_ESC(("log_level")), s_device_config.log_level,              //
             MG_ESC(("pin_count")), s_device_config.pin_count,              //
             MG_ESC(("pin_map")), print_shorts, s_device_config.pin_map,
             s_device_config.pin_count,  //
             MG_ESC(("pin_state")), print_bools, s_device_config.pin_state,
             s_device_config.pin_count,                                  //
             MG_ESC(("crnt_fw")), print_fw_status, MG_FIRMWARE_CURRENT,  //
             MG_ESC(("prev_fw")), print_fw_status, MG_FIRMWARE_PREVIOUS);

  memset(&pub_opts, 0, sizeof(pub_opts));
  mg_snprintf(topic, sizeof(topic), "%s/%s/status", g_root_topic, g_device_id);
  pub_opts.topic = mg_str(topic);
  pub_opts.message = mg_str_n((char *) io.buf, io.len);
  pub_opts.qos = s_qos;
  pub_opts.retain = true;
  mg_mqtt_pub(c, &pub_opts);
  mg_iobuf_free(&io);
}

static void publish_response(struct mg_connection *c, char *buf, size_t len) {
  struct mg_mqtt_opts pub_opts;
  char topic[100];
  mg_snprintf(topic, sizeof(topic), "%s/%s/tx", g_root_topic, g_device_id);
  memset(&pub_opts, 0, sizeof(pub_opts));
  pub_opts.topic = mg_str(topic);
  pub_opts.message = mg_str_n(buf, len);
  pub_opts.qos = s_qos;
  mg_mqtt_pub(c, &pub_opts);
}

static void subscribe(struct mg_connection *c) {
  char *rx_topic = mg_mprintf("%s/%s/rx", g_root_topic, g_device_id);
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
  struct device_config dc = s_device_config;
  dc.pin_count = (int) mg_json_get_long(r->frame, "$.params.pin_count", -1);
  dc.log_level = (int) mg_json_get_long(r->frame, "$.params.log_level", -1);

  if (dc.log_level < 0 || dc.log_level > MG_LL_VERBOSE) {
    mg_rpc_err(r, -32602, "Log level must be from 0 to 4");
  } else if (dc.pin_count <= 0 || dc.pin_count > MAX_PINS) {
    mg_rpc_err(r, -32602, "Pin count must be from 1 to %d", MAX_PINS);
  } else {
    int i, val;
    for (i = 0; i < dc.pin_count; i++) {
      char path[50];
      mg_snprintf(path, sizeof(path), "$.params.pin_map[%d]", i);
      dc.pin_map[i] = (uint16_t) mg_json_get_long(r->frame, path, 0);
      mg_snprintf(path, sizeof(path), "$.params.pin_state[%d]", i);
      if ((val = (int) mg_json_get_long(r->frame, path, -1)) >= 0) {
        gpio_write(dc.pin_map[i], val);
      }
      dc.pin_state[i] = gpio_read(dc.pin_map[i]);
      // MG_INFO(("%d %d %d", i, dc.pin_map[i], dc.pin_state[i]));
    }
    mg_log_set(dc.log_level);
    s_device_config = dc;
    mg_rpc_ok(r, "true");
  }
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
  int len = 0;
  char *buf = mg_json_get_b64(r->frame, "$.params.chunk", &len);
  if (buf == NULL) {
    mg_rpc_err(r, 1, "Error processing the binary chunk.");
  } else {
    if (ofs < 0 || tot < 0) {
      mg_rpc_err(r, 1, "offset and total not set");
    } else if (ofs == 0 && mg_ota_begin((size_t) tot) == false) {
      mg_rpc_err(r, 1, "mg_ota_begin(%ld) failed\n", tot);
    } else if (len > 0 && mg_ota_write(buf, len) == false) {
      mg_rpc_err(r, 1, "mg_ota_write(%lu) @%ld failed\n", len, ofs);
      mg_ota_end();
    } else if (len == 0 && mg_ota_end() == false) {
      mg_rpc_err(r, 1, "mg_ota_end() failed\n", tot);
    } else {
      mg_rpc_ok(r, "%m", MG_ESC("ok"));
      if (len == 0) {  // Successful mg_ota_end() called, schedule device reboot
        mg_timer_add(s_conn->mgr, 500, 0, (void (*)(void *)) mg_device_reset,
                     NULL);
      }
    }
    free(buf);
  }
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    MG_INFO(("%lu CREATED", c->id));
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    MG_INFO(("Device %s is connected", g_device_id));
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    MG_INFO(("%lu CONNECTED to %s", c->id, g_mqtt_server_url));
    subscribe(c);
    publish_status(c);
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    struct mg_iobuf io = {0, 0, 0, 512};
    struct mg_rpc_req r = {&s_rpc, NULL, mg_pfn_iobuf,
                           &io,    NULL, {mm->data.ptr, mm->data.len}};
    size_t clipped_len = mm->data.len > 512 ? 512 : mm->data.len;
    MG_INFO(("%lu RECEIVED %.*s <- %.*s", c->id, clipped_len, mm->data.ptr,
             mm->topic.len, mm->topic.ptr));
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
}

// Timer function - recreate client connection if it is closed
static void timer_reconnect(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  if (s_conn == NULL) {
    struct mg_mqtt_opts opts;
    char topic[100], message[100];
    mg_snprintf(topic, sizeof(topic), "%s/%s/status", g_root_topic,
                g_device_id);
    mg_snprintf(message, sizeof(message), "{%m:%m,%m:{%m:%m}}",
                MG_ESC("method"), MG_ESC("status.notify"), MG_ESC("params"),
                MG_ESC("status"), MG_ESC("offline"));
    memset(&opts, 0, sizeof(opts));
    opts.clean = true;
    opts.qos = s_qos;
    opts.topic = mg_str(topic);
    opts.version = 4;
    opts.keepalive = MQTT_KEEPALIVE_SEC;
    opts.retain = true;
    opts.message = mg_str(message);
    s_conn = mg_mqtt_connect(mgr, g_mqtt_server_url, &opts, fn, NULL);
  }
}

static void timer_ping(void *arg) {
  mg_mqtt_send_header(s_conn, MQTT_CMD_PINGREQ, 0, 0);
  (void) arg;
}

void web_init(struct mg_mgr *mgr) {
  int i, ping_interval_ms = MQTT_KEEPALIVE_SEC * 1000 - 500;
  set_device_id();
  s_device_config.log_level = (int) mg_log_level;
  s_device_config.pin_count = 5;
  s_device_config.pin_map[0] = 10;
  s_device_config.pin_map[1] = 11;
  s_device_config.pin_map[2] = 12;
  s_device_config.pin_map[3] = 13;
  s_device_config.pin_map[4] = 25;
  for (i = 0; i < s_device_config.pin_count; i++) {
    s_device_config.pin_state[i] = gpio_read(s_device_config.pin_map[i]);
  }

  // Configure JSON-RPC functions we're going to handle
  mg_rpc_add(&s_rpc, mg_str("config.set"), rpc_config_set, NULL);
  mg_rpc_add(&s_rpc, mg_str("ota.commit"), rpc_ota_commit, NULL);
  mg_rpc_add(&s_rpc, mg_str("ota.rollback"), rpc_ota_rollback, NULL);
  mg_rpc_add(&s_rpc, mg_str("ota.upload"), rpc_ota_upload, NULL);
  mg_rpc_add(&s_rpc, mg_str("device.reset"), rpc_device_reset, NULL);

  mg_timer_add(mgr, 3000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_reconnect,
               mgr);
  mg_timer_add(mgr, ping_interval_ms, MG_TIMER_REPEAT, timer_ping, mgr);
}

void web_free(void) {
  mg_rpc_del(&s_rpc, NULL);  // Deallocate RPC handlers
  free(g_device_id);
}
