// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static void t_fn(void *arg) {  // Tick periodically
  MG_INFO(("tick"));
  (void) arg;
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) mg_http_reply(c, 200, "", "hi\n");
  (void) ev_data;
}

static struct mg_mgr mgr;

void mgmp_init(void) {
  mg_mgr_init(&mgr);        // Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level

  mg_timer_add(&mgr, 1000, MG_TIMER_REPEAT, t_fn, &mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, &mgr);
}

void mgmp_poll(int t) {
  mg_mgr_poll(&mgr, t);
}
