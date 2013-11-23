#include "internal.h"

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
  int headers_len, data_len, i, fdin[2] = {-1, -1}, fdout[2] = {-1, -1};
  const char *status, *status_text;
  char buf[16384], *pbuf, dir[PATH_MAX], *p;
  struct mg_request_info ri;
  struct cgi_env_block blk;
  FILE *in = NULL, *out = NULL;
  pid_t pid = (pid_t) -1;

  memset(&ri, 0, sizeof(ri));
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

