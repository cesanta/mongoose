// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

static bool s_led1;
static char s_version[10];

static void read_status(void) {
  MG_INFO(("READING STATUS"));
  // s_led1 = gpio_read(LED1_PIN);
  mg_snprintf(s_version, sizeof(s_version), "1.2.3");
}

static void write_status(void) {
  MG_INFO(("WRITING STATUS"));
  // gpio_write(LED1_PIN, s_led1);
}

static struct mg_field fields_status[] = {
    {"led1", MG_VAL_BOOL, &s_led1, sizeof(s_led1)},
    {"version", MG_VAL_STR, &s_version, sizeof(s_version)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static struct mg_field_set field_set_status = {
    "status", fields_status, read_status, write_status, 0, 0, NULL,
};

void mg_dash_init(struct mg_mgr *mgr) {
  static struct mg_dash dash;  // Important: keep it static!
  MG_DASH_ADD_FIELD_SET(&dash, &field_set_status);
  mg_http_listen(mgr, MG_HTTP_ADDR, mg_dash_ev_handler, &dash);
}

void mg_dash_poll(struct mg_mgr *mgr) {
  (void) mgr;
}
