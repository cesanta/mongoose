#include "net.h"

#define MQTT_SERVER "mqtt://broker.hivemq.com:1883"
struct mg_connection *mqtt_connection;
const char *mqtt_subscribe_topic = "mg/rx";
const char *mqtt_publish_topic = "mg/tx";

struct mg_mgr mgr;  // Mongoose event manager
struct mg_tcpip_if mif = {.mac = {2, 0, 1, 2, 3, 5}};  // network interface

void mqtt_publish(const char *message) {
  struct mg_mqtt_opts opts = {};
  opts.topic = mg_str(mqtt_publish_topic);
  opts.message = mg_str(message);
  if (mqtt_connection) mg_mqtt_pub(mqtt_connection, &opts);
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_MQTT_OPEN) {
    MG_INFO(("%lu CONNECTED to %s", c->id, MQTT_SERVER));
    struct mg_mqtt_opts opts = {};
    opts.topic = mg_str(mqtt_subscribe_topic);
    mg_mqtt_sub(c, &opts);
    MG_INFO(("%lu SUBSCRIBED to %s", c->id, mqtt_subscribe_topic));
  } else if (ev == MG_EV_MQTT_MSG) {
    // Received MQTT message
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_INFO(("%lu RECEIVED %.*s <- %.*s", c->id, (int) mm->data.len,
             mm->data.ptr, (int) mm->topic.len, mm->topic.ptr));
    exec_command(mm->data.ptr, mm->data.len);
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("%lu CLOSED", c->id));
    mqtt_connection = NULL;
  }
}

static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  struct mg_mqtt_opts opts;
  memset(&opts, 0, sizeof(opts));
  opts.clean = true;
  if (mif.state == MG_TCPIP_STATE_READY && mqtt_connection == NULL) {
    mqtt_connection = mg_mqtt_connect(mgr, MQTT_SERVER, &opts, fn, NULL);
  }
  // MG_INFO(("Eth: %d, MQTT: %p", mif.state, mqtt_connection));
}

void net_init(void) {
  // Set Mongoose logging function to a serial print
  mg_mgr_init(&mgr);

  mif.driver = &mg_tcpip_driver_w5500;
  mif.driver_data = &spi;
  mg_tcpip_init(&mgr, &mif);

  // Start a timer that keeps MQTT connection alive
  mg_timer_add(&mgr, 3000, MG_TIMER_REPEAT, timer_fn, &mgr);
}
