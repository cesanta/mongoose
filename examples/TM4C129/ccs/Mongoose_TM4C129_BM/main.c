/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 *
 *
 * This example sets up a simple HTTP server using the Mongoose multi-protocol
 * networking library.
 *
 * It assumes TivaWare is installed under
 * ${TI_PRODUCTS_DIR}/TivaWare_C_Series-2.1.3.156. ${TI_PRODUCTS_DIR} is usually
 * C:\ti on Windows, ~/ti on Linux.
 *
 * If you have a different version installed, please edit the SDK_ROOT path
 * variable under Project -> Properties -> Resource -> Linked Resources.
 */

#include <stdbool.h>
#include <stdint.h>

#include "mongoose.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/flash.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "utils/lwiplib.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "drivers/pinout.h"

#define SYSTICKHZ 100
#define SYSTICKMS (1000 / SYSTICKHZ)

struct mg_mgr g_mgr;

// The main Mongoose event handler.
void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  static int led_value = 0;
  if (ev == MG_EV_POLL) return;
  // UARTprintf("%p: ev %d\r\n", nc, ev);
  switch (ev) {
    case MG_EV_ACCEPT: {
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      UARTprintf("%p: Connection from %s\r\n", nc, addr);
      break;
    }
    case MG_EV_HTTP_REQUEST: {
      struct http_message *hm = (struct http_message *) ev_data;
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      UARTprintf("%p: HTTP request\r\n", nc);
      mg_send_response_line(nc, 200,
                            "Content-Type: text/html\r\n"
                            "Connection: close");
      mg_printf(nc,
                "\r\n<h1>Hello, %s!</h1>\r\n"
                "You asked for %.*s\r\n",
                addr, (int) hm->uri.len, hm->uri.p);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      led_value ^= CLP_D1;
      LEDWrite(CLP_D1, led_value);
      break;
    }
    case MG_EV_CLOSE: {
      UARTprintf("%p: Connection closed\r\n", nc);
      break;
    }
  }
}

int gettimeofday(struct timeval *tv, void *tzvp) {
  tv->tv_sec = time(NULL);
  tv->tv_usec = 0;
  return 0;
}

void mg_lwip_mgr_schedule_poll(struct mg_mgr *mgr) {
}

// Special IP address values that lwiplib uses.
#define IP_LINK_DOWN (0xffffffffU)
#define IP_LINK_UP (0)

// Invoked by lwiplib every HOST_TMR_INTERVAL ms (defined in lwipopts.h).
void lwIPHostTimerHandler(void) {
  static uint32_t old_ip = IP_LINK_DOWN;
  uint32_t ip = lwIPLocalIPAddrGet();
  if (ip != old_ip) {
    switch (ip) {
      case IP_LINK_DOWN: {
        UARTprintf("Link down.\r\n");
        LEDWrite(CLP_D2, 0);
        break;
      }
      case IP_LINK_UP: {
        UARTprintf("Link up.\r\n");
        LEDWrite(CLP_D2, 0);
        break;
      }
      default: {
        UARTprintf("IP: http://%s/\r\n", ipaddr_ntoa((const ip_addr_t *) &ip));
        LEDWrite(CLP_D2, CLP_D2);
        break;
      }
    }
    old_ip = ip;
  }

  mg_mgr_poll(&g_mgr, 0);
}

void SysTickIntHandler(void) {
  // Call the lwIP timer handler.
  lwIPTimer(SYSTICKMS);
}

int main(void) {
  //
  // Make sure the main oscillator is enabled because this is required by
  // the PHY.  The system must have a 25MHz crystal attached to the OSC
  // pins. The SYSCTL_MOSC_HIGHFREQ parameter is used when the crystal
  // frequency is 10MHz or higher.
  //
  SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);

  // Run from the PLL at 120 MHz.
  uint32_t sys_clk =
      MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                              SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                             120000000);

  // Configure the device pins.
  PinoutSet(true, false);

  // Configure UART.
  UARTStdioConfig(0, 115200, sys_clk);
  UARTprintf("Mongoose example\n\n");

  // Configure board's LEDs: D2 is on when IP has been acquired,
  // D1 is toggled on every HTTP request.
  MAP_GPIOPinTypeGPIOOutput(CLP_D1_PORT, CLP_D1_PIN);
  MAP_GPIOPinTypeGPIOOutput(CLP_D2_PORT, CLP_D2_PIN);
  LEDWrite(CLP_D1, 0);
  LEDWrite(CLP_D2, 0);

  //
  // Configure the hardware MAC address for Ethernet Controller filtering of
  // incoming packets.  The MAC address will be stored in the non-volatile
  // USER0 and USER1 registers.
  //
  uint32_t u0, u1;
  MAP_FlashUserGet(&u0, &u1);
  uint8_t mac[6];
  mac[0] = ((u0 >> 0) & 0xff);
  mac[1] = ((u0 >> 8) & 0xff);
  mac[2] = ((u0 >> 16) & 0xff);
  mac[3] = ((u1 >> 0) & 0xff);
  mac[4] = ((u1 >> 8) & 0xff);
  mac[5] = ((u1 >> 16) & 0xff);

  // Initialize the lwIP library, using DHCP.
  lwIPInit(sys_clk, mac, 0, 0, 0, IPADDR_USE_DHCP);

  // Configure Mongoose, create an HTTP listener.
  mg_mgr_init(&g_mgr, NULL);
  const char *err;
  struct mg_bind_opts opts = {};
  opts.error_string = &err;
  struct mg_connection *nc = mg_bind_opt(&g_mgr, "80", ev_handler, opts);
  if (nc == NULL) {
    UARTprintf("Failed to create listener: %s\r\n", err);
    return 1;
  }
  mg_set_protocol_http_websocket(nc);

  // Configure SysTick for a periodic interrupt.
  MAP_SysTickPeriodSet(sys_clk / SYSTICKHZ);
  MAP_SysTickEnable();
  MAP_SysTickIntEnable();

  // Loop forever.  All the work is done in interrupt handlers.
  while (true) {
  }
}
