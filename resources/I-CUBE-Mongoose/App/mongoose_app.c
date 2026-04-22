// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial
//
// See guides:
// https://mongoose.ws/docs/guides/web-dashboard-guide/ - device dashboard

#include "mongoose.h"

#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define HTTP_ADDR "http://0.0.0.0:8000"
#else
#define HTTP_ADDR "http://0.0.0.0:80"
#endif

static struct mg_mgr s_mgr;

// Connection event handler function
static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/hello"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%d}\n", MG_ESC("status"), 1);
    } else {
      mg_http_reply(c, 200, "", "Hello from Mongoose %s\n", MG_VERSION);
    }
  }
}
void mongoose_init(void) {
  mg_mgr_init(&s_mgr);
  mg_http_listen(&s_mgr, HTTP_ADDR, http_ev_handler, NULL);
}

void mongoose_poll(void) {
  mg_mgr_poll(&s_mgr, 1);
}
