// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "certs.h"
#include "mongoose.h"

struct mg_mgr mgr;

static const char *s_debug_level = "3";
static time_t s_boot_timestamp = 0;
static struct mg_connection *s_sntp_conn = NULL;
static const char *s_url =
    "mqtts://a3nkain3cvvy7l-ats.iot.us-east-1.amazonaws.com";

static const char *s_rx_topic = "d/rx";
static const char *s_tx_topic = "d/tx";
static int s_qos = 1;
static int s_connected = 0;

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%p %s", c->fd, (char *) ev_data));
  } else if (ev == MG_EV_CONNECT) {
    // Set up 2-way TLS that is required by AWS IoT
    struct mg_tls_opts opts = {.ca = s_ca, .cert = s_cert, .certkey = s_key};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    struct mg_str topic = mg_str(s_rx_topic);
    MG_INFO(("Connected to %s", s_url));
    MG_INFO(("Subscribing to %s", s_rx_topic));
    mg_mqtt_sub(c, topic, s_qos);
    c->label[0] = 'X';  // Set a label that we're logged in
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we receive MQTT message, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_INFO(("Received on %.*s : %.*s", (int) mm->topic.len, mm->topic.ptr,
             (int) mm->data.len, mm->data.ptr));
  } else if (ev == MG_EV_POLL && c->label[0] == 'X') {
    static unsigned long prev_second;
    unsigned long now_second = (*(unsigned long *) ev_data) / 1000;
    if (now_second != prev_second) {
      struct mg_str topic = mg_str(s_tx_topic), data = mg_str("{\"a\":123}");
      MG_INFO(("Publishing to %s", s_tx_topic));
      mg_mqtt_pub(c, topic, data, s_qos, false);
      prev_second = now_second;
    }
  }
}

// We have no valid system time(), and we need it for TLS. Implement it
time_t time(time_t *tp) {
  time_t t = s_boot_timestamp + k_uptime_get() / 1000;
  if (tp != NULL) *tp = t;
  return t;
}

// SNTP callback. Modifies s_boot_timestamp, to make time() correct
static void sfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_SNTP_TIME) {
    int64_t t = *(int64_t *) ev_data;
    MG_INFO(("Got SNTP time: %lld ms from epoch", t));
    s_boot_timestamp = (time_t) ((t - mg_millis()) / 1000);

    // We need correct time in order to get HTTPs working, therefore,
    // making https request from SMTP callback
    if (!s_connected) {
      MG_INFO(("Connecting to    : [%s]", s_url));
      struct mg_mqtt_opts opts = {.clean = true};
      mg_mqtt_connect(&mgr, s_url, &opts, fn,
                      NULL);  // Create client connection
      s_connected = 1;
    }
  } else if (ev == MG_EV_CLOSE) {
    s_sntp_conn = NULL;
  }
}

// Periodic timer syncs time via SNTP
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  if (s_sntp_conn == NULL) s_sntp_conn = mg_sntp_connect(mgr, NULL, sfn, NULL);
  if (s_boot_timestamp < 9999) mg_sntp_request(s_sntp_conn);
}

int main(int argc, char *argv[]) {
  mg_log_set(s_debug_level);

  mg_mgr_init(&mgr);
  mg_timer_add(&mgr, 5000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  mg_http_connect(&mgr, s_url, fn, NULL);  // Create client connection

  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
