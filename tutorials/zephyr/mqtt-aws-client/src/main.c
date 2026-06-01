// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "certs.h"
#include "mongoose.h"
#include <zephyr/net/net_mgmt.h>

struct mg_mgr mgr;

static const char *s_url = "mqtts://a3nkain3cvvy7l-ats.iot.us-east-1.amazonaws.com";

static const char *s_rx_topic = "d/rx";
static const char *s_tx_topic = "d/tx";
static int s_qos = 1;
static int s_connected = 0;

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%p %s", c->fd, (char *) ev_data));
  } else if (ev == MG_EV_CONNECT) {
    // Set up 2-way TLS that is required by AWS IoT
    struct mg_tls_opts opts = {.ca = (char *) s_ca, .cert = (char *) s_cert, .key = (char *) s_key};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    struct mg_str topic = mg_str(s_rx_topic);
    MG_INFO(("Connected to %s", s_url));
    MG_INFO(("Subscribing to %s", s_rx_topic));
    struct mg_mqtt_opts sub_opts;
    memset(&sub_opts, 0, sizeof(sub_opts));
    sub_opts.topic = topic;
    sub_opts.qos = s_qos;
    mg_mqtt_sub(c, &sub_opts);
    c->data[0] = 'X';  // Set a label that we're logged in
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we receive MQTT message, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_INFO(("Received on %.*s : %.*s", mm->topic.len, mm->topic.buf,
             mm->data.len, mm->data.buf));
  } else if (ev == MG_EV_POLL && c->data[0] == 'X') {
    static unsigned long prev_second;
    unsigned long now_second = (*(unsigned long *) ev_data) / 1000;
    if (now_second != prev_second) {
      struct mg_str topic = mg_str(s_tx_topic), data = mg_str("{\"a\":123}");
      MG_INFO(("Publishing to %s", s_tx_topic));
      struct mg_mqtt_opts pub_opts;
      memset(&pub_opts, 0, sizeof(pub_opts));
      pub_opts.topic = topic;
      pub_opts.message = data;
      pub_opts.qos = s_qos, pub_opts.retain = false;
      mg_mqtt_pub(c, &pub_opts);
      prev_second = now_second;
    }
  }
}

// Zephyr: Define a semaphore and network management callback to be able to wait
// until our IP address is ready. The main function will start and block on this
// semaphore until this event handler releases it when the network is ready
K_SEM_DEFINE(run, 0, 1);

static void zeh(struct net_mgmt_event_callback *cb,
// https://docs.zephyrproject.org/latest/releases/migration-guide-4.2.html#networking
#if ZEPHYR_VERSION_CODE < 0x40200
                uint32_t mgmt_event,
#else
                uint64_t mgmt_event,
#endif
                struct net_if *iface) {
  if (mgmt_event == NET_EVENT_L4_CONNECTED) k_sem_give(&run);
}

int main(int argc, char *argv[]) {
  // Zephyr: Register the network management callback and block on the semaphore
  struct net_mgmt_event_callback ncb;
  net_mgmt_init_event_callback(&ncb, zeh, NET_EVENT_L4_CONNECTED);
  net_mgmt_add_event_callback(&ncb);
  k_sem_take(&run, K_FOREVER);

  mg_log_set(MG_LL_DEBUG);
  mg_mgr_init(&mgr);

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));

  uint64_t sntp_timer = 0;
  for (;;) {
    uint64_t period = mg_boot_timestamp_ms == 0 ? 1000 : 24 * 3600 * 1000;
    if (mg_timer_expired(&sntp_timer, period, mg_millis())) {
      mg_sntp_connect(&mgr, NULL, NULL, NULL);
    }
    // Connect to AWS IoT once we have a valid time (needed for mutual TLS)
    if (mg_boot_timestamp_ms > 0 && !s_connected) {
      MG_INFO(("Connecting to    : [%s]", s_url));
      struct mg_mqtt_opts opts = {.clean = true};
      mg_mqtt_connect(&mgr, s_url, &opts, fn, NULL);
      s_connected = 1;
    }
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
  return 0;
}
