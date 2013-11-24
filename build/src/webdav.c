#include "internal.h"

// Writes PROPFIND properties for a collection element
static void print_props(struct mg_connection *conn, const char* uri,
                        struct file *filep) {
  char mtime[64];
  gmt_time_string(mtime, sizeof(mtime), &filep->modification_time);
  conn->num_bytes_sent += mg_printf(conn,
      "<d:response>"
       "<d:href>%s</d:href>"
       "<d:propstat>"
        "<d:prop>"
         "<d:resourcetype>%s</d:resourcetype>"
         "<d:getcontentlength>%" INT64_FMT "</d:getcontentlength>"
         "<d:getlastmodified>%s</d:getlastmodified>"
        "</d:prop>"
        "<d:status>HTTP/1.1 200 OK</d:status>"
       "</d:propstat>"
      "</d:response>\n",
      uri,
      filep->is_directory ? "<d:collection/>" : "",
      filep->size,
      mtime);
}

static void print_dav_dir_entry(struct de *de, void *data) {
  char href[PATH_MAX];
  char href_encoded[PATH_MAX];
  struct mg_connection *conn = (struct mg_connection *) data;
  mg_snprintf(href, sizeof(href), "%s%s",
              conn->request_info.uri, de->file_name);
  mg_url_encode(href, href_encoded, PATH_MAX-1);
  print_props(conn, href_encoded, &de->file);
}

static void handle_propfind(struct mg_connection *conn, const char *path,
                            struct file *filep) {
  const char *depth = mg_get_header(conn, "Depth");

  conn->must_close = 1;
  conn->status_code = 207;
  mg_printf(conn, "HTTP/1.1 207 Multi-Status\r\n"
            "Connection: close\r\n"
            "Content-Type: text/xml; charset=utf-8\r\n\r\n");

  conn->num_bytes_sent += mg_printf(conn,
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
      "<d:multistatus xmlns:d='DAV:'>\n");

  // Print properties for the requested resource itself
  print_props(conn, conn->request_info.uri, filep);

  // If it is a directory, print directory entries too if Depth is not 0
  if (filep->is_directory &&
      !mg_strcasecmp(conn->ctx->config[ENABLE_DIRECTORY_LISTING], "yes") &&
      (depth == NULL || strcmp(depth, "0") != 0)) {
    scan_directory(conn, path, conn, &print_dav_dir_entry);
  }

  conn->num_bytes_sent += mg_printf(conn, "%s\n", "</d:multistatus>");
}
