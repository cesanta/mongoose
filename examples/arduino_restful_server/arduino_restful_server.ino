/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 * 
 * Build and run instructions:
 * To run with Arduino Ethernet (W5100) shield:
 * -----------------------------------------------------------
 *  1. Add (Sketch->Add file...) the following files to sketch:
 *     - /mongoose/mongoose.h
 *     - /mongoose/mongoose.c
 *     - /mongoose/platforms/arduino_ethernet_W5100/avrsupport.h
 *     - /mongoose/platforms/arduino_ethernet_W5100/avrsupport.cpp
 *  2. Make board_ip and board_mac variables suitable for your network and board
 *  3. Uncomment line #include <Ethernet.h>
 *  4. Compile & flash sketch
 *  5. Run curl http://<board_ip/blink
 *     LED attached to PIN 13 will blink and board free memory size and uptime will responsed
 *
 * To run with Adafruit WiFi (CC3000) shield:
 * -----------------------------------------------------------
 *  1. Add (Sketch->Add files...) the following files to sketch:
 *     - /mongoose/mongoose.h
 *     - /mongoose/mongoose.c
 *     - /mongoose/platforms/arduino_ethernet_W5100/avrsupport.h
 *     - /mongoose/platforms/arduino_ethernet_W5100/avrsupport.cpp
 *  2. Import Adafruit CC3000 library for mongoose (select Sketch->Import Library...->Add library... and point 
 *     /mongoose/platforms/arduino_wifi_CC3000/adafruit_CC3000_lib_mongoose folder
 *  3. Make the following variables suitable for your network
 *     - board_ip
 *     - subnet_mask
 *     - gateway
 *     - dns 
 *     - wlan_ssid
 *     - wlan_pwd
 *     - wlan_security
 *  5. Uncomment line #include <Adafruit_CC3000.h>
 *  4. Compile & flash sketch
 *  5. Run curl http://<board_ip/blink
 *     LED attached to PIN 13 will blink and board free memory size and uptime will responsed
 *
 */

//#include <Ethernet.h>
//#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "mongoose.h"

// CHANGE THESE VARIABLES
// NB: Devices with the same address must not end up on the same network.
// Use MAC address provided by device manufacturer (e.g. on a sticker).
// If there isn't one, use a random one from the locally administered range.
// See http://en.wikipedia.org/wiki/MAC_address for details.
static uint8_t board_mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

static uint8_t board_ip[] = {192, 168, 10, 8};

#ifdef WIFI_CC3000
static uint8_t subnet_mask[] = {255, 255, 255, 0};
static uint8_t gateway[] = {192, 168, 10, 254};
static uint8_t dns_ip[] = {192, 168, 10, 254};

static const char *wlan_ssid = "mynetwork";     
static const char *wlan_pwd = "mypassword";
static int wlan_security = WLAN_SEC_WPA2;
#endif

///////////////////////////////////////////////

static const char *s_http_port = "60000";

static uint32_t IP2U32(uint8_t* iparr) {
  return ((uint32_t)iparr[0] << 24) | ((uint32_t)iparr[1] << 16) | (iparr[2] << 8) | (iparr[3]);
}

static void rfs_ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  char buf[100];
  int clen;

  switch (ev) {
    case NS_HTTP_REQUEST:      
      if (mg_vcmp(&hm->uri, "/blink") == 0) {
        blink(1, 500);
      }

      clen = snprintf(buf, sizeof(buf),
                      "Free memory size: %d Uptime: %d",
                      (int)get_freememsize(), (int)time(NULL));

      mg_printf_http_chunk(nc, "HTTP/1.1 200 OK\r\n"
                               "Content-Length: %d\r\n"
                               "Transfer-Encoding: chunked\r\n\r\n"
                               "%s",
                               clen, buf);

      mg_send_http_chunk(nc, "", 0);
      break;
    case NS_SEND:
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      break;
      
    default:
      break;
  }
}

static struct mg_connection *nc;
static struct mg_mgr mgr;

void setup() {
  Serial.begin(9600);
  Serial.println("Initialization...");
#if defined(ETHERNET_W5100)
  avr_netinit(board_mac, board_ip);
#elif defined(WIFI_CC3000)
  if (avr_netinit(wlan_ssid, wlan_pwd, wlan_security, IP2U32(board_ip), 
              IP2U32(subnet_mask), IP2U32(gateway), IP2U32(dns_ip)) != 0) {
    Serial.println("Initialization error, check network settings");
    return;
  };
#endif

  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, s_http_port, rfs_ev_handler);
  mg_set_protocol_http_websocket(nc);
  Serial.println("Initialization done");
}

void loop() {
  mg_mgr_poll(&mgr, 1000);
}
