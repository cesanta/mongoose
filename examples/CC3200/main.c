/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#include "gpio.h"
#include "gpio_if.h"
#include "i2c_if.h"

#include "simplelink.h"
#include "device.h"

#include "osi.h"

#include "mongoose.h"

#include "bm222.h"
#include "tmp006.h"

#define WIFI_SSID "YourWiFi"
#define WIFI_PASS "YourPassword"

#define DATA_SAMPLING_INTERVAL_MS 20
#define CONSOLE_BAUD_RATE 115200
#define CONSOLE_UART UARTA0_BASE
#define CONSOLE_UART_PERIPH PRCM_UARTA0
#define SYS_CLK 80000000
#define MG_TASK_STACK_SIZE 8192

#define BM222_ADDR 0x18
#define TMP006_ADDR 0x41

struct sj_event {
  int type;
  void *data;
};

OsiMsgQ_t s_v7_q;
struct mg_mgr mg_mgr;

static struct bm222_ctx *s_accel_ctx;

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
    GPIO_IF_LedToggle(MCU_RED_LED_GPIO);
  }
}

static void data_ev_handler(struct mg_connection *nc, int ev, void *p);

struct temp_data {
  double ts;
  double volt;
  double temp;
};

static struct temp_data s_temp_data;

static void mg_ev_handler(struct mg_connection *nc, int ev, void *p) {
  LOG(LL_DEBUG, ("Ev: %d", ev));
  switch (ev) {
    case MG_EV_ACCEPT: {
      char addr[32];
      mg_conn_addr_to_str(
          nc, addr, sizeof(addr),
          MG_SOCK_STRINGIFY_REMOTE | MG_SOCK_STRINGIFY_IP |
          MG_SOCK_STRINGIFY_PORT);
      LOG(LL_INFO, ("%p conn from %s", nc, addr));
      break;
    }
    case MG_EV_HTTP_REQUEST: {
      char addr[32];
      struct http_message *hm = (struct http_message *) p;
      mg_conn_addr_to_str(
          nc, addr, sizeof(addr),
          MG_SOCK_STRINGIFY_REMOTE | MG_SOCK_STRINGIFY_IP |
          MG_SOCK_STRINGIFY_PORT);
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
      nc->ev_timer_time = mg_time(); /* Immediately */
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
      bm222_get_data(s_accel_ctx);
      nc->ev_timer_time = mg_time() + (DATA_SAMPLING_INTERVAL_MS * 0.001);
    }
  }
}

struct conn_state {
  struct temp_data td;
  double last_sent_td;
  unsigned char led;
  double last_sent_led;
  double last_sent_acc;
};

static void send_temp(struct mg_connection *nc, const struct temp_data *td) {
  if (td->ts == 0) return;
  mg_printf_websocket_frame(nc, WEBSOCKET_OP_TEXT,
                            "{\"t\": 0, \"ts\": %lf, \"sv\": %lf, \"dt\": %lf}",
                            td->ts, td->volt, td->temp);
}

static void send_led(struct mg_connection *nc, double ts, unsigned char led) {
  if (ts == 0) return;
  mg_printf_websocket_frame(nc, WEBSOCKET_OP_TEXT,
                            "{\"t\": 1, \"ts\": %lf, \"v\": %d}", ts, led);
}

static void send_acc_sample(struct mg_connection *nc,
                            const struct bm222_sample *s) {
  if (s->ts == 0) return;
  mg_printf_websocket_frame(
      nc, WEBSOCKET_OP_TEXT,
      "{\"t\": 2, \"ts\": %lf, \"x\": %d, \"y\": %d, \"z\": %d}", s->ts, s->x,
      s->y, s->z);
}

static double send_acc_data_since(struct mg_connection *nc,
                                  const struct bm222_ctx *ctx, double since) {
  int i = (ctx->last_index + 1) % BM222_NUM_SAMPLES, n = BM222_NUM_SAMPLES;
  double last_sent_ts = 0;
  for (; n > 0; i = (i + 1) % BM222_NUM_SAMPLES, n--) {
    const struct bm222_sample *s = ctx->data + i;
    if (s->ts <= since) continue;
    send_acc_sample(nc, s);
    last_sent_ts = s->ts;
  }
  return last_sent_ts;
}

static void process_command(struct mg_connection *nc, unsigned char *data,
                            size_t len) {
  struct json_token *toks = parse_json2((const char *) data, len);
  if (toks == NULL) {
    LOG(LL_ERROR, ("Invalid command: %.*s", (int) len, data));
    return;
  }
  struct json_token *t = find_json_token(toks, "t");
  if (t == NULL) {
    LOG(LL_ERROR, ("Missing type field: %.*s", (int) len, data));
    goto out_free;
  }
  if (t->len == 1 && *t->ptr == '1') {
    struct json_token *v = find_json_token(toks, "v");
    if (v == NULL) {
      LOG(LL_ERROR, ("Missing value: %.*s", (int) len, data));
      goto out_free;
    }
    if (v->len != 1) {
      LOG(LL_ERROR, ("Invalid value: %.*s", (int) len, data));
      goto out_free;
    }
    switch (*v->ptr) {
      case '0': {
        GPIO_IF_LedOff(MCU_RED_LED_GPIO);
        break;
      }
      case '1': {
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
        break;
      }
      case '2': {
        GPIO_IF_LedToggle(MCU_RED_LED_GPIO);
        break;
      }
      default: {
        LOG(LL_ERROR, ("Invalid value: %.*s", (int) len, data));
        goto out_free;
      }
    }
  } else {
    LOG(LL_ERROR, ("Unknown command: %.*s", (int) t->len, t->ptr));
    goto out_free;
  }
out_free:
  free(toks);
}

static void data_ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct conn_state *cs = (struct conn_state *) nc->user_data;
  if (cs == NULL) {
    cs = (struct conn_state *) calloc(1, sizeof(*cs));
    nc->user_data = cs;
  }
  switch (ev) {
    case MG_EV_POLL:
    case MG_EV_TIMER: {
      const double now = mg_time();
      const unsigned char led = GPIO_IF_LedStatus(MCU_RED_LED_GPIO);
      /* Send if there was a change or repeat last data every second. */
      if (s_temp_data.volt != cs->td.volt || s_temp_data.temp != cs->td.temp ||
          now > cs->last_sent_td + 1.0) {
        memcpy(&cs->td, &s_temp_data, sizeof(cs->td));
        send_temp(nc, &cs->td);
        cs->last_sent_td = now;
      }
      if (led != cs->led || now > cs->last_sent_led + 1.0) {
        send_led(nc, now, led);
        cs->led = led;
        cs->last_sent_led = now;
      }
      if (s_accel_ctx != NULL) {
        const struct bm222_sample *ls =
            s_accel_ctx->data + s_accel_ctx->last_index;
        if (cs->last_sent_acc == 0) {
          send_acc_sample(nc, ls);
          cs->last_sent_acc = ls->ts;
        } else {
          double last_sent_ts =
              send_acc_data_since(nc, s_accel_ctx, cs->last_sent_acc);
          if (last_sent_ts > 0) cs->last_sent_acc = last_sent_ts;
        }
      }
      nc->ev_timer_time = now + 0.05;
      break;
    }
    case MG_EV_WEBSOCKET_FRAME: {
      struct websocket_message *wm = (struct websocket_message *) ev_data;
      process_command(nc, wm->data, wm->size);
      break;
    }
    case MG_EV_CLOSE: {
      LOG(LL_INFO, ("%p closed", nc));
      free(cs);
      break;
    }
  }
}

static void mg_task(void *arg) {
  LOG(LL_INFO, ("MG task running"));
  GPIO_IF_LedToggle(MCU_RED_LED_GPIO);

  osi_MsgQCreate(&s_v7_q, "V7", sizeof(struct sj_event), 32 /* len */);

  sl_Start(NULL, NULL, NULL);
  if (!tmp006_init(TMP006_ADDR, TMP006_CONV_2, false)) {
    LOG(LL_ERROR, ("Failed to init temperature sensor"));
  } else {
    LOG(LL_INFO, ("Temperature sensor initialized"));
  }
  s_accel_ctx = bm222_init(BM222_ADDR);
  if (s_accel_ctx == NULL) {
    LOG(LL_ERROR, ("Failed to init accelerometer"));
  } else {
    LOG(LL_INFO, ("Accelerometer initialized"));
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

  struct mg_connection *nc = mg_bind(&mg_mgr, "80", mg_ev_handler);
  if (nc != NULL) {
    mg_set_protocol_http_websocket(nc);
    nc->ev_timer_time = mg_time(); /* Start data collection */
  } else {
    LOG(LL_ERROR, ("Failed to create listener: %s", err));
  }

  while (1) {
    struct sj_event e;
    mg_mgr_poll(&mg_mgr, 0);
    if (osi_MsgQRead(&s_v7_q, &e, 1) != OSI_OK) continue;
  }
}

#ifndef USE_TIRTOS
/* Int vector table, defined in startup_gcc.c */
extern void (*const g_pfnVectors[])(void);
#endif

int main() {
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
  GPIO_IF_LedToggle(MCU_RED_LED_GPIO);

  if (VStartSimpleLinkSpawnTask(8) != 0) {
	LOG(LL_ERROR, ("Failed to create SL task"));
  }
  if (osi_TaskCreate(mg_task, (const signed char *) "mg", MG_TASK_STACK_SIZE, NULL, 3, NULL) != 0) {
	LOG(LL_ERROR, ("Failed to create MG task"));
  }

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
