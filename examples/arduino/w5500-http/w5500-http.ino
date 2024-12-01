#include <SPI.h>
#include "mongoose.h"

#define LED_PIN LED_BUILTIN  // LED pin - can be LED_BUILTIN
#define SS_PIN 17    // Slave select pin

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

// Crude function to get available RAM, for quick profiling
size_t getFreeRAM(void) {
  size_t size = 0, increment = 100;
  void *p;
  while ((p = malloc(size)) != NULL) free(p), size += increment;
  return size;
}

void setup() {
  Serial.begin(115200);       // Initialise serial
  while (!Serial) delay(50);  // for debug output

  SPI.begin();               // Iniitialise SPI
  pinMode(SS_PIN, OUTPUT);   // to communicate with W5500 Ethernet module
  pinMode(LED_PIN, OUTPUT);  // Initialise LED

  mg_mgr_init(&mgr);        // Initialise Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_log_set_fn([](char ch, void *) { Serial.print(ch); }, NULL);  // Log serial
  mif.driver = &mg_tcpip_driver_w5500;  // Use W5500 built-in driver
  mif.driver_data = &spi;               // Pass SPI interface to W5500 driver
  mg_tcpip_init(&mgr, &mif);            // Initialise built-in TCP/IP stack

  // Setup HTTP listener. Respond "ok" on any HTTP request
  mg_http_listen(
      &mgr, "http://0.0.0.0",
      [](struct mg_connection *c, int ev, void *ev_data) {
        if (ev == MG_EV_HTTP_MSG) {
          struct mg_http_message *hm = (struct mg_http_message *) ev_data;
          if (mg_match(hm->uri, mg_str("/api/led/on"), NULL)) {
            digitalWrite(LED_PIN, HIGH);
            mg_http_reply(c, 200, "", "{%m: %d}\n", MG_ESC("led"), digitalRead(LED_PIN));
          } else if (mg_match(hm->uri, mg_str("/api/led/off"), NULL)) {
            digitalWrite(LED_PIN, LOW);
            mg_http_reply(c, 200, "", "{%m: %d}\n", MG_ESC("led"), digitalRead(LED_PIN));
          } else {
            mg_http_reply(c, 200, "", "ok, free RAM: %u\n", getFreeRAM());
          }
        }
      },
      &mgr);
}

void loop() {
  mg_mgr_poll(&mgr, 1);
}
