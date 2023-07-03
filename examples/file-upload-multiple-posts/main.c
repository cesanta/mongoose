// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

// HTTP request handler function. It implements the following endpoints:
//   /upload - Saves the next file chunk
//   all other URI - serves web_root/ directory
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/upload")) {
      char path[80], name[64];
      mg_http_get_var(&hm->query, "name", name, sizeof(name));
      mg_snprintf(path, sizeof(path), "/tmp/%s", name);
      if (name[0] == '\0') {
        mg_http_reply(c, 400, "", "%s", "name required");
      } else if (!mg_path_is_sane(path)) {
        mg_http_reply(c, 400, "", "%s", "invalid path");
      } else {
        mg_http_upload(c, hm, &mg_fs_posix, path, 99999);
      }
    } else {
      struct mg_http_serve_opts opts = {.root_dir = "web_root"};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_http_listen(&mgr, "http://localhost:8000", cb, NULL);

  for (;;) mg_mgr_poll(&mgr, 50);
  mg_mgr_free(&mgr);

  return 0;
}
