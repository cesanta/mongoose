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

#include <stdbool.h>
#include "i2c_if.h"

#include "simplelink.h"
#include "device.h"

#include "oslib/osi.h"

#include "mongoose.h"
#include "tmp006.h"

#define CONSOLE_BAUD_RATE 115200
#define CONSOLE_UART UARTA0_BASE
#define CONSOLE_UART_PERIPH PRCM_UARTA0
#define SYS_CLK 80000000

#define WIFI_SSID "YourWiFi"
#define WIFI_PASS "YourPassword"

#define TMP006_ADDR 0x41

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

static void data_ev_handler(struct mg_connection *nc, int ev, void *p);

struct temp_data {
  double ts;
  double volt;
  double temp;
};

static struct temp_data s_temp_data;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  LOG(LL_DEBUG, ("Ev: %d", ev));
  switch (ev) {
    case MG_EV_ACCEPT: {
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      LOG(LL_INFO, ("%p conn from %s", nc, addr));
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
      struct mg_serve_http_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.document_root = ".";
      mg_serve_http(nc, (struct http_message *) p, opts);
      break;
    }
    case MG_EV_CLOSE: {
      LOG(LL_INFO, ("%p closed", nc));
      break;
    }
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      LOG(LL_INFO, ("%p switching to data mode", nc));
      nc->handler = data_ev_handler;
      nc->ev_timer_time = mg_time();  /* Immediately */
      break;
    }
    case MG_EV_TIMER: {
      double volt = tmp006_read_sensor_voltage(TMP006_ADDR);
      double temp = tmp006_read_die_temp(TMP006_ADDR);
      if (volt != TMP006_INVALID_READING && temp != TMP006_INVALID_READING) {
        s_temp_data.temp = temp;
        s_temp_data.volt = volt;
        s_temp_data.ts = mg_time();
        LOG(LL_DEBUG, ("V = %lf mV, T = %lf C", volt, temp));
      }
      nc->ev_timer_time = mg_time() + 0.05;
    }
  }
}

struct conn_data {
  double last_ts;
  double last_temp;
  double last_volt;
};

static void send_temp(struct mg_connection *nc) {
  LOG(LL_DEBUG, ("%p sending temp data", nc));
  mg_printf_websocket_frame(
      nc, WEBSOCKET_OP_TEXT,
      "{\"type\": \"temp\", \"ts\": %lf, \"v\": %lf, \"t\": %lf}",
      s_temp_data.ts, s_temp_data.volt, s_temp_data.temp);
}

static void data_ev_handler(struct mg_connection *nc, int ev, void *p) {
  struct conn_data *cd = (struct conn_data *) nc->user_data;
  if (cd == NULL) {
    cd = (struct conn_data *) calloc(1, sizeof(*cd));
    nc->user_data = cd;
  }
  switch (ev) {
    case MG_EV_POLL:
    case MG_EV_TIMER: {
      const double now = mg_time();
      /* Send if there was a change or repeat last data every second. */
      if (s_temp_data.volt != cd->last_volt ||
          s_temp_data.temp != cd->last_temp ||
          now > cd->last_ts + 1.0) {
        send_temp(nc);
        cd->last_ts = now;
        cd->last_volt = s_temp_data.volt;
        cd->last_temp = s_temp_data.temp;
      }
      nc->ev_timer_time = now + 0.05;
      break;
    }
    case MG_EV_CLOSE: {
      LOG(LL_INFO, ("%p closed", nc));
      free(cd);
      break;
    }
  }
}

static void mg_task(void *arg) {
  LOG(LL_INFO, ("Hello, world!"));

  osi_MsgQCreate(&s_v7_q, "V7", sizeof(struct sj_event), 32 /* len */);

  sl_Start(NULL, NULL, NULL);
  if (!tmp006_set_config(TMP006_ADDR, TMP006_CONV_2, false)) {
    LOG(LL_ERROR, ("Failed to init temperature sensor"));
  }

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
    nc->ev_timer_time = mg_time();  /* Start data collection */
  } else {
    LOG(LL_ERROR, ("Failed to create listener: %s", err));
  }

  while (1) {
    struct sj_event e;
    mg_mgr_poll(&mg_mgr, 0);
    if (osi_MsgQRead(&s_v7_q, &e, 1) != OSI_OK) continue;
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

  MAP_PinTypeI2C(PIN_01, PIN_MODE_1); /* SDA */
  MAP_PinTypeI2C(PIN_02, PIN_MODE_1); /* SCL */
  I2C_IF_Open(I2C_MASTER_MODE_FST);

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

