/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <inc/hw_types.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>

#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/pin.h>
#include <driverlib/prcm.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/uart.h>
#include <driverlib/utils.h>

#include <example/common/gpio_if.h>
#include <example/common/i2c_if.h>

/* Mongoose.h brings in SimpleLink support. Do not include simplelink.h. */
#include <mongoose.h>
#include "cs_dbg.h"
#include <simplelink/include/device.h>

#include "data.h"
#include "wifi.h"

/* Set up an AP or connect to existing WiFi network. */
#define WIFI_AP_SSID "Mongoose"
#define WIFI_AP_PASS ""
#define WIFI_AP_CHAN 6
// #define WIFI_STA_SSID "YourWiFi"
// #define WIFI_STA_PASS "YourPass"

#define DATA_COLLECTION_INTERVAL_MS 20

#define CONSOLE_BAUD_RATE 115200
#define CONSOLE_UART UARTA0_BASE
#define CONSOLE_UART_PERIPH PRCM_UARTA0
#define MGOS_TASK_PRIORITY 3
#define MG_TASK_STACK_SIZE 8192

#define BM222_ADDR 0x18
#define TMP006_ADDR 0x41

void fs_slfs_set_new_file_size(const char *name, size_t size);

static const char *upload_form =
    "\
<h1>Upload file</h1> \
<form action='/upload' method='POST' enctype='multipart/form-data'> \
  <input type='file' name='file'> \
  <input type='submit' value='Upload'> \
</form>";

static struct mg_str upload_fname(struct mg_connection *nc,
                                  struct mg_str fname) {
  struct mg_str lfn;
  char *fn = malloc(fname.len + 4);
  memcpy(fn, "SL:", 3);
  memcpy(fn + 3, fname.p, fname.len);
  fn[3 + fname.len] = '\0';
  if (nc->user_data != NULL) {
    intptr_t cl = (intptr_t) nc->user_data;
    if (cl >= 0) {
      fs_slfs_set_new_file_size(fn + 3, cl);
    }
  }
  lfn.len = fname.len + 4;
  lfn.p = fn;
  return lfn;
}

static void mg_ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    case MG_EV_ACCEPT: {
      char addr[32];
      mg_conn_addr_to_str(nc, addr, sizeof(addr), MG_SOCK_STRINGIFY_REMOTE |
                                                      MG_SOCK_STRINGIFY_IP |
                                                      MG_SOCK_STRINGIFY_PORT);
      LOG(LL_INFO, ("%p conn from %s", nc, addr));
      break;
    }
    case MG_EV_HTTP_REQUEST: {
      char addr[32];
      struct http_message *hm = (struct http_message *) ev_data;
      cs_stat_t st;
      mg_conn_addr_to_str(nc, addr, sizeof(addr), MG_SOCK_STRINGIFY_REMOTE |
                                                      MG_SOCK_STRINGIFY_IP |
                                                      MG_SOCK_STRINGIFY_PORT);
      LOG(LL_INFO,
          ("HTTP request from %s: %.*s %.*s", addr, (int) hm->method.len,
           hm->method.p, (int) hm->uri.len, hm->uri.p));
      if (mg_vcmp(&hm->uri, "/upload") == 0 ||
          (mg_vcmp(&hm->uri, "/") == 0 && mg_stat("SL:index.html", &st) != 0)) {
        mg_send(nc, upload_form, strlen(upload_form));
        nc->flags |= MG_F_SEND_AND_CLOSE;
        break;
      }
      struct mg_serve_http_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.document_root = "SL:";
      mg_serve_http(nc, hm, opts);
      break;
    }
    case MG_EV_CLOSE: {
      LOG(LL_INFO, ("%p closed", nc));
      break;
    }
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      LOG(LL_INFO, ("%p switching to data mode", nc));
      nc->handler = data_conn_handler;
      nc->ev_timer_time = mg_time(); /* Immediately */
      break;
    }
    case MG_EV_TIMER: {
      data_collect();
      nc->ev_timer_time = mg_time() + (DATA_COLLECTION_INTERVAL_MS * 0.001);
      break;
    }
    /* SimpleLink FS requires pre-declaring max file size. We use Content-Length
     * for that purpose - it will not exactly match file size, but is guaranteed
     * to exceed it and should be close enough. */
    case MG_EV_HTTP_MULTIPART_REQUEST: {
      struct http_message *hm = (struct http_message *) ev_data;
      struct mg_str *cl_header = mg_get_http_header(hm, "Content-Length");
      intptr_t cl = -1;
      if (cl_header != NULL && cl_header->len < 20) {
        char buf[20];
        memcpy(buf, cl_header->p, cl_header->len);
        buf[cl_header->len] = '\0';
        cl = atoi(buf);
        if (cl < 0) cl = -1;
      }
      nc->user_data = (void *) cl;
      break;
    }
    case MG_EV_HTTP_PART_BEGIN:
    case MG_EV_HTTP_PART_DATA:
    case MG_EV_HTTP_PART_END: {
      struct mg_http_multipart_part *mp =
          (struct mg_http_multipart_part *) ev_data;
      if (ev == MG_EV_HTTP_PART_BEGIN) {
        LOG(LL_INFO, ("Begin file upload: %s", mp->file_name));
      } else if (ev == MG_EV_HTTP_PART_END) {
        LOG(LL_INFO, ("End file upload: %s", mp->file_name));
      }
      mg_file_upload_handler(nc, ev, ev_data, upload_fname);
    }
  }
}

static void mg_init(struct mg_mgr *mgr) {
  LOG(LL_INFO, ("MG task running"));

  stop_nwp(); /* See function description in wifi.c */
  LOG(LL_INFO, ("Starting NWP..."));
  int role = sl_Start(0, 0, 0);
  if (role < 0) {
    LOG(LL_ERROR, ("Failed to start NWP"));
    return;
  }

  {
    SlVersionFull ver;
    unsigned char opt = SL_DEVICE_GENERAL_VERSION;
    unsigned char len = sizeof(ver);

    memset(&ver, 0, sizeof(ver));
    sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &opt, &len,
              (unsigned char *) (&ver));
    LOG(LL_INFO, ("NWP v%ld.%ld.%ld.%ld started, host v%ld.%ld.%ld.%ld",
                  ver.NwpVersion[0], ver.NwpVersion[1], ver.NwpVersion[2],
                  ver.NwpVersion[3], SL_MAJOR_VERSION_NUM, SL_MINOR_VERSION_NUM,
                  SL_VERSION_NUM, SL_SUB_VERSION_NUM));
  }

  GPIO_IF_LedToggle(MCU_RED_LED_GPIO);

  data_init_sensors(TMP006_ADDR, BM222_ADDR);

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

  struct mg_connection *nc = mg_bind_opt(mgr, "80", mg_ev_handler, opts);
  if (nc != NULL) {
    mg_set_protocol_http_websocket(nc);
    nc->ev_timer_time = mg_time(); /* Start data collection */
  } else {
    LOG(LL_ERROR, ("Failed to create listener: %s", err));
  }
}

#ifndef USE_TIRTOS
/* Int vector table, defined in startup_gcc.c */
extern void (*const g_pfnVectors[])(void);
#endif

int main(void) {
#ifndef USE_TIRTOS
  MAP_IntVTableBaseSet((unsigned long) &g_pfnVectors[0]);
#endif
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

  setvbuf(stdout, NULL, _IOLBF, 0);
  setvbuf(stderr, NULL, _IOLBF, 0);
  cs_log_set_level(LL_INFO);
  cs_log_set_file(stdout);

  LOG(LL_INFO, ("Hello, world!"));

  MAP_PinTypeI2C(PIN_01, PIN_MODE_1); /* SDA */
  MAP_PinTypeI2C(PIN_02, PIN_MODE_1); /* SCL */
  I2C_IF_Open(I2C_MASTER_MODE_FST);

  /* Set up the red LED. Note that amber and green cannot be used as they share
   * pins with I2C. */
  MAP_PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);
  MAP_PinTypeGPIO(PIN_64, PIN_MODE_0, false);
  MAP_GPIODirModeSet(GPIOA1_BASE, 0x2, GPIO_DIR_MODE_OUT);
  GPIO_IF_LedConfigure(LED1);
  GPIO_IF_LedOn(MCU_RED_LED_GPIO);

  if (VStartSimpleLinkSpawnTask(8) != 0) {
    LOG(LL_ERROR, ("Failed to create SL task"));
  }

  if (!mg_start_task(MGOS_TASK_PRIORITY, MG_TASK_STACK_SIZE, mg_init)) {
    LOG(LL_ERROR, ("Failed to create MG task"));
  }

  osi_start();

  return 0;
}

/* These are FreeRTOS hooks for various life situations. */
void vApplicationMallocFailedHook(void) {
}

void vApplicationIdleHook(void) {
}

void vApplicationStackOverflowHook(OsiTaskHandle *th, signed char *tn) {
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

#ifndef __TI_COMPILER_VERSION__
int _gettimeofday_r(struct _reent *r, struct timeval *tp, void *tz) {
#else
int gettimeofday(struct timeval *tp, void *tz) {
#endif
  unsigned long sec;
  unsigned short msec;
  MAP_PRCMRTCGet(&sec, &msec);
  tp->tv_sec = sec;
  tp->tv_usec = ((unsigned long) msec) * 1000;
  return 0;
}
