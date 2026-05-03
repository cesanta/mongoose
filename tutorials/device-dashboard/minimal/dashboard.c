// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

static bool s_led1;
static char s_version[10];

static void read_status(void) {
  MG_INFO(("READING STATUS"));
  mg_snprintf(s_version, sizeof(s_version), "1.2.3");

  // These conditionals make this file work on both microcontroller and desktop
  // Useful for developing the UI on desktop, and copying to an MCU project.
#if MG_ARCH == MG_ARCH_CUBE
  s_led1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);  // Read LED state into field
#endif
}

static void write_status(void) {
  MG_INFO(("WRITING STATUS"));

#if MG_ARCH == MG_ARCH_CUBE
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, s_led1);  // Write LED state from field
#endif
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
  mg_mem_files = mg_packed_files;
  mg_http_listen(mgr, MG_HTTP_ADDR, mg_dash_ev_handler, &dash);
}

void mg_dash_poll(struct mg_mgr *mgr) {
  (void) mgr;
}
