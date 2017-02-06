/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "data.h"

#include <stdio.h>
#include <stdlib.h>

#include "gpio_if.h"

#include "bm222.h"
#include "tmp006.h"
#include "cs_dbg.h"

struct temp_data {
  double ts;
  double volt;
  double temp;
};

struct conn_state {
  struct temp_data td;
  double last_sent_td;
  unsigned char led;
  double last_sent_led;
  double last_sent_acc;
};

static int s_tmp006_addr;
static struct temp_data s_temp_data;
static struct bm222_ctx *s_accel_ctx;

void data_init_sensors(int tmp006_addr, int bm222_addr) {
  s_tmp006_addr = tmp006_addr;
  if (!tmp006_init(tmp006_addr, TMP006_CONV_2, false)) {
    LOG(LL_ERROR, ("Failed to init temperature sensor"));
  } else {
    LOG(LL_INFO, ("Temperature sensor initialized"));
  }
  s_accel_ctx = bm222_init(bm222_addr);
  if (s_accel_ctx == NULL) {
    LOG(LL_ERROR, ("Failed to init accelerometer"));
  } else {
    LOG(LL_INFO, ("Accelerometer initialized"));
  }
}

void data_collect(void) {
  double volt = tmp006_read_sensor_voltage(s_tmp006_addr);
  double temp = tmp006_read_die_temp(s_tmp006_addr);
  if (volt != TMP006_INVALID_READING && temp != TMP006_INVALID_READING) {
    s_temp_data.temp = temp;
    s_temp_data.volt = volt;
    s_temp_data.ts = mg_time();
    LOG(LL_DEBUG, ("V = %lf mV, T = %lf C", volt, temp));
  }
  bm222_get_data(s_accel_ctx);
}

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
  // TODO(lsm): use proper JSON parser
  int cmd, val;
  if (sscanf((char *) data, "{\"t\":%d,\"v\":%d}", &cmd, &val) != 2) {
    LOG(LL_ERROR, ("Invalid request: %.*s", (int) len, data));
    return;
  }
  if (cmd == 1) {
    switch (val) {
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
        return;
      }
    }
  } else {
    LOG(LL_ERROR, ("Unknown command: %.*s", (int) len, data));
    return;
  }
}

void data_conn_handler(struct mg_connection *nc, int ev, void *ev_data) {
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
