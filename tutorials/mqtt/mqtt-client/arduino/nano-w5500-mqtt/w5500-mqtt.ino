#include <SPI.h>
#include "mongoose.h"

#define MQTT_SERVER "mqtt://broker.hivemq.com:1883"
#define MQTT_SUB_TOPIC "mg/rx"  // Subscribe to this topic
#define MQTT_PUB_TOPIC "mg/tx"  // Publish to this topic
#define SS_PIN 2                // Slave select pin
#define LED_PIN 21              // LED pin

struct mg_connection *mqtt_connection;
struct mg_tcpip_spi spi = {
    NULL,  // SPI metadata
    [](void *) { digitalWrite(SS_PIN, LOW); SPI.beginTransaction(SPISettings()); },
    [](void *) { digitalWrite(SS_PIN, HIGH); SPI.endTransaction(); },
    [](void *, uint8_t c) { return SPI.transfer(c); }, // Execute transaction
};
struct mg_mgr mgr;                                     // Mongoose event manager
struct mg_tcpip_if mif = {.mac = {2, 0, 1, 2, 3, 5}};  // Network interface

uint64_t mg_millis(void) {
  return millis();
}
bool mg_random(void *buf, size_t len) {  // For TLS
  uint8_t *p = (uint8_t *) buf;
  while (len--) *p++ = (unsigned char) (rand() & 255);
  return true;
}

void mqtt_publish(const char *message) {
  struct mg_mqtt_opts opts = {};
  opts.topic = mg_str(MQTT_PUB_TOPIC);
  opts.message = mg_str(message);
  if (mqtt_connection) mg_mqtt_pub(mqtt_connection, &opts);
}

// Crude function to get available RAM, for quick profiling
size_t getFreeRAM(void) {
  size_t size = 0, increment = 100;
  void *p;
  while ((p = malloc(size)) != NULL) free(p), size += increment;
  return size;
}

// Implement LED control over MQTT: "on" and "off" commands
void handle_command(struct mg_str msg) {
  if (msg.len == 3 && memcmp(msg.buf, "off", 3) == 0) {
    digitalWrite(LED_PIN, LOW);
    mqtt_publish("done - off");
  } else if (msg.len == 2 && memcmp(msg.buf, "on", 2) == 0) {
    digitalWrite(LED_PIN, HIGH);
    mqtt_publish("done - on");
  }
  MG_INFO(("Free RAM: %u bytes", getFreeRAM()));
}

static void mqtt_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_MQTT_OPEN) {
    MG_INFO(("%lu CONNECTED to %s", c->id, MQTT_SERVER));
    struct mg_mqtt_opts opts = {};
    opts.topic = mg_str(MQTT_SUB_TOPIC);
    mg_mqtt_sub(c, &opts);
    MG_INFO(("%lu SUBSCRIBED to %s", c->id, MQTT_SUB_TOPIC));
  } else if (ev == MG_EV_MQTT_MSG) {
    // Received MQTT message
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_INFO(("%lu RECEIVED %.*s <- %.*s", c->id, (int) mm->data.len,
             mm->data.buf, (int) mm->topic.len, mm->topic.buf));
    handle_command(mm->data);
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("%lu CLOSED", c->id));
    mqtt_connection = NULL;
  }
}

void reconnect_if_not_connected(void) {
  if (mif.state == MG_TCPIP_STATE_READY && mqtt_connection == NULL) {
    struct mg_mqtt_opts opts = {};
    opts.clean = true;
    mqtt_connection =
        mg_mqtt_connect(&mgr, MQTT_SERVER, &opts, mqtt_ev_handler, NULL);
  }
}

void setup() {
  Serial.begin(115200);       // Initialise serial
  while (!Serial) delay(50);  // for debug output

  SPI.begin();                   // Iniitialise SPI
  pinMode(SS_PIN, OUTPUT);       // to communicate with W5500 Ethernet module
  pinMode(LED_PIN, OUTPUT);  // Initialise LED

  mg_mgr_init(&mgr);        // Initialise Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_log_set_fn([](char ch, void *) { Serial.print(ch); }, NULL);  // Log serial
  mif.driver = &mg_tcpip_driver_w5500;  // Use W5500 built-in driver
  mif.driver_data = &spi;               // Pass SPI interface to W5500 driver
  mg_tcpip_init(&mgr, &mif);            // Initialise built-in TCP/IP stack
}

void loop() {
  mg_mgr_poll(&mgr, 1);          // Process network events
  reconnect_if_not_connected();  // Reconnect to MQTT server if needed
}
