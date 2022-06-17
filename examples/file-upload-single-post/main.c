// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Streaming upload example. Demonstrates how to use chunked encoding
// to send large payload in smaller chunks. To test, use curl utility:
//
// curl http://localhost:8000/upload?name=a.txt --data-binary @large_file.txt

#include "mongoose.h"

// HTTP request handler function. It implements the following endpoints:
//   /upload - Saves the next file chunk
//   all other URI - serves web_root/ directory
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_http_message *hm = (struct mg_http_message *) ev_data;
  if (ev == MG_EV_HTTP_CHUNK && mg_http_match_uri(hm, "/upload")) {
    MG_INFO(("Got chunk len %lu", (unsigned long) hm->chunk.len));
    MG_INFO(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
    // MG_INFO(("Chunk data:\n%.*s", (int) hm->chunk.len, hm->chunk.ptr));
    mg_http_delete_chunk(c, hm);
    if (hm->chunk.len == 0) {
      MG_INFO(("Last chunk received, sending response"));
      mg_http_reply(c, 200, "", "ok (chunked)\n");
    }
  } else if (ev == MG_EV_HTTP_MSG && mg_http_match_uri(hm, "/upload")) {
    MG_INFO(("Got all %lu bytes!", (unsigned long) hm->body.len));
    MG_INFO(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
    // MG_INFO(("Body:\n%.*s", (int) hm->body.len, hm->body.ptr));
    mg_http_reply(c, 200, "", "ok (%lu)\n", (unsigned long) hm->body.len);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_serve_opts opts = {.root_dir = "web_root"};
    mg_http_serve_dir(c, hm, &opts);
  }
}

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_log_set("3");
  mg_http_listen(&mgr, "http://localhost:8000", cb, NULL);

  for (;;) mg_mgr_poll(&mgr, 50);
  mg_mgr_free(&mgr);

  return 0;
}
