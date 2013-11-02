#include "internal.h"

// Return number of bytes left to read for this connection
static int64_t left_to_read(const struct mg_connection *conn) {
  return conn->content_len + conn->request_len - conn->num_bytes_read;
}

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

static void sockaddr_to_string(char *buf, size_t len,
                                     const union usa *usa) {
  buf[0] = '\0';
#if defined(USE_IPV6)
  inet_ntop(usa->sa.sa_family, usa->sa.sa_family == AF_INET ?
            (void *) &usa->sin.sin_addr :
            (void *) &usa->sin6.sin6_addr, buf, len);
#elif defined(_WIN32)
  // Only Windoze Vista (and newer) have inet_ntop()
  strncpy(buf, inet_ntoa(usa->sin.sin_addr), len);
#else
  inet_ntop(usa->sa.sa_family, (void *) &usa->sin.sin_addr, buf, len);
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
}

// Write data to the IO channel - opened file descriptor, socket or SSL
// descriptor. Return number of bytes written.
static int64_t push(FILE *fp, SOCKET sock, SSL *ssl, const char *buf,
                    int64_t len) {
  int64_t sent;
  int n, k;

  (void) ssl;  // Get rid of warning
  sent = 0;
  while (sent < len) {

    // How many bytes we send in this iteration
    k = len - sent > INT_MAX ? INT_MAX : (int) (len - sent);

#if !defined(NO_SSL)
    if (ssl != NULL) {
      n = SSL_write(ssl, buf + sent, k);
    } else
#endif
    if (fp != NULL) {
      n = (int) fwrite(buf + sent, 1, (size_t) k, fp);
      if (ferror(fp))
        n = -1;
    } else {
      n = send(sock, buf + sent, (size_t) k, MSG_NOSIGNAL);
    }

    if (n <= 0)
      break;

    sent += n;
  }

  return sent;
}

// Read from IO channel - opened file descriptor, socket, or SSL descriptor.
// Return negative value on error, or number of bytes read on success.
static int pull(FILE *fp, struct mg_connection *conn, char *buf, int len) {
  int nread;

  if (len <= 0) return 0;
  if (fp != NULL) {
    // Use read() instead of fread(), because if we're reading from the CGI
    // pipe, fread() may block until IO buffer is filled up. We cannot afford
    // to block and must pass all read bytes immediately to the client.
    nread = read(fileno(fp), buf, (size_t) len);
#ifndef NO_SSL
  } else if (conn->ssl != NULL) {
    nread = SSL_read(conn->ssl, buf, len);
#endif
  } else {
    nread = recv(conn->client.sock, buf, (size_t) len, 0);
  }
  if (nread > 0) {
    conn->num_bytes_read += nread;
  }

  return conn->ctx->stop_flag ? -1 : nread;
}

static int pull_all(FILE *fp, struct mg_connection *conn, char *buf, int len) {
  int n, nread = 0;

  while (len > 0 && conn->ctx->stop_flag == 0) {
    n = pull(fp, conn, buf + nread, len);
    if (n < 0) {
      nread = n;  // Propagate the error
      break;
    } else if (n == 0) {
      break;  // No more data to read
    } else {
      nread += n;
      len -= n;
    }
  }

  return nread;
}

int mg_read(struct mg_connection *conn, void *buf, int len) {
  int n, buffered_len, nread = 0;
  int64_t left;

  if (conn->content_len <= 0) {
    return 0;
  }

  // conn->buf           body
  //    |=================|==========|===============|
  //    |<--request_len-->|                          |
  //    |<-----------data_len------->|      conn->buf + conn->buf_size

  // First, check for data buffered in conn->buf by read_request().
  if (len > 0 && (buffered_len = conn->data_len - conn->request_len) > 0) {
    char *body = conn->buf + conn->request_len;
    if (buffered_len > len) buffered_len = len;
    if (buffered_len > conn->content_len) buffered_len = (int)conn->content_len;

    memcpy(buf, body, (size_t) buffered_len);
    memmove(body, body + buffered_len,
            &conn->buf[conn->data_len] - &body[buffered_len]);
    len -= buffered_len;
    conn->data_len -= buffered_len;
    nread += buffered_len;
  }

  // Read data from the socket.
  if (len > 0 && (left = left_to_read(conn)) > 0) {
    if (left < len) {
      len = (int) left;
    }
    n = pull_all(NULL, conn, (char *) buf + nread, (int) len);
    nread = n >= 0 ? nread + n : n;
  }

  return nread;
}

int mg_write(struct mg_connection *conn, const void *buf, int len) {
  time_t now;
  int64_t n, total, allowed;

  if (conn->throttle > 0) {
    if ((now = time(NULL)) != conn->last_throttle_time) {
      conn->last_throttle_time = now;
      conn->last_throttle_bytes = 0;
    }
    allowed = conn->throttle - conn->last_throttle_bytes;
    if (allowed > (int64_t) len) {
      allowed = len;
    }
    if ((total = push(NULL, conn->client.sock, conn->ssl, (const char *) buf,
                      (int64_t) allowed)) == allowed) {
      buf = (char *) buf + total;
      conn->last_throttle_bytes += total;
      while (total < (int64_t) len && conn->ctx->stop_flag == 0) {
        allowed = conn->throttle > (int64_t) len - total ?
          (int64_t) len - total : conn->throttle;
        if ((n = push(NULL, conn->client.sock, conn->ssl, (const char *) buf,
                      (int64_t) allowed)) != allowed) {
          break;
        }
        sleep(1);
        conn->last_throttle_bytes = allowed;
        conn->last_throttle_time = time(NULL);
        buf = (char *) buf + n;
        total += n;
      }
    }
  } else {
    total = push(NULL, conn->client.sock, conn->ssl, (const char *) buf,
                 (int64_t) len);
  }
  return (int) total;
}

int mg_url_decode(const char *src, int src_len, char *dst,
                  int dst_len, int is_form_url_encoded) {
  int i, j, a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

  for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++) {
    if (src[i] == '%' && i < src_len - 2 &&
        isxdigit(* (const unsigned char *) (src + i + 1)) &&
        isxdigit(* (const unsigned char *) (src + i + 2))) {
      a = tolower(* (const unsigned char *) (src + i + 1));
      b = tolower(* (const unsigned char *) (src + i + 2));
      dst[j] = (char) ((HEXTOI(a) << 4) | HEXTOI(b));
      i += 2;
    } else if (is_form_url_encoded && src[i] == '+') {
      dst[j] = ' ';
    } else {
      dst[j] = src[i];
    }
  }

  dst[j] = '\0'; // Null-terminate the destination

  return i >= src_len ? j : -1;
}

int mg_get_var(const char *data, size_t data_len, const char *name,
               char *dst, size_t dst_len) {
  const char *p, *e, *s;
  size_t name_len;
  int len;

  if (dst == NULL || dst_len == 0) {
    len = -2;
  } else if (data == NULL || name == NULL || data_len == 0) {
    len = -1;
    dst[0] = '\0';
  } else {
    name_len = strlen(name);
    e = data + data_len;
    len = -1;
    dst[0] = '\0';

    // data is "var1=val1&var2=val2...". Find variable first
    for (p = data; p + name_len < e; p++) {
      if ((p == data || p[-1] == '&') && p[name_len] == '=' &&
          !mg_strncasecmp(name, p, name_len)) {

        // Point p to variable value
        p += name_len + 1;

        // Point s to the end of the value
        s = (const char *) memchr(p, '&', (size_t)(e - p));
        if (s == NULL) {
          s = e;
        }
        assert(s >= p);

        // Decode variable into destination buffer
        len = mg_url_decode(p, (size_t)(s - p), dst, dst_len, 1);

        // Redirect error code from -1 to -2 (destination buffer too small).
        if (len == -1) {
          len = -2;
        }
        break;
      }
    }
  }

  return len;
}

int mg_get_cookie(const char *cookie_header, const char *var_name,
                  char *dst, size_t dst_size) {
  const char *s, *p, *end;
  int name_len, len = -1;

  if (dst == NULL || dst_size == 0) {
    len = -2;
  } else if (var_name == NULL || (s = cookie_header) == NULL) {
    len = -1;
    dst[0] = '\0';
  } else {
    name_len = (int) strlen(var_name);
    end = s + strlen(s);
    dst[0] = '\0';

    for (; (s = mg_strcasestr(s, var_name)) != NULL; s += name_len) {
      if (s[name_len] == '=') {
        s += name_len + 1;
        if ((p = strchr(s, ' ')) == NULL)
          p = end;
        if (p[-1] == ';')
          p--;
        if (*s == '"' && p[-1] == '"' && p > s + 1) {
          s++;
          p--;
        }
        if ((size_t) (p - s) < dst_size) {
          len = p - s;
          mg_strlcpy(dst, s, (size_t) len + 1);
        } else {
          len = -3;
        }
        break;
      }
    }
  }
  return len;
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

// Check whether full request is buffered. Return:
//   -1  if request is malformed
//    0  if request is not yet fully buffered
//   >0  actual request length, including last \r\n\r\n
static int get_request_len(const char *buf, int buf_len) {
  int i;

  for (i = 0; i < buf_len; i++) {
    // Control characters are not allowed but >=128 is.
    // Abort scan as soon as one malformed character is found;
    // don't let subsequent \r\n\r\n win us over anyhow
    if (!isprint(* (const unsigned char *) &buf[i]) && buf[i] != '\r' &&
        buf[i] != '\n' && * (const unsigned char *) &buf[i] < 128) {
      return -1;
    } else if (buf[i] == '\n' && i + 1 < buf_len && buf[i + 1] == '\n') {
      return i + 2;
    } else if (buf[i] == '\n' && i + 2 < buf_len && buf[i + 1] == '\r' &&
               buf[i + 2] == '\n') {
      return i + 3;
    }
  }

  return 0;
}

// Protect against directory disclosure attack by removing '..',
// excessive '/' and '\' characters
static void remove_double_dots_and_double_slashes(char *s) {
  char *p = s;

  while (*s != '\0') {
    *p++ = *s++;
    if (s[-1] == '/' || s[-1] == '\\') {
      // Skip all following slashes, backslashes and double-dots
      while (s[0] != '\0') {
        if (s[0] == '/' || s[0] == '\\') {
          s++;
        } else if (s[0] == '.' && s[1] == '.') {
          s += 2;
        } else {
          break;
        }
      }
    }
  }
  *p = '\0';
}

static const struct {
  const char *extension;
  size_t ext_len;
  const char *mime_type;
} builtin_mime_types[] = {
  {".html", 5, "text/html"},
  {".htm", 4, "text/html"},
  {".shtm", 5, "text/html"},
  {".shtml", 6, "text/html"},
  {".css", 4, "text/css"},
  {".js",  3, "application/x-javascript"},
  {".ico", 4, "image/x-icon"},
  {".gif", 4, "image/gif"},
  {".jpg", 4, "image/jpeg"},
  {".jpeg", 5, "image/jpeg"},
  {".png", 4, "image/png"},
  {".svg", 4, "image/svg+xml"},
  {".txt", 4, "text/plain"},
  {".torrent", 8, "application/x-bittorrent"},
  {".wav", 4, "audio/x-wav"},
  {".mp3", 4, "audio/x-mp3"},
  {".mid", 4, "audio/mid"},
  {".m3u", 4, "audio/x-mpegurl"},
  {".ogg", 4, "application/ogg"},
  {".ram", 4, "audio/x-pn-realaudio"},
  {".xml", 4, "text/xml"},
  {".json",  5, "text/json"},
  {".xslt", 5, "application/xml"},
  {".xsl", 4, "application/xml"},
  {".ra",  3, "audio/x-pn-realaudio"},
  {".doc", 4, "application/msword"},
  {".exe", 4, "application/octet-stream"},
  {".zip", 4, "application/x-zip-compressed"},
  {".xls", 4, "application/excel"},
  {".tgz", 4, "application/x-tar-gz"},
  {".tar", 4, "application/x-tar"},
  {".gz",  3, "application/x-gunzip"},
  {".arj", 4, "application/x-arj-compressed"},
  {".rar", 4, "application/x-arj-compressed"},
  {".rtf", 4, "application/rtf"},
  {".pdf", 4, "application/pdf"},
  {".swf", 4, "application/x-shockwave-flash"},
  {".mpg", 4, "video/mpeg"},
  {".webm", 5, "video/webm"},
  {".mpeg", 5, "video/mpeg"},
  {".mov", 4, "video/quicktime"},
  {".mp4", 4, "video/mp4"},
  {".m4v", 4, "video/x-m4v"},
  {".asf", 4, "video/x-ms-asf"},
  {".avi", 4, "video/x-msvideo"},
  {".bmp", 4, "image/bmp"},
  {".ttf", 4, "application/x-font-ttf"},
  {NULL,  0, NULL}
};

const char *mg_get_builtin_mime_type(const char *path) {
  const char *ext;
  size_t i, path_len;

  path_len = strlen(path);

  for (i = 0; builtin_mime_types[i].extension != NULL; i++) {
    ext = path + (path_len - builtin_mime_types[i].ext_len);
    if (path_len > builtin_mime_types[i].ext_len &&
        mg_strcasecmp(ext, builtin_mime_types[i].extension) == 0) {
      return builtin_mime_types[i].mime_type;
    }
  }

  return "text/plain";
}

// Look at the "path" extension and figure what mime type it has.
// Store mime type in the vector.
static void get_mime_type(struct mg_context *ctx, const char *path,
                          struct vec *vec) {
  struct vec ext_vec, mime_vec;
  const char *list, *ext;
  size_t path_len;

  path_len = strlen(path);

  // Scan user-defined mime types first, in case user wants to
  // override default mime types.
  list = ctx->config[EXTRA_MIME_TYPES];
  while ((list = next_option(list, &ext_vec, &mime_vec)) != NULL) {
    // ext now points to the path suffix
    ext = path + path_len - ext_vec.len;
    if (mg_strncasecmp(ext, ext_vec.ptr, ext_vec.len) == 0) {
      *vec = mime_vec;
      return;
    }
  }

  vec->ptr = mg_get_builtin_mime_type(path);
  vec->len = strlen(vec->ptr);
}

void mg_url_encode(const char *src, char *dst, size_t dst_len) {
  static const char *dont_escape = "._-$,;~()";
  static const char *hex = "0123456789abcdef";
  const char *end = dst + dst_len - 1;

  for (; *src != '\0' && dst < end; src++, dst++) {
    if (isalnum(*(const unsigned char *) src) ||
        strchr(dont_escape, * (const unsigned char *) src) != NULL) {
      *dst = *src;
    } else if (dst + 2 < end) {
      dst[0] = '%';
      dst[1] = hex[(* (const unsigned char *) src) >> 4];
      dst[2] = hex[(* (const unsigned char *) src) & 0xf];
      dst += 2;
    }
  }

  *dst = '\0';
}

static void print_dir_entry(const struct de *de) {
  char size[64], mod[64], href[PATH_MAX * 3];
  const char *slash = de->file.is_directory ? "/" : "";

  if (de->file.is_directory) {
    mg_snprintf(size, sizeof(size), "%s", "[DIRECTORY]");
  } else {
     // We use (signed) cast below because MSVC 6 compiler cannot
     // convert unsigned __int64 to double. Sigh.
    if (de->file.size < 1024) {
      mg_snprintf(size, sizeof(size), "%d", (int) de->file.size);
    } else if (de->file.size < 0x100000) {
      mg_snprintf(size, sizeof(size),
                  "%.1fk", (double) de->file.size / 1024.0);
    } else if (de->file.size < 0x40000000) {
      mg_snprintf(size, sizeof(size),
                  "%.1fM", (double) de->file.size / 1048576);
    } else {
      mg_snprintf(size, sizeof(size),
                  "%.1fG", (double) de->file.size / 1073741824);
    }
  }
  strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M",
           localtime(&de->file.modification_time));
  mg_url_encode(de->file_name, href, sizeof(href));
  de->conn->num_bytes_sent += mg_chunked_printf(de->conn,
      "<tr><td><a href=\"%s%s%s\">%s%s</a></td>"
      "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
      de->conn->request_info.uri, href, slash, de->file_name, slash, mod, size);
}

// This function is called from send_directory() and used for
// sorting directory entries by size, or name, or modification time.
// On windows, __cdecl specification is needed in case if project is built
// with __stdcall convention. qsort always requires __cdels callback.
static int WINCDECL compare_dir_entries(const void *p1, const void *p2) {
  const struct de *a = (const struct de *) p1, *b = (const struct de *) p2;
  const char *query_string = a->conn->request_info.query_string;
  int cmp_result = 0;

  if (query_string == NULL) {
    query_string = "na";
  }

  if (a->file.is_directory && !b->file.is_directory) {
    return -1;  // Always put directories on top
  } else if (!a->file.is_directory && b->file.is_directory) {
    return 1;   // Always put directories on top
  } else if (*query_string == 'n') {
    cmp_result = strcmp(a->file_name, b->file_name);
  } else if (*query_string == 's') {
    cmp_result = a->file.size == b->file.size ? 0 :
      a->file.size > b->file.size ? 1 : -1;
  } else if (*query_string == 'd') {
    cmp_result = a->file.modification_time == b->file.modification_time ? 0 :
      a->file.modification_time > b->file.modification_time ? 1 : -1;
  }

  return query_string[1] == 'd' ? -cmp_result : cmp_result;
}

static int must_hide_file(struct mg_connection *conn, const char *path) {
  const char *pw_pattern = "**" PASSWORDS_FILE_NAME "$";
  const char *pattern = conn->ctx->config[HIDE_FILES];
  return match_prefix(pw_pattern, strlen(pw_pattern), path) > 0 ||
    (pattern != NULL && match_prefix(pattern, strlen(pattern), path) > 0);
}

static int scan_directory(struct mg_connection *conn, const char *dir,
                          void *data, void (*cb)(struct de *, void *)) {
  char path[PATH_MAX];
  struct dirent *dp;
  DIR *dirp;
  struct de de;

  if ((dirp = opendir(dir)) == NULL) {
    return 0;
  } else {
    de.conn = conn;

    while ((dp = readdir(dirp)) != NULL) {
      // Do not show current dir and hidden files
      if (!strcmp(dp->d_name, ".") ||
          !strcmp(dp->d_name, "..") ||
          must_hide_file(conn, dp->d_name)) {
        continue;
      }

      mg_snprintf(path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);

      // If we don't memset stat structure to zero, mtime will have
      // garbage and strftime() will segfault later on in
      // print_dir_entry(). memset is required only if mg_stat()
      // fails. For more details, see
      // http://code.google.com/p/mongoose/issues/detail?id=79
      memset(&de.file, 0, sizeof(de.file));
      mg_stat(path, &de.file);

      de.file_name = dp->d_name;
      cb(&de, data);
    }
    (void) closedir(dirp);
  }
  return 1;
}

static int remove_directory(struct mg_connection *conn, const char *dir) {
  char path[PATH_MAX];
  struct dirent *dp;
  DIR *dirp;
  struct de de;

  if ((dirp = opendir(dir)) == NULL) {
    return 0;
  } else {
    de.conn = conn;

    while ((dp = readdir(dirp)) != NULL) {
      // Do not show current dir, but show hidden files
      if (!strcmp(dp->d_name, ".") ||
          !strcmp(dp->d_name, "..")) {
        continue;
      }

      mg_snprintf(path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);

      // If we don't memset stat structure to zero, mtime will have
      // garbage and strftime() will segfault later on in
      // print_dir_entry(). memset is required only if mg_stat()
      // fails. For more details, see
      // http://code.google.com/p/mongoose/issues/detail?id=79
      memset(&de.file, 0, sizeof(de.file));
      mg_stat(path, &de.file);
      if(de.file.modification_time) {
          if(de.file.is_directory) {
              remove_directory(conn, path);
          } else {
              mg_remove(path);
          }
      }

    }
    (void) closedir(dirp);

    rmdir(dir);
  }

  return 1;
}

struct dir_scan_data {
  struct de *entries;
  int num_entries;
  int arr_size;
};

// Behaves like realloc(), but frees original pointer on failure
static void *realloc2(void *ptr, size_t size) {
  void *new_ptr = realloc(ptr, size);
  if (new_ptr == NULL) {
    free(ptr);
  }
  return new_ptr;
}

static void dir_scan_callback(struct de *de, void *data) {
  struct dir_scan_data *dsd = (struct dir_scan_data *) data;

  if (dsd->entries == NULL || dsd->num_entries >= dsd->arr_size) {
    dsd->arr_size *= 2;
    dsd->entries = (struct de *) realloc2(dsd->entries, dsd->arr_size *
                                          sizeof(dsd->entries[0]));
  }
  if (dsd->entries == NULL) {
    // TODO(lsm): propagate an error to the caller
    dsd->num_entries = 0;
  } else {
    dsd->entries[dsd->num_entries].file_name = mg_strdup(de->file_name);
    dsd->entries[dsd->num_entries].file = de->file;
    dsd->entries[dsd->num_entries].conn = de->conn;
    dsd->num_entries++;
  }
}

static void handle_directory_request(struct mg_connection *conn,
                                     const char *dir) {
  int i, sort_direction;
  struct dir_scan_data data = { NULL, 0, 128 };

  if (!scan_directory(conn, dir, &data, dir_scan_callback)) {
    send_http_error(conn, 500, "Cannot open directory",
                    "Error: opendir(%s): %s", dir, strerror(ERRNO));
    return;
  }

  sort_direction = conn->request_info.query_string != NULL &&
    conn->request_info.query_string[1] == 'd' ? 'a' : 'd';

  conn->must_close = 1;
  mg_printf(conn, "%s",
            "HTTP/1.1 200 OK\r\n"
            "Transfer-Encoding: Chunked\r\n"
            "Content-Type: text/html; charset=utf-8\r\n\r\n");

  conn->num_bytes_sent += mg_chunked_printf(conn,
      "<html><head><title>Index of %s</title>"
      "<style>th {text-align: left;}</style></head>"
      "<body><h1>Index of %s</h1><pre><table cellpadding=\"0\">"
      "<tr><th><a href=\"?n%c\">Name</a></th>"
      "<th><a href=\"?d%c\">Modified</a></th>"
      "<th><a href=\"?s%c\">Size</a></th></tr>"
      "<tr><td colspan=\"3\"><hr></td></tr>",
      conn->request_info.uri, conn->request_info.uri,
      sort_direction, sort_direction, sort_direction);

  // Print first entry - link to a parent directory
  conn->num_bytes_sent += mg_chunked_printf(conn,
      "<tr><td><a href=\"%s%s\">%s</a></td>"
      "<td>&nbsp;%s</td><td>&nbsp;&nbsp;%s</td></tr>\n",
      conn->request_info.uri, "..", "Parent directory", "-", "-");

  // Sort and print directory entries
  qsort(data.entries, (size_t) data.num_entries, sizeof(data.entries[0]),
        compare_dir_entries);
  for (i = 0; i < data.num_entries; i++) {
    print_dir_entry(&data.entries[i]);
    free(data.entries[i].file_name);
  }
  free(data.entries);

  conn->num_bytes_sent += mg_chunked_printf(conn, "%s",
                                            "</table></body></html>");
  conn->num_bytes_sent += mg_write(conn, "0\r\n\r\n", 5);
  conn->status_code = 200;
}

// Send len bytes from the opened file to the client.
static void send_file_data(struct mg_connection *conn, FILE *fp,
                           int64_t offset, int64_t len) {
  char buf[MG_BUF_LEN];
  int num_read, num_written, to_read;

  // If offset is beyond file boundaries, don't send anything
  if (offset > 0 && fseeko(fp, offset, SEEK_SET) != 0) {
    return;
  }

  while (len > 0) {
    // Calculate how much to read from the file in the buffer
    to_read = sizeof(buf);
    if ((int64_t) to_read > len) {
      to_read = (int) len;
    }

    // Read from file, exit the loop on error
    if ((num_read = fread(buf, 1, (size_t) to_read, fp)) <= 0) {
      break;
    }

    // Send read bytes to the client, exit the loop on error
    if ((num_written = mg_write(conn, buf, (size_t) num_read)) != num_read) {
      break;
    }

    // Both read and were successful, adjust counters
    conn->num_bytes_sent += num_written;
    len -= num_written;
  }
}

static int parse_range_header(const char *header, int64_t *a, int64_t *b) {
  return sscanf(header, "bytes=%" INT64_FMT "-%" INT64_FMT, a, b);
}

static void gmt_time_string(char *buf, size_t buf_len, time_t *t) {
  strftime(buf, buf_len, "%a, %d %b %Y %H:%M:%S GMT", gmtime(t));
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


// Parse HTTP headers from the given buffer, advance buffer to the point
// where parsing stopped.
static void parse_http_headers(char **buf, struct mg_request_info *ri) {
  int i;

  for (i = 0; i < (int) ARRAY_SIZE(ri->http_headers); i++) {
    ri->http_headers[i].name = skip_quoted(buf, ":", " ", 0);
    ri->http_headers[i].value = skip(buf, "\r\n");
    if (ri->http_headers[i].name[0] == '\0')
      break;
    ri->num_headers = i + 1;
  }
}

static int is_valid_http_method(const char *method) {
  return !strcmp(method, "GET") || !strcmp(method, "POST") ||
    !strcmp(method, "HEAD") || !strcmp(method, "CONNECT") ||
    !strcmp(method, "PUT") || !strcmp(method, "DELETE") ||
    !strcmp(method, "OPTIONS") || !strcmp(method, "PROPFIND")
    || !strcmp(method, "MKCOL")
          ;
}

// Parse HTTP request, fill in mg_request_info structure.
// This function modifies the buffer by NUL-terminating
// HTTP request components, header names and header values.
static int parse_http_message(char *buf, int len, struct mg_request_info *ri) {
  int is_request, request_length = get_request_len(buf, len);
  if (request_length > 0) {
    // Reset attributes. DO NOT TOUCH is_ssl, remote_ip, remote_port
    ri->remote_user = ri->request_method = ri->uri = ri->http_version = NULL;
    ri->num_headers = 0;

    buf[request_length - 1] = '\0';

    // RFC says that all initial whitespaces should be ingored
    while (*buf != '\0' && isspace(* (unsigned char *) buf)) {
      buf++;
    }
    ri->request_method = skip(&buf, " ");
    ri->uri = skip(&buf, " ");
    ri->http_version = skip(&buf, "\r\n");

    // HTTP message could be either HTTP request or HTTP response, e.g.
    // "GET / HTTP/1.0 ...." or  "HTTP/1.0 200 OK ..."
    is_request = is_valid_http_method(ri->request_method);
    if ((is_request && memcmp(ri->http_version, "HTTP/", 5) != 0) ||
        (!is_request && memcmp(ri->request_method, "HTTP/", 5) != 0)) {
      request_length = -1;
    } else {
      if (is_request) {
        ri->http_version += 5;
      }
      parse_http_headers(&buf, ri);
    }
  }
  return request_length;
}

// Keep reading the input (either opened file descriptor fd, or socket sock,
// or SSL descriptor ssl) into buffer buf, until \r\n\r\n appears in the
// buffer (which marks the end of HTTP request). Buffer buf may already
// have some data. The length of the data is stored in nread.
// Upon every read operation, increase nread by the number of bytes read.
static int read_request(FILE *fp, struct mg_connection *conn,
                        char *buf, int bufsiz, int *nread) {
  int request_len, n = 0;

  request_len = get_request_len(buf, *nread);
  while (conn->ctx->stop_flag == 0 &&
         *nread < bufsiz &&
         request_len == 0 &&
         (n = pull(fp, conn, buf + *nread, bufsiz - *nread)) > 0) {
    *nread += n;
    assert(*nread <= bufsiz);
    request_len = get_request_len(buf, *nread);
  }

  return request_len <= 0 && n <= 0 ? -1 : request_len;
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

static int forward_body_data(struct mg_connection *conn, FILE *fp,
                             SOCKET sock, SSL *ssl) {
  const char *expect, *body;
  char buf[MG_BUF_LEN];
  int nread, buffered_len, success = 0;
  int64_t left;

  expect = mg_get_header(conn, "Expect");
  assert(fp != NULL);

  if (conn->content_len == INT64_MAX) {
    send_http_error(conn, 411, "Length Required", "%s", "");
  } else if (expect != NULL && mg_strcasecmp(expect, "100-continue")) {
    send_http_error(conn, 417, "Expectation Failed", "%s", "");
  } else {
    if (expect != NULL) {
      (void) mg_printf(conn, "%s", "HTTP/1.1 100 Continue\r\n\r\n");
    }

    buffered_len = conn->data_len - conn->request_len;
    body = conn->buf + conn->request_len;
    assert(buffered_len >= 0);

    if (buffered_len > 0) {
      if ((int64_t) buffered_len > conn->content_len) {
        buffered_len = (int) conn->content_len;
      }
      push(fp, sock, ssl, body, (int64_t) buffered_len);
      memmove((char *) body, body + buffered_len, buffered_len);
      conn->data_len -= buffered_len;
    }

    nread = 0;
    while (conn->num_bytes_read < conn->content_len + conn->request_len) {
      left = left_to_read(conn);
      if (left > (int64_t) sizeof(buf)) {
        left = sizeof(buf);
      }
      nread = pull(NULL, conn, buf, (int) left);
      if (nread <= 0 || push(fp, sock, ssl, buf, nread) != nread) {
        break;
      }
    }

    if (left_to_read(conn) == 0) {
      success = nread >= 0;
    }

    // Each error code path in this function must send an error
    if (!success) {
      send_http_error(conn, 577, http_500_error, "%s", "");
    }
  }

  return success;
}

#if !defined(NO_CGI)
// This structure helps to create an environment for the spawned CGI program.
// Environment is an array of "VARIABLE=VALUE\0" ASCIIZ strings,
// last element must be NULL.
// However, on Windows there is a requirement that all these VARIABLE=VALUE\0
// strings must reside in a contiguous buffer. The end of the buffer is
// marked by two '\0' characters.
// We satisfy both worlds: we create an envp array (which is vars), all
// entries are actually pointers inside buf.
struct cgi_env_block {
  struct mg_connection *conn;
  char buf[CGI_ENVIRONMENT_SIZE]; // Environment buffer
  int len; // Space taken
  char *vars[MAX_CGI_ENVIR_VARS]; // char **envp
  int nvars; // Number of variables
};

static char *addenv(struct cgi_env_block *block,
                    PRINTF_FORMAT_STRING(const char *fmt), ...)
  PRINTF_ARGS(2, 3);

// Append VARIABLE=VALUE\0 string to the buffer, and add a respective
// pointer into the vars array.
static char *addenv(struct cgi_env_block *block, const char *fmt, ...) {
  int n, space;
  char *added;
  va_list ap;

  // Calculate how much space is left in the buffer
  space = sizeof(block->buf) - block->len - 2;
  assert(space >= 0);

  // Make a pointer to the free space int the buffer
  added = block->buf + block->len;

  // Copy VARIABLE=VALUE\0 string into the free space
  va_start(ap, fmt);
  n = mg_vsnprintf(added, (size_t) space, fmt, ap);
  va_end(ap);

  // Make sure we do not overflow buffer and the envp array
  if (n > 0 && n + 1 < space &&
      block->nvars < (int) ARRAY_SIZE(block->vars) - 2) {
    // Append a pointer to the added string into the envp array
    block->vars[block->nvars++] = added;
    // Bump up used length counter. Include \0 terminator
    block->len += n + 1;
  } else {
    cry(block->conn, "%s: CGI env buffer truncated for [%s]", __func__, fmt);
  }

  return added;
}

static void prepare_cgi_environment(struct mg_connection *conn,
                                    const char *prog,
                                    struct cgi_env_block *blk) {
  const struct mg_request_info *ri = &conn->request_info;
  const char *s, *slash;
  struct vec var_vec;
  char *p, src_addr[IP_ADDR_STR_LEN];
  int  i;

  blk->len = blk->nvars = 0;
  blk->conn = conn;
  sockaddr_to_string(src_addr, sizeof(src_addr), &conn->client.rsa);

  addenv(blk, "SERVER_NAME=%s", conn->ctx->config[AUTHENTICATION_DOMAIN]);
  addenv(blk, "SERVER_ROOT=%s", conn->ctx->config[DOCUMENT_ROOT]);
  addenv(blk, "DOCUMENT_ROOT=%s", conn->ctx->config[DOCUMENT_ROOT]);
  addenv(blk, "SERVER_SOFTWARE=%s/%s", "Mongoose", mg_version());

  // Prepare the environment block
  addenv(blk, "%s", "GATEWAY_INTERFACE=CGI/1.1");
  addenv(blk, "%s", "SERVER_PROTOCOL=HTTP/1.1");
  addenv(blk, "%s", "REDIRECT_STATUS=200"); // For PHP

  // TODO(lsm): fix this for IPv6 case
  addenv(blk, "SERVER_PORT=%d", ntohs(conn->client.lsa.sin.sin_port));

  addenv(blk, "REQUEST_METHOD=%s", ri->request_method);
  addenv(blk, "REMOTE_ADDR=%s", src_addr);
  addenv(blk, "REMOTE_PORT=%d", ri->remote_port);
  addenv(blk, "REQUEST_URI=%s%s%s", ri->uri,
         ri->query_string == NULL ? "" : "?",
         ri->query_string == NULL ? "" : ri->query_string);

  // SCRIPT_NAME
  if (conn->path_info != NULL) {
    addenv(blk, "SCRIPT_NAME=%.*s",
           (int) (strlen(ri->uri) - strlen(conn->path_info)), ri->uri);
    addenv(blk, "PATH_INFO=%s", conn->path_info);
  } else {
    s = strrchr(prog, '/');
    slash = strrchr(ri->uri, '/');
    addenv(blk, "SCRIPT_NAME=%.*s%s",
           slash == NULL ? 0 : (int) (slash - ri->uri), ri->uri,
           s == NULL ? prog : s);
  }

  addenv(blk, "SCRIPT_FILENAME=%s", prog);
  addenv(blk, "PATH_TRANSLATED=%s", prog);
  addenv(blk, "HTTPS=%s", conn->ssl == NULL ? "off" : "on");

  if ((s = mg_get_header(conn, "Content-Type")) != NULL)
    addenv(blk, "CONTENT_TYPE=%s", s);

  if (ri->query_string != NULL) {
    addenv(blk, "QUERY_STRING=%s", ri->query_string);
  }

  if ((s = mg_get_header(conn, "Content-Length")) != NULL)
    addenv(blk, "CONTENT_LENGTH=%s", s);

  if ((s = getenv("PATH")) != NULL)
    addenv(blk, "PATH=%s", s);

#if defined(_WIN32)
  if ((s = getenv("COMSPEC")) != NULL) {
    addenv(blk, "COMSPEC=%s", s);
  }
  if ((s = getenv("SYSTEMROOT")) != NULL) {
    addenv(blk, "SYSTEMROOT=%s", s);
  }
  if ((s = getenv("SystemDrive")) != NULL) {
    addenv(blk, "SystemDrive=%s", s);
  }
  if ((s = getenv("ProgramFiles")) != NULL) {
    addenv(blk, "ProgramFiles=%s", s);
  }
  if ((s = getenv("ProgramFiles(x86)")) != NULL) {
    addenv(blk, "ProgramFiles(x86)=%s", s);
  }
  if ((s = getenv("CommonProgramFiles(x86)")) != NULL) {
    addenv(blk, "CommonProgramFiles(x86)=%s", s);
  }
#else
  if ((s = getenv("LD_LIBRARY_PATH")) != NULL)
    addenv(blk, "LD_LIBRARY_PATH=%s", s);
#endif // _WIN32

  if ((s = getenv("PERLLIB")) != NULL)
    addenv(blk, "PERLLIB=%s", s);

  if (ri->remote_user != NULL) {
    addenv(blk, "REMOTE_USER=%s", ri->remote_user);
    addenv(blk, "%s", "AUTH_TYPE=Digest");
  }

  // Add all headers as HTTP_* variables
  for (i = 0; i < ri->num_headers; i++) {
    p = addenv(blk, "HTTP_%s=%s",
        ri->http_headers[i].name, ri->http_headers[i].value);

    // Convert variable name into uppercase, and change - to _
    for (; *p != '=' && *p != '\0'; p++) {
      if (*p == '-')
        *p = '_';
      *p = (char) toupper(* (unsigned char *) p);
    }
  }

  // Add user-specified variables
  s = conn->ctx->config[CGI_ENVIRONMENT];
  while ((s = next_option(s, &var_vec, NULL)) != NULL) {
    addenv(blk, "%.*s", (int) var_vec.len, var_vec.ptr);
  }

  blk->vars[blk->nvars++] = NULL;
  blk->buf[blk->len++] = '\0';

  assert(blk->nvars < (int) ARRAY_SIZE(blk->vars));
  assert(blk->len > 0);
  assert(blk->len < (int) sizeof(blk->buf));
}

static void handle_cgi_request(struct mg_connection *conn, const char *prog) {
  int headers_len, data_len, i, fdin[2], fdout[2];
  const char *status, *status_text;
  char buf[16384], *pbuf, dir[PATH_MAX], *p;
  struct mg_request_info ri;
  struct cgi_env_block blk;
  FILE *in = NULL, *out = NULL;
  pid_t pid = (pid_t) -1;

  prepare_cgi_environment(conn, prog, &blk);

  // CGI must be executed in its own directory. 'dir' must point to the
  // directory containing executable program, 'p' must point to the
  // executable program name relative to 'dir'.
  (void) mg_snprintf(dir, sizeof(dir), "%s", prog);
  if ((p = strrchr(dir, '/')) != NULL) {
    *p++ = '\0';
  } else {
    dir[0] = '.', dir[1] = '\0';
    p = (char *) prog;
  }

  if (pipe(fdin) != 0 || pipe(fdout) != 0) {
    send_http_error(conn, 500, http_500_error,
        "Cannot create CGI pipe: %s", strerror(ERRNO));
    goto done;
  }

  pid = spawn_process(conn, p, blk.buf, blk.vars, fdin[0], fdout[1], dir);
  if (pid == (pid_t) -1) {
    send_http_error(conn, 500, http_500_error,
        "Cannot spawn CGI process [%s]: %s", prog, strerror(ERRNO));
    goto done;
  }

  // Make sure child closes all pipe descriptors. It must dup them to 0,1
  set_close_on_exec(fdin[0]);
  set_close_on_exec(fdin[1]);
  set_close_on_exec(fdout[0]);
  set_close_on_exec(fdout[1]);

  // Parent closes only one side of the pipes.
  // If we don't mark them as closed, close() attempt before
  // return from this function throws an exception on Windows.
  // Windows does not like when closed descriptor is closed again.
  (void) close(fdin[0]);
  (void) close(fdout[1]);
  fdin[0] = fdout[1] = -1;


  if ((in = fdopen(fdin[1], "wb")) == NULL ||
      (out = fdopen(fdout[0], "rb")) == NULL) {
    send_http_error(conn, 500, http_500_error,
        "fopen: %s", strerror(ERRNO));
    goto done;
  }

  setbuf(in, NULL);
  setbuf(out, NULL);

  // Send POST data to the CGI process if needed
  if (!strcmp(conn->request_info.request_method, "POST") &&
      !forward_body_data(conn, in, INVALID_SOCKET, NULL)) {
    goto done;
  }

  // Close so child gets an EOF.
  fclose(in);
  in = NULL;
  fdin[1] = -1;

  // Now read CGI reply into a buffer. We need to set correct
  // status code, thus we need to see all HTTP headers first.
  // Do not send anything back to client, until we buffer in all
  // HTTP headers.
  data_len = 0;
  headers_len = read_request(out, conn, buf, sizeof(buf), &data_len);
  if (headers_len <= 0) {
    send_http_error(conn, 500, http_500_error,
                    "CGI program sent malformed or too big (>%u bytes) "
                    "HTTP headers: [%.*s]",
                    (unsigned) sizeof(buf), data_len, buf);
    goto done;
  }
  pbuf = buf;
  buf[headers_len - 1] = '\0';
  parse_http_headers(&pbuf, &ri);

  // Make up and send the status line
  status_text = "OK";
  if ((status = get_header(&ri, "Status")) != NULL) {
    conn->status_code = atoi(status);
    status_text = status;
    while (isdigit(* (unsigned char *) status_text) || *status_text == ' ') {
      status_text++;
    }
  } else if (get_header(&ri, "Location") != NULL) {
    conn->status_code = 302;
  } else {
    conn->status_code = 200;
  }
  if (get_header(&ri, "Connection") != NULL &&
      !mg_strcasecmp(get_header(&ri, "Connection"), "keep-alive")) {
    conn->must_close = 1;
  }
  (void) mg_printf(conn, "HTTP/1.1 %d %s\r\n", conn->status_code,
                   status_text);

  // Send headers
  for (i = 0; i < ri.num_headers; i++) {
    mg_printf(conn, "%s: %s\r\n",
              ri.http_headers[i].name, ri.http_headers[i].value);
  }
  mg_write(conn, "\r\n", 2);

  // Send chunk of data that may have been read after the headers
  conn->num_bytes_sent += mg_write(conn, buf + headers_len,
                                   (size_t)(data_len - headers_len));

  // Read the rest of CGI output and send to the client
  send_file_data(conn, out, 0, INT64_MAX);

done:
  if (pid != (pid_t) -1) {
    kill(pid, SIGKILL);
  }
  if (fdin[0] != -1) {
    close(fdin[0]);
  }
  if (fdout[1] != -1) {
    close(fdout[1]);
  }

  if (in != NULL) {
    fclose(in);
  } else if (fdin[1] != -1) {
    close(fdin[1]);
  }

  if (out != NULL) {
    fclose(out);
  } else if (fdout[0] != -1) {
    close(fdout[0]);
  }
}
#endif // !NO_CGI

// For a given PUT path, create all intermediate subdirectories
// for given path. Return 0 if the path itself is a directory,
// or -1 on error, 1 if OK.
static int put_dir(const char *path) {
  char buf[PATH_MAX];
  const char *s, *p;
  struct file file = STRUCT_FILE_INITIALIZER;
  int len, res = 1;

  for (s = p = path + 2; (p = strchr(s, '/')) != NULL; s = ++p) {
    len = p - path;
    if (len >= (int) sizeof(buf)) {
      res = -1;
      break;
    }
    memcpy(buf, path, len);
    buf[len] = '\0';

    // Try to create intermediate directory
    DEBUG_TRACE(("mkdir(%s)", buf));
    if (!mg_stat(buf, &file) && mg_mkdir(buf, 0755) != 0) {
      res = -1;
      break;
    }

    // Is path itself a directory?
    if (p[1] == '\0') {
      res = 0;
    }
  }

  return res;
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

#if defined(USE_WEBSOCKET)

// START OF SHA-1 code
// Copyright(c) By Steve Reid <steve@edmweb.com>
#define SHA1HANDSOFF
#if defined(__sun)
#include "solarisfixes.h"
#endif

union char64long16 { unsigned char c[64]; uint32_t l[16]; };

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

static uint32_t blk0(union char64long16 *block, int i) {
  // Forrest: SHA expect BIG_ENDIAN, swap if LITTLE_ENDIAN
  if (!is_big_endian()) {
    block->l[i] = (rol(block->l[i], 24) & 0xFF00FF00) |
      (rol(block->l[i], 8) & 0x00FF00FF);
  }
  return block->l[i];
}

#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(block, i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
} SHA1_CTX;

static void SHA1Transform(uint32_t state[5], const unsigned char buffer[64]) {
  uint32_t a, b, c, d, e;
  union char64long16 block[1];

  memcpy(block, buffer, 64);
  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];
  R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
  R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
  R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
  R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
  R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
  R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
  R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
  R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
  R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
  R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
  R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
  R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
  R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
  R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
  R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
  R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
  R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
  R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
  R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
  R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;
  a = b = c = d = e = 0;
  memset(block, '\0', sizeof(block));
}

static void SHA1Init(SHA1_CTX* context) {
  context->state[0] = 0x67452301;
  context->state[1] = 0xEFCDAB89;
  context->state[2] = 0x98BADCFE;
  context->state[3] = 0x10325476;
  context->state[4] = 0xC3D2E1F0;
  context->count[0] = context->count[1] = 0;
}

static void SHA1Update(SHA1_CTX* context, const unsigned char* data,
                       uint32_t len) {
  uint32_t i, j;

  j = context->count[0];
  if ((context->count[0] += len << 3) < j)
    context->count[1]++;
  context->count[1] += (len>>29);
  j = (j >> 3) & 63;
  if ((j + len) > 63) {
    memcpy(&context->buffer[j], data, (i = 64-j));
    SHA1Transform(context->state, context->buffer);
    for ( ; i + 63 < len; i += 64) {
      SHA1Transform(context->state, &data[i]);
    }
    j = 0;
  }
  else i = 0;
  memcpy(&context->buffer[j], &data[i], len - i);
}

static void SHA1Final(unsigned char digest[20], SHA1_CTX* context) {
  unsigned i;
  unsigned char finalcount[8], c;

  for (i = 0; i < 8; i++) {
    finalcount[i] = (unsigned char)((context->count[(i >= 4 ? 0 : 1)]
                                     >> ((3-(i & 3)) * 8) ) & 255);
  }
  c = 0200;
  SHA1Update(context, &c, 1);
  while ((context->count[0] & 504) != 448) {
    c = 0000;
    SHA1Update(context, &c, 1);
  }
  SHA1Update(context, finalcount, 8);
  for (i = 0; i < 20; i++) {
    digest[i] = (unsigned char)
      ((context->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
  }
  memset(context, '\0', sizeof(*context));
  memset(&finalcount, '\0', sizeof(finalcount));
}
// END OF SHA1 CODE

static void base64_encode(const unsigned char *src, int src_len, char *dst) {
  static const char *b64 =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int i, j, a, b, c;

  for (i = j = 0; i < src_len; i += 3) {
    a = src[i];
    b = i + 1 >= src_len ? 0 : src[i + 1];
    c = i + 2 >= src_len ? 0 : src[i + 2];

    dst[j++] = b64[a >> 2];
    dst[j++] = b64[((a & 3) << 4) | (b >> 4)];
    if (i + 1 < src_len) {
      dst[j++] = b64[(b & 15) << 2 | (c >> 6)];
    }
    if (i + 2 < src_len) {
      dst[j++] = b64[c & 63];
    }
  }
  while (j % 4 != 0) {
    dst[j++] = '=';
  }
  dst[j++] = '\0';
}

void mg_websocket_handshake(struct mg_connection *conn) {
  static const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  char buf[100], sha[20], b64_sha[sizeof(sha) * 2];
  SHA1_CTX sha_ctx;

  mg_snprintf(buf, sizeof(buf), "%s%s",
              mg_get_header(conn, "Sec-WebSocket-Key"), magic);
  SHA1Init(&sha_ctx);
  SHA1Update(&sha_ctx, (unsigned char *) buf, strlen(buf));
  SHA1Final((unsigned char *) sha, &sha_ctx);
  base64_encode((unsigned char *) sha, sizeof(sha), b64_sha);
  mg_printf(conn, "%s%s%s",
            "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: ", b64_sha, "\r\n\r\n");
}

int mg_websocket_read(struct mg_connection *conn, int *bits, char **data) {
  // Pointer to the beginning of the portion of the incoming websocket message
  // queue. The original websocket upgrade request is never removed,
  // so the queue begins after it.
  unsigned char *buf = (unsigned char *) conn->buf + conn->request_len;
  int n, stop = 0;
  size_t i, len, mask_len, data_len, header_len, body_len;
  char mask[4];

  assert(conn->content_len == 0);

  // Loop continuously, reading messages from the socket, invoking the callback,
  // and waiting repeatedly until an error occurs.
  while (!stop) {
    header_len = 0;
    // body_len is the length of the entire queue in bytes
    // len is the length of the current message
    // data_len is the length of the current message's data payload
    // header_len is the length of the current message's header
    if ((body_len = conn->data_len - conn->request_len) >= 2) {
      len = buf[1] & 127;
      mask_len = buf[1] & 128 ? 4 : 0;
      if (len < 126 && body_len >= mask_len) {
        data_len = len;
        header_len = 2 + mask_len;
      } else if (len == 126 && body_len >= 4 + mask_len) {
        header_len = 4 + mask_len;
        data_len = ((((int) buf[2]) << 8) + buf[3]);
      } else if (body_len >= 10 + mask_len) {
        header_len = 10 + mask_len;
        data_len = (((uint64_t) htonl(* (uint32_t *) &buf[2])) << 32) +
          htonl(* (uint32_t *) &buf[6]);
      }
    }

    // Data layout is as follows:
    //  conn->buf               buf
    //     v                     v              frame1           | frame2
    //     |---------------------|----------------|--------------|-------
    //     |                     |<--header_len-->|<--data_len-->|
    //     |<-conn->request_len->|<-----body_len----------->|
    //     |<-------------------conn->data_len------------->|

    if (header_len > 0) {
      // Allocate space to hold websocket payload
      if ((*data = malloc(data_len)) == NULL) {
        // Allocation failed, exit the loop and then close the connection
        // TODO: notify user about the failure
        data_len = 0;
        break;
      }

      // Save mask and bits, otherwise it may be clobbered by memmove below
      *bits = buf[0];
      memcpy(mask, buf + header_len - mask_len, mask_len);

      // Read frame payload into the allocated buffer.
      assert(body_len >= header_len);
      if (data_len + header_len > body_len) {
        len = body_len - header_len;
        memcpy(*data, buf + header_len, len);
        // TODO: handle pull error
        pull_all(NULL, conn, *data + len, data_len - len);
        conn->data_len = conn->request_len;
      } else {
        len = data_len + header_len;
        memcpy(*data, buf + header_len, data_len);
        memmove(buf, buf + len, body_len - len);
        conn->data_len -= len;
      }

      // Apply mask if necessary
      if (mask_len > 0) {
        for (i = 0; i < data_len; i++) {
          (*data)[i] ^= mask[i % 4];
        }
      }

      return data_len;
    } else {
      // Buffering websocket request
      if ((n = pull(NULL, conn, conn->buf + conn->data_len,
                    conn->buf_size - conn->data_len)) <= 0) {
        break;
      }
      conn->data_len += n;
    }
  }

  return 0;
}

int mg_websocket_write(struct mg_connection* conn, int opcode,
                       const char *data, size_t data_len) {
    unsigned char *copy;
    size_t copy_len = 0;
    int retval = -1;

    if ((copy = (unsigned char *) malloc(data_len + 10)) == NULL) {
      return -1;
    }

    copy[0] = 0x80 + (opcode & 0x0f);

    // Frame format: http://tools.ietf.org/html/rfc6455#section-5.2
    if (data_len < 126) {
      // Inline 7-bit length field
      copy[1] = data_len;
      memcpy(copy + 2, data, data_len);
      copy_len = 2 + data_len;
    } else if (data_len <= 0xFFFF) {
      // 16-bit length field
      copy[1] = 126;
      * (uint16_t *) (copy + 2) = htons(data_len);
      memcpy(copy + 4, data, data_len);
      copy_len = 4 + data_len;
    } else {
      // 64-bit length field
      copy[1] = 127;
      * (uint32_t *) (copy + 2) = htonl((uint64_t) data_len >> 32);
      * (uint32_t *) (copy + 6) = htonl(data_len & 0xffffffff);
      memcpy(copy + 10, data, data_len);
      copy_len = 10 + data_len;
    }

    // Not thread safe
    if (copy_len > 0) {
      retval = mg_write(conn, copy, copy_len);
    }
    free(copy);

    return retval;
}
#endif // !USE_WEBSOCKET

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

static int set_throttle(const char *spec, uint32_t remote_ip, const char *uri) {
  int throttle = 0;
  struct vec vec, val;
  uint32_t net, mask;
  char mult;
  double v;

  while ((spec = next_option(spec, &vec, &val)) != NULL) {
    mult = ',';
    if (sscanf(val.ptr, "%lf%c", &v, &mult) < 1 || v < 0 ||
        (lowercase(&mult) != 'k' && lowercase(&mult) != 'm' && mult != ',')) {
      continue;
    }
    v *= lowercase(&mult) == 'k' ? 1024 : lowercase(&mult) == 'm' ? 1048576 : 1;
    if (vec.len == 1 && vec.ptr[0] == '*') {
      throttle = (int) v;
    } else if (parse_net(vec.ptr, &net, &mask) > 0) {
      if ((remote_ip & mask) == net) {
        throttle = (int) v;
      }
    } else if (match_prefix(vec.ptr, vec.len, uri) > 0) {
      throttle = (int) v;
    }
  }

  return throttle;
}

static uint32_t get_remote_ip(const struct mg_connection *conn) {
  return ntohl(* (uint32_t *) &conn->client.rsa.sin.sin_addr);
}

FILE *mg_upload(struct mg_connection *conn, const char *destination_dir,
                char *path, int path_len) {
  const char *content_type_header, *boundary_start;
  char *buf, fname[1024], boundary[100], *s;
  int bl, n, i, j, headers_len, boundary_len, eof, buf_len, to_read, len = 0;
  FILE *fp;

  // Request looks like this:
  //
  // POST /upload HTTP/1.1
  // Host: 127.0.0.1:8080
  // Content-Length: 244894
  // Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryRVr
  //
  // ------WebKitFormBoundaryRVr
  // Content-Disposition: form-data; name="file"; filename="accum.png"
  // Content-Type: image/png
  //
  //  <89>PNG
  //  <PNG DATA>
  // ------WebKitFormBoundaryRVr

  // Extract boundary string from the Content-Type header
  if ((content_type_header = mg_get_header(conn, "Content-Type")) == NULL ||
      (boundary_start = mg_strcasestr(content_type_header,
                                      "boundary=")) == NULL ||
      (sscanf(boundary_start, "boundary=\"%99[^\"]\"", boundary) == 0 &&
       sscanf(boundary_start, "boundary=%99s", boundary) == 0) ||
      boundary[0] == '\0') {
    return NULL;
  }

  boundary_len = strlen(boundary);
  bl = boundary_len + 4;  // \r\n--<boundary>

  //                     buf
  // conn->buf            |<--------- buf_len ------>|
  //    |=================|==========|===============|
  //    |<--request_len-->|<--len--->|               |
  //    |<-----------data_len------->|      conn->buf + conn->buf_size

  buf = conn->buf + conn->request_len;
  buf_len = conn->buf_size - conn->request_len;
  len = conn->data_len - conn->request_len;

  for (;;) {
    // Pull in headers
    assert(len >= 0 && len <= buf_len);
    to_read = buf_len - len;
    if (to_read > left_to_read(conn)) {
      to_read = (int) left_to_read(conn);
    }
    while (len < buf_len &&
           (n = pull(NULL, conn, buf + len, to_read)) > 0) {
      len += n;
    }
    if ((headers_len = get_request_len(buf, len)) <= 0) {
      break;
    }

    // Fetch file name.
    fname[0] = '\0';
    for (i = j = 0; i < headers_len; i++) {
      if (buf[i] == '\r' && buf[i + 1] == '\n') {
        buf[i] = buf[i + 1] = '\0';
        // TODO(lsm): don't expect filename to be the 3rd field,
        // parse the header properly instead.
        sscanf(&buf[j], "Content-Disposition: %*s %*s filename=\"%1023[^\"]",
               fname);
        j = i + 2;
      }
    }

    // Give up if the headers are not what we expect
    if (fname[0] == '\0') {
      break;
    }

    // Move data to the beginning of the buffer
    assert(len >= headers_len);
    memmove(buf, &buf[headers_len], len - headers_len);
    len -= headers_len;
    conn->data_len = conn->request_len + len;

    // We open the file with exclusive lock held. This guarantee us
    // there is no other thread can save into the same file simultaneously.
    fp = NULL;

    // Construct destination file name. Do not allow paths to have slashes.
    if ((s = strrchr(fname, '/')) == NULL &&
        (s = strrchr(fname, '\\')) == NULL) {
      s = fname;
    }

    // Open file in binary mode. TODO: set an exclusive lock.
    snprintf(path, path_len, "%s/%s", destination_dir, s);
    if ((fp = fopen(path, "wb")) == NULL) {
      break;
    }

    // Read POST data, write into file until boundary is found.
    eof = n = 0;
    do {
      len += n;
      for (i = 0; i < len - bl; i++) {
        if (!memcmp(&buf[i], "\r\n--", 4) &&
            !memcmp(&buf[i + 4], boundary, boundary_len)) {
          // Found boundary, that's the end of file data.
          fwrite(buf, 1, i, fp);
          eof = 1;
          memmove(buf, &buf[i + bl], len - (i + bl));
          len -= i + bl;
          break;
        }
      }
      if (!eof && len > bl) {
        fwrite(buf, 1, len - bl, fp);
        memmove(buf, &buf[len - bl], bl);
        len = bl;
      }
      to_read = buf_len - len;
      if (to_read > left_to_read(conn)) {
        to_read = (int) left_to_read(conn);
      }
    } while (!eof && (n = pull(NULL, conn, buf + len, to_read)) > 0);
    conn->data_len = conn->request_len + len;

    if (eof) {
      rewind(fp);
      return fp;
    } else {
      fclose(fp);
    }
  }

  return NULL;
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
  conn->throttle = set_throttle(conn->ctx->config[THROTTLE],
                                get_remote_ip(conn), ri->uri);
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
  conn->must_close = conn->request_len = conn->throttle = 0;
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

static int getreq(struct mg_connection *conn, char *ebuf, size_t ebuf_len) {
  const char *cl;

  ebuf[0] = '\0';
  reset_per_request_attributes(conn);
  conn->request_len = read_request(NULL, conn, conn->buf, conn->buf_size,
                                   &conn->data_len);
  assert(conn->request_len < 0 || conn->data_len >= conn->request_len);

  if (conn->request_len == 0 && conn->data_len == conn->buf_size) {
    snprintf(ebuf, ebuf_len, "%s", "Request Too Large");
  } else if (conn->request_len <= 0) {
    snprintf(ebuf, ebuf_len, "%s", "Client closed connection");
  } else if (parse_http_message(conn->buf, conn->buf_size,
                                &conn->request_info) <= 0) {
    snprintf(ebuf, ebuf_len, "Bad request: [%.*s]", conn->data_len, conn->buf);
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
  return ebuf[0] == '\0';
}

static void process_new_connection(struct mg_connection *conn) {
  struct mg_request_info *ri = &conn->request_info;
  int keep_alive_enabled, keep_alive, discard_len;
  char ebuf[100];

  keep_alive_enabled = !strcmp(conn->ctx->config[ENABLE_KEEP_ALIVE], "yes");
  keep_alive = 0;

  // Important: on new connection, reset the receiving buffer. Credit goes
  // to crule42.
  conn->data_len = 0;
  do {
    if (!getreq(conn, ebuf, sizeof(ebuf))) {
      send_http_error(conn, 500, "Server Error", "%s", ebuf);
      conn->must_close = 1;
    } else if (!is_valid_uri(conn->request_info.uri)) {
      snprintf(ebuf, sizeof(ebuf), "Invalid URI: [%s]", ri->uri);
      send_http_error(conn, 400, "Bad Request", "%s", ebuf);
    } else if (strcmp(ri->http_version, "1.0") &&
               strcmp(ri->http_version, "1.1")) {
      snprintf(ebuf, sizeof(ebuf), "Bad HTTP version: [%s]", ri->http_version);
      send_http_error(conn, 505, "Bad HTTP version", "%s", ebuf);
    }

    if (ebuf[0] == '\0') {
      handle_request(conn);
      call_user(MG_REQUEST_END, conn, (void *) (int *) conn->status_code);
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
  (void) pthread_mutex_lock(&ctx->mutex);
  DEBUG_TRACE(("going idle"));

  // If the queue is empty, wait. We're idle at this point.
  while (ctx->sq_head == ctx->sq_tail && ctx->stop_flag == 0) {
    pthread_cond_wait(&ctx->sq_full, &ctx->mutex);
  }

  // If we're stopping, sq_head may be equal to sq_tail.
  if (ctx->sq_head > ctx->sq_tail) {
    // Copy socket from the queue and increment tail
    *sp = ctx->queue[ctx->sq_tail % ARRAY_SIZE(ctx->queue)];
    ctx->sq_tail++;
    DEBUG_TRACE(("grabbed socket %d, going busy", sp->sock));

    // Wrap pointers if needed
    while (ctx->sq_tail > (int) ARRAY_SIZE(ctx->queue)) {
      ctx->sq_tail -= ARRAY_SIZE(ctx->queue);
      ctx->sq_head -= ARRAY_SIZE(ctx->queue);
    }
  }

  (void) pthread_cond_signal(&ctx->sq_empty);
  (void) pthread_mutex_unlock(&ctx->mutex);

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
  (void) pthread_mutex_lock(&ctx->mutex);
  ctx->num_threads--;
  (void) pthread_cond_signal(&ctx->cond);
  assert(ctx->num_threads >= 0);
  (void) pthread_mutex_unlock(&ctx->mutex);

  DEBUG_TRACE(("exiting"));
  return NULL;
}

// Master thread adds accepted socket to a queue
static void produce_socket(struct mg_context *ctx, const struct socket *sp) {
  (void) pthread_mutex_lock(&ctx->mutex);

  // If the queue is full, wait
  while (ctx->stop_flag == 0 &&
         ctx->sq_head - ctx->sq_tail >= (int) ARRAY_SIZE(ctx->queue)) {
    (void) pthread_cond_wait(&ctx->sq_empty, &ctx->mutex);
  }

  if (ctx->sq_head - ctx->sq_tail < (int) ARRAY_SIZE(ctx->queue)) {
    // Copy socket to the queue and increment head
    ctx->queue[ctx->sq_head % ARRAY_SIZE(ctx->queue)] = *sp;
    ctx->sq_head++;
    DEBUG_TRACE(("queued socket %d", sp->sock));
  }

  (void) pthread_cond_signal(&ctx->sq_full);
  (void) pthread_mutex_unlock(&ctx->mutex);
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
  pthread_cond_broadcast(&ctx->sq_full);

  // Wait until all threads finish
  (void) pthread_mutex_lock(&ctx->mutex);
  while (ctx->num_threads > 0) {
    (void) pthread_cond_wait(&ctx->cond, &ctx->mutex);
  }
  (void) pthread_mutex_unlock(&ctx->mutex);

  // All threads exited, no sync is needed. Destroy mutex and condvars
  (void) pthread_mutex_destroy(&ctx->mutex);
  (void) pthread_cond_destroy(&ctx->cond);
  (void) pthread_cond_destroy(&ctx->sq_empty);
  (void) pthread_cond_destroy(&ctx->sq_full);

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

  (void) pthread_mutex_init(&ctx->mutex, NULL);
  (void) pthread_cond_init(&ctx->cond, NULL);
  (void) pthread_cond_init(&ctx->sq_empty, NULL);
  (void) pthread_cond_init(&ctx->sq_full, NULL);

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
