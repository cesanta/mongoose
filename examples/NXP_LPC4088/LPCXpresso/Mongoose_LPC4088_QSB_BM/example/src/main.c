/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/raw.h"
#include "lwip/timers.h"
#include "netif/etharp.h"

#include "board.h"
#include "lpc_phy.h"
#include "arch/lpc17xx_40xx_emac.h"
#include "arch/lpc_arch.h"

/* IP address configuration. */
#define USE_DHCP 1 /* For static IP, set to 0 and configure options below */
#if !USE_DHCP
#define STATIC_IP "192.168.0.111"
#define STATIC_NM "255.255.255.0"
#define STATIC_GW "192.168.0.1"
#endif

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
      Board_LED_Toggle(2);
      break;
    }
    case MG_EV_CLOSE: {
      printf("%p: Connection closed\r\n", nc);
      break;
    }
  }
}

void handle_eth(struct netif *eth_if) {
  /* PHY link status. */
  uint32_t status = lpcPHYStsPoll();

  if (status & PHY_LINK_CHANGED) {
    if (status & PHY_LINK_CONNECTED) {
      Board_LED_Set(0, true);

      if (status & PHY_LINK_SPEED100) {
        Chip_ENET_Set100Mbps(LPC_ETHERNET);
      } else {
        Chip_ENET_Set10Mbps(LPC_ETHERNET);
      }
      if (status & PHY_LINK_FULLDUPLX) {
        Chip_ENET_SetFullDuplex(LPC_ETHERNET);
      } else {
        Chip_ENET_SetHalfDuplex(LPC_ETHERNET);
      }

      netif_set_link_up(eth_if);
      printf("Link up\r\n");
    } else {
      Board_LED_Set(0, false);
      netif_set_link_down(eth_if);
      printf("Link down\r\n");
    }
  }

  Board_LED_Set(1, (eth_if->dhcp->state == DHCP_BOUND));

  /* Handle packets as part of this loop, not in the IRQ handler */
  lpc_enetif_input(eth_if);

  /* Free TX buffers that are done sending */
  lpc_tx_reclaim(eth_if);
}

int gettimeofday(struct timeval *tv, void *tzvp) {
  tv->tv_sec = time(NULL);
  tv->tv_usec = 0;
  return 0;
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

  SystemCoreClockUpdate();
  Board_Init();
  SysTick_Enable(1);

  lwip_init();

  Board_LED_Set(0, false); /* Link state */
  Board_LED_Set(1, false); /* DHCP state */
  Board_LED_Set(2, false); /* HTTP request activity indicator */
  Board_LED_Set(3, false); /* Error indicator */

#if USE_DHCP
  netif_add(&eth0, NULL, NULL, NULL, NULL, lpc_enetif_init, ethernet_input);
  printf("Waiting for DHCP...\r\n");
  dhcp_start(&eth0);
  u8_t os = 0xff, ds;
  do {
    ds = eth0.dhcp->state;
    if (ds != os) {
      printf("  DHCP state: %d\r\n", ds);
      os = ds;
    }
    handle_eth(&eth0);
    sys_check_timeouts();
  } while (ds != DHCP_BOUND);
  printf("DHCP bound.\r\n");
#else
  ip_addr_t ip, nm, gw;
  if (!ipaddr_aton(STATIC_IP, &ip) || !ipaddr_aton(STATIC_NM, &nm) ||
      !ipaddr_aton(STATIC_GW, &gw)) {
    printf("Invalid static IP configuration.\r\n");
    Board_LED_Set(3, true);
    return 1;
  } else {
    netif_add(&eth0, &ip, &nm, &gw, NULL, lpc_enetif_init, ethernet_input);
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
    Board_LED_Set(3, true);
    return 1;
  }
  mg_set_protocol_http_websocket(nc);
  printf("Server address: http://%s/\r\n", ipaddr_ntoa(&eth0.ip_addr));

  while (1) {
    /* Ethernet link status, low level input. */
    handle_eth(&eth0);

    /* LWIP timers - ARP, DHCP, TCP, etc. */
    sys_check_timeouts();

    /* Mongoose poll */
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
