#include <SPI.h>
#include "mongoose.h"

#define LED_PIN 21  // Slave select pin
#define SS_PIN 17   // Slave select pin

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
        if (ev == MG_EV_HTTP_MSG) mg_http_reply(c, 200, "", "ok\n");
      },
      &mgr);
}

void loop() {
  mg_mgr_poll(&mgr, 1);
}
