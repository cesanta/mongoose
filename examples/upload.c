// Copyright (c) 2004-2012 Sergey Lyubka
// This file is a part of mongoose project, http://github.com/valenok/mongoose

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include "mongoose.h"

// Make sure that form has enctype="multipart/form-data" attribute
static const char *html_form =
  "<html><body>Upload example."
  "<form method=\"POST\" action=\"/handle_post_request\" "
  "  enctype=\"multipart/form-data\">"
  "<input type=\"file\" name=\"file\" /> <br/>"
  "<input type=\"submit\" value=\"Upload\" />"
  "</form></body></html>";
 
static const char *HTTP_500 = "HTTP/1.0 500 Server Error\r\n\r\n";

static void handle_file_upload(struct mg_connection *conn) {
  const char *cl_header;
  char post_data[16 * 1024], path[999], file_name[1024], mime_type[100],
       buf[BUFSIZ], *eop, *s, *p;
  FILE *fp;
  long long int cl, written;
  int fd, n, post_data_len;

  // Figure out total content length. Return if it is not present or invalid.
  cl_header = mg_get_header(conn, "Content-Length");
  if (cl_header == NULL || (cl = strtoll(cl_header, NULL, 10)) <= 0) {
    mg_printf(conn, "%s%s", HTTP_500, "Invalid Conent-Length");
    return;
  }

  // Read the initial chunk into memory. This should be multipart POST data.
  // Parse headers, where we should find file name and content-type.
  post_data_len = mg_read(conn, post_data, sizeof(post_data));
  file_name[0] = mime_type[0] = '\0';
  for (s = p = post_data; p < &post_data[post_data_len]; p++) {
    if (p[0] == '\r' && p[1] == '\n') {
      if (s == p) {
        p += 2;
        break;  // End of headers
      }
      p[0] = p[1] = '\0';
      sscanf(s, "Content-Type: %99s", mime_type);
      // TODO(lsm): don't expect filename to be the 3rd field,
      // parse the header properly instead.
      sscanf(s, "Content-Disposition: %*s %*s filename=\"%1023[^\"]",
             file_name);
      s = p + 2;
    }
  }

  // Finished parsing headers. Now "p" points to the first byte of data.
  // Calculate file size
  cl -= p - post_data;      // Subtract headers size
  cl -= strlen(post_data);  // Subtract the boundary marker at the end
  cl -= 6;                  // Subtract "\r\n" before and after boundary

  // Construct destination file name. Write to /tmp, do not allow
  // paths that contain slashes.
  if ((s = strrchr(file_name, '/')) == NULL) {
    s = file_name;
  }
  snprintf(path, sizeof(path), "/tmp/%s", s);

  if (file_name[0] == '\0') {
    mg_printf(conn, "%s%s", HTTP_500, "Can't get file name");
  } else if (cl <= 0) {
    mg_printf(conn, "%s%s", HTTP_500, "Empty file");
  } else if ((fd = open(path, O_CREAT | O_TRUNC |
                        O_WRONLY | O_EXLOCK | O_CLOEXEC)) < 0) {
    // We're opening the file with exclusive lock held. This guarantee us that
    // there is no other thread can save into the same file simultaneously.
    mg_printf(conn, "%s%s", HTTP_500, "Cannot open file");
  } else if ((fp = fdopen(fd, "w")) == NULL) {
    mg_printf(conn, "%s%s", HTTP_500, "Cannot reopen file stream");
    close(fd);
  } else {
    // Success. Write data into the file.
    eop = post_data + post_data_len;
    n = p + cl > eop ? (int) (eop - p) : (int) cl;
    (void) fwrite(p, 1, n, fp);
    written = n;
    while (written < cl &&
           (n = mg_read(conn, buf, cl - written > (long long) sizeof(buf) ?
                        sizeof(buf) : cl - written)) > 0) {
      (void) fwrite(buf, 1, n, fp);
      written += n;
    }
    (void) fclose(fp);
    mg_printf(conn, "HTTP/1.0 200 OK\r\n\r\n"
              "Saved to [%s], written %llu bytes", path, cl);
  }
}

static void *callback(enum mg_event event, struct mg_connection *conn) {
  const struct mg_request_info *ri = mg_get_request_info(conn);

  if (event == MG_NEW_REQUEST) {
    if (!strcmp(ri->uri, "/handle_post_request")) {
      handle_file_upload(conn);
    } else {
      // Show HTML form.
      mg_printf(conn, "HTTP/1.0 200 OK\r\n"
                "Content-Length: %d\r\n"
                "Content-Type: text/html\r\n\r\n%s",
                (int) strlen(html_form), html_form);
    }
    // Mark as processed
    return "";
  } else {
    return NULL;
  }
}

int main(void) {
  struct mg_context *ctx;
  const char *options[] = {"listening_ports", "8080", NULL};

  ctx = mg_start(&callback, NULL, options);
  getchar();  // Wait until user hits "enter"
  mg_stop(ctx);

  return 0;
}
