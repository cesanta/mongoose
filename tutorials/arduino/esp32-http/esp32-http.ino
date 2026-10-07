#include "mongoose.h"
#include "WiFi.h"

#define LED_PIN LED_BUILTIN
#define WIFI_SSID "wifi_network_name"  // Change this
#define WIFI_PASS "wifi_password"      // And this

struct mg_mgr mgr;

// Crude function to get available RAM, for quick profiling
size_t getFreeRAM(void) {
  size_t size = 0, increment = 1024;
  void *p;
  while ((p = malloc(size)) != NULL) free(p), size += increment;
  return size;
}

void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (mg_match(hm->uri, mg_str("/api/led/on"), NULL)) {
      digitalWrite(LED_PIN, HIGH);
      mg_http_reply(c, 200, "", "{%m: %d}\n", MG_ESC("led"), digitalRead(LED_PIN));
    } else if (mg_match(hm->uri, mg_str("/api/led/off"), NULL)) {
      digitalWrite(LED_PIN, LOW);
      mg_http_reply(c, 200, "", "{%m: %d}\n", MG_ESC("led"), digitalRead(LED_PIN));
    } else {
      mg_http_reply(c, 200, "", "ok, free RAM: %u\n", xPortGetFreeHeapSize());
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial) delay(50);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status() != WL_CONNECTED) Serial.print("."), delay(100);

  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);
  mg_log_set_fn([](char ch, void *) { Serial.print(ch); }, NULL);
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);
}

void loop() {
  mg_mgr_poll(&mgr, 0);
}

extern "C" int lwip_hook_ip6_input(struct pbuf *p, struct netif *inp) __attribute__((weak));
extern "C" int lwip_hook_ip6_input(struct pbuf *p, struct netif *inp) {
  if (ip6_addr_isany_val(inp->ip6_addr[0].u_addr.ip6)) {
    pbuf_free(p);
    return 1;
  }
  return 0;
}
