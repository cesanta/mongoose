/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"

static const char *s_http_port = "8000";
static const char *s_access_key_id = NULL;
static const char *s_secret_access_key = NULL;
static struct mg_serve_http_opts s_http_server_opts;

static void send_error_result(struct mg_connection *nc, const char *msg) {
  mg_printf_http_chunk(nc, "Error: %s", msg);
  mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

static void link_conns(struct mg_connection *nc1, struct mg_connection *nc2) {
  nc1->user_data = nc2;
  nc2->user_data = nc1;
}

static void unlink_conns(struct mg_connection *nc1) {
  struct mg_connection *nc2 = (struct mg_connection *) nc1->user_data;
  if (nc1->user_data != NULL) {
    nc1->user_data = NULL;
    nc2->user_data = NULL;
  }
}

/* S3 client handler */
static void s3_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  struct mg_connection *nc2 = (struct mg_connection *) nc->user_data;

  switch (ev) {
    case MG_EV_HTTP_REPLY:
      if (nc2 != NULL) {
        mg_printf_http_chunk(nc2, "%s%.*s",
                             (hm->resp_code == 200 ? "" : "Error: "),
                             (int) hm->message.len, hm->message.p);
        mg_send_http_chunk(nc2, "", 0);
      }
      unlink_conns(nc);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    case MG_EV_CLOSE:
      unlink_conns(nc);
      break;
    default:
      break;
  }
}

static void send_s3_request(struct mg_connection *nc, const char *file_name,
                            const char *file_data, const char *host,
                            const char *bucket) {
  char host_port[100];
  struct mg_connection *s3_conn;

  snprintf(host_port, sizeof(host_port), "%s:80", host);
  s3_conn = mg_connect(nc->mgr, host_port, s3_handler);

  if (s3_conn == NULL) {
    send_error_result(nc, "s3 connection failed");
  } else {
    const char *content_type = "text/plain", *method = "PUT";
    char date[100], to_sign[500], signature[100], sha1[20], req[1000];
    time_t now = time(NULL);

    link_conns(nc, s3_conn);
    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
    mg_set_protocol_http_websocket(s3_conn);

    /* Prepare S3 authorization header */
    snprintf(to_sign, sizeof(to_sign), "%s\n\n%s\n%s\n/%s/%s", method,
             content_type, date, bucket, file_name);
    cs_hmac_sha1((unsigned char *) s_secret_access_key,
                 strlen(s_secret_access_key), (unsigned char *) to_sign,
                 strlen(to_sign), (unsigned char *) sha1);
    mg_base64_encode((unsigned char *) sha1, sizeof(sha1), signature);
    snprintf(req, sizeof(req),
             "%s /%s HTTP/1.1\r\n"
             "Host: %s.%s\r\n"
             "Date: %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %lu\r\n"
             "Authorization: AWS %s:%s\r\n"
             "\r\n",
             method, file_name, bucket, host, date, content_type,
             (unsigned long) strlen(file_data), s_access_key_id, signature);
    mg_printf(s3_conn, "%s%s", req, file_data);
    /* S3 request sent, wait for a reply */
  }
}

static void handle_api_call(struct mg_connection *nc, struct http_message *hm) {
  char file_name[100], file_data[100], host[100], bucket[100];

  /* Get form variables */
  mg_get_http_var(&hm->body, "file_name", file_name, sizeof(file_name));
  mg_get_http_var(&hm->body, "file_data", file_data, sizeof(file_data));
  mg_get_http_var(&hm->body, "host", host, sizeof(host));
  mg_get_http_var(&hm->body, "bucket", bucket, sizeof(bucket));

  /* Send headers */
  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

  /* Send body */
  if (file_name[0] == '\0' || file_data[0] == '\0' || bucket[0] == '\0') {
    send_error_result(nc, "bad input");
  } else {
    send_s3_request(nc, file_name, file_data, host, bucket);
  }
}

/* Server handler */
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      if (mg_vcmp(&hm->uri, "/upload") == 0) {
        handle_api_call(nc, hm); /* Handle RESTful call */
      } else {
        mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
      }
      break;
    case MG_EV_CLOSE:
      unlink_conns(nc);
      break;
    default:
      break;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  int i;
  char *cp;

  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";
  s_http_server_opts.enable_directory_listing = "yes";

  /* Use current binary directory as document root */
  if (argc > 0 && ((cp = strrchr(argv[0], '/')) != NULL ||
                   (cp = strrchr(argv[0], '/')) != NULL)) {
    *cp = '\0';
    s_http_server_opts.document_root = argv[0];
  }

  /* Process command line options to customize HTTP server */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-D") == 0 && i + 1 < argc) {
      mgr.hexdump_file = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      s_http_port = argv[++i];
    } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
      s_access_key_id = argv[++i];
    } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
      s_secret_access_key = argv[++i];
    }
  }

  if (s_access_key_id == NULL || s_secret_access_key == NULL) {
    fprintf(stderr,
            "Usage: %s -a access_key_id -s s_secret_access_key "
            "[-p port] [-D hexdump_file]\n",
            argv[0]);
    exit(1);
  }

  printf("Starting RESTful server on port %s\n", s_http_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
