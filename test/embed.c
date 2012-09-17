// Copyright (c) 2004-2009 Sergey Lyubka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Unit test for the mongoose web server. Tests embedded API.


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include "mongoose.h"

#if !defined(LISTENING_PORT)
#define LISTENING_PORT "23456"
#endif

static const char *standard_reply = "HTTP/1.1 200 OK\r\n"
  "Content-Type: text/plain\r\n"
  "Connection: close\r\n\r\n";

static void test_get_var(struct mg_connection *conn,
                         const struct mg_request_info *ri) {
  char *var, *buf;
  size_t buf_len;
  const char *cl;
  int var_len;

  mg_printf(conn, "%s", standard_reply);

  buf_len = 0;
  var = buf = NULL;
  cl = mg_get_header(conn, "Content-Length");
  mg_printf(conn, "cl: %p\n", cl);
  if ((!strcmp(ri->request_method, "POST") ||
       !strcmp(ri->request_method, "PUT"))
      && cl != NULL) {
    buf_len = atoi(cl);
    buf = malloc(buf_len);
    /* Read in two pieces, to test continuation */
    if (buf_len > 2) {
      mg_read(conn, buf, 2);
      mg_read(conn, buf + 2, buf_len - 2);
    } else {
      mg_read(conn, buf, buf_len);
    }
  } else if (ri->query_string != NULL) {
    buf_len = strlen(ri->query_string);
    buf = malloc(buf_len + 1);
    strcpy(buf, ri->query_string);
  }
  var = malloc(buf_len + 1);
  var_len = mg_get_var(buf, buf_len, "my_var", var, buf_len + 1);
  mg_printf(conn, "Value: [%s]\n", var);
  mg_printf(conn, "Value size: [%d]\n", var_len);
  free(buf);
  free(var);
}

static void test_get_header(struct mg_connection *conn,
                            const struct mg_request_info *ri) {
  const char *value;
  int i;

  mg_printf(conn, "%s", standard_reply);
  printf("HTTP headers: %d\n", ri->num_headers);
  for (i = 0; i < ri->num_headers; i++) {
    printf("[%s]: [%s]\n", ri->http_headers[i].name, ri->http_headers[i].value);
  }

  value = mg_get_header(conn, "Host");
  if (value != NULL) {
    mg_printf(conn, "Value: [%s]", value);
  }
}

static void test_get_request_info(struct mg_connection *conn,
                                  const struct mg_request_info *ri) {
  int i;

  mg_printf(conn, "%s", standard_reply);

  mg_printf(conn, "Method: [%s]\n", ri->request_method);
  mg_printf(conn, "URI: [%s]\n", ri->uri);
  mg_printf(conn, "HTTP version: [%s]\n", ri->http_version);

  for (i = 0; i < ri->num_headers; i++) {
    mg_printf(conn, "HTTP header [%s]: [%s]\n",
              ri->http_headers[i].name,
              ri->http_headers[i].value);
  }

  mg_printf(conn, "Query string: [%s]\n",
            ri->query_string ? ri->query_string: "");
  mg_printf(conn, "Remote IP: [%lu]\n", ri->remote_ip);
  mg_printf(conn, "Remote port: [%d]\n", ri->remote_port);
  mg_printf(conn, "Remote user: [%s]\n",
            ri->remote_user ? ri->remote_user : "");
}

static void test_error(struct mg_connection *conn,
                       const struct mg_request_info *ri) {
  (void) ri;
  mg_printf(conn, "HTTP/1.1 %d XX\r\n"
            "Conntection: close\r\n\r\n", mg_get_reply_status_code(conn));
  mg_printf(conn, "Error: [%d]", mg_get_reply_status_code(conn));
}

static void test_post(struct mg_connection *conn,
                      const struct mg_request_info *ri) {
  const char *cl;
  char *buf;
  int len;

  mg_printf(conn, "%s", standard_reply);
  if (strcmp(ri->request_method, "POST") == 0 &&
      (cl = mg_get_header(conn, "Content-Length")) != NULL) {
    len = atoi(cl);
    if ((buf = malloc(len)) != NULL) {
      mg_write(conn, buf, len);
      free(buf);
    }
  }
}

static const struct test_config {
  enum mg_event event;
  const char *uri;
  void (*func)(struct mg_connection *, const struct mg_request_info *);
} test_config[] = {
  {MG_NEW_REQUEST, "/test_get_header", &test_get_header},
  {MG_NEW_REQUEST, "/test_get_var", &test_get_var},
  {MG_NEW_REQUEST, "/test_get_request_info", &test_get_request_info},
  {MG_NEW_REQUEST, "/test_post", &test_post},
  {MG_HTTP_ERROR, "", &test_error},
  {0, NULL, NULL}
};

static void *callback(enum mg_event event,
                      struct mg_connection *conn) {
  const struct mg_request_info *request_info = mg_get_request_info(conn);
  int i;

  for (i = 0; test_config[i].uri != NULL; i++) {
    if (event == test_config[i].event &&
        (event == MG_HTTP_ERROR ||
         !strcmp(request_info->uri, test_config[i].uri))) {
      test_config[i].func(conn, request_info);
      return "processed";
    }
  }

  return NULL;
}

int main(void) {
  struct mg_context *ctx;
  const char *options[] = {"listening_ports", LISTENING_PORT, NULL};

  ctx = mg_start(callback, NULL, options);
  pause();
  return 0;
}
