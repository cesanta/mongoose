// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

struct event {
  int index;
  char message[100];
};

static struct event s_event;

static void read_event(void) {
  mg_snprintf(s_event.message, sizeof(s_event.message), "my ev %d",
              s_event.index);
}

static void write_event(void) {
  // Do nothing. Dashboard sets the s_event.index
}

static struct mg_field fields_event[] = {
    {"index", MG_VAL_INT, &s_event.index, sizeof(s_event.index)},
    {"message", MG_VAL_STR, &s_event.message, sizeof(s_event.message)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static struct mg_field_set field_set_event = {
    "event", fields_event, read_event, write_event, 0, 0, NULL,
};

void mg_dash_init(struct mg_mgr *mgr) {
  static struct mg_dash dash;  // Important: keep it static!
  MG_DASH_ADD_FIELD_SET(&dash, &field_set_event);
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
