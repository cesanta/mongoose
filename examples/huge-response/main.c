// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved
//
// Example that demonstrates how to send a large responses with limited memory.
// We're going to send a JSON array of many integer values, s_data.
// The idea is to send a response in small chunks, and let the client request
// the next chunk.
// Periodically, s_data changes, which is tracked by s_version.
// Client requests a range and a version, to ensure data integrity.
//
//  1. Start this server, type `make`
//  2. Open http://localhost:8000 in your browser

#include "mongoose.h"

static const char *s_listen_on = "http://localhost:8000";
static const char *s_root_dir = "web_root";

#define DATA_SIZE 10000        // Total number of elements
#define CHUNK_SIZE 100         // Max number returned in one API call
static int s_data[DATA_SIZE];  // Simulate some complex big data
static long s_version = 0;     // Data "version"

static long getparam(struct mg_http_message *hm, const char *json_path) {
  double dv = 0;
  mg_json_get_num(hm->body, json_path, &dv);
  return dv;
}

static size_t printdata(mg_pfn_t out, void *ptr, va_list *ap) {
  unsigned start = va_arg(*ap, unsigned);
  unsigned max = start + CHUNK_SIZE;
  const char *comma = "";
  size_t n = 0;
  if (max > DATA_SIZE) max = DATA_SIZE;
  while (start < max) {
    n += mg_xprintf(out, ptr, "%s%d", comma, s_data[start]);
    comma = ",";
    start++;
  }
  return n;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = ev_data;
    if (mg_http_match_uri(hm, "/api/data")) {
      const char *headers = "content-type: text/json\r\n";
      long start = getparam(hm, "$.start");
      long version = getparam(hm, "$.version");
      MG_DEBUG(("%.*s", (int) hm->body.len, hm->body.ptr));
      if (version > 0 && version != s_version) {
        // Version mismatch: s_data has changed while client fetches it
        // Tell client to restart
        mg_http_reply(c, 200, headers, "{%m:%m, %m:%ld}", MG_ESC("error"),
                      MG_ESC("wrong version"), MG_ESC("version"), version);
      } else {
        // Return data, up to CHUNK_SIZE elements
        mg_http_reply(c, 200, headers, "{%m:%ld,%m:%ld,%m:[%M]}",
                      MG_ESC("version"), s_version, MG_ESC("start"), start,
                      MG_ESC("data"), printdata, start);
      }
    } else {
      struct mg_http_serve_opts opts = {0};
      opts.root_dir = s_root_dir;
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
  MG_INFO(("Listening on %s", s_listen_on));
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
