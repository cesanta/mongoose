// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example MQTT client. It performs the following steps:
//    1. Connects to the AWS IoT MQTT server
//    2. When connected, subscribes to the topic `s_rx_topic`
//    3. Publishes message `hello` to the `s_tx_topic` periodically
//

// How to build and run this example:
// 1. Login to AWS IoT
// 2. Click "Settings" on the left bar, copy the domain, change s_url below
// 3. Click Security -> Policies -> Create Policy, fill fields:
//               Name        : PolicyAllow
//               Action      : iot:*
//               Resource ARN: *
//               Effect      : allow
//        then, click "Create"
// 4. Create EC private key file and CSR (Certificate Signing Request)
//      type "make csr", see Makefile
// 5. Click Security -> Certificates -> Add Certificate -> Create Certificate
//      Choose "Create certificate with certificate signing request (CSR)"
//      Choose "crt.csr" created on a previous step
//      Choose "Active" to activate certificate
//      Click Create
//      Downoad AmazonRootCA1.pem as ca.pem and generated certificate as crt.pem
//      Select certificate, attach PolicyAllow to it
// 6. Type "make" to build and run the example

static const char *s_url =
    "mqtts://a1pjwh2bop1ojt-ats.iot.eu-west-1.amazonaws.com";
static const char *s_rx_topic = "d/rx";
static const char *s_tx_topic = "d/tx";
static int s_qos = 1;

#include "mongoose.h"

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    if (mg_url_is_ssl(s_url)) {
      struct mg_tls_opts opts = {.ca = mg_unpacked("/ca.pem"),
                                 .cert = mg_unpacked("/crt.pem"),
                                 .key = mg_unpacked("/key.pem"),
                                 .name = mg_url_host(s_url)};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%p %s", c->fd, (char *) ev_data));
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
    MG_INFO(("Received on %.*s : %.*s", (int) mm->topic.len, mm->topic.buf,
             (int) mm->data.len, mm->data.buf));
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

  if (ev == MG_EV_ERROR || ev == MG_EV_CLOSE) {
    MG_INFO(("Got event %d, stopping...", ev));
    *(bool *) c->fn_data = true;  // Signal that we're done
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_mqtt_opts opts = {.clean = true};
  bool done = false;
  mg_log_set(MG_LL_DEBUG);
  mg_mgr_init(&mgr);                               // Initialise event manager
  MG_INFO(("Connecting to %s", s_url));            // Inform that we're starting
  mg_mqtt_connect(&mgr, s_url, &opts, fn, &done);  // Create client connection
  while (!done) mg_mgr_poll(&mgr, 1000);           // Loop until done
  mg_mgr_free(&mgr);                               // Finished, cleanup
  return 0;
}
