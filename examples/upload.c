// Copyright (c) 2004-2012 Sergey Lyubka
// This file is a part of mongoose project, http://github.com/valenok/mongoose

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define strtoll strtol
typedef __int64 int64_t;
#else
#include <inttypes.h>
#include <unistd.h>
#endif // !_WIN32

#include "mongoose.h"

static int begin_request_handler(struct mg_connection *conn) {
  if (!strcmp(mg_get_request_info(conn)->uri, "/handle_post_request")) {
    mg_printf(conn, "%s", "HTTP/1.0 200 OK\r\n\r\n");
    mg_upload(conn, "/tmp");
  } else {
    // Show HTML form. Make sure it has enctype="multipart/form-data" attr.
    static const char *html_form =
      "<html><body>Upload example."
      "<form method=\"POST\" action=\"/handle_post_request\" "
      "  enctype=\"multipart/form-data\">"
      "<input type=\"file\" name=\"file\" /> <br/>"
      "<input type=\"submit\" value=\"Upload\" />"
      "</form></body></html>";

    mg_printf(conn, "HTTP/1.0 200 OK\r\n"
              "Content-Length: %d\r\n"
              "Content-Type: text/html\r\n\r\n%s",
              (int) strlen(html_form), html_form);
  }

  // Mark request as processed
  return 1;
}

static void upload_handler(struct mg_connection *conn, const char *path) {
  mg_printf(conn, "Saved [%s]", path);
}

int main(void) {
  struct mg_context *ctx;
  const char *options[] = {"listening_ports", "8080", NULL};
  struct mg_callbacks callbacks;

  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = begin_request_handler;
  callbacks.upload = upload_handler;
  ctx = mg_start(&callbacks, NULL, options);
  getchar();  // Wait until user hits "enter"
  mg_stop(ctx);

  return 0;
}
