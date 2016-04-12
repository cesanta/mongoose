/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/* Set up an AP or connect to existing WiFi network. */
#define WIFI_AP_SSID "Mongoose"
#define WIFI_AP_PASS ""
#define WIFI_AP_CHAN 6
//#define WIFI_STA_SSID "YourWiFi"
//#define WIFI_STA_PASS "YourPass"

#define MG_TASK_PRIORITY 3
#define MG_TASK_STACK_SIZE 8192

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/WiFi.h>

/* Example/Board Header file */
#include "Board.h"

/* Mongoose.h brings in SimpleLink support. Do not include simplelink.h. */
#include <mongoose.h>

#include "wifi.h"

void mg_ev_handler(struct mg_connection *nc, int ev, void *p) {
  static const char *reply_fmt =
      "HTTP/1.0 200 OK\r\n"
      "Connection: close\r\n"
      "Content-Type: text/plain\r\n"
      "\r\n"
      "Hello %s\n";
  LOG(LL_DEBUG, ("conn %p ev %d", nc, ev));

  switch (ev) {
    case MG_EV_ACCEPT: {
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      LOG(LL_INFO, ("Connection %p from %s", nc, addr));
      break;
    }
    case MG_EV_HTTP_REQUEST: {
      char addr[32];
      struct http_message *hm = (struct http_message *) p;
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      LOG(LL_INFO,
          ("HTTP request from %s: %.*s %.*s", addr, (int) hm->method.len,
           hm->method.p, (int) hm->uri.len, hm->uri.p));
      mg_printf(nc, reply_fmt, addr);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    }
    case MG_EV_CLOSE: {
      LOG(LL_INFO, ("Connection %p closed", nc));
      break;
    }
  }
}

static void mg_init(struct mg_mgr *mgr) {
  WiFi_Params        wifiParams;
  WiFi_Handle        handle;

  LOG(LL_INFO, ("MG task running"));

  /* Open WiFi driver */
  WiFi_Params_init(&wifiParams);
  wifiParams.bitRate = 2000000;
  handle = WiFi_open(Board_WIFI, Board_WIFI_SPI, NULL, &wifiParams);
  if (handle == NULL) {
    System_abort("WiFi driver failed to open.");
  }

  sl_Start(0, 0, 0);

  sl_fs_init();

#if defined(WIFI_STA_SSID)
  if (!wifi_setup_sta(WIFI_STA_SSID, WIFI_STA_PASS)) {
    LOG(LL_ERROR, ("Error setting up WiFi station"));
  }
#elif defined(WIFI_AP_SSID)
  if (!wifi_setup_ap(WIFI_AP_SSID, WIFI_AP_PASS, WIFI_AP_CHAN)) {
    LOG(LL_ERROR, ("Error setting up WiFi AP"));
  }
#else
#error WiFi not configured
#endif

  /* We don't need SimpleLink's web server. */
  sl_NetAppStop(SL_NET_APP_HTTP_SERVER_ID);

  const char *err = "";
  struct mg_bind_opts opts;
  memset(&opts, 0, sizeof(opts));
  opts.error_string = &err;

  struct mg_connection *nc = mg_bind(mgr, "80", mg_ev_handler);
  if (nc != NULL) {
    mg_set_protocol_http_websocket(nc);
  } else {
    LOG(LL_ERROR, ("Failed to create listener: %s", err));
  }
}

int main(void) {
    Board_initGeneral();
    Board_initGPIO();
    Board_initWiFi();

    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IOLBF, 0);
    cs_log_set_level(LL_INFO);
    cs_log_set_file(stdout);

    if (!mg_start_task(MG_TASK_PRIORITY, MG_TASK_STACK_SIZE, mg_init)) {
      LOG(LL_ERROR, ("Error starting Mongoose task"));
      return 1;
    }

    LOG(LL_INFO, ("OK"));

    osi_start();

    return 0;
}

void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *e,
                                  SlHttpServerResponse_t *resp) {
}

void SimpleLinkSockEventHandler(SlSockEvent_t *e) {
}

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *e) {
  LOG(LL_ERROR, ("status %d sender %d", e->EventData.deviceEvent.status,
                 e->EventData.deviceEvent.sender));
}
