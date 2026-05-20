#include "dash.h"
#include "ota.h"
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

#define CONN_OTA 'O'
#define CONN_UPLOAD 'F'
#define CONN_HANDLED 'Z'

static struct mg_str trimq(struct mg_str s) {  // Trim double quotes
  if (s.len > 1 && s.buf[0] == '"') s.len -= 2, s.buf++;
  return s;
}

static void mg_dash_broadcast(struct mg_mgr *mgr, int level, const char *fmt,
                              ...) {
  struct mg_connection *c;
  va_list ap;
  for (c = mgr->conns; c != NULL; c = c->next) {
    int user_level = *(int *) c->data;
    if (!c->is_websocket) continue;
    if (level > 0 && user_level < level) continue;
    if (c->send.len > MG_DASH_MAX_SEND_BUF_SIZE) {
      mg_error(c, "%lu buffered data %lu > MG_DASH_MAX_SEND_BUF_SIZE", c->id,
               c->send.len);
    } else {
      va_start(ap, fmt);
      mg_ws_vprintf(c, WEBSOCKET_OP_TEXT, fmt, &ap);
      va_end(ap);
    }
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
  int level = va_arg(*ap, int);
  struct mg_field_set *set = mg_dash_find_field_set(dash, *name);
  size_t n = 0;
  if (name->len == 0) {
    struct mg_field_set *fs;
    const char *comma = "";
    n += mg_xprintf(fn, arg, "{");
    for (fs = dash->sets; fs != NULL; fs = fs->next) {
      if (fs->read_level > 0 && level < fs->read_level) continue;
      if (fs->reader) fs->reader();
      n += mg_xprintf(fn, arg, comma);
      n += mg_xprintf(fn, arg, "%m:", MG_ESC(fs->name));
      n += mg_xprintf(fn, arg, "%M", mg_print_field_set, fs);
      comma = ",";
    }
    n += mg_xprintf(fn, arg, "}");
  } else if (set != NULL &&
             (set->read_level <= 0 || level >= set->read_level)) {
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
  int level = va_arg(*ap, int);
  struct mg_str name = trimq(mg_json_get_tok(*req, "$.params"));
  return mg_xprintf(fn, arg, "%M", mg_dash_print_name, dash, &name, level);
}

void mg_dash_send_change(struct mg_mgr *mgr, struct mg_field_set *set) {
  if (set->reader) set->reader();
  mg_dash_broadcast(mgr, set->read_level, "{%m:%m,%m:{%m:%M}}",
                    MG_ESC("method"), MG_ESC("change"), MG_ESC("params"),
                    MG_ESC(set->name), mg_print_field_set, set);
}

static int mg_dash_parse_field(struct mg_str json, struct mg_field *f) {
  char json_path[128];
  bool ok = false;
  mg_snprintf(json_path, sizeof(json_path), "$.%s", f->name);
  if (f->type == MG_VAL_BOOL) {
    ok = f->value_size == sizeof(bool) &&
         mg_json_get_bool(json, json_path, (bool *) f->value);
  } else if (f->type == MG_VAL_INT) {
    double d;
    if (f->value_size == sizeof(int) && mg_json_get_num(json, json_path, &d) &&
        d == (int) d) {
      *(int *) f->value = (int) d;
      ok = true;
    }
  } else if (f->type == MG_VAL_DBL) {
    ok = f->value_size == sizeof(double) &&
         mg_json_get_num(json, json_path, (double *) f->value);
  } else if (f->type == MG_VAL_STR && f->value_size > 0) {
    ok = mg_json_unescape(json, json_path, (char *) f->value, f->value_size);
  } else if (f->type == MG_VAL_RAW && f->value_size > 0) {
    ok = mg_snprintf((char *) f->value, f->value_size, "%.*s", json.len,
                     json.buf) == json.len;
  }
  return ok;
}

static int mg_dash_apply(struct mg_connection *c, struct mg_dash *dash,
                         struct mg_str json, int level) {
  struct mg_str key, val;
  size_t ofs = 0;
  int total_count = 0;
  while ((ofs = mg_json_next(json, ofs, &key, &val)) > 0) {
    struct mg_field_set *set = mg_dash_find_field_set(dash, trimq(key));
    int count = 0;
    if (set != NULL && (set->write_level <= 0 || level >= set->write_level)) {
      size_t i;
      for (i = 0; set->fields[i].name != NULL; i++) {
        if (mg_dash_parse_field(val, &set->fields[i])) count++;
      }
      if (count) {
        if (set->writer) set->writer();
        mg_dash_send_change(c->mgr, set);
        total_count += count;
      }
    } else if (set == NULL) {
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
  int level = *(int *) c->data;
  if (mg_match(method, mg_str("get"), NULL)) {
    mg_dash_success(c, req, "%M", mg_dash_print, &req, dash, level);
  } else if (mg_match(method, mg_str("set"), NULL)) {
    struct mg_str params = trimq(mg_json_get_tok(req, "$.params"));
    int count = mg_dash_apply(c, dash, params, level);
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

static inline void mg_log_http_req(struct mg_connection *c,
                                   struct mg_http_message *hm) {
  int len = 0;
  size_t n, spaces = 0;
  struct mg_http_message tmp;
  memset(&tmp, 0, sizeof(tmp));
  len = mg_http_parse((char *) c->send.buf, c->send.len, &tmp);
  n = (len < 0 || (size_t) len > c->send.len) ? c->send.len : (size_t) len;
  while ((n + spaces) < c->send.len && spaces < c->send.len &&
         (c->send.buf[c->send.len - spaces - 1] == '\r' ||
          c->send.buf[c->send.len - spaces - 1] == '\n'))
    spaces++;
  MG_DEBUG(("%lu %.*s %.*s %.*s: %lu %.*s -> %lu %.*s", c->id, hm->method.len,
            hm->method.buf, hm->uri.len, hm->uri.buf, c->send.len > 15 ? 3 : 0,
            &c->send.buf[9], hm->body.len, hm->body.len, hm->body.buf,
            c->send.len - n, c->send.len - n - spaces, c->send.buf + n));
}

static struct mg_str mg_dash_file_name(struct mg_http_message *hm) {
  struct mg_str name = mg_str_n(hm->uri.buf + 4, hm->uri.len - 4);  // - /fs/
  // Decode file name in-place - directly into the request buffer
  int len = mg_url_decode(name.buf, name.len, name.buf, name.len + 1, 0);
  if (len > 0 && (size_t) len <= name.len) name.len = (size_t) len;
  return name;
}

static void upload_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  // struct mg_dash *dash = (struct mg_dash *) c->fn_data;
  struct mg_upload_state *us = (struct mg_upload_state *) c->data;
  if (ev == MG_EV_HTTP_HDRS) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_str name = mg_dash_file_name(hm);
    if (sizeof(*us) > sizeof(c->data)) {
      MG_ERROR(
          ("FAIL: sizeof(c->data) == %lu, need %lu."
           " Set #define MG_DATA_SIZE XXX",
           sizeof(c->data), sizeof(*us)));
      mg_http_reply(c, 500, NULL, ":(\n");
      c->is_draining = 1;
    } else if (mg_path_is_sane(name) == false) {
      // Bad file name, return error. Protect from traversal, etc
      mg_http_reply(c, 400, NULL, "Bad name");
      c->is_draining = 1;
    } else {
      bool ok = false;
      if (us->marker == CONN_UPLOAD) {
        char path[128];
        mg_snprintf(path, sizeof(path), "/tmp/%.*s", (int) name.len, name.buf);
        us->ctx = fopen(path, "wb+");
        if (us->ctx == NULL) MG_ERROR(("open(%s): %d", path, errno));
        if (us->ctx != NULL) ok = true;
      } else {
        ok = mg_ota_begin(hm->body.len);
      }
      if (ok == false) {
        mg_http_reply(c, 500, NULL, "Upload failed to start\n");
        c->is_draining = 1;
      } else {
        if (us->marker == CONN_UPLOAD) mg_dash_file_add(name, hm->body.len);
        us->expected = hm->body.len;
        us->received = 0;
        mg_iobuf_del(&c->recv, 0, hm->head.len);
        mg_call(c, MG_EV_READ, &c->recv.len);
      }
    }
  } else if (ev == MG_EV_READ && us->expected > 0 && c->recv.len > 0) {
    // Write uploaded data

    size_t alignment = 512;  // Maximum flash write granularity (iMXRT, Pico)
    size_t left = us->expected > us->received ? us->expected - us->received : 0;
    size_t aligned = c->recv.len < left ? MG_ROUND_DOWN(c->recv.len, alignment)
                                        : c->recv.len;
    bool ok = true, is_ota = us->marker == CONN_OTA;
    if (aligned > 0 && !is_ota) {
      ok = (fwrite(c->recv.buf, 1, aligned, (FILE *) us->ctx) == aligned);
    } else if (aligned > 0 && is_ota) {
      ok = mg_ota_write(c->recv.buf, aligned);
    }

    us->received += aligned;
    // MG_DEBUG(("%lu chunk: %lu/%lu, %lu/%lu, ok: %d", c->id, aligned,
    //           c->recv.len, us->received, us->expected, ok));
    mg_iobuf_del(&c->recv, 0, aligned);  // Delete received data

    if (ok == false) {
      // Some sort of failure - send error response and cleanup
      mg_http_reply(c, 400, "", "Upload error\n");
      c->is_draining = 1;  // Close connection when response it sent
      if (is_ota) mg_ota_end();
      if (!is_ota && us->ctx != NULL) (void) fclose((FILE *) us->ctx);
      memset(us, 0, sizeof(*us));
    } else if (us->received >= us->expected) {
      // Uploaded everything. Send success response and cleanup
      mg_http_reply(c, 200, NULL, "%lu ok\n", us->received);
      c->is_draining = 1;  // Close connection when response it sent
      MG_INFO(("%lu done, %lu bytes", c->id, us->received));
      if (is_ota) mg_ota_end();
      if (!is_ota && us->ctx != NULL) (void) fclose((FILE *) us->ctx);
      if (!is_ota) mg_dash_send_change(c->mgr, &set_files);
      memset(us, 0, sizeof(*us));
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
  static struct mg_dash_user admin = {NULL, "admin", "admin", 9, (uint64_t) -1};
  struct mg_dash_user *u, *tmp, *result = NULL;

  if (dash->authenticate == NULL) return &admin;
  mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
  // MG_DEBUG(("user [%s], pass: [%s], h: %.*s", user, pass, hm->head.len,
  // hm->head.buf));

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
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_HTTP_HDRS && c->data[0] == 0) {
    // Received headers - check authentication and possibly start uploads/ota
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_dash_user *u = mg_dash_authenticate(hm, dash);

    if (mg_match(hm->uri, mg_str("/api/hi"), NULL) ||
        mg_match(hm->uri, mg_str("/api/logout"), NULL)) {
      // Do nothing, handle them MG_EV_HTTP_MSG. We bypass auth for those
    } else if (u == NULL && mg_match(hm->uri, mg_str("/api/#"), NULL)) {
      mg_http_reply(c, 403, MG_JSON_HEADERS, "Not Authorised\n");
      c->data[0] = CONN_HANDLED;
    } else if (mg_match(hm->uri, mg_str("/api/login"), NULL) && u != NULL) {
      mg_handle_login(c, u);
      c->data[0] = CONN_HANDLED;
    } else if (mg_match(hm->uri, mg_str("/api/ota"), NULL)) {
      // Start OTA - in a separate handler function
      c->data[0] = CONN_OTA;
      c->fn = upload_ev_handler;
      c->pfn = NULL;
      mg_call(c, ev, ev_data);
    } else if (mg_match(hm->uri, mg_str("/fs/#"), NULL) &&
               (mg_strcasecmp(hm->method, mg_str("POST")) == 0 ||
                mg_strcasecmp(hm->method, mg_str("PUT")) == 0)) {
      // Start file upload - in a separate function
      c->data[0] = CONN_UPLOAD;
      c->fn = upload_ev_handler;
      c->pfn = NULL;
      mg_call(c, ev, ev_data);
    }
    if (c->data[0] != '\0') mg_log_http_req(c, hm);
  } else if (ev == MG_EV_HTTP_MSG && c->data[0] != '\0') {
    // The response has been send in EV_HDRS path, so we're not reponding
    // anything but clearing the c->data[0] flag for the next request.
    c->data[0] = 0;
    c->is_resp = 0;
  } else if (ev == MG_EV_HTTP_MSG && c->data[0] == '\0') {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_dash_user *u = mg_dash_authenticate(hm, dash);
    int level = u == NULL ? 0 : u->level;
    struct mg_str parts[5];
    memset(parts, 0, sizeof(parts));

    if (mg_match(hm->uri, mg_str("/api/hi"), NULL)) {
      mg_http_reply(c, 200, MG_JSON_HEADERS, "hi\n");
    } else if (mg_match(hm->uri, mg_str("/api/logout"), NULL)) {
      mg_handle_logout(c);
      mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m}", MG_ESC("method"),
                   MG_ESC("logout"));
    } else if (mg_match(hm->uri, mg_str("/api/websocket"), NULL)) {
      *(int *) c->data = level;
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_match(hm->uri, mg_str("/fs/#"), NULL)) {
      struct mg_str name = mg_dash_file_name(hm);
      char path[128];
      mg_snprintf(path, sizeof(path), "/tmp/%.*s", name.len, name.buf);
      if (mg_strcasecmp(hm->method, mg_str("DELETE")) == 0) {
        // Delete file
        mg_dash_file_del(name);
        mg_dash_send_change(c->mgr, &set_files);
        mg_http_reply(c, 200, NULL, "true");
      } else {
        // Serve file
        mg_http_serve_file(c, hm, path, NULL);
      }
    } else if (mg_match(hm->uri, mg_str("/api/get/#"), parts) ||
               mg_match(hm->uri, mg_str("/api/get"), NULL)) {
      mg_http_reply(c, 200, MG_JSON_HEADERS, "%M\n", mg_dash_print_name, dash,
                    &parts[0], level);
    } else if (mg_match(hm->uri, mg_str("/api/set"), NULL)) {
      int count = mg_dash_apply(c, dash, hm->body, level);
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
      mg_log_http_req(c, hm);
    }
  } else if (ev == MG_EV_WS_OPEN) {
    // WS connection established, send change notifications for all data
    struct mg_field_set *fs;
    for (fs = dash->sets; fs != NULL; fs = fs->next) {
      mg_dash_send_change(c->mgr, fs);
    }
    mg_dash_broadcast(c->mgr, 0, "{%m:%m}", MG_ESC("method"), MG_ESC("ready"));
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
