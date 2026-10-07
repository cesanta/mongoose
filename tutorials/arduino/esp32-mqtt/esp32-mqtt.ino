#include "WiFi.h"
#include "mongoose.h"

#define WIFI_SSID "wifi_network_name"  // Change this
#define WIFI_PASS "wifi_password"      // And this

#define MQTT_SERVER "mqtt://broker.hivemq.com:1883"
#define MQTT_SUB_TOPIC "mg/rx"  // Subscribe to this topic
#define MQTT_PUB_TOPIC "mg/tx"  // Publish to this topic

#define LED_PIN 0

struct mg_mgr mgr;
struct mg_connection *mqtt_connection;

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
  if (mqtt_connection == NULL) {
    struct mg_mqtt_opts opts = {};
    opts.clean = true;
    mqtt_connection =
        mg_mqtt_connect(&mgr, MQTT_SERVER, &opts, mqtt_ev_handler, NULL);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial) delay(50);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) Serial.print("."), delay(100);

  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);
  mg_log_set_fn([](char ch, void *) { Serial.print(ch); }, NULL);
}

void loop() {
  mg_mgr_poll(&mgr, 1);          // Process network events
  reconnect_if_not_connected();  // Reconnect to MQTT server if needed
}

extern "C" int lwip_hook_ip6_input(struct pbuf *p, struct netif *inp)
    __attribute__((weak));
extern "C" int lwip_hook_ip6_input(struct pbuf *p, struct netif *inp) {
  if (ip6_addr_isany_val(inp->ip6_addr[0].u_addr.ip6)) {
    pbuf_free(p);
    return 1;
  }
  return 0;
}
