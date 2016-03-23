/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Driverlib includes */
#include "hw_types.h"

#include "hw_ints.h"
#include "hw_memmap.h"
#include "interrupt.h"
#include "pin.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "uart.h"
#include "utils.h"

#include "simplelink.h"
#include "device.h"

#include "oslib/osi.h"

#include <mongoose.h>

#define CONSOLE_BAUD_RATE 115200
#define CONSOLE_UART UARTA0_BASE
#define CONSOLE_UART_PERIPH PRCM_UARTA0

#define WIFI_SSID "YourWiFi"
#define WIFI_PASS "YourPassword"

struct sj_event {
  int type;
  void *data;
};

OsiMsgQ_t s_v7_q;
struct mg_mgr mg_mgr;

void SimpleLinkWlanEventHandler(SlWlanEvent_t *e) {
  if (e->Event == SL_WLAN_CONNECT_EVENT) {
    LOG(LL_INFO, ("WiFi: connected"));
  } else {
    LOG(LL_INFO, ("WiFi: event %d", e->Event));
  }
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *e) {
  if (e->Event == SL_NETAPP_IPV4_IPACQUIRED_EVENT) {
    SlIpV4AcquiredAsync_t *ed = &e->EventData.ipAcquiredV4;
    LOG(LL_INFO, ("IP: %lu.%lu.%lu.%lu", SL_IPV4_BYTE(ed->ip, 3),
             SL_IPV4_BYTE(ed->ip, 2), SL_IPV4_BYTE(ed->ip, 1),
             SL_IPV4_BYTE(ed->ip, 0)));
  }
}


static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  LOG(LL_DEBUG, ("Ev: %d", ev));
  if (ev == MG_EV_HTTP_REQUEST) {
    struct mg_serve_http_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.document_root = ".";
    mg_serve_http(nc, (struct http_message *) p, opts);
  }
}

static void mg_task(void *arg) {
  LOG(LL_INFO, ("Hello, world!"));

  osi_MsgQCreate(&s_v7_q, "V7", sizeof(struct sj_event), 32 /* len */);

  sl_Start(NULL, NULL, NULL);

  if (strlen(WIFI_SSID) > 0) {
    int ret;
    SlSecParams_t sp;
    sl_WlanSetMode(ROLE_STA);
    sl_Stop(0);
    sl_Start(NULL, NULL, NULL);
    sl_WlanDisconnect();
    LOG(LL_INFO, ("WiFi: connecting to %s", WIFI_SSID));
    sp.Key = (_i8 *) WIFI_PASS;
    sp.KeyLen = strlen(WIFI_PASS);
    sp.Type = sp.KeyLen ? SL_SEC_TYPE_WPA : SL_SEC_TYPE_OPEN;
    ret = sl_WlanConnect((const _i8 *) WIFI_SSID, strlen(WIFI_SSID), 0, &sp, 0);
    if (ret != 0) {
      LOG(LL_ERROR, ("WlanConnect error: %d\n", ret));
    }
  } else {
    LOG(LL_INFO, ("Not connecting to WiFi."));
    sl_Start(NULL, NULL, NULL);
  }

  /* We don't need TI's web server. */
  sl_NetAppStop(SL_NET_APP_HTTP_SERVER_ID);

  mg_mgr_init(&mg_mgr, NULL);

  const char *err = "";
  struct mg_bind_opts opts;
  memset(&opts, 0, sizeof(opts));
  opts.error_string = &err;

  struct mg_connection *nc = mg_bind(&mg_mgr, "80", ev_handler);
  if (nc != NULL) {
    mg_set_protocol_http_websocket(nc);
  } else {
    LOG(LL_ERROR, ("Failed to create listener: %s", err));
  }

  while (1) {
    struct sj_event e;
    mg_mgr_poll(&mg_mgr, 0);
    if (osi_MsgQRead(&s_v7_q, &e, 1000) != OSI_OK) continue;
  }
}

/* Int vector table, defined in startup_gcc.c */
extern void (*const g_pfnVectors[])(void);

int main() {
  MAP_IntVTableBaseSet((unsigned long) &g_pfnVectors[0]);
  MAP_IntEnable(FAULT_SYSTICK);
  MAP_IntMasterEnable();
  PRCMCC3200MCUInit();

  /* Console UART init. */
  MAP_PRCMPeripheralClkEnable(CONSOLE_UART_PERIPH, PRCM_RUN_MODE_CLK);
  MAP_PinTypeUART(PIN_55, PIN_MODE_3); /* PIN_55 -> UART0_TX */
  MAP_PinTypeUART(PIN_57, PIN_MODE_3); /* PIN_57 -> UART0_RX */
  MAP_UARTConfigSetExpClk(
      CONSOLE_UART, MAP_PRCMPeripheralClockGet(CONSOLE_UART_PERIPH),
      CONSOLE_BAUD_RATE,
      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
  MAP_UARTFIFOLevelSet(CONSOLE_UART, UART_FIFO_TX1_8, UART_FIFO_RX4_8);
  MAP_UARTFIFOEnable(CONSOLE_UART);

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  cs_log_set_level(LL_INFO);

  VStartSimpleLinkSpawnTask(8);
  osi_TaskCreate(mg_task, (const signed char *) "mg", 8192, NULL, 3, NULL);
  osi_start();

  return 0;
}

/* These are FreeRTOS hooks for various life situations. */
void vApplicationMallocFailedHook() {
}

void vApplicationIdleHook() {
}

void vApplicationStackOverflowHook(OsiTaskHandle *th, signed char *tn) {
}

void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *e,
                                  SlHttpServerResponse_t *resp) {
}

void SimpleLinkSockEventHandler(SlSockEvent_t *e) {
}

