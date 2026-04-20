// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define HTTP_ADDR "http://0.0.0.0:8000"
#else
#define HTTP_ADDR "http://0.0.0.0:80"
#endif

static int s_threshold = 17;
static char s_location[20] = "Galway";

static void location_get(union mg_val *val) {
  val->s = mg_str(s_location);
}
static void location_set(const union mg_val *val) {
  mg_json_unescape(val->s, "$", s_location, sizeof(s_location));
}

static void threshold_get(union mg_val *val) {
  val->i = s_threshold;
}
static void threshold_set(const union mg_val *val) {
  s_threshold = val->i;
}

// Modify this. This represents device's state to the dashboard
static struct mg_field fields[] = {
    {"location", MG_VAL_STR, location_get, location_set},
    {"threshold", MG_VAL_INT, threshold_get, threshold_set},
    {0, 0, 0, 0},
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
