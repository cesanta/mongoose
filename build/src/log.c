#include "internal.h"

static void log_header(const struct mg_connection *conn, const char *header,
                       FILE *fp) {
  const char *header_value;

  if ((header_value = mg_get_header(conn, header)) == NULL) {
    (void) fprintf(fp, "%s", " -");
  } else {
    (void) fprintf(fp, " \"%s\"", header_value);
  }
}

static void log_access(const struct mg_connection *conn) {
  const struct mg_request_info *ri;
  FILE *fp;
  char date[64], src_addr[IP_ADDR_STR_LEN];

  fp = conn->ctx->config[ACCESS_LOG_FILE] == NULL ?  NULL :
    fopen(conn->ctx->config[ACCESS_LOG_FILE], "a+");

  if (fp == NULL)
    return;

  strftime(date, sizeof(date), "%d/%b/%Y:%H:%M:%S %z",
           localtime(&conn->birth_time));

  ri = &conn->request_info;
  flockfile(fp);

  sockaddr_to_string(src_addr, sizeof(src_addr), &conn->client.rsa);
  fprintf(fp, "%s - %s [%s] \"%s %s HTTP/%s\" %d %" INT64_FMT,
          src_addr, ri->remote_user == NULL ? "-" : ri->remote_user, date,
          ri->request_method ? ri->request_method : "-",
          ri->uri ? ri->uri : "-", ri->http_version,
          conn->status_code, conn->num_bytes_sent);
  log_header(conn, "Referer", fp);
  log_header(conn, "User-Agent", fp);
  fputc('\n', fp);
  fflush(fp);

  funlockfile(fp);
  fclose(fp);
}
