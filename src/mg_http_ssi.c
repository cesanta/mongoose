/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_HTTP && MG_ENABLE_HTTP_SSI && MG_ENABLE_FILESYSTEM

static void mg_send_ssi_file(struct mg_connection *nc, struct http_message *hm,
                             const char *path, FILE *fp, int include_level,
                             const struct mg_serve_http_opts *opts);

static void mg_send_file_data(struct mg_connection *nc, FILE *fp) {
  char buf[BUFSIZ];
  size_t n;
  while ((n = mg_fread(buf, 1, sizeof(buf), fp)) > 0) {
    mg_send(nc, buf, n);
  }
}

static void mg_do_ssi_include(struct mg_connection *nc, struct http_message *hm,
                              const char *ssi, char *tag, int include_level,
                              const struct mg_serve_http_opts *opts) {
  char file_name[MG_MAX_PATH], path[MG_MAX_PATH], *p;
  FILE *fp;

  /*
   * sscanf() is safe here, since send_ssi_file() also uses buffer
   * of size MG_BUF_LEN to get the tag. So strlen(tag) is always < MG_BUF_LEN.
   */
  if (sscanf(tag, " virtual=\"%[^\"]\"", file_name) == 1) {
    /* File name is relative to the webserver root */
    snprintf(path, sizeof(path), "%s/%s", opts->document_root, file_name);
  } else if (sscanf(tag, " abspath=\"%[^\"]\"", file_name) == 1) {
    /*
     * File name is relative to the webserver working directory
     * or it is absolute system path
     */
    snprintf(path, sizeof(path), "%s", file_name);
  } else if (sscanf(tag, " file=\"%[^\"]\"", file_name) == 1 ||
             sscanf(tag, " \"%[^\"]\"", file_name) == 1) {
    /* File name is relative to the currect document */
    snprintf(path, sizeof(path), "%s", ssi);
    if ((p = strrchr(path, DIRSEP)) != NULL) {
      p[1] = '\0';
    }
    snprintf(path + strlen(path), sizeof(path) - strlen(path), "%s", file_name);
  } else {
    mg_printf(nc, "Bad SSI #include: [%s]", tag);
    return;
  }

  if ((fp = mg_fopen(path, "rb")) == NULL) {
    mg_printf(nc, "SSI include error: mg_fopen(%s): %s", path,
              strerror(mg_get_errno()));
  } else {
    mg_set_close_on_exec((sock_t) fileno(fp));
    if (mg_match_prefix(opts->ssi_pattern, strlen(opts->ssi_pattern), path) >
        0) {
      mg_send_ssi_file(nc, hm, path, fp, include_level + 1, opts);
    } else {
      mg_send_file_data(nc, fp);
    }
    fclose(fp);
  }
}

#if MG_ENABLE_HTTP_SSI_EXEC
static void do_ssi_exec(struct mg_connection *nc, char *tag) {
  char cmd[BUFSIZ];
  FILE *fp;

  if (sscanf(tag, " \"%[^\"]\"", cmd) != 1) {
    mg_printf(nc, "Bad SSI #exec: [%s]", tag);
  } else if ((fp = popen(cmd, "r")) == NULL) {
    mg_printf(nc, "Cannot SSI #exec: [%s]: %s", cmd, strerror(mg_get_errno()));
  } else {
    mg_send_file_data(nc, fp);
    pclose(fp);
  }
}
#endif /* MG_ENABLE_HTTP_SSI_EXEC */

/*
 * SSI directive has the following format:
 * <!--#directive parameter=value parameter=value -->
 */
static void mg_send_ssi_file(struct mg_connection *nc, struct http_message *hm,
                             const char *path, FILE *fp, int include_level,
                             const struct mg_serve_http_opts *opts) {
  static const struct mg_str btag = MG_MK_STR("<!--#");
  static const struct mg_str d_include = MG_MK_STR("include");
  static const struct mg_str d_call = MG_MK_STR("call");
#if MG_ENABLE_HTTP_SSI_EXEC
  static const struct mg_str d_exec = MG_MK_STR("exec");
#endif
  char buf[BUFSIZ], *p = buf + btag.len; /* p points to SSI directive */
  int ch, len, in_ssi_tag;

  if (include_level > 10) {
    mg_printf(nc, "SSI #include level is too deep (%s)", path);
    return;
  }

  in_ssi_tag = len = 0;
  while ((ch = fgetc(fp)) != EOF) {
    if (in_ssi_tag && ch == '>' && buf[len - 1] == '-' && buf[len - 2] == '-') {
      size_t i = len - 2;
      in_ssi_tag = 0;

      /* Trim closing --> */
      buf[i--] = '\0';
      while (i > 0 && buf[i] == ' ') {
        buf[i--] = '\0';
      }

      /* Handle known SSI directives */
      if (strncmp(p, d_include.p, d_include.len) == 0) {
        mg_do_ssi_include(nc, hm, path, p + d_include.len + 1, include_level,
                          opts);
      } else if (strncmp(p, d_call.p, d_call.len) == 0) {
        struct mg_ssi_call_ctx cctx;
        memset(&cctx, 0, sizeof(cctx));
        cctx.req = hm;
        cctx.file = mg_mk_str(path);
        cctx.arg = mg_mk_str(p + d_call.len + 1);
        mg_call(nc, NULL, nc->user_data, MG_EV_SSI_CALL,
                (void *) cctx.arg.p); /* NUL added above */
        mg_call(nc, NULL, nc->user_data, MG_EV_SSI_CALL_CTX, &cctx);
#if MG_ENABLE_HTTP_SSI_EXEC
      } else if (strncmp(p, d_exec.p, d_exec.len) == 0) {
        do_ssi_exec(nc, p + d_exec.len + 1);
#endif
      } else {
        /* Silently ignore unknown SSI directive. */
      }
      len = 0;
    } else if (ch == '<') {
      in_ssi_tag = 1;
      if (len > 0) {
        mg_send(nc, buf, (size_t) len);
      }
      len = 0;
      buf[len++] = ch & 0xff;
    } else if (in_ssi_tag) {
      if (len == (int) btag.len && strncmp(buf, btag.p, btag.len) != 0) {
        /* Not an SSI tag */
        in_ssi_tag = 0;
      } else if (len == (int) sizeof(buf) - 2) {
        mg_printf(nc, "%s: SSI tag is too large", path);
        len = 0;
      }
      buf[len++] = ch & 0xff;
    } else {
      buf[len++] = ch & 0xff;
      if (len == (int) sizeof(buf)) {
        mg_send(nc, buf, (size_t) len);
        len = 0;
      }
    }
  }

  /* Send the rest of buffered data */
  if (len > 0) {
    mg_send(nc, buf, (size_t) len);
  }
}

MG_INTERNAL void mg_handle_ssi_request(struct mg_connection *nc,
                                       struct http_message *hm,
                                       const char *path,
                                       const struct mg_serve_http_opts *opts) {
  FILE *fp;
  struct mg_str mime_type;
  DBG(("%p %s", nc, path));

  if ((fp = mg_fopen(path, "rb")) == NULL) {
    mg_http_send_error(nc, 404, NULL);
  } else {
    mg_set_close_on_exec((sock_t) fileno(fp));

    mime_type = mg_get_mime_type(path, "text/plain", opts);
    mg_send_response_line(nc, 200, opts->extra_headers);
    mg_printf(nc,
              "Content-Type: %.*s\r\n"
              "Connection: close\r\n\r\n",
              (int) mime_type.len, mime_type.p);
    mg_send_ssi_file(nc, hm, path, fp, 0, opts);
    fclose(fp);
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
}

#endif /* MG_ENABLE_HTTP_SSI && MG_ENABLE_HTTP && MG_ENABLE_FILESYSTEM */
