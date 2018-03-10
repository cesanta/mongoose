/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_HTTP && MG_ENABLE_HTTP_WEBDAV

MG_INTERNAL int mg_is_dav_request(const struct mg_str *s) {
  static const char *methods[] = {
    "PUT",
    "DELETE",
    "MKCOL",
    "PROPFIND",
    "MOVE"
#if MG_ENABLE_FAKE_DAVLOCK
    ,
    "LOCK",
    "UNLOCK"
#endif
  };
  size_t i;

  for (i = 0; i < ARRAY_SIZE(methods); i++) {
    if (mg_vcmp(s, methods[i]) == 0) {
      return 1;
    }
  }

  return 0;
}

static int mg_mkdir(const char *path, uint32_t mode) {
#ifndef _WIN32
  return mkdir(path, mode);
#else
  (void) mode;
  return _mkdir(path);
#endif
}

static void mg_print_props(struct mg_connection *nc, const char *name,
                           cs_stat_t *stp) {
  char mtime[64];
  time_t t = stp->st_mtime; /* store in local variable for NDK compile */
  struct mg_str name_esc = mg_url_encode(mg_mk_str(name));
  mg_gmt_time_string(mtime, sizeof(mtime), &t);
  mg_printf(nc,
            "<d:response>"
            "<d:href>%s</d:href>"
            "<d:propstat>"
            "<d:prop>"
            "<d:resourcetype>%s</d:resourcetype>"
            "<d:getcontentlength>%" INT64_FMT
            "</d:getcontentlength>"
            "<d:getlastmodified>%s</d:getlastmodified>"
            "</d:prop>"
            "<d:status>HTTP/1.1 200 OK</d:status>"
            "</d:propstat>"
            "</d:response>\n",
            name_esc.p, S_ISDIR(stp->st_mode) ? "<d:collection/>" : "",
            (int64_t) stp->st_size, mtime);
  free((void *) name_esc.p);
}

MG_INTERNAL void mg_handle_propfind(struct mg_connection *nc, const char *path,
                                    cs_stat_t *stp, struct http_message *hm,
                                    struct mg_serve_http_opts *opts) {
  static const char header[] =
      "HTTP/1.1 207 Multi-Status\r\n"
      "Connection: close\r\n"
      "Content-Type: text/xml; charset=utf-8\r\n\r\n"
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
      "<d:multistatus xmlns:d='DAV:'>\n";
  static const char footer[] = "</d:multistatus>\n";
  const struct mg_str *depth = mg_get_http_header(hm, "Depth");

  /* Print properties for the requested resource itself */
  if (S_ISDIR(stp->st_mode) &&
      strcmp(opts->enable_directory_listing, "yes") != 0) {
    mg_printf(nc, "%s", "HTTP/1.1 403 Directory Listing Denied\r\n\r\n");
  } else {
    char uri[MG_MAX_PATH];
    mg_send(nc, header, sizeof(header) - 1);
    snprintf(uri, sizeof(uri), "%.*s", (int) hm->uri.len, hm->uri.p);
    mg_print_props(nc, uri, stp);
    if (S_ISDIR(stp->st_mode) && (depth == NULL || mg_vcmp(depth, "0") != 0)) {
      mg_scan_directory(nc, path, opts, mg_print_props);
    }
    mg_send(nc, footer, sizeof(footer) - 1);
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
}

#if MG_ENABLE_FAKE_DAVLOCK
/*
 * Windows explorer (probably there are another WebDav clients like it)
 * requires LOCK support in webdav. W/out this, it still works, but fails
 * to save file: shows error message and offers "Save As".
 * "Save as" works, but this message is very annoying.
 * This is fake lock, which doesn't lock something, just returns LOCK token,
 * UNLOCK always answers "OK".
 * With this fake LOCK Windows Explorer looks happy and saves file.
 * NOTE: that is not DAV LOCK imlementation, it is just a way to shut up
 * Windows native DAV client. This is why FAKE LOCK is not enabed by default
 */
MG_INTERNAL void mg_handle_lock(struct mg_connection *nc, const char *path) {
  static const char *reply =
      "HTTP/1.1 207 Multi-Status\r\n"
      "Connection: close\r\n"
      "Content-Type: text/xml; charset=utf-8\r\n\r\n"
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
      "<d:multistatus xmlns:d='DAV:'>\n"
      "<D:lockdiscovery>\n"
      "<D:activelock>\n"
      "<D:locktoken>\n"
      "<D:href>\n"
      "opaquelocktoken:%s%u"
      "</D:href>"
      "</D:locktoken>"
      "</D:activelock>\n"
      "</D:lockdiscovery>"
      "</d:multistatus>\n";
  mg_printf(nc, reply, path, (unsigned int) mg_time());
  nc->flags |= MG_F_SEND_AND_CLOSE;
}
#endif

MG_INTERNAL void mg_handle_mkcol(struct mg_connection *nc, const char *path,
                                 struct http_message *hm) {
  int status_code = 500;
  if (hm->body.len != (size_t) ~0 && hm->body.len > 0) {
    status_code = 415;
  } else if (!mg_mkdir(path, 0755)) {
    status_code = 201;
  } else if (errno == EEXIST) {
    status_code = 405;
  } else if (errno == EACCES) {
    status_code = 403;
  } else if (errno == ENOENT) {
    status_code = 409;
  } else {
    status_code = 500;
  }
  mg_http_send_error(nc, status_code, NULL);
}

static int mg_remove_directory(const struct mg_serve_http_opts *opts,
                               const char *dir) {
  char path[MG_MAX_PATH];
  struct dirent *dp;
  cs_stat_t st;
  DIR *dirp;

  if ((dirp = opendir(dir)) == NULL) return 0;

  while ((dp = readdir(dirp)) != NULL) {
    if (mg_is_file_hidden((const char *) dp->d_name, opts, 1)) {
      continue;
    }
    snprintf(path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);
    mg_stat(path, &st);
    if (S_ISDIR(st.st_mode)) {
      mg_remove_directory(opts, path);
    } else {
      remove(path);
    }
  }
  closedir(dirp);
  rmdir(dir);

  return 1;
}

MG_INTERNAL void mg_handle_move(struct mg_connection *c,
                                const struct mg_serve_http_opts *opts,
                                const char *path, struct http_message *hm) {
  const struct mg_str *dest = mg_get_http_header(hm, "Destination");
  if (dest == NULL) {
    mg_http_send_error(c, 411, NULL);
  } else {
    const char *p = (char *) memchr(dest->p, '/', dest->len);
    if (p != NULL && p[1] == '/' &&
        (p = (char *) memchr(p + 2, '/', dest->p + dest->len - p)) != NULL) {
      char buf[MG_MAX_PATH];
      snprintf(buf, sizeof(buf), "%s%.*s", opts->dav_document_root,
               (int) (dest->p + dest->len - p), p);
      if (rename(path, buf) == 0) {
        mg_http_send_error(c, 200, NULL);
      } else {
        mg_http_send_error(c, 418, NULL);
      }
    } else {
      mg_http_send_error(c, 500, NULL);
    }
  }
}

MG_INTERNAL void mg_handle_delete(struct mg_connection *nc,
                                  const struct mg_serve_http_opts *opts,
                                  const char *path) {
  cs_stat_t st;
  if (mg_stat(path, &st) != 0) {
    mg_http_send_error(nc, 404, NULL);
  } else if (S_ISDIR(st.st_mode)) {
    mg_remove_directory(opts, path);
    mg_http_send_error(nc, 204, NULL);
  } else if (remove(path) == 0) {
    mg_http_send_error(nc, 204, NULL);
  } else {
    mg_http_send_error(nc, 423, NULL);
  }
}

/* Return -1 on error, 1 on success. */
static int mg_create_itermediate_directories(const char *path) {
  const char *s;

  /* Create intermediate directories if they do not exist */
  for (s = path + 1; *s != '\0'; s++) {
    if (*s == '/') {
      char buf[MG_MAX_PATH];
      cs_stat_t st;
      snprintf(buf, sizeof(buf), "%.*s", (int) (s - path), path);
      buf[sizeof(buf) - 1] = '\0';
      if (mg_stat(buf, &st) != 0 && mg_mkdir(buf, 0755) != 0) {
        return -1;
      }
    }
  }

  return 1;
}

MG_INTERNAL void mg_handle_put(struct mg_connection *nc, const char *path,
                               struct http_message *hm) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  cs_stat_t st;
  const struct mg_str *cl_hdr = mg_get_http_header(hm, "Content-Length");
  int rc, status_code = mg_stat(path, &st) == 0 ? 200 : 201;

  mg_http_free_proto_data_file(&pd->file);
  if ((rc = mg_create_itermediate_directories(path)) == 0) {
    mg_printf(nc, "HTTP/1.1 %d OK\r\nContent-Length: 0\r\n\r\n", status_code);
  } else if (rc == -1) {
    mg_http_send_error(nc, 500, NULL);
  } else if (cl_hdr == NULL) {
    mg_http_send_error(nc, 411, NULL);
  } else if ((pd->file.fp = mg_fopen(path, "w+b")) == NULL) {
    mg_http_send_error(nc, 500, NULL);
  } else {
    const struct mg_str *range_hdr = mg_get_http_header(hm, "Content-Range");
    int64_t r1 = 0, r2 = 0;
    pd->file.type = DATA_PUT;
    mg_set_close_on_exec((sock_t) fileno(pd->file.fp));
    pd->file.cl = to64(cl_hdr->p);
    if (range_hdr != NULL &&
        mg_http_parse_range_header(range_hdr, &r1, &r2) > 0) {
      status_code = 206;
      fseeko(pd->file.fp, r1, SEEK_SET);
      pd->file.cl = r2 > r1 ? r2 - r1 + 1 : pd->file.cl - r1;
    }
    mg_printf(nc, "HTTP/1.1 %d OK\r\nContent-Length: 0\r\n\r\n", status_code);
    /* Remove HTTP request from the mbuf, leave only payload */
    mbuf_remove(&nc->recv_mbuf, hm->message.len - hm->body.len);
    mg_http_transfer_file_data(nc);
  }
}

#endif /* MG_ENABLE_HTTP && MG_ENABLE_HTTP_WEBDAV */
