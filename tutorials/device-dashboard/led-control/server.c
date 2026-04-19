// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define HTTP_ADDR "http://0.0.0.0:8000"
#else
#define HTTP_ADDR "http://0.0.0.0:80"
#endif

static const char *s_version = "1.2.3";
static bool s_led1 = true;  // Mock LED

static void led1_get(union mg_val *val) {
  val->b = s_led1;
}
static void led1_set(const union mg_val *val) {
  s_led1 = val->b;
}

static void firmware_version_get(union mg_val *val) {
  val->s = mg_str(s_version);
}

// Modify this. This represents device's state to the dashboard
static struct mg_field fields[] = {
    {"led1", MG_VAL_BOOL, led1_get, led1_set},
    {"firmware_version", MG_VAL_STR, firmware_version_get, NULL},
    {NULL, 0, 0, 0},
};

struct mg_mgr mgr;
struct mg_dash dash = {fields};

void mongoose_init(void) {
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, HTTP_ADDR, mg_dash_ev_handler, &dash);
}

void mongoose_poll(void) {
  mg_mgr_poll(&mgr, 1);
}

int main(void) {
  mongoose_init();
  for (;;) {
    mongoose_poll();
  }
  return 0;
}
