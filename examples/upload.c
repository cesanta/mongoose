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

static void *callback(enum mg_event event, struct mg_connection *conn) {
  if (event == MG_NEW_REQUEST) {
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
    // Mark as processed
    return "";
  } else if (event == MG_UPLOAD) {
    mg_printf(conn, "Saved [%s]", mg_get_request_info(conn)->ev_data);
  }

  return NULL;
}

int main(void) {
  struct mg_context *ctx;
  const char *options[] = {"listening_ports", "8080", NULL};

  ctx = mg_start(&callback, NULL, options);
  getchar();  // Wait until user hits "enter"
  pause();
  mg_stop(ctx);

  return 0;
}
