/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "mbed.h"
#include "mongoose.h"

#define WIFI_STA_SSID "yourssid"
#define WIFI_STA_PASS "yourpass"

#define HTTP_SERVER_PORT "80"

DigitalOut led_green(LED1);
DigitalOut led_red(LED3);
DigitalOut led_blue(LED4);

void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    case MG_EV_ACCEPT: {
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      printf("%p: Connection from %s\r\n", nc, addr);
      break;
    }
    case MG_EV_HTTP_REQUEST: {
      struct http_message *hm = (struct http_message *) ev_data;
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      printf("%p: %.*s %.*s\r\n", nc, (int) hm->method.len, hm->method.p,
             (int) hm->uri.len, hm->uri.p);
      mg_send_response_line(nc, 200,
                            "Content-Type: text/html\r\n"
                            "Connection: close");
      mg_printf(nc,
                "\r\n<h1>Hello, %s!</h1>\r\n"
                "You asked for %.*s\r\n",
                addr, (int) hm->uri.len, hm->uri.p);
      nc->flags |= MG_F_SEND_AND_CLOSE;

      led_blue = !led_blue;
      break;
    }
    case MG_EV_CLOSE: {
      printf("%p: Connection closed\r\n", nc);
      break;
    }
  }
}

int main() {
  struct mg_mgr mgr;

  Serial pc(SERIAL_TX, SERIAL_RX, 115200);
  printf("Mongoose demo\n");
  led_green = 1; /* off */
  led_blue = 1;  /* off */

  SimpleLinkInterface wifi(PG_10, PG_11, SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS);

  sl_NetAppStop(SL_NET_APP_HTTP_SERVER_ID);

  wifi.connect(WIFI_STA_SSID, WIFI_STA_PASS);

  const char *ip = wifi.get_ip_address();
  const char *gw = wifi.get_gateway();
  const char *mac = wifi.get_mac_address();
  printf("IP address is: %s\n", ip ? ip : "No IP");
  printf("GW address is: %s\n", gw ? gw : "No IP");
  printf("MAC address is: %s\n", mac ? mac : "No MAC");

  mg_mgr_init(&mgr, NULL);

  const char *err;
  struct mg_bind_opts opts = {};
  opts.error_string = &err;
  mg_connection *nc = mg_bind_opt(&mgr, HTTP_SERVER_PORT, ev_handler, opts);
  if (nc == NULL) {
    printf("Failed to create listener: %s\r\n", err);
    led_red = 0; /* on */
    return 1;
  }
  mg_set_protocol_http_websocket(nc);
  printf("Server address: http://%s:%s\n", ip, HTTP_SERVER_PORT);

  while (true) {
    mg_mgr_poll(&mgr, 1000);
    led_green = !led_green;
  }
}
