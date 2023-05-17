// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// This example demonstrates how to push data over a REST API
// We're going to send a JSON array of some integer values, s_data.
// Periodically, s_data changes, which is tracked by s_version.
// Clients inform their current version, we only send on version mismatch,
// or when version is zero (client has no data); this minimizes traffic and
// avoids updating the UI unnecesarily.
//
//  1. Start this server, type `make`
//  2. Open http://localhost:8000 in your browser

#include "mongoose.h"

static const char *s_listen_on = "http://localhost:8000";
static const char *s_root_dir = "web_root";

#define DATA_SIZE 10           // Total number of elements
static int s_data[DATA_SIZE];  // Simulate some complex data
static long s_version = 0;     // Data "version"

static long getparam(struct mg_http_message *hm, const char *json_path) {
  double dv = 0;
  mg_json_get_num(hm->body, json_path, &dv);
  return dv;
}

static size_t printdata(mg_pfn_t out, void *ptr, va_list *ap) {
  const char *comma = "";
  size_t n = 0;
  for (int i = 0; i < DATA_SIZE; ++i) {
    n += mg_xprintf(out, ptr, "%s%d", comma, s_data[i]);
    comma = ",";
  }
  return n;
  (void) ap;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = ev_data;
    if (mg_http_match_uri(hm, "/api/data")) {
      long version = getparam(hm, "$.version");
      if (version > 0 && version == s_version) {
        // Version match: no changes
        mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                      "{%m:%m,%m:%ld}\n", MG_ESC("status"), MG_ESC("no change"),
                      MG_ESC("version"), version);
      } else {
        // Version mismatch, return data
        mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                      "{%m:%ld,%m:[%M]}\n", MG_ESC("version"), s_version,
                      MG_ESC("data"), printdata);
      }
    } else {
      struct mg_http_serve_opts opts = {.root_dir = s_root_dir};
      mg_http_serve_dir(c, hm, &opts);
    }
  }
  (void) fn_data;
}

static void timer_fn(void *arg) {
  for (int i = 0; i < DATA_SIZE; i++) {
    s_data[i] = rand();
  }
  s_version++;
  (void) arg;
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  srand(time(NULL));
  mg_timer_add(&mgr, 5000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, NULL);
  mg_http_listen(&mgr, s_listen_on, fn, NULL);
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
