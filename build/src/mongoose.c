#include "internal.h"

static int call_user(int type, struct mg_connection *conn, void *p) {
  if (conn != NULL && conn->ctx != NULL) {
    conn->event.user_data = conn->ctx->user_data;
    conn->event.type = type;
    conn->event.event_param = p;
    conn->event.request_info = &conn->request_info;
    conn->event.conn = conn;
  }
  return conn == NULL || conn->ctx == NULL || conn->ctx->event_handler == NULL ?
    0 : conn->ctx->event_handler(&conn->event);
}

static int mg_socketpair(sock_t sp[2]) {
  struct sockaddr_in sa;
  sock_t sock, ret = -1;
  socklen_t len = sizeof(sa);

  sp[0] = sp[1] = INVALID_SOCKET;

  (void) memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(0);
  sa.sin_addr.s_addr = htonl(0x7f000001);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET &&
      !bind(sock, (struct sockaddr *) &sa, len) &&
      !listen(sock, 1) &&
      !getsockname(sock, (struct sockaddr *) &sa, &len) &&
      (sp[0] = socket(AF_INET, SOCK_STREAM, 6)) != -1 &&
      !connect(sp[0], (struct sockaddr *) &sa, len) &&
      (sp[1] = accept(sock,(struct sockaddr *) &sa, &len)) != INVALID_SOCKET) {
    set_close_on_exec(sp[0]);
    set_close_on_exec(sp[1]);
    ret = 0;
  } else {
    if (sp[0] != INVALID_SOCKET) closesocket(sp[0]);
    if (sp[1] != INVALID_SOCKET) closesocket(sp[1]);
    sp[0] = sp[1] = INVALID_SOCKET;
  }
  closesocket(sock);

  return ret;
}

static FILE *mg_fopen(const char *path, const char *mode) {
#ifdef _WIN32
  wchar_t wbuf[PATH_MAX], wmode[20];
  to_unicode(path, wbuf, ARRAY_SIZE(wbuf));
  MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, ARRAY_SIZE(wmode));
  return _wfopen(wbuf, wmode);
#else
  return fopen(path, mode);
#endif
}

// Print error message to the opened error log stream.
static void cry(struct mg_connection *conn, const char *fmt, ...) {
  char buf[MG_BUF_LEN], src_addr[IP_ADDR_STR_LEN];
  va_list ap;
  FILE *fp;
  time_t timestamp;

  va_start(ap, fmt);
  (void) vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  // Do not lock when getting the callback value, here and below.
  // I suppose this is fine, since function cannot disappear in the
  // same way string option can.
  if (call_user(MG_EVENT_LOG, conn, buf) == 0) {
    fp = conn->ctx == NULL || conn->ctx->config[ERROR_LOG_FILE] == NULL ? NULL :
      fopen(conn->ctx->config[ERROR_LOG_FILE], "a+");

    if (fp != NULL) {
      flockfile(fp);
      timestamp = time(NULL);

      sockaddr_to_string(src_addr, sizeof(src_addr), &conn->client.rsa);
      fprintf(fp, "[%010lu] [error] [client %s] ", (unsigned long) timestamp,
              src_addr);

      if (conn->request_info.request_method != NULL) {
        fprintf(fp, "%s %s: ", conn->request_info.request_method,
                conn->request_info.uri);
      }

      fprintf(fp, "%s", buf);
      fputc('\n', fp);
      funlockfile(fp);
      fclose(fp);
    }
  }
}

const char *mg_version(void) {
  return MONGOOSE_VERSION;
}

// HTTP 1.1 assumes keep alive if "Connection:" header is not set
// This function must tolerate situations when connection info is not
// set up, for example if request parsing failed.
static int should_keep_alive(const struct mg_connection *conn) {
  const char *http_version = conn->request_info.http_version;
  const char *header = mg_get_header(conn, "Connection");
  if (conn->must_close ||
      conn->status_code == 401 ||
      mg_strcasecmp(conn->ctx->config[ENABLE_KEEP_ALIVE], "yes") != 0 ||
      (header != NULL && mg_strcasecmp(header, "keep-alive") != 0) ||
      (header == NULL && http_version && strcmp(http_version, "1.1"))) {
    return 0;
  }
  return 1;
}

static const char *suggest_connection_header(const struct mg_connection *conn) {
  return should_keep_alive(conn) ? "keep-alive" : "close";
}

static void send_http_error(struct mg_connection *conn, int status,
                            const char *reason, const char *fmt, ...) {
  char buf[MG_BUF_LEN];
  va_list ap;
  int len = 0;

  conn->status_code = status;
  buf[0] = '\0';

  // Errors 1xx, 204 and 304 MUST NOT send a body
  if (status > 199 && status != 204 && status != 304) {
    len = mg_snprintf(buf, sizeof(buf), "Error %d: %s", status, reason);
    buf[len++] = '\n';

    va_start(ap, fmt);
    len += mg_vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);
    va_end(ap);
  }
  DEBUG_TRACE(("[%s]", buf));

  if (call_user(MG_HTTP_ERROR, conn, (void *) (long) status) == 0) {
    mg_printf(conn, "HTTP/1.1 %d %s\r\n"
              "Content-Length: %d\r\n"
              "Connection: %s\r\n\r\n", status, reason, len,
              suggest_connection_header(conn));
    conn->num_bytes_sent += mg_printf(conn, "%s", buf);
  }

  if (status >= 500) {
    conn->must_close = 1;
  }
}

// Return 1 if real file has been found, 0 otherwise
static int convert_uri_to_file_name(struct mg_connection *conn, char *buf,
                                    size_t buf_len, struct file *filep) {
  struct vec a, b;
  const char *rewrite, *uri = conn->request_info.uri,
        *root = conn->ctx->config[DOCUMENT_ROOT];
  char *p;
  int match_len;
  char gz_path[PATH_MAX];
  char const* accept_encoding;

  // No filesystem access
  if (root == NULL) {
    return 0;
  }

  // Using buf_len - 1 because memmove() for PATH_INFO may shift part
  // of the path one byte on the right.
  // If document_root is NULL, leave the file empty.
  mg_snprintf(buf, buf_len - 1, "%s%s", root, uri);

  rewrite = conn->ctx->config[REWRITE];
  while ((rewrite = next_option(rewrite, &a, &b)) != NULL) {
    if ((match_len = match_prefix(a.ptr, a.len, uri)) > 0) {
      mg_snprintf(buf, buf_len - 1, "%.*s%s", (int) b.len, b.ptr,
                  uri + match_len);
      break;
    }
  }

  if (mg_stat(buf, filep)) {
    return 1;
  }

  // if we can't find the actual file, look for the file
  // with the same name but a .gz extension. If we find it,
  // use that and set the gzipped flag in the file struct
  // to indicate that the response need to have the content-
  // encoding: gzip header
  // we can only do this if the browser declares support
  if ((accept_encoding = mg_get_header(conn, "Accept-Encoding")) != NULL) {
    if (strstr(accept_encoding,"gzip") != NULL) {
      snprintf(gz_path, sizeof(gz_path), "%s.gz", buf);
      if (mg_stat(gz_path, filep)) {
        filep->gzipped = 1;
        return 1;
      }
    }
  }

  // Support PATH_INFO for CGI scripts.
  for (p = buf + strlen(root == NULL ? "" : root); *p != '\0'; p++) {
    if (*p == '/') {
      *p = '\0';
      if (match_prefix(conn->ctx->config[CGI_EXTENSIONS],
                       strlen(conn->ctx->config[CGI_EXTENSIONS]), buf) > 0 &&
          mg_stat(buf, filep)) {
        // Shift PATH_INFO block one character right, e.g.
        //  "/x.cgi/foo/bar\x00" => "/x.cgi\x00/foo/bar\x00"
        // conn->path_info is pointing to the local variable "path" declared
        // in handle_request(), so PATH_INFO is not valid after
        // handle_request returns.
        conn->path_info = p + 1;
        memmove(p + 2, p + 1, strlen(p + 1) + 1);  // +1 is for trailing \0
        p[1] = '/';
        return 1;
      } else {
        *p = '/';
      }
    }
  }

  return 0;
}

static void construct_etag(char *buf, size_t buf_len,
                           const struct file *filep) {
  snprintf(buf, buf_len, "\"%lx.%" INT64_FMT "\"",
           (unsigned long) filep->modification_time, filep->size);
}

static void fclose_on_exec(FILE *fp) {
  if (fp != NULL) {
#ifndef _WIN32
    fcntl(fileno(fp), F_SETFD, FD_CLOEXEC);
#endif
  }
}

static void handle_file_request(struct mg_connection *conn, const char *path,
                                struct file *filep) {
  char date[64], lm[64], etag[64], range[64];
  const char *msg = "OK", *hdr;
  time_t curtime = time(NULL);
  int64_t cl, r1, r2;
  struct vec mime_vec;
  int n;
  char gz_path[PATH_MAX];
  char const* encoding = "";
  FILE *fp;

  get_mime_type(conn->ctx, path, &mime_vec);
  cl = filep->size;
  conn->status_code = 200;
  range[0] = '\0';

  // if this file is in fact a pre-gzipped file, rewrite its filename
  // it's important to rewrite the filename after resolving
  // the mime type from it, to preserve the actual file's type
  if (filep->gzipped) {
    snprintf(gz_path, sizeof(gz_path), "%s.gz", path);
    path = gz_path;
    encoding = "Content-Encoding: gzip\r\n";
  }

  if ((fp = mg_fopen(path, "rb")) == NULL) {
    send_http_error(conn, 500, http_500_error,
                    "fopen(%s): %s", path, strerror(ERRNO));
    return;
  }

  fclose_on_exec(fp);

  // If Range: header specified, act accordingly
  r1 = r2 = 0;
  hdr = mg_get_header(conn, "Range");
  if (hdr != NULL && (n = parse_range_header(hdr, &r1, &r2)) > 0 &&
      r1 >= 0 && r2 >= 0) {
    // actually, range requests don't play well with a pre-gzipped
    // file (since the range is specified in the uncmpressed space)
    if (filep->gzipped) {
      send_http_error(conn, 501, "Not Implemented",
                      "range requests in gzipped files are not supported");
      return;
    }
    conn->status_code = 206;
    cl = n == 2 ? (r2 > cl ? cl : r2) - r1 + 1: cl - r1;
    mg_snprintf(range, sizeof(range),
                "Content-Range: bytes "
                "%" INT64_FMT "-%"
                INT64_FMT "/%" INT64_FMT "\r\n",
                r1, r1 + cl - 1, filep->size);
    msg = "Partial Content";
  }

  // Prepare Etag, Date, Last-Modified headers. Must be in UTC, according to
  // http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3
  gmt_time_string(date, sizeof(date), &curtime);
  gmt_time_string(lm, sizeof(lm), &filep->modification_time);
  construct_etag(etag, sizeof(etag), filep);

  (void) mg_printf(conn,
      "HTTP/1.1 %d %s\r\n"
      "Date: %s\r\n"
      "Last-Modified: %s\r\n"
      "Etag: %s\r\n"
      "Content-Type: %.*s\r\n"
      "Content-Length: %" INT64_FMT "\r\n"
      "Connection: %s\r\n"
      "Accept-Ranges: bytes\r\n"
      "%s%s%s\r\n",
      conn->status_code, msg, date, lm, etag, (int) mime_vec.len,
      mime_vec.ptr, cl, suggest_connection_header(conn), range, encoding,
      EXTRA_HTTP_HEADERS);

  if (strcmp(conn->request_info.request_method, "HEAD") != 0) {
    send_file_data(conn, fp, r1, cl);
  }
  fclose(fp);
}

void mg_send_file(struct mg_connection *conn, const char *path) {
  struct file file = STRUCT_FILE_INITIALIZER;
  if (mg_stat(path, &file)) {
    handle_file_request(conn, path, &file);
  } else {
    send_http_error(conn, 404, "Not Found", "%s", "File not found");
  }
}

// For given directory path, substitute it to valid index file.
// Return 0 if index file has been found, -1 if not found.
// If the file is found, it's stats is returned in stp.
static int substitute_index_file(struct mg_connection *conn, char *path,
                                 size_t path_len, struct file *filep) {
  const char *list = conn->ctx->config[INDEX_FILES];
  struct file file = STRUCT_FILE_INITIALIZER;
  struct vec filename_vec;
  size_t n = strlen(path);
  int found = 0;

  // The 'path' given to us points to the directory. Remove all trailing
  // directory separator characters from the end of the path, and
  // then append single directory separator character.
  while (n > 0 && path[n - 1] == '/') {
    n--;
  }
  path[n] = '/';

  // Traverse index files list. For each entry, append it to the given
  // path and see if the file exists. If it exists, break the loop
  while ((list = next_option(list, &filename_vec, NULL)) != NULL) {

    // Ignore too long entries that may overflow path buffer
    if (filename_vec.len > path_len - (n + 2))
      continue;

    // Prepare full path to the index file
    mg_strlcpy(path + n + 1, filename_vec.ptr, filename_vec.len + 1);

    // Does it exist?
    if (mg_stat(path, &file)) {
      // Yes it does, break the loop
      *filep = file;
      found = 1;
      break;
    }
  }

  // If no index file exists, restore directory path
  if (!found) {
    path[n] = '\0';
  }

  return found;
}

// Return True if we should reply 304 Not Modified.
static int is_not_modified(const struct mg_connection *conn,
                           const struct file *filep) {
  char etag[64];
  const char *ims = mg_get_header(conn, "If-Modified-Since");
  const char *inm = mg_get_header(conn, "If-None-Match");
  construct_etag(etag, sizeof(etag), filep);
  return (inm != NULL && !mg_strcasecmp(etag, inm)) ||
    (ims != NULL && filep->modification_time <= parse_date_string(ims));
}

static void mkcol(struct mg_connection *conn, const char *path) {
  int rc, body_len;
  struct de de;

  memset(&de.file, 0, sizeof(de.file));
  mg_stat(path, &de.file);

  if (de.file.modification_time) {
      send_http_error(conn, 405, "Method Not Allowed",
                      "mkcol(%s): %s", path, strerror(ERRNO));
      return;
  }

  body_len = conn->data_len - conn->request_len;
  if(body_len > 0) {
      send_http_error(conn, 415, "Unsupported media type",
                      "mkcol(%s): %s", path, strerror(ERRNO));
      return;
  }

  rc = mg_mkdir(path, 0755);

  if (rc == 0) {
    conn->status_code = 201;
    mg_printf(conn, "HTTP/1.1 %d Created\r\n\r\n", conn->status_code);
  } else if (rc == -1) {
      if(errno == EEXIST)
        send_http_error(conn, 405, "Method Not Allowed",
                      "mkcol(%s): %s", path, strerror(ERRNO));
      else if(errno == EACCES)
          send_http_error(conn, 403, "Forbidden",
                        "mkcol(%s): %s", path, strerror(ERRNO));
      else if(errno == ENOENT)
          send_http_error(conn, 409, "Conflict",
                        "mkcol(%s): %s", path, strerror(ERRNO));
      else
          send_http_error(conn, 500, http_500_error,
                          "fopen(%s): %s", path, strerror(ERRNO));
  }
}

static void put_file(struct mg_connection *conn, const char *path) {
  struct file file = STRUCT_FILE_INITIALIZER;
  FILE *fp;
  const char *range;
  int64_t r1, r2;
  int rc;

  conn->status_code = mg_stat(path, &file) ? 200 : 201;

  if ((rc = put_dir(path)) == 0) {
    mg_printf(conn, "HTTP/1.1 %d OK\r\n\r\n", conn->status_code);
  } else if (rc == -1) {
    send_http_error(conn, 500, http_500_error,
                    "put_dir(%s): %s", path, strerror(ERRNO));
  } else if ((fp = mg_fopen(path, "wb+")) == NULL) {
    fclose(fp);
    send_http_error(conn, 500, http_500_error,
                    "fopen(%s): %s", path, strerror(ERRNO));
  } else {
    fclose_on_exec(fp);
    range = mg_get_header(conn, "Content-Range");
    r1 = r2 = 0;
    if (range != NULL && parse_range_header(range, &r1, &r2) > 0) {
      conn->status_code = 206;
      fseeko(fp, r1, SEEK_SET);
    }
    if (!forward_body_data(conn, fp, INVALID_SOCKET, NULL)) {
      conn->status_code = 500;
    }
    mg_printf(conn, "HTTP/1.1 %d OK\r\nContent-Length: 0\r\n\r\n",
              conn->status_code);
    fclose(fp);
  }
}

static void send_ssi_file(struct mg_connection *, const char *, FILE *, int);
static void do_ssi_include(struct mg_connection *conn, const char *ssi,
                           char *tag, int include_level) {
  char file_name[MG_BUF_LEN], path[PATH_MAX], *p;
  FILE *fp;

  // sscanf() is safe here, since send_ssi_file() also uses buffer
  // of size MG_BUF_LEN to get the tag. So strlen(tag) is always < MG_BUF_LEN.
  if (sscanf(tag, " virtual=\"%[^\"]\"", file_name) == 1) {
    // File name is relative to the webserver root
    (void) mg_snprintf(path, sizeof(path), "%s%c%s",
        conn->ctx->config[DOCUMENT_ROOT], '/', file_name);
  } else if (sscanf(tag, " abspath=\"%[^\"]\"", file_name) == 1) {
    // File name is relative to the webserver working directory
    // or it is absolute system path
    (void) mg_snprintf(path, sizeof(path), "%s", file_name);
  } else if (sscanf(tag, " file=\"%[^\"]\"", file_name) == 1 ||
             sscanf(tag, " \"%[^\"]\"", file_name) == 1) {
    // File name is relative to the currect document
    (void) mg_snprintf(path, sizeof(path), "%s", ssi);
    if ((p = strrchr(path, '/')) != NULL) {
      p[1] = '\0';
    }
    (void) mg_snprintf(path + strlen(path),
        sizeof(path) - strlen(path), "%s", file_name);
  } else {
    cry(conn, "Bad SSI #include: [%s]", tag);
    return;
  }

  if ((fp = mg_fopen(path, "rb")) == NULL) {
    cry(conn, "Cannot open SSI #include: [%s]: fopen(%s): %s",
        tag, path, strerror(ERRNO));
  } else {
    fclose_on_exec(fp);
    if (match_prefix(conn->ctx->config[SSI_EXTENSIONS],
                     strlen(conn->ctx->config[SSI_EXTENSIONS]), path) > 0) {
      send_ssi_file(conn, path, fp, include_level + 1);
    } else {
      send_file_data(conn, fp, 0, INT64_MAX);
    }
    fclose(fp);
  }
}

#if !defined(NO_POPEN)
static void do_ssi_exec(struct mg_connection *conn, char *tag) {
  char cmd[MG_BUF_LEN];
  FILE *fp;

  if (sscanf(tag, " \"%[^\"]\"", cmd) != 1) {
    cry(conn, "Bad SSI #exec: [%s]", tag);
  } else if ((fp = popen(cmd, "r")) == NULL) {
    cry(conn, "Cannot SSI #exec: [%s]: %s", cmd, strerror(ERRNO));
  } else {
    send_file_data(conn, fp, 0, INT64_MAX);
    pclose(fp);
  }
}
#endif // !NO_POPEN

static void send_ssi_file(struct mg_connection *conn, const char *path,
                          FILE *fp, int include_level) {
  char buf[MG_BUF_LEN];
  int ch, offset, len, in_ssi_tag;

  if (include_level > 10) {
    cry(conn, "SSI #include level is too deep (%s)", path);
    return;
  }

  in_ssi_tag = len = offset = 0;
  while ((ch = fgetc(fp)) != EOF) {
    if (in_ssi_tag && ch == '>') {
      in_ssi_tag = 0;
      buf[len++] = (char) ch;
      buf[len] = '\0';
      assert(len <= (int) sizeof(buf));
      if (len < 6 || memcmp(buf, "<!--#", 5) != 0) {
        // Not an SSI tag, pass it
        (void) mg_write(conn, buf, (size_t) len);
      } else {
        if (!memcmp(buf + 5, "include", 7)) {
          do_ssi_include(conn, path, buf + 12, include_level);
#if !defined(NO_POPEN)
        } else if (!memcmp(buf + 5, "exec", 4)) {
          do_ssi_exec(conn, buf + 9);
#endif // !NO_POPEN
        } else {
          cry(conn, "%s: unknown SSI " "command: \"%s\"", path, buf);
        }
      }
      len = 0;
    } else if (in_ssi_tag) {
      if (len == 5 && memcmp(buf, "<!--#", 5) != 0) {
        // Not an SSI tag
        in_ssi_tag = 0;
      } else if (len == (int) sizeof(buf) - 2) {
        cry(conn, "%s: SSI tag is too large", path);
        len = 0;
      }
      buf[len++] = ch & 0xff;
    } else if (ch == '<') {
      in_ssi_tag = 1;
      if (len > 0) {
        mg_write(conn, buf, (size_t) len);
      }
      len = 0;
      buf[len++] = ch & 0xff;
    } else {
      buf[len++] = ch & 0xff;
      if (len == (int) sizeof(buf)) {
        mg_write(conn, buf, (size_t) len);
        len = 0;
      }
    }
  }

  // Send the rest of buffered data
  if (len > 0) {
    mg_write(conn, buf, (size_t) len);
  }
}

static void handle_ssi_file_request(struct mg_connection *conn,
                                    const char *path) {
  struct vec mime_vec;
  FILE *fp;

  if ((fp = mg_fopen(path, "rb")) == NULL) {
    send_http_error(conn, 500, http_500_error, "fopen(%s): %s", path,
                    strerror(ERRNO));
  } else {
    conn->must_close = 1;
    fclose_on_exec(fp);
    get_mime_type(conn->ctx, path, &mime_vec);
    mg_printf(conn, "HTTP/1.1 200 OK\r\n"
              "Content-Type: %.*s\r\n"
              "Connection: close\r\n\r\n",
              (int) mime_vec.len, mime_vec.ptr);
    send_ssi_file(conn, path, fp, 0);
    fclose(fp);
  }
}

static void handle_options_request(struct mg_connection *conn) {
  static const char reply[] = "HTTP/1.1 200 OK\r\n"
    "Allow: GET, POST, HEAD, CONNECT, PUT, DELETE, OPTIONS, PROPFIND, MKCOL\r\n"
    "DAV: 1\r\n\r\n";

  conn->status_code = 200;
  mg_write(conn, reply, sizeof(reply) - 1);
}

static int isbyte(int n) {
  return n >= 0 && n <= 255;
}

static int parse_net(const char *spec, uint32_t *net, uint32_t *mask) {
  int n, a, b, c, d, slash = 32, len = 0;

  if ((sscanf(spec, "%d.%d.%d.%d/%d%n", &a, &b, &c, &d, &slash, &n) == 5 ||
      sscanf(spec, "%d.%d.%d.%d%n", &a, &b, &c, &d, &n) == 4) &&
      isbyte(a) && isbyte(b) && isbyte(c) && isbyte(d) &&
      slash >= 0 && slash < 33) {
    len = n;
    *net = ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | d;
    *mask = slash ? 0xffffffffU << (32 - slash) : 0;
  }

  return len;
}

static int is_put_or_delete_request(const struct mg_connection *conn) {
  const char *s = conn->request_info.request_method;
  return s != NULL && (!strcmp(s, "PUT") ||
                       !strcmp(s, "DELETE") ||
                       !strcmp(s, "MKCOL"));
}

static int get_first_ssl_listener_index(const struct mg_context *ctx) {
  int i, index = -1;
  for (i = 0; index == -1 && i < ctx->num_listening_sockets; i++) {
    index = ctx->listening_sockets[i].is_ssl ? i : -1;
  }
  return index;
}

static void redirect_to_https_port(struct mg_connection *conn, int ssl_index) {
  char host[1025];
  const char *host_header;

  if ((host_header = mg_get_header(conn, "Host")) == NULL ||
      sscanf(host_header, "%1024[^:]", host) == 0) {
    // Cannot get host from the Host: header. Fallback to our IP address.
    sockaddr_to_string(host, sizeof(host), &conn->client.lsa);
  }

  mg_printf(conn, "HTTP/1.1 302 Found\r\nLocation: https://%s:%d%s\r\n\r\n",
            host, (int) ntohs(conn->ctx->listening_sockets[ssl_index].
                              lsa.sin.sin_port), conn->request_info.uri);
}

static void handle_delete_request(struct mg_connection *conn,
                                  const char *path) {
  struct file file = STRUCT_FILE_INITIALIZER;

  if (!mg_stat(path, &file)) {
    send_http_error(conn, 404, "Not Found", "%s", "File not found");
  } else if (!file.modification_time) {
    send_http_error(conn, 500, http_500_error, "remove(%s): %s", path,
                    strerror(ERRNO));
  } else if (file.is_directory) {
    remove_directory(conn, path);
    send_http_error(conn, 204, "No Content", "%s", "");
  } else if (mg_remove(path) == 0) {
    send_http_error(conn, 204, "No Content", "%s", "");
  } else {
    send_http_error(conn, 423, "Locked", "remove(%s): %s", path,
                    strerror(ERRNO));
  }
}

// This is the heart of the Mongoose's logic.
// This function is called when the request is read, parsed and validated,
// and Mongoose must decide what action to take: serve a file, or
// a directory, or call embedded function, etcetera.
static void handle_request(struct mg_connection *conn) {
  struct mg_request_info *ri = &conn->request_info;
  char path[PATH_MAX];
  int uri_len, ssl_index;
  struct file file = STRUCT_FILE_INITIALIZER;

  if ((conn->request_info.query_string = strchr(ri->uri, '?')) != NULL) {
    * ((char *) conn->request_info.query_string++) = '\0';
  }
  uri_len = (int) strlen(ri->uri);
  mg_url_decode(ri->uri, uri_len, (char *) ri->uri, uri_len + 1, 0);
  remove_double_dots_and_double_slashes((char *) ri->uri);
  path[0] = '\0';
  convert_uri_to_file_name(conn, path, sizeof(path), &file);

  // Perform redirect and auth checks before calling begin_request() handler.
  // Otherwise, begin_request() would need to perform auth checks and redirects.
  if (!conn->client.is_ssl && conn->client.ssl_redir &&
      (ssl_index = get_first_ssl_listener_index(conn->ctx)) > -1) {
    redirect_to_https_port(conn, ssl_index);
  } else if (!is_put_or_delete_request(conn) &&
             !check_authorization(conn, path)) {
    send_authorization_request(conn);
  } else if (call_user(MG_REQUEST_BEGIN, conn, (void *) ri->uri) == 1) {
    // Do nothing, callback has served the request
  } else if (!strcmp(ri->request_method, "OPTIONS")) {
    handle_options_request(conn);
  } else if (conn->ctx->config[DOCUMENT_ROOT] == NULL) {
    send_http_error(conn, 404, "Not Found", "Not Found");
  } else if (is_put_or_delete_request(conn) &&
             (is_authorized_for_put(conn) != 1)) {
    send_authorization_request(conn);
  } else if (!strcmp(ri->request_method, "PUT")) {
    put_file(conn, path);
  } else if (!strcmp(ri->request_method, "MKCOL")) {
    mkcol(conn, path);
  } else if (!strcmp(ri->request_method, "DELETE")) {
    handle_delete_request(conn, path);
  } else if (file.modification_time == (time_t) 0 ||
             must_hide_file(conn, path)) {
    send_http_error(conn, 404, "Not Found", "%s", "File not found");
  } else if (file.is_directory && ri->uri[uri_len - 1] != '/') {
    mg_printf(conn, "HTTP/1.1 301 Moved Permanently\r\n"
              "Location: %s/\r\n\r\n", ri->uri);
  } else if (!strcmp(ri->request_method, "PROPFIND")) {
    handle_propfind(conn, path, &file);
  } else if (file.is_directory &&
             !substitute_index_file(conn, path, sizeof(path), &file)) {
    if (!mg_strcasecmp(conn->ctx->config[ENABLE_DIRECTORY_LISTING], "yes")) {
      handle_directory_request(conn, path);
    } else {
      send_http_error(conn, 403, "Directory Listing Denied",
          "Directory listing denied");
    }
#ifdef USE_LUA
  } else if (match_prefix("**.lp$", 6, path) > 0) {
    handle_lsp_request(conn, path, &file, NULL);
#endif
#if !defined(NO_CGI)
  } else if (match_prefix(conn->ctx->config[CGI_EXTENSIONS],
                          strlen(conn->ctx->config[CGI_EXTENSIONS]),
                          path) > 0) {
    if (strcmp(ri->request_method, "POST") &&
        strcmp(ri->request_method, "HEAD") &&
        strcmp(ri->request_method, "GET")) {
      send_http_error(conn, 501, "Not Implemented",
                      "Method %s is not implemented", ri->request_method);
    } else {
      handle_cgi_request(conn, path);
    }
#endif // !NO_CGI
  } else if (match_prefix(conn->ctx->config[SSI_EXTENSIONS],
                          strlen(conn->ctx->config[SSI_EXTENSIONS]),
                          path) > 0) {
    handle_ssi_file_request(conn, path);
  } else if (is_not_modified(conn, &file)) {
    send_http_error(conn, 304, "Not Modified", "%s", "");
  } else {
    handle_file_request(conn, path, &file);
  }
}

static void close_all_listening_sockets(struct mg_context *ctx) {
  int i;
  for (i = 0; i < ctx->num_listening_sockets; i++) {
    closesocket(ctx->listening_sockets[i].sock);
  }
  free(ctx->listening_sockets);
}

static int is_valid_port(unsigned int port) {
  return port > 0 && port < 0xffff;
}

// Valid listening port specification is: [ip_address:]port[s]
// Examples: 80, 443s, 127.0.0.1:3128, 1.2.3.4:8080s
// TODO(lsm): add parsing of the IPv6 address
static int parse_port_string(const struct vec *vec, struct socket *so) {
  unsigned int a, b, c, d, ch, port;
  int len;
#if defined(USE_IPV6)
  char buf[100];
#endif

  // MacOS needs that. If we do not zero it, subsequent bind() will fail.
  // Also, all-zeroes in the socket address means binding to all addresses
  // for both IPv4 and IPv6 (INADDR_ANY and IN6ADDR_ANY_INIT).
  memset(so, 0, sizeof(*so));
  so->lsa.sin.sin_family = AF_INET;

  if (sscanf(vec->ptr, "%u.%u.%u.%u:%u%n", &a, &b, &c, &d, &port, &len) == 5) {
    // Bind to a specific IPv4 address, e.g. 192.168.1.5:8080
    so->lsa.sin.sin_addr.s_addr = htonl((a << 24) | (b << 16) | (c << 8) | d);
    so->lsa.sin.sin_port = htons((uint16_t) port);
#if defined(USE_IPV6)

  } else if (sscanf(vec->ptr, "[%49[^]]]:%d%n", buf, &port, &len) == 2 &&
             inet_pton(AF_INET6, buf, &so->lsa.sin6.sin6_addr)) {
    // IPv6 address, e.g. [3ffe:2a00:100:7031::1]:8080
    so->lsa.sin6.sin6_family = AF_INET6;
    so->lsa.sin6.sin6_port = htons((uint16_t) port);
#endif
  } else if (sscanf(vec->ptr, "%u%n", &port, &len) == 1) {
    // If only port is specified, bind to IPv4, INADDR_ANY
    so->lsa.sin.sin_port = htons((uint16_t) port);
  } else {
    port = len = 0;   // Parsing failure. Make port invalid.
  }

  ch = vec->ptr[len];  // Next character after the port number
  so->is_ssl = ch == 's';
  so->ssl_redir = ch == 'r';

  // Make sure the port is valid and vector ends with 's', 'r' or ','
  return is_valid_port(port) &&
    (ch == '\0' || ch == 's' || ch == 'r' || ch == ',');
}

static int set_ports_option(struct mg_context *ctx) {
  const char *list = ctx->config[LISTENING_PORTS];
  int on = 1, success = 1;
#if defined(USE_IPV6)
  int off = 0;
#endif
  struct vec vec;
  struct socket so, *ptr;

  while (success && (list = next_option(list, &vec, NULL)) != NULL) {
    if (!parse_port_string(&vec, &so)) {
      cry(fc(ctx), "%s: %.*s: invalid port spec. Expecting list of: %s",
          __func__, (int) vec.len, vec.ptr, "[IP_ADDRESS:]PORT[s|r]");
      success = 0;
    } else if (so.is_ssl && ctx->ssl_ctx == NULL) {
      cry(fc(ctx), "Cannot add SSL socket, is -ssl_certificate option set?");
      success = 0;
    } else if ((so.sock = socket(so.lsa.sa.sa_family, SOCK_STREAM, 6)) ==
               INVALID_SOCKET ||
               // On Windows, SO_REUSEADDR is recommended only for
               // broadcast UDP sockets
               setsockopt(so.sock, SOL_SOCKET, SO_REUSEADDR,
                          (void *) &on, sizeof(on)) != 0 ||
#if defined(USE_IPV6)
               (so.lsa.sa.sa_family == AF_INET6 &&
                setsockopt(so.sock, IPPROTO_IPV6, IPV6_V6ONLY, (void *) &off,
                           sizeof(off)) != 0) ||
#endif
               bind(so.sock, &so.lsa.sa, so.lsa.sa.sa_family == AF_INET ?
                    sizeof(so.lsa.sin) : sizeof(so.lsa)) != 0 ||
               listen(so.sock, SOMAXCONN) != 0) {
      cry(fc(ctx), "%s: cannot bind to %.*s: %d (%s)", __func__,
          (int) vec.len, vec.ptr, ERRNO, strerror(errno));
      closesocket(so.sock);
      success = 0;
    } else if ((ptr = (struct socket *) realloc(ctx->listening_sockets,
                              (ctx->num_listening_sockets + 1) *
                              sizeof(ctx->listening_sockets[0]))) == NULL) {
      closesocket(so.sock);
      success = 0;
    } else {
      set_close_on_exec(so.sock);
      ctx->listening_sockets = ptr;
      ctx->listening_sockets[ctx->num_listening_sockets] = so;
      ctx->num_listening_sockets++;
    }
  }

  if (!success) {
    close_all_listening_sockets(ctx);
  }

  return success;
}

// Verify given socket address against the ACL.
// Return -1 if ACL is malformed, 0 if address is disallowed, 1 if allowed.
static int check_acl(struct mg_context *ctx, uint32_t remote_ip) {
  int allowed, flag;
  uint32_t net, mask;
  struct vec vec;
  const char *list = ctx->config[ACCESS_CONTROL_LIST];

  // If any ACL is set, deny by default
  allowed = list == NULL ? '+' : '-';

  while ((list = next_option(list, &vec, NULL)) != NULL) {
    flag = vec.ptr[0];
    if ((flag != '+' && flag != '-') ||
        parse_net(&vec.ptr[1], &net, &mask) == 0) {
      cry(fc(ctx), "%s: subnet must be [+|-]x.x.x.x[/x]", __func__);
      return -1;
    }

    if (net == (remote_ip & mask)) {
      allowed = flag;
    }
  }

  return allowed == '+';
}

#if !defined(_WIN32)
static int set_uid_option(struct mg_context *ctx) {
  struct passwd *pw;
  const char *uid = ctx->config[RUN_AS_USER];
  int success = 0;

  if (uid == NULL) {
    success = 1;
  } else {
    if ((pw = getpwnam(uid)) == NULL) {
      cry(fc(ctx), "%s: unknown user [%s]", __func__, uid);
    } else if (setgid(pw->pw_gid) == -1) {
      cry(fc(ctx), "%s: setgid(%s): %s", __func__, uid, strerror(errno));
    } else if (setuid(pw->pw_uid) == -1) {
      cry(fc(ctx), "%s: setuid(%s): %s", __func__, uid, strerror(errno));
    } else {
      success = 1;
    }
  }

  return success;
}
#endif // !_WIN32

static int set_gpass_option(struct mg_context *ctx) {
  struct file file = STRUCT_FILE_INITIALIZER;
  const char *path = ctx->config[GLOBAL_PASSWORDS_FILE];
  if (path != NULL && !mg_stat(path, &file)) {
    cry(fc(ctx), "Cannot open %s: %s", path, strerror(ERRNO));
    return 0;
  }
  return 1;
}

static int set_acl_option(struct mg_context *ctx) {
  return check_acl(ctx, (uint32_t) 0x7f000001UL) != -1;
}

static void reset_per_request_attributes(struct mg_connection *conn) {
  conn->path_info = NULL;
  conn->num_bytes_sent = conn->num_bytes_read = 0;
  conn->status_code = -1;
  conn->must_close = conn->request_len = 0;
}

static void close_socket_gracefully(struct mg_connection *conn) {
#if defined(_WIN32)
  char buf[MG_BUF_LEN];
  int n;
#endif
  struct linger linger;

  // Set linger option to avoid socket hanging out after close. This prevent
  // ephemeral port exhaust problem under high QPS.
  linger.l_onoff = 1;
  linger.l_linger = 1;
  setsockopt(conn->client.sock, SOL_SOCKET, SO_LINGER,
             (char *) &linger, sizeof(linger));

  // Send FIN to the client
  shutdown(conn->client.sock, SHUT_WR);
  set_non_blocking_mode(conn->client.sock);

#if defined(_WIN32)
  // Read and discard pending incoming data. If we do not do that and close the
  // socket, the data in the send buffer may be discarded. This
  // behaviour is seen on Windows, when client keeps sending data
  // when server decides to close the connection; then when client
  // does recv() it gets no data back.
  do {
    n = pull(NULL, conn, buf, sizeof(buf));
  } while (n > 0);
#endif

  // Now we know that our FIN is ACK-ed, safe to close
  closesocket(conn->client.sock);
}

static void close_connection(struct mg_connection *conn) {
  conn->must_close = 1;

#ifndef NO_SSL
  if (conn->ssl != NULL) {
    // Run SSL_shutdown twice to ensure completly close SSL connection
    SSL_shutdown(conn->ssl);
    SSL_free(conn->ssl);
    conn->ssl = NULL;
  }
#endif
  if (conn->client.sock != INVALID_SOCKET) {
    close_socket_gracefully(conn);
    conn->client.sock = INVALID_SOCKET;
  }
}

void mg_close_connection(struct mg_connection *conn) {
#ifndef NO_SSL
  if (conn->client_ssl_ctx != NULL) {
    SSL_CTX_free((SSL_CTX *) conn->client_ssl_ctx);
  }
#endif
  close_connection(conn);
  free(conn);
}

static int is_valid_uri(const char *uri) {
  // Conform to http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5.1.2
  // URI can be an asterisk (*) or should start with slash.
  return uri[0] == '/' || (uri[0] == '*' && uri[1] == '\0');
}

static const char *getreq(struct mg_connection *conn) {
  const char *cl, *ret = NULL;

  reset_per_request_attributes(conn);
  conn->request_len = read_request(NULL, conn, conn->buf, conn->buf_size,
                                   &conn->data_len);
  assert(conn->request_len < 0 || conn->data_len >= conn->request_len);

  if (conn->request_len == 0 && conn->data_len == conn->buf_size) {
    conn->status_code = 413;
    ret = "Request Entity Too Large";
  } else if (conn->request_len <= 0) {
    conn->status_code = 0;
    ret = "Client closed connection";
  } else if (parse_http_message(conn->buf, conn->buf_size,
                                &conn->request_info) <= 0) {
    conn->status_code = 400;
    ret = "Bad Request";
  } else {
    // Request is valid. Set content_len attribute by parsing Content-Length
    // If Content-Length is absent, set content_len to 0 if request is GET,
    // and set it to INT64_MAX otherwise. Setting to INT64_MAX instructs
    // mg_read() to read from the socket until socket is closed.
    // The reason for treating GET and POST/PUT differently is that libraries
    // like jquery do not set Content-Length in GET requests, and we don't
    // want mg_read() to hang waiting until socket is timed out.
    // See https://github.com/cesanta/mongoose/pull/121 for more.
    conn->content_len = INT64_MAX;
    if (!mg_strcasecmp(conn->request_info.request_method, "GET")) {
      conn->content_len = 0;
    }
    if ((cl = get_header(&conn->request_info, "Content-Length")) != NULL) {
      conn->content_len = strtoll(cl, NULL, 10);
    }
    conn->birth_time = time(NULL);
  }

  return ret;
}

static void process_new_connection(struct mg_connection *conn) {
  struct mg_request_info *ri = &conn->request_info;
  int keep_alive_enabled, keep_alive, discard_len;
  const char *msg = NULL;

  keep_alive_enabled = !strcmp(conn->ctx->config[ENABLE_KEEP_ALIVE], "yes");
  keep_alive = 0;

  // Important: on new connection, reset the receiving buffer. Credit goes
  // to crule42.
  conn->data_len = 0;
  do {
    if ((msg = getreq(conn)) != NULL) {
      conn->must_close = 1;
    } else if (!is_valid_uri(conn->request_info.uri)) {
      msg = "Bad Request";
      conn->status_code = 400;
    } else if (strcmp(ri->http_version, "1.0") &&
               strcmp(ri->http_version, "1.1")) {
      msg = "Bad HTTP Version";
      conn->status_code = 505;
    }

    if (msg != NULL) {
      // Do not send anything to the client on timeout
      // see https://github.com/cesanta/mongoose/issues/261
      if (conn->status_code > 0) {
        send_http_error(conn, conn->status_code, msg, "%s", "");
      }
    } else {
      handle_request(conn);
      call_user(MG_REQUEST_END, conn, (void *) (long) conn->status_code);
      log_access(conn);
    }
    if (ri->remote_user != NULL) {
      free((void *) ri->remote_user);
      // Important! When having connections with and without auth
      // would cause double free and then crash
      ri->remote_user = NULL;
    }

    // NOTE(lsm): order is important here. should_keep_alive() call
    // is using parsed request, which will be invalid after memmove's below.
    // Therefore, memorize should_keep_alive() result now for later use
    // in loop exit condition.
    keep_alive = conn->ctx->stop_flag == 0 && keep_alive_enabled &&
      conn->content_len >= 0 && should_keep_alive(conn);

    // Discard all buffered data for this request
    discard_len = conn->content_len >= 0 && conn->request_len > 0 &&
      conn->request_len + conn->content_len < (int64_t) conn->data_len ?
      (int) (conn->request_len + conn->content_len) : conn->data_len;
    assert(discard_len >= 0);
    memmove(conn->buf, conn->buf + discard_len, conn->data_len - discard_len);
    conn->data_len -= discard_len;
    assert(conn->data_len >= 0);
    assert(conn->data_len <= conn->buf_size);
  } while (keep_alive);
}

// Worker threads take accepted socket from the queue
static int consume_socket(struct mg_context *ctx, struct socket *sp) {
  recv(ctx->ctl[1], (void *) sp, sizeof(*sp), 0);
  return !ctx->stop_flag;
}

static void *worker_thread(void *thread_func_param) {
  struct mg_context *ctx = (struct mg_context *) thread_func_param;
  struct mg_connection *conn;

  conn = (struct mg_connection *) calloc(1, sizeof(*conn) + MAX_REQUEST_SIZE);
  if (conn == NULL) {
    cry(fc(ctx), "%s", "Cannot create new connection struct, OOM");
  } else {
    conn->buf_size = MAX_REQUEST_SIZE;
    conn->buf = (char *) (conn + 1);
    conn->ctx = ctx;
    conn->event.user_data = ctx->user_data;

    call_user(MG_THREAD_BEGIN, conn, NULL);

    // Call consume_socket() even when ctx->stop_flag > 0, to let it signal
    // sq_empty condvar to wake up the master waiting in produce_socket()
    while (consume_socket(ctx, &conn->client)) {
      conn->birth_time = time(NULL);

      // Fill in IP, port info early so even if SSL setup below fails,
      // error handler would have the corresponding info.
      // Thanks to Johannes Winkelmann for the patch.
      // TODO(lsm): Fix IPv6 case
      conn->request_info.remote_port = ntohs(conn->client.rsa.sin.sin_port);
      memcpy(&conn->request_info.remote_ip,
             &conn->client.rsa.sin.sin_addr.s_addr, 4);
      conn->request_info.remote_ip = ntohl(conn->request_info.remote_ip);
      conn->request_info.is_ssl = conn->client.is_ssl;

      if (!conn->client.is_ssl
#ifndef NO_SSL
          || sslize(conn, conn->ctx->ssl_ctx, SSL_accept)
#endif
         ) {
        process_new_connection(conn);
      }

      close_connection(conn);
    }
    call_user(MG_THREAD_END, conn, NULL);
    free(conn);
  }

  // Signal master that we're done with connection and exiting
  send(ctx->ctl[1], "x", 1, 0);

  DEBUG_TRACE(("exiting"));
  return NULL;
}

// Master thread adds accepted socket to a queue
static void produce_socket(struct mg_context *ctx, const struct socket *sp) {
  send(ctx->ctl[0], (void *) sp, sizeof(*sp), 0);
}

static int set_sock_timeout(SOCKET sock, int milliseconds) {
#ifdef _WIN32
  DWORD t = milliseconds;
#else
  struct timeval t;
  t.tv_sec = milliseconds / 1000;
  t.tv_usec = (milliseconds * 1000) % 1000000;
#endif
  return setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (void *) &t, sizeof(t)) ||
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (void *) &t, sizeof(t));
}

static void accept_new_connection(const struct socket *listener,
                                  struct mg_context *ctx) {
  struct socket so;
  char src_addr[IP_ADDR_STR_LEN];
  socklen_t len = sizeof(so.rsa);
  int on = 1;

  if ((so.sock = accept(listener->sock, &so.rsa.sa, &len)) == INVALID_SOCKET) {
  } else if (!check_acl(ctx, ntohl(* (uint32_t *) &so.rsa.sin.sin_addr))) {
    sockaddr_to_string(src_addr, sizeof(src_addr), &so.rsa);
    cry(fc(ctx), "%s: %s is not allowed to connect", __func__, src_addr);
    closesocket(so.sock);
  } else {
    // Put so socket structure into the queue
    DEBUG_TRACE(("Accepted socket %d", (int) so.sock));
    set_close_on_exec(so.sock);
    so.is_ssl = listener->is_ssl;
    so.ssl_redir = listener->ssl_redir;
    getsockname(so.sock, &so.lsa.sa, &len);
    // Set TCP keep-alive. This is needed because if HTTP-level keep-alive
    // is enabled, and client resets the connection, server won't get
    // TCP FIN or RST and will keep the connection open forever. With TCP
    // keep-alive, next keep-alive handshake will figure out that the client
    // is down and will close the server end.
    // Thanks to Igor Klopov who suggested the patch.
    setsockopt(so.sock, SOL_SOCKET, SO_KEEPALIVE, (void *) &on, sizeof(on));
    set_sock_timeout(so.sock, atoi(ctx->config[REQUEST_TIMEOUT]));
    produce_socket(ctx, &so);
  }
}

static void *master_thread(void *thread_func_param) {
  struct mg_context *ctx = (struct mg_context *) thread_func_param;
  struct pollfd *pfd;
  int i;

  // Increase priority of the master thread
#if defined(_WIN32)
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
#endif

#if defined(ISSUE_317)
  struct sched_param sched_param;
  sched_param.sched_priority = sched_get_priority_max(SCHED_RR);
  pthread_setschedparam(pthread_self(), SCHED_RR, &sched_param);
#endif

  call_user(MG_THREAD_BEGIN, fc(ctx), NULL);

  pfd = (struct pollfd *) calloc(ctx->num_listening_sockets, sizeof(pfd[0]));
  while (pfd != NULL && ctx->stop_flag == 0) {
    for (i = 0; i < ctx->num_listening_sockets; i++) {
      pfd[i].fd = ctx->listening_sockets[i].sock;
      pfd[i].events = POLLIN;
    }

    if (poll(pfd, ctx->num_listening_sockets, 200) > 0) {
      for (i = 0; i < ctx->num_listening_sockets; i++) {
        // NOTE(lsm): on QNX, poll() returns POLLRDNORM after the
        // successfull poll, and POLLIN is defined as (POLLRDNORM | POLLRDBAND)
        // Therefore, we're checking pfd[i].revents & POLLIN, not
        // pfd[i].revents == POLLIN.
        if (ctx->stop_flag == 0 && (pfd[i].revents & POLLIN)) {
          accept_new_connection(&ctx->listening_sockets[i], ctx);
        }
      }
    }
  }
  free(pfd);
  DEBUG_TRACE(("stopping workers"));

  // Stop signal received: somebody called mg_stop. Quit.
  close_all_listening_sockets(ctx);

  // Wakeup workers that are waiting for connections to handle.
  for (i = 0; i < ctx->num_threads; i++) {
    struct socket dummy;
    send(ctx->ctl[0], (void *) &dummy, sizeof(dummy), 0);
  }

  // Wait until all threads finish
  for (i = 0; i < ctx->num_threads; i++) {
    char ch;
    recv(ctx->ctl[0], &ch, 1, 0);
  }

#if !defined(NO_SSL)
  uninitialize_ssl(ctx);
#endif
  DEBUG_TRACE(("exiting"));

  call_user(MG_THREAD_END, fc(ctx), NULL);

  // Signal mg_stop() that we're done.
  // WARNING: This must be the very last thing this
  // thread does, as ctx becomes invalid after this line.
  ctx->stop_flag = 2;
  return NULL;
}

static void free_context(struct mg_context *ctx) {
  int i;

  // Deallocate config parameters
  for (i = 0; i < NUM_OPTIONS; i++) {
    if (ctx->config[i] != NULL)
      free(ctx->config[i]);
  }

#ifndef NO_SSL
  // Deallocate SSL context
  if (ctx->ssl_ctx != NULL) {
    SSL_CTX_free(ctx->ssl_ctx);
  }
  if (ssl_mutexes != NULL) {
    free(ssl_mutexes);
    ssl_mutexes = NULL;
  }
#endif // !NO_SSL

  // Deallocate context itself
  free(ctx);
}

void mg_stop(struct mg_context *ctx) {
  ctx->stop_flag = 1;

  // Wait until mg_fini() stops
  while (ctx->stop_flag != 2) {
    (void) mg_sleep(10);
  }
  free_context(ctx);

#if defined(_WIN32) && !defined(__SYMBIAN32__)
  (void) WSACleanup();
#endif // _WIN32
}

struct mg_context *mg_start(const char **options,
                            mg_event_handler_t func,
                            void *user_data) {
  struct mg_context *ctx;
  const char *name, *value, *default_value;
  int i;

#if defined(_WIN32) && !defined(__SYMBIAN32__)
  WSADATA data;
  WSAStartup(MAKEWORD(2,2), &data);
#endif // _WIN32

  // Allocate context and initialize reasonable general case defaults.
  // TODO(lsm): do proper error handling here.
  if ((ctx = (struct mg_context *) calloc(1, sizeof(*ctx))) == NULL) {
    return NULL;
  }
  ctx->event_handler = func;
  ctx->user_data = user_data;

  while (options && (name = *options++) != NULL) {
    if ((i = get_option_index(name)) == -1) {
      cry(fc(ctx), "Invalid option: %s", name);
      free_context(ctx);
      return NULL;
    } else if ((value = *options++) == NULL) {
      cry(fc(ctx), "%s: option value cannot be NULL", name);
      free_context(ctx);
      return NULL;
    }
    if (ctx->config[i] != NULL) {
      cry(fc(ctx), "warning: %s: duplicate option", name);
      free(ctx->config[i]);
    }
    ctx->config[i] = mg_strdup(value);
    DEBUG_TRACE(("[%s] -> [%s]", name, value));
  }

  // Set default value if needed
  for (i = 0; config_options[i * 2] != NULL; i++) {
    default_value = config_options[i * 2 + 1];
    if (ctx->config[i] == NULL && default_value != NULL) {
      ctx->config[i] = mg_strdup(default_value);
    }
  }

  // NOTE(lsm): order is important here. SSL certificates must
  // be initialized before listening ports. UID must be set last.
  if (!set_gpass_option(ctx) ||
#if !defined(NO_SSL)
      !set_ssl_option(ctx) ||
#endif
      !set_ports_option(ctx) ||
#if !defined(_WIN32)
      !set_uid_option(ctx) ||
#endif
      !set_acl_option(ctx)) {
    free_context(ctx);
    return NULL;
  }

#if !defined(_WIN32) && !defined(__SYMBIAN32__)
  // Ignore SIGPIPE signal, so if browser cancels the request, it
  // won't kill the whole process.
  (void) signal(SIGPIPE, SIG_IGN);
#endif // !_WIN32

  mg_socketpair(ctx->ctl);

  // Start master (listening) thread
  mg_start_thread(master_thread, ctx);

  // Start worker threads
  for (i = 0; i < atoi(ctx->config[NUM_THREADS]); i++) {
    if (mg_start_thread(worker_thread, ctx) != 0) {
      cry(fc(ctx), "Cannot start worker thread: %ld", (long) ERRNO);
    } else {
      ctx->num_threads++;
    }
  }

  return ctx;
}
