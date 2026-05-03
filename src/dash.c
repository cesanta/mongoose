#include "dash.h"
#include "util.h"

#define MG_NO_CACHE_HEADERS "Cache-Control: no-cache\r\n"
#define MG_JSON_HEADERS "Content-Type: application/json\r\n" MG_NO_CACHE_HEADERS

struct mg_dash_user {
  struct mg_dash_user *next;
  char name[32];    // User name
  char token[21];   // Login token
  int level;        // Access level
  uint64_t expire;  // Expiration timestamp
};

struct mg_upload_state {
  char marker;      // Tells that we're a file upload connection
  size_t expected;  // POST data length, bytes
  size_t received;  // Already received bytes
  void *ctx;        // OTA context
};

static struct mg_str trimq(struct mg_str s) {  // Trim double quotes
  if (s.len > 1 && s.buf[0] == '"') s.len -= 2, s.buf++;
  return s;
}

static void mg_dash_broadcast(struct mg_mgr *mgr, const char *fmt, ...) {
  struct mg_connection *c;
  va_list ap;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (!c->is_websocket) continue;
    if (c->send.len > MG_DASH_MAX_SEND_BUF_SIZE) {
      MG_ERROR(("%lu buffered data %lu > MG_DASH_MAX_SEND_BUF_SIZE, throttled",
                c->id, c->send.len));
      continue;
    }
    va_start(ap, fmt);
    mg_ws_vprintf(c, WEBSOCKET_OP_TEXT, fmt, &ap);
    va_end(ap);
  }
}

static size_t mg_print_fmt(mg_pfn_t out, void *param, va_list *ap) {
  const char *fmt = va_arg(*ap, const char *);
  ap = va_arg(*ap, va_list *);
  return mg_vxprintf(out, param, fmt, ap);
}

static void mg_dash_success(struct mg_connection *c, struct mg_str req,
                            const char *fmt, ...) {
  struct mg_str id = mg_json_get_tok(req, "$.id");
  va_list ap;
  va_start(ap, fmt);
  mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%.*s,%m:%M}", MG_ESC("id"), id.len,
               id.buf, MG_ESC("result"), mg_print_fmt, fmt, &ap);
  va_end(ap);
}

static void mg_dash_error(struct mg_connection *c, struct mg_str req,
                          const char *fmt, ...) {
  struct mg_str id = mg_json_get_tok(req, "$.id");
  va_list ap;
  va_start(ap, fmt);
  mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%.*s,%m:{%m:%d,%m:%m}}", MG_ESC("id"),
               id.len, id.buf, MG_ESC("error"), MG_ESC("code"), -1,
               MG_ESC("message"), mg_print_fmt, fmt, &ap);
  va_end(ap);
}

static struct mg_field_set *mg_dash_find_field_set(struct mg_dash *dash,
                                                   struct mg_str name) {
  struct mg_field_set *fs;
  for (fs = dash->sets; fs != NULL; fs = fs->next) {
    if (mg_strcmp(name, mg_str(fs->name)) == 0) return fs;
  }
  return NULL;
}

// static struct mg_field *mg_dash_find_field(struct mg_field *fields,
//                                            struct mg_str name) {
//   size_t i;
//   for (i = 0; fields != NULL && fields[i].name != NULL; i++) {
//     if (mg_strcmp(name, mg_str(fields[i].name)) == 0) return &fields[i];
//   }
//   return NULL;
// }

static size_t mg_print_field(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_field *f = va_arg(*ap, struct mg_field *);
  size_t n = 0;
  n += mg_xprintf(fn, arg, "%m:", MG_ESC(f->name));
  if (f->type == MG_VAL_BOOL) {
    n += mg_xprintf(fn, arg, "%s", *(bool *) f->value ? "true" : "false");
  } else if (f->type == MG_VAL_INT) {
    n += mg_xprintf(fn, arg, "%d", *(int *) f->value);
  } else if (f->type == MG_VAL_DBL) {
    n += mg_xprintf(fn, arg, "%.2f", *(double *) f->value);
  } else if (f->type == MG_VAL_STR) {
    n += mg_xprintf(fn, arg, "%m", MG_ESC(f->value));
  } else if (f->type == MG_VAL_RAW) {
    n += mg_xprintf(fn, arg, "%s", f->value);
  } else {
    n += mg_xprintf(fn, arg, "null");
  }
  return n;
}

static size_t mg_print_field_set(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_field_set *set = va_arg(*ap, struct mg_field_set *);
  size_t i, n = 0;
  n += mg_xprintf(fn, arg, "{");
  for (i = 0; set != NULL && set->fields[i].name != NULL; i++) {
    if (i > 0) n += mg_xprintf(fn, arg, ",");
    n += mg_xprintf(fn, arg, "%M", mg_print_field, &set->fields[i]);
  }
  n += mg_xprintf(fn, arg, "}");
  return n;
}

static size_t mg_dash_print_name(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_dash *dash = va_arg(*ap, struct mg_dash *);
  struct mg_str *name = va_arg(*ap, struct mg_str *);
  struct mg_field_set *set = mg_dash_find_field_set(dash, *name);
  size_t n = 0;
  if (name->len == 0) {
    struct mg_field_set *fs;
    const char *comma = "";
    n += mg_xprintf(fn, arg, "{");
    for (fs = dash->sets; fs != NULL; fs = fs->next) {
      if (fs->reader) fs->reader();
      n += mg_xprintf(fn, arg, comma);
      n += mg_xprintf(fn, arg, "%m:", MG_ESC(fs->name));
      n += mg_xprintf(fn, arg, "%M", mg_print_field_set, fs);
      comma = ",";
    }
    n += mg_xprintf(fn, arg, "}");
  } else if (set != NULL) {
    if (set->reader) set->reader();
    n += mg_xprintf(fn, arg, "%M", mg_print_field_set, set);
  } else {
    n += mg_xprintf(fn, arg, "null");
  }
  return n;
}

static size_t mg_dash_print(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_str *req = va_arg(*ap, struct mg_str *);
  struct mg_dash *dash = va_arg(*ap, struct mg_dash *);
  struct mg_str name = trimq(mg_json_get_tok(*req, "$.params"));
  return mg_xprintf(fn, arg, "%M", mg_dash_print_name, dash, &name);
}

void mg_dash_send_change(struct mg_mgr *mgr, struct mg_field_set *set) {
  if (set->reader) set->reader();
  mg_dash_broadcast(mgr, "{%m:%m,%m:{%m:%M}}", MG_ESC("method"),
                    MG_ESC("change"), MG_ESC("params"), MG_ESC(set->name),
                    mg_print_field_set, set);
}

static int mg_dash_parse_field(struct mg_str json, struct mg_field *f) {
  char json_path[128];
  bool ok = false;
  mg_snprintf(json_path, sizeof(json_path), "$.%s", f->name);
  if (f->type == MG_VAL_BOOL) {
    ok = mg_json_get_bool(json, json_path, (bool *) f->value);
  } else if (f->type == MG_VAL_INT) {
    double d;
    if (mg_json_get_num(json, json_path, &d) && d == (int) d) {
      *(int *) f->value = (int) d;
      ok = true;
    }
  } else if (f->type == MG_VAL_DBL) {
    ok = mg_json_get_num(json, json_path, (double *) f->value);
  } else if (f->type == MG_VAL_STR) {
    ok = mg_json_unescape(json, json_path, (char *) f->value, f->value_size);
  } else if (f->type == MG_VAL_RAW) {
    ok = mg_snprintf((char *) f->value, f->value_size, "%.*s", json.len,
                     json.buf) == json.len;
  }
  return ok;
}

static int mg_dash_apply(struct mg_connection *c, struct mg_dash *dash,
                         struct mg_str json) {
  struct mg_str key, val;
  size_t ofs = 0;
  int total_count = 0;
  while ((ofs = mg_json_next(json, ofs, &key, &val)) > 0) {
    struct mg_field_set *set = mg_dash_find_field_set(dash, trimq(key));
    int count = 0;
    if (set != NULL) {
      size_t i;
      for (i = 0; set->fields[i].name != NULL; i++) {
        if (mg_dash_parse_field(val, &set->fields[i])) count++;
      }
      if (count) {
        if (set->writer) set->writer();
        mg_dash_send_change(c->mgr, set);
        total_count += count;
      }
    } else {
      MG_ERROR(("UNKNOWN SET: [%.*s]", key.len, key.buf));
    }
  }
  return total_count;
}

static void mg_dash_process_msg(struct mg_connection *c,
                                struct mg_ws_message *wm,
                                struct mg_dash *dash) {
  struct mg_str req = wm->data;
  struct mg_str method = trimq(mg_json_get_tok(req, "$.method"));
  if (mg_match(method, mg_str("get"), NULL)) {
    mg_dash_success(c, req, "%M", mg_dash_print, &req, dash);
  } else if (mg_match(method, mg_str("set"), NULL)) {
    struct mg_str params = trimq(mg_json_get_tok(req, "$.params"));
    int count = mg_dash_apply(c, dash, params);
    mg_dash_success(c, req, "%d", count);
  } else {
    mg_dash_error(c, req, "%s", "unknown method");
  }
}

void mg_dash_file_add(struct mg_str name, size_t size) {
  struct mg_dash_file *f = (struct mg_dash_file *) mg_calloc(1, sizeof(*f));
  f->name = mg_strdup(name).buf;
  f->size = size;
  f->next = mg_dash_files;
  mg_dash_files = f;
}

void mg_dash_file_del(struct mg_str name) {
  struct mg_dash_file **head, *f;
  for (head = &mg_dash_files, f = *head; f; head = &(*head)->next, f = *head) {
    if (mg_strcmp(mg_str(f->name), name) == 0) {
      MG_INFO(("Deleting %s", f->name));
      *head = f->next;
      mg_free(f->name);
      mg_free(f);
      return;
    }
  }
}

// Files array: [{"name": "foo.txt", "size": 1234}]
struct mg_dash_file *mg_dash_files;
static char s_files[1024];

static void read_files(void) {
  size_t len = 0;
  struct mg_dash_file *f;
  len += mg_snprintf(s_files + len, sizeof(s_files) - len, "[");
  for (f = mg_dash_files; f; f = f->next) {
    len += mg_snprintf(s_files + len, sizeof(s_files) - len, "%s{%m:%m,%m:%u}",
                       len > 1 ? "," : "", MG_ESC("name"), MG_ESC(f->name),
                       MG_ESC("size"), f->size);
  }
  len += mg_snprintf(s_files + len, sizeof(s_files) - len, "]");
}

static struct mg_field fields_files[] = {
    {"data", MG_VAL_RAW, s_files, sizeof(s_files)},
    {NULL, MG_VAL_INT, NULL, 0},
};

static struct mg_field_set set_files = {
    "files", fields_files, read_files, NULL, 0, 0, NULL,
};

static void file_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  // struct mg_dash *dash = (struct mg_dash *) c->fn_data;
  struct mg_upload_state *us = (struct mg_upload_state *) c->data;
  if ((ev == MG_EV_HTTP_HDRS || ev == MG_EV_HTTP_MSG) && us->marker == 0) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (hm->uri.len > 7) hm->uri.len -= 7, hm->uri.buf += 7;  // Strip /files/
    if (sizeof(*us) > sizeof(c->data)) {
      mg_error(c,
               "FAILURE: sizeof(c->data) == %lu, need %lu."
               " Set #define MG_DATA_SIZE XXX",
               sizeof(c->data), sizeof(*us));
    } else if (mg_strcmp(hm->query, mg_str("action=delete")) == 0) {
      // Query string ?action=delete - file deletion
      mg_dash_file_del(hm->uri);
      mg_dash_send_change(c->mgr, &set_files);
      mg_http_reply(c, 200, NULL, "true");
    } else if (mg_path_is_sane(hm->uri) == false) {
      // Bad file name, return error. Protect from traversal, etc
      mg_http_reply(c, 400, NULL, "Bad name");
    } else if (mg_match(hm->method, mg_str("POST"), NULL) ||
               mg_match(hm->method, mg_str("PUT"), NULL)) {
      // File upload
      char path[128];
      mg_snprintf(path, sizeof(path), "/tmp/%.*s", hm->uri.len, hm->uri.buf);
      us->ctx = fopen(path, "wb+");
      if (us->ctx == NULL) {
        mg_http_reply(c, 500, NULL, "File upload error %d\n", errno);
      } else {
        mg_dash_file_add(hm->uri, hm->body.len);
        MG_DEBUG(("%lu Starting upload, [%.*s] %lu", c->id, hm->uri.len,
                  hm->uri.buf, hm->body.len));
        us->marker = 'U';
        us->expected = hm->body.len;
        us->received = 0;
        mg_iobuf_del(&c->recv, 0, hm->head.len);
        c->pfn = NULL;
        c->fn = file_ev_handler;
        mg_call(c, MG_EV_READ, &c->recv.len);
      }
    } else {
      // Serve file
      char path[128];
      mg_snprintf(path, sizeof(path), "/tmp/%.*s", hm->uri.len, hm->uri.buf);
      mg_http_serve_file(c, hm, path, NULL);
    }
  } else if (ev == MG_EV_READ && us->marker == 'U') {
    // Write uploaded data
    MG_DEBUG(("Uploading.. recv.len=%u", c->recv.len));
    us->received += c->recv.len;
    fwrite(c->recv.buf, 1, c->recv.len, (FILE *) us->ctx);
    c->recv.len = 0;
    if (us->received >= us->expected) {
      fclose((FILE *) us->ctx);
      MG_DEBUG(("%lu Uploaded %lu", c->id, us->expected));
      mg_http_reply(c, 200, NULL, "%lu uploaded\n", us->expected);
      mg_dash_send_change(c->mgr, &set_files);
      memset(us, 0, sizeof(*us));
      c->is_draining = 1;
    }
  }
}

static uint64_t mg_dash_make_expiration_time(struct mg_dash *dash) {
  unsigned t = (unsigned) dash->session_auto_expiration_seconds;
  if (t == 0) t = 3600;  // Default expiration time in seconds
  return mg_millis() + t * 1000;
}

// Parse HTTP requests, return authenticated user or NULL
static struct mg_dash_user *mg_dash_authenticate(struct mg_http_message *hm,
                                                 struct mg_dash *dash) {
  static struct mg_dash_user *s_users;  // List of authenticated users
  char user[100], pass[100];
  struct mg_dash_user *u, *tmp, *result = NULL;

  if (dash->authenticate == NULL) return NULL;
  mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));

  // Remove expired users
  for (u = s_users; u != NULL; u = tmp) {
    tmp = u->next;
    if (u->expire < mg_millis()) {
      MG_DEBUG(("Deleting expired auth %s/%d %llu %u", u->name, u->level,
                u->expire, mg_millis() - u->expire));
      LIST_DELETE(struct mg_dash_user, &s_users, u);
    }
  }

  if (pass[0] != '\0') {
    struct mg_str *ah = mg_http_get_header(hm, "Authorization");
    if (ah != NULL) {
      // Auth header and password are set, auth by user/password via glue API
      int num_users = 0, level = dash->authenticate(user, pass);
      MG_DEBUG(("user %s, level: %d", user, level));
      if (level > 0) {  // Proceed only if the firmware authenticated us
        for (u = s_users; u != NULL && result == NULL;
             u = u->next, num_users++) {
          if (strcmp(user, u->name) == 0) {
            u->expire = mg_dash_make_expiration_time(dash);
            result = u;
          }
        }
        // Not yet authenticated, add to the list
        if (result == NULL && num_users < 10) {
          result = (struct mg_dash_user *) mg_calloc(1, sizeof(*result));
          mg_snprintf(result->name, sizeof(result->name), "%s", user);
          mg_random_str(result->token, sizeof(result->token) - 1);
          result->level = level, result->next = s_users, s_users = result;
          result->expire = mg_dash_make_expiration_time(dash);
        }
      }
    } else if (ah == NULL) {
      for (u = s_users; u != NULL && result == NULL; u = u->next) {
        if (strcmp(u->token, pass) == 0) {
          u->expire = mg_dash_make_expiration_time(dash);
          result = u;
        }
      }
    }
  }

  // MG_DEBUG(("[%s/%s] -> %s", user, pass, result ? "OK" : "FAIL"));
  return result;
}

static void mg_handle_login(struct mg_connection *c, struct mg_dash_user *u) {
  char cookie[256];
  mg_snprintf(cookie, sizeof(cookie),
              "Set-Cookie: access_token=%s; Path=/; "
              "%sHttpOnly; SameSite=Lax; Max-Age=%d\r\n%s",
              u->token, c->is_tls ? "Secure; " : "", 3600 * 24,
              MG_JSON_HEADERS);
  mg_http_reply(c, 200, cookie, "{%m:%m,%m:%d}\n",  //
                MG_ESC("user"), MG_ESC(u->name),    //
                MG_ESC("level"), u->level);
}

static void mg_handle_logout(struct mg_connection *c) {
  char cookie[256];
  mg_snprintf(cookie, sizeof(cookie),
              "Set-Cookie: access_token=; Path=/; "
              "Expires=Thu, 01 Jan 1970 00:00:00 UTC; "
              "%sHttpOnly; Max-Age=0; \r\n",
              c->is_tls ? "Secure; " : "");
  mg_http_reply(c, 401, cookie, "Unauthorized\n");
}

void mg_dash_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_dash *dash = (struct mg_dash *) c->fn_data;
  if (ev == MG_EV_OPEN) {
    if (mg_dash_find_field_set(dash, mg_str("files")) == NULL) {
      MG_DASH_ADD_FIELD_SET(dash, &set_files);
    }
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_dash_user *u = mg_dash_authenticate(hm, dash);
    struct mg_str parts[5];
    memset(parts, 0, sizeof(parts));

    MG_DEBUG(("%lu %p %s", c->id, u, u ? u->name : ""));

    if (mg_match(hm->uri, mg_str("/api/hi"), NULL)) {
      mg_http_reply(c, 200, MG_JSON_HEADERS, "hi\n");
    } else if (mg_match(hm->uri, mg_str("/api/logout"), NULL)) {
      mg_handle_logout(c);
      mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m}", MG_ESC("method"),
                   MG_ESC("logout"));
    } else if (mg_match(hm->uri, mg_str("/api/login"), NULL) && u != NULL) {
      mg_handle_login(c, u);
    } else if (dash->authenticate && u == NULL &&
               mg_match(hm->uri, mg_str("/api/#"), NULL)) {
      mg_http_reply(c, 403, MG_JSON_HEADERS, "Not Authorised\n");
    } else if (mg_match(hm->uri, mg_str("/api/websocket"), NULL)) {
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_match(hm->uri, mg_str("/files/#"), NULL)) {
      file_ev_handler(c, ev, ev_data);
    } else if (mg_match(hm->uri, mg_str("/api/get/#"), parts) ||
               mg_match(hm->uri, mg_str("/api/get"), NULL)) {
      mg_http_reply(c, 200, MG_JSON_HEADERS, "%M\n", mg_dash_print_name, dash,
                    &parts[0]);
    } else if (mg_match(hm->uri, mg_str("/api/set"), NULL)) {
      int count = mg_dash_apply(c, dash, hm->body);
      mg_http_reply(c, 200, MG_JSON_HEADERS, "%d\n", count);
    } else if (mg_match(hm->uri, mg_str("/"), NULL)) {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.fs = &mg_fs_packed;
      mg_http_serve_file(c, hm, "/dashboard.html", &opts);
    } else {
      struct mg_dash_custom_handler *ch = dash->custom_handlers;
      for (ch = dash->custom_handlers; ch != NULL; ch = ch->next) {
        if (mg_match(hm->uri, ch->uri_pattern, NULL)) {
          ch->handler(c, ev, ev_data);
          break;
        }
      }
      if (ch == NULL) mg_http_reply(c, 404, MG_JSON_HEADERS, "Not Found");
    }
  } else if (ev == MG_EV_WS_OPEN) {
    // WS connection established, send change notifications for all data
    struct mg_field_set *fs;
    for (fs = dash->sets; fs != NULL; fs = fs->next) {
      mg_dash_send_change(c->mgr, fs);
    }
    mg_dash_broadcast(c->mgr, "{%m:%m}", MG_ESC("method"), MG_ESC("ready"));
  } else if (ev == MG_EV_WS_MSG) {
    // Add this to automatically handle "get" and "set" JSON-RPC calls
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    if (dash == NULL) {
      mg_dash_error(c, wm->data, "%s", "no dash defined");
    } else {
      mg_dash_process_msg(c, wm, dash);
    }
  }
}
