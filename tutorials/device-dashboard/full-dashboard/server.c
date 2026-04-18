// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define HTTP_ADDR "http://0.0.0.0:8000"
#else
#define HTTP_ADDR "http://0.0.0.0:80"
#endif

static bool s_led;

static void get_led(union mg_val *val) {
  val->b = s_led;
}

static void set_led(const union mg_val *val) {
  s_led = val->b;
}

// Modify this. This represents device's state to the dashboard
static struct mg_field fields[] = {
    {"led", MG_VAL_BOOL, get_led, set_led},
    {NULL, 0, 0, 0},
};

void run_mongoose(void) {
  struct mg_mgr mgr;
  struct mg_dash dash = {fields};
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, HTTP_ADDR, mg_dash_ev_handler, &dash);
  for (;;) {
    mg_mgr_poll(&mgr, 10);
  }
}

int main(void) {
  run_mongoose();
  return 0;
}
