
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
 *  2. Buils and run in console /Users/alex/Projects/mongoose/examples/restful_server example
 *  3. Make board_ip and board_mac variables suitable for your network and board
 *  4. Uncomment line #include <Ethernet.h>
 *  5. Change IP address in s_target_address variable to IP address of host running restful_server
 *  6. Compile & flash sketch
 *  7. restful_server server will start to show current uptime and free memory size (with 5 second interval) 
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
 *  3. Buils and run in console /Users/alex/Projects/mongoose/examples/restful_server example
 *  4. Make the following variables suitable for your network
 *     - board_ip
 *     - subnet_mask
 *     - gateway
 *     - dns 
 *     - wlan_ssid
 *     - wlan_pwd
 *     - wlan_security
 *  5. Uncomment line #include <Adafruit_CC3000.h>
 *  6. Compile & flash sketch
 *  7. restful_server server will start to show current uptime and free memory size (with 5 second interval) *
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
static uint8_t board_ip[] = {192, 168, 10, 177};

#ifdef WIFI_CC3000
static uint8_t subnet_mask[] = {255, 255, 255, 0};
static uint8_t gateway[] = {192, 168, 10, 254};
static uint8_t dns_ip[] = {192, 168, 10, 254};

static const char *wlan_ssid = "mynetwork";     
static const char *wlan_pwd = "mypassword";
static int wlan_security = WLAN_SEC_WPA2;
#endif

static const char *s_target_address = "192.168.10.3:8000";

/////////////////////////////////////////////

static const char *s_request = "/printcontent";

static uint32_t IP2U32(uint8_t* iparr) {
  return ((uint32_t)iparr[0] << 24) | ((uint32_t)iparr[1] << 16) | (iparr[2] << 8) | (iparr[3]);
}

static int get_data_to_send(char* buf, int buf_size) {
  // Adding data to send
  // It could be any sensor data, now just put uptime & free memory size here
  return snprintf(buf, buf_size, "Uptime: %lus Free memory: %db",
                  millis()/1000, get_freememsize());
}
static void rfc_ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  int connect_status;

  switch (ev) {
    case NS_CONNECT:
      connect_status = * (int *) ev_data;

      if (connect_status == 0) {
        char buf[100];
        int len = get_data_to_send(buf, sizeof(buf));
        mg_printf(nc, "POST %s HTTP/1.0\r\nHost: %s\r\nContent-Lenght: %d"
                  "\r\n\r\n%s", s_request, s_target_address, len, buf);
        nc->flags |= MG_F_SEND_AND_CLOSE;
      } else {
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      }
      break;
    default:
      break;
  }
}

static struct mg_mgr mgr;
static struct mg_connection *nc;

void setup()
{
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
  Serial.println("Initialization done");  
}

void loop() {
  nc = mg_connect(&mgr, s_target_address, rfc_ev_handler);
  if (nc != NULL) {
    mg_set_protocol_http_websocket(nc);
  }

  uint32_t time_to_finish = millis() + 5000;
  while (millis() < time_to_finish) {
    mg_mgr_poll(&mgr, 1000);  
  }
}

