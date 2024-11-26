#include <SPI.h>
#include "mongoose.h"

#define SS_PIN 17    // Slave select pin
struct mg_mgr mgr;  // Mongoose event manager
struct mg_tcpip_spi spi = {
    NULL,                                               // SPI data
    [](void *) { digitalWrite(SS_PIN, LOW); },          // begin transation
    [](void *) { digitalWrite(SS_PIN, HIGH); },         // end transaction
    [](void *, uint8_t c) { return SPI.transfer(c); },  // execute transaction
};
struct mg_tcpip_if mif = {.mac = {2, 0, 1, 2, 3, 5}};  // network interface

void setup() {
  Serial.begin(115200);
  mg_log_set_fn([](char ch, void *) { Serial.print(ch); }, NULL);
  mg_log_set(MG_LL_DEBUG);
  while (!Serial) (void) 0;

  pinMode(SS_PIN, OUTPUT);
  SPI.begin();

  mg_mgr_init(&mgr);
  // delay(3000);
  MG_INFO(("Starting TCP/IP stack..."));
  SPI.transfer(0);
  MG_INFO(("--->..."));

  mif.driver = &mg_tcpip_driver_w5500;
  mif.driver_data = &spi;
  mg_tcpip_init(&mgr, &mif);

  // Start a 5 sec timer, print status message periodically
  mg_timer_add(
      &mgr, 5000, MG_TIMER_REPEAT,
      [](void *) {
        MG_INFO(("ethernet: %s", mg_tcpip_driver_w5500.up(&mif) ? "up" : "down"));
      },
      NULL);

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
