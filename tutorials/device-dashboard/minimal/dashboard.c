// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

static bool s_led1;
static char s_version[10];

static bool status_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  (void) u;
  if (op == MG_DASH_READ) {
    MG_INFO(("READING STATUS"));
    mg_snprintf(s_version, sizeof(s_version), "1.2.3");
    // These conditionals make this file work on both microcontroller and desktop
#if MG_ARCH == MG_ARCH_CUBE
    s_led1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
#endif
    return true;
  }
  if (op == MG_DASH_WRITE) {
    MG_INFO(("WRITING STATUS"));
#if MG_ARCH == MG_ARCH_CUBE
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, s_led1);
#endif
    return true;
  }
  return false;
}

static struct mg_field fields_status[] = {
    {"led1", MG_VAL_BOOL, &s_led1, sizeof(s_led1)},
    {"version", MG_VAL_STR, &s_version, sizeof(s_version)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static struct mg_field_set field_set_status = {
    "status", fields_status, status_fn, NULL, NULL, NULL,
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

// On desktop, build with -DMAIN flag to include main().
// On embedded system, run this code in your main() function
#ifdef MAIN
int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_dash_init(&mgr);

  for (;;) {
    mg_mgr_poll(&mgr, 1);
    mg_dash_poll(&mgr);
  }

  return 0;
}
#endif
