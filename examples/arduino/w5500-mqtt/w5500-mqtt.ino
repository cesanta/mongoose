#include <SPI.h>
#include "net.h"

#define SS_PIN 17   // Slave select pin
struct mg_tcpip_spi spi = {
    NULL,                                               // SPI data
    [](void *) { digitalWrite(SS_PIN, LOW); },          // Begin transaction
    [](void *) { digitalWrite(SS_PIN, HIGH); },         // End transaction
    [](void *, uint8_t c) { return SPI.transfer(c); },  // Execute transaction
};

void exec_command(const char *req, size_t req_len) {
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

void setup() {
  Serial.begin(115200);
  SPI.begin();
  pinMode(SS_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  mg_log_set_fn([](char ch, void *) { Serial.print(ch); }, NULL);
  net_init();
}

void loop() {
  if (Serial.available()) process_input(Serial.read());
  mg_mgr_poll(&mgr, 1);
}
