/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * This example shows how mongoose can be used as a reverse
 * proxy for another http server.
 *
 * A common setup is to have a frontend web server that delegates
 * some urls to a backend web server.
 *
 * In this example we create two webservers. The frontend listens on port
 * 8000 and servers a static file and forwards any call matching the /api prefix
 * to the backend.
 *
 * The backend listens on port 8001 and replies a simple JSON object which
 * shows the request URI that the backend http server receives.
 *
 * Things to try out:
 *
 * curl http://localhost:8000/
 * curl http://localhost:8000/api
 * curl http://localhost:8000/api/foo
 * curl http://localhost:8001/foo
 *
 * The reverse proxy functionality is enabled via the url rewrite functionality:
 *
 * ```
 *   s_frontend_server_opts.url_rewrites =
 *     "/api=http://localhost:8001,/=frontend/hello.html";
 * ```
 *
 * This example maps the /api to a remote http server, and / to a
 * specific file on the filesystem.
 *
 * Obviously you can use any http server as the backend, we spawn
 * another web server from the same process in order to make the example easy
 * to run.
 */

#include "../../mongoose.h"

static const char *s_frontend_port = "8000";
static struct mg_serve_http_opts s_frontend_server_opts;

static const char *s_backend_port = "8001";
static struct mg_serve_http_opts s_backend_server_opts;

static void frontend_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      mg_serve_http(nc, hm, s_frontend_server_opts); /* Serve static content */
      break;
    default:
      break;
  }
}

static void backend_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  int i;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      mg_send_response_line(nc, 200,
                            "Content-Type: text/html\r\n"
                            "Connection: close\r\n");
      mg_printf(nc,
                "{\"uri\": \"%.*s\", \"method\": \"%.*s\", \"body\": \"%.*s\", "
                "\"headers\": {",
                (int) hm->uri.len, hm->uri.p, (int) hm->method.len,
                hm->method.p, (int) hm->body.len, hm->body.p);

      for (i = 0; i < MG_MAX_HTTP_HEADERS && hm->header_names[i].len > 0; i++) {
        struct mg_str hn = hm->header_names[i];
        struct mg_str hv = hm->header_values[i];
        mg_printf(nc, "%s\"%.*s\": \"%.*s\"", (i != 0 ? "," : ""), (int) hn.len,
                  hn.p, (int) hv.len, hv.p);
      }

      mg_printf(nc, "}}");

      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    default:
      break;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  int i;

  /* Open listening socket */
  mg_mgr_init(&mgr, NULL);

  /* configure frontend web server */
  nc = mg_bind(&mgr, s_frontend_port, frontend_handler);
  mg_set_protocol_http_websocket(nc);

  s_frontend_server_opts.document_root = "frontend";
  s_frontend_server_opts.url_rewrites =
      "/api=http://localhost:8001,/=frontend/hello.html";

  /* configure backend web server */
  nc = mg_bind(&mgr, s_backend_port, backend_handler);
  mg_set_protocol_http_websocket(nc);

  s_backend_server_opts.document_root = "backend";

  /* Parse command line arguments */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-D") == 0) {
      mgr.hexdump_file = argv[++i];
    } else if (strcmp(argv[i], "-r") == 0) {
      s_frontend_server_opts.document_root = argv[++i];
    }
  }

  printf("Starting web server on port %s\n", s_frontend_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
}
