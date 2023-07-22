#include <SPI.h>
#include "mongoose.h"

#define MQTT_SERVER "mqtt://broker.hivemq.com:1883"
struct mg_connection *mqtt_connection;
const char *mqtt_subscribe_topic = "mg/rx";
const char *mqtt_publish_topic = "mg/tx";

#define SS_PIN 17   // Slave select pin
struct mg_mgr mgr;  // Mongoose event manager
struct mg_tcpip_spi spi = {
    NULL,                                               // SPI data
    [](void *) { digitalWrite(SS_PIN, LOW); },          // Begin transaction
    [](void *) { digitalWrite(SS_PIN, HIGH); },         // End transaction
    [](void *, uint8_t c) { return SPI.transfer(c); },  // Execute transaction
};
struct mg_tcpip_if mif = {.mac = {2, 0, 1, 2, 3, 5}};  // network interface

static void mqtt_publish(const char *message) {
  struct mg_mqtt_opts opts = {};
  opts.topic = mg_str(mqtt_publish_topic);
  opts.message = mg_str(message);
  if (mqtt_connection) mg_mqtt_pub(mqtt_connection, &opts);
}

static void exec_command(const char *req, size_t req_len) {
  char res[100];
  if (req_len == 2 && strncmp(req, "on", req_len) == 0) {
    digitalWrite(LED_BUILTIN, true);
    snprintf(res, sizeof(res), "LED on");
  } else if (req_len == 3 && strncmp(req, "off", req_len) == 0) {
    digitalWrite(LED_BUILTIN, false);
    snprintf(res, sizeof(res), "LED off");
  } else {
    snprintf(res, sizeof(res), "Unknown command: [%.*s]", (int) req_len, req);
  }
  Serial.println(res);
  mqtt_publish(res);
}

static void process_input(char c) {
  static char buf[100];
  static size_t len = 0;
  char response[100];
  if (c != '\n' && c != '\0') buf[len++] = c;  // Append to the buffer
  if (len >= sizeof(buf)) len = 0;             // On overflow, reset
  if (c == '\n' && len > 0) {
    exec_command(buf, len);
    len = 0;
  }
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
  MG_INFO(("Eth: %d, MQTT: %p", mif.state, mqtt_connection));
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  pinMode(SS_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Set Mongoose logging function to a serial print
  mg_log_set_fn([](char ch, void *) { Serial.print(ch); }, NULL);
  mg_mgr_init(&mgr);

  mif.driver = &mg_tcpip_driver_w5500;
  mif.driver_data = &spi;
  mg_tcpip_init(&mgr, &mif);

  // Start a timer that keeps MQTT connection alive
  mg_timer_add(&mgr, 3000, MG_TIMER_REPEAT, timer_fn, &mgr);
}

void loop() {
  if (Serial.available()) process_input(Serial.read());
  mg_mgr_poll(&mgr, 1);
}
