#include <SoftwareSerial.h>
#include "mongoose.h"

#define MQTT_SERVER "mqtt://broker.hivemq.com:8883"
#define MQTT_SUB_TOPIC "mg/rx"  // Subscribe to this topic
#define MQTT_PUB_TOPIC "mg/tx"  // Publish to this topic

static const char *script[] = {
  "AT\r\n", "*OK\r\n",
  "ATZ\r\n", "*OK\r\n",
  "AT+CPIN?\r\n", "*OK\r\n",
  "AT+CNMI=0,0,0,0,0\r\n", "*OK\r\n",
  "AT+CGDCONT=1,\"IP\",\"iot.1nce.net\"\r\n", "*OK\r\n",
  "AT+CGDATA=\"PPP\",1\r\n", "*CONNECT\r\n",
  NULL,
};

// Visit https://mongoose.ws/tls/ and copy-paste CA certificate for your server
#define TLS_CA \
  ""

// We use Serial1 or software serial to communicate with the modem
#define LED_PIN LED_BUILTIN
#define RST_PIN 10
#define ModemSerial Serial1
//#define RX_PIN 9
//#define TX_PIN 8
//SoftwareSerial ModemSerial(RX_PIN, TX_PIN);

struct mg_connection *mqtt_connection;
struct mg_tcpip_driver_ppp_data driver_data;
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

void handle_command(struct mg_str msg) {
  if (msg.len == 3 && memcmp(msg.buf, "off", 3) == 0) {
    digitalWrite(LED_PIN, LOW);
    mqtt_publish("done - off");
  } else if (msg.len == 2 && memcmp(msg.buf, "on", 2) == 0) {
    digitalWrite(LED_PIN, HIGH);
    mqtt_publish("done - on");
  }
}

static void mqtt_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_CONNECT && mg_url_is_ssl(MQTT_SERVER)) {
    struct mg_tls_opts opts = {};
    opts.ca = mg_str(TLS_CA);
    mg_tls_init(c, &opts);
  }
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
  Serial.begin(115200);       // Initialise serial for logs
  while (!Serial) delay(50);  // for debug output

  pinMode(LED_PIN, OUTPUT);  // Initialise pins
  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, HIGH);
  //pinMode(RX_PIN, INPUT);   // Only required for
  //pinMode(TX_PIN, OUTPUT);  // software serial
  ModemSerial.begin(115200);  // Serial for modem communication

  mg_mgr_init(&mgr);        // Initialise Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_log_set_fn([](char c, void *) { Serial.print(c); }, NULL);  // Log serial

  mif.driver = &mg_tcpip_driver_ppp;  // Initialise built-in TCP/IP stack
  mif.driver_data = &driver_data;     // with the cellular driver
  driver_data.script = script;
  driver_data.reset = [](void *) { digitalWrite(RST_PIN, LOW); delay(1); digitalWrite(RST_PIN, HIGH); },
  driver_data.tx = [](void *, uint8_t c) { ModemSerial.write(c); },
  driver_data.rx = [](void *) { return ModemSerial.available() ? ModemSerial.read() : -1; },
  mg_tcpip_init(&mgr, &mif);
  mif.enable_dhcp_client = false;
}

void loop() {
  mg_mgr_poll(&mgr, 1);          // Process network events
  reconnect_if_not_connected();  // Reconnect to MQTT server if needed
}
