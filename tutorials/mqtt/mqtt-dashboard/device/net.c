// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "net.h"
#include <string.h>
#include "mongoose.h"

char *g_mqtt_server_url = MQTT_SERVER_URL;
char *g_root_topic = MQTT_ROOT_TOPIC;
char *g_device_id;

static uint8_t s_qos = 1;             // MQTT QoS
static struct mg_connection *s_conn;  // MQTT Client connection
static struct mg_rpc *s_rpc = NULL;   // List of registered RPC methods

struct device_state {
  bool led_status;
  char firmware_version[20];
};

static struct device_state s_device_state;

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
  if (id.buf != NULL) {
    mg_snprintf(buf, sizeof(buf), "%s", id.buf);
    mg_free((void *) id.buf);
  }
#endif

  if (buf[0] == '\0') mg_snprintf(buf, sizeof(buf), "%s", "MyDeviceID");

  buf[sizeof(buf) - 1] = '\0';
  g_device_id = strdup(buf);
}

static void publish_status(struct mg_connection *c) {
  char topic[100];
  struct mg_mqtt_opts pub_opts;
  struct mg_iobuf io = {0, 0, 0, 512};

  // Print JSON notification into the io buffer
  mg_xprintf(
      mg_pfn_iobuf, &io,
      "{%m:%m,%m:{%m:%m,%m:%s,%m:%m}}",                                    //
      MG_ESC("method"), MG_ESC("status.notify"), MG_ESC("params"),         //
      MG_ESC("status"), MG_ESC("online"),                                  //
      MG_ESC("led_status"), s_device_state.led_status ? "true" : "false",  //
      MG_ESC("firmware_version"), MG_ESC(s_device_state.firmware_version));

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
  MG_INFO(("%lu SUBSCRIBED to %.*s", c->id, (int) subt.len, subt.buf));
  mg_free(rx_topic);
}

static void rpc_state_set(struct mg_rpc_req *r) {
  mg_json_get_bool(r->frame, "$.params.led_status", &s_device_state.led_status);
  mg_rpc_ok(r, "true");
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
    }
    mg_free(buf);
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
                           &io,    NULL, {mm->data.buf, mm->data.len}};
    size_t clipped_len = mm->data.len > 512 ? 512 : mm->data.len;
    MG_INFO(("%lu RECEIVED %.*s <- %.*s", c->id, clipped_len, mm->data.buf,
             mm->topic.len, mm->topic.buf));
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

static void timer_sntp(void *param) {  // SNTP timer function. Sync up time
  static uint64_t hourly_timer = 0;
  uint64_t t1 = mg_now(), t2 = mg_millis();
  if (t1 < t2 + 3600 || mg_timer_expired(&hourly_timer, 3600000, t2)) {
    mg_sntp_connect(param, "udp://time.google.com:123", NULL, NULL);
  }
}

void web_init(struct mg_mgr *mgr) {
  int ping_interval_ms = MQTT_KEEPALIVE_SEC * 1000 - 500;
  int flags = MG_TIMER_REPEAT;
  set_device_id();

  // Initialize device state
  s_device_state.led_status = false;
  mg_snprintf(s_device_state.firmware_version,
              sizeof(s_device_state.firmware_version), "%s",
              g_firmware_version);

  // Configure JSON-RPC functions we're going to handle
  mg_rpc_add(&s_rpc, mg_str("state.set"), rpc_state_set, NULL);
  mg_rpc_add(&s_rpc, mg_str("ota.upload"), rpc_ota_upload, NULL);

  mg_timer_add(mgr, 3000, flags | MG_TIMER_RUN_NOW, timer_reconnect, mgr);
  mg_timer_add(mgr, ping_interval_ms, flags, timer_ping, mgr);
  mg_timer_add(mgr, 2000, flags, timer_sntp, mgr);
}

void web_free(void) {
  mg_rpc_del(&s_rpc, NULL);  // Deallocate RPC handlers
  free(g_device_id);
}
