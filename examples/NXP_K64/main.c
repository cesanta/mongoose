/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"

#include <stdio.h>
#include <stdlib.h>

#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timers.h"
#include "netif/etharp.h"

#include "ethernetif.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

#include "pin_mux.h"

/* IP address configuration. */
#define USE_DHCP 1 /* For static IP, set to 0 and configure options below */
#if !USE_DHCP
#define STATIC_IP "192.168.0.111"
#define STATIC_NM "255.255.255.0"
#define STATIC_GW "192.168.0.1"
#endif

void gpio_init() {
  CLOCK_EnableClock(kCLOCK_PortA);
  CLOCK_EnableClock(kCLOCK_PortB);
  CLOCK_EnableClock(kCLOCK_PortE);
  PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN,
                 kPORT_MuxAsGpio);
  LED_RED_INIT(0);
  PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN,
                 kPORT_MuxAsGpio);
  LED_GREEN_INIT(0);
  PORT_SetPinMux(BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN,
                 kPORT_MuxAsGpio);
  LED_BLUE_INIT(0);
  LED_BLUE_OFF();
}

void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  if (ev == MG_EV_POLL) return;
  /* printf("ev %d\r\n", ev); */
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
      LED_BLUE_TOGGLE();
      break;
    }
    case MG_EV_CLOSE: {
      printf("%p: Connection closed\r\n", nc);
      break;
    }
  }
}

/*
 * This is a callback invoked by Mongoose to signal that a poll is needed soon.
 * Since we're in a tight polling loop anyway (see below), we don't need to do
 * anything.
 */
void mg_lwip_mgr_schedule_poll(struct mg_mgr *mgr) {
}

int main(void) {
  struct netif eth0;
  MPU_Type *base = MPU;
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  /* Disable MPU. */
  base->CESR &= ~MPU_CESR_VLD_MASK;
  lwip_init();
  gpio_init();
  LED_RED_ON();
  printf("Waiting for link...\r\n");
#if USE_DHCP
  netif_add(&eth0, NULL, NULL, NULL, NULL, ethernetif_init, ethernet_input);
  printf("Waiting for DHCP...\r\n");
  LED_GREEN_ON();
  dhcp_start(&eth0);
  u8_t os = 0xff, ds;
  do {
    ds = eth0.dhcp->state;
    if (ds != os) {
      printf("  DHCP state: %d\r\n", ds);
      os = ds;
    }
    sys_check_timeouts();
  } while (ds != DHCP_BOUND);
  printf("DHCP bound.\r\n");
#else
  ip_addr_t ip, nm, gw;
  if (!ipaddr_aton(STATIC_IP, &ip) || !ipaddr_aton(STATIC_NM, &nm) ||
      !ipaddr_aton(STATIC_GW, &gw)) {
    printf("Invalid static IP configuration.\r\n");
    return 1;
  } else {
    netif_add(&eth0, &ip, &nm, &gw, NULL, ethernetif_init, ethernet_input);
    netif_set_up(&eth0);
  }
#endif
  netif_set_default(&eth0);
  printf("Setting up HTTP server...\r\n");

  struct mg_mgr mgr;
  mg_mgr_init(&mgr, NULL);

  const char *err;
  struct mg_bind_opts opts = {};
  opts.error_string = &err;
  struct mg_connection *nc = mg_bind_opt(&mgr, "80", ev_handler, opts);
  if (nc == NULL) {
    printf("Failed to create listener: %s\r\n", err);
    LED_RED_ON();
    LED_GREEN_OFF();
    return 1;
  }
  mg_set_protocol_http_websocket(nc);
  printf("Server address: http://%s/\r\n", ipaddr_ntoa(&eth0.ip_addr));
  LED_RED_OFF();
  LED_GREEN_ON();

  while (1) {
    sys_check_timeouts();
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
