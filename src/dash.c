#include "dash.h"
#include "http.h"
#include "util.h"

#define MG_NO_CACHE_HEADERS "Cache-Control: no-cache\r\n"
#define MG_JSON_HEADERS "Content-Type: application/json\r\n" MG_NO_CACHE_HEADERS

#define CONN_HANDLED 'Z'

struct mg_dash_cdata {
  char marker;
  struct mg_dash_user *u;
  struct mg_dash *dash;
};

static struct mg_dash_user s_guest;
static struct mg_dash_user *s_users;  // List of authenticated users

static struct mg_str trimq(struct mg_str s) {  // Trim double quotes
  if (s.len > 1 && s.buf[0] == '"') s.len -= 2, s.buf++;
  return s;
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
  } else if (f->type == MG_VAL_UINT64) {
    n += mg_xprintf(fn, arg, "%llu", (uint64_t) *(uint64_t *) f->value);
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

static int mg_dash_array_size(struct mg_field_set *set,
                              struct mg_dash_user *u) {
  int saved = *set->index, sz = -1;
  *set->index = -1;
  if (set->fn) {
    if (set->fn(MG_DASH_READ, u)) sz = *set->index;
  } else if (set->get_dir) {
    mg_dash_dir_read(set, u);
    sz = *set->index;
  }
  *set->index = saved;
  return sz;
}

static size_t mg_dash_print_array(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_field_set *set = va_arg(*ap, struct mg_field_set *);
  int from = va_arg(*ap, int);
  int to = va_arg(*ap, int);
  struct mg_dash_user *u = va_arg(*ap, struct mg_dash_user *);
  bool started = false;
  int saved = *set->index;
  size_t n = 0;
  *set->index = from;
  n += mg_xprintf(fn, arg, "[");
  for (;;) {
    bool done = to >= 0 && *set->index > to;
    if (!done) {
      if (set->fn)
        set->fn(MG_DASH_READ, u);
      else if (set->get_dir)
        mg_dash_dir_read(set, u);
      done = *set->index < 0;
    }
    if (done) break;
    n += mg_xprintf(fn, arg, "%s%M", started ? "," : "", mg_print_field_set,
                    set);
    started = true;
    (*set->index)++;
  }
  n += mg_xprintf(fn, arg, "]");
  *set->index = saved;
  return n;
}

static size_t mg_dash_print_endpoint(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_dash *dash = va_arg(*ap, struct mg_dash *);
  struct mg_dash_user *u = va_arg(*ap, struct mg_dash_user *);
  struct mg_str *name = va_arg(*ap, struct mg_str *);
  struct mg_str *from_str = va_arg(*ap, struct mg_str *);
  struct mg_str *to_str = va_arg(*ap, struct mg_str *);
  struct mg_field_set *set = mg_dash_find_field_set(dash, *name);
  size_t n = 0;
  if (name->len == 0) {
    struct mg_field_set *fs;
    const char *comma = "";
    n += mg_xprintf(fn, arg, "{");
    for (fs = dash->sets; fs != NULL; fs = fs->next) {
      if (fs->index != NULL) {
        int sz = mg_dash_array_size(fs, u);
        if (sz < 0) continue;
        n += mg_xprintf(fn, arg, comma);
        n += mg_xprintf(fn, arg, "%m:%d", MG_ESC(fs->name), sz);
      } else {
        if (fs->fn && !fs->fn(MG_DASH_READ, u)) continue;
        n += mg_xprintf(fn, arg, comma);
        n += mg_xprintf(fn, arg, "%m:%M", MG_ESC(fs->name), mg_print_field_set,
                        fs);
      }
      comma = ",";
    }
    n += mg_xprintf(fn, arg, "}");
  } else if (set != NULL && (set->fn == NULL || set->fn(MG_DASH_READ, u))) {
    if (set->index != NULL && from_str != NULL && from_str->len > 0) {
      int from = 0, to = 0;
      mg_str_to_num(*from_str, 10, &from, sizeof(from));
      to = from;
      if (to_str != NULL && to_str->len > 0) {
        mg_str_to_num(*to_str, 10, &to, sizeof(to));
      }
      n += mg_xprintf(fn, arg, "%M", mg_dash_print_array, set, from, to, u);
    } else if (set->index != NULL) {
      n += mg_xprintf(fn, arg, "%d", mg_dash_array_size(set, u));
    } else {
      n += mg_xprintf(fn, arg, "%M", mg_print_field_set, set);
    }
  } else {
    n += mg_xprintf(fn, arg, "null");
  }
  return n;
}

void mg_dash_send_change(struct mg_mgr *mgr, struct mg_field_set *set) {
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    struct mg_dash_cdata *d;
    struct mg_dash_user *u;
    if (!c->is_websocket) continue;
    d = (struct mg_dash_cdata *) c->data;
    u = d->u;
    if (u == NULL) continue;
    if (set->index != NULL && *set->index < 0) {
      int sz = mg_dash_array_size(set, u);
      if (sz < 0) continue;
      mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m,%m:{%m:%d}}", MG_ESC("method"),
                   MG_ESC("change"), MG_ESC("params"), MG_ESC(set->name), sz);
    } else {
      int saved_idx = set->index != NULL ? *set->index : 0;
      bool ok = set->fn ? set->fn(MG_DASH_READ, u)
                        : (set->get_dir ? mg_dash_dir_read(set, u) : true);
      if (!ok) {
        if (set->index != NULL) *set->index = saved_idx;
        continue;
      }
      if (set->index != NULL) {
        char key[64];
        mg_snprintf(key, sizeof(key), "%s/%d", set->name, *set->index);
        mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m,%m:{%m:%M}}",
                     MG_ESC("method"), MG_ESC("change"), MG_ESC("params"),
                     MG_ESC(key), mg_print_field_set, set);
        *set->index = saved_idx;
      } else {
        mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m,%m:{%m:%M}}",
                     MG_ESC("method"), MG_ESC("change"), MG_ESC("params"),
                     MG_ESC(set->name), mg_print_field_set, set);
      }
    }
  }
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
  } else if (f->type == MG_VAL_UINT64) {
    double d;
    if (f->value_size == sizeof(uint64_t) &&
        mg_json_get_num(json, json_path, &d) && d == (double) (int64_t) d) {
      *(uint64_t *) f->value = (uint64_t) d;
      ok = true;
    }
  } else if (f->type == MG_VAL_DBL) {
    ok = f->value_size == sizeof(double) &&
         mg_json_get_num(json, json_path, (double *) f->value);
  } else if (f->type == MG_VAL_STR && f->value_size > 0) {
    struct mg_str tok = mg_json_get_tok(json, json_path);
    ok = tok.len >= 2 && tok.buf[0] == '"' &&
         tok.buf[tok.len - 1] == '"';
    if (ok) mg_json_unescape(json, json_path, (char *) f->value, f->value_size);
  } else if (f->type == MG_VAL_RAW && f->value_size > 0) {
    ok = mg_snprintf((char *) f->value, f->value_size, "%.*s", json.len,
                     json.buf) == json.len;
  }
  return ok;
}

static int mg_dash_apply(struct mg_connection *c, struct mg_dash *dash,
                         struct mg_str json, struct mg_dash_user *u) {
  struct mg_str key, val;
  size_t ofs = 0;
  int total_count = 0;
  while ((ofs = mg_json_next(json, ofs, &key, &val)) > 0) {
    struct mg_field_set *set = mg_dash_find_field_set(dash, trimq(key));
    int count = 0;
    if (set == NULL) {
      MG_ERROR(("UNKNOWN SET: [%.*s]", key.len, key.buf));
      continue;
    }
    if (set->fn != NULL && !set->fn(MG_DASH_WRITE, u)) continue;  // auth check
    {
      size_t i;
      for (i = 0; set->fields[i].name != NULL; i++) {
        if (mg_dash_parse_field(val, &set->fields[i])) count++;
      }
    }
    if (count) {
      if (set->fn) set->fn(MG_DASH_WRITE, u);  // apply side effects
      mg_dash_send_change(c->mgr, set);
      total_count += count;
    }
  }
  return total_count;
}

bool mg_dash_dir_read(struct mg_field_set *set, struct mg_dash_user *u) {
  char dir[256], fname[128] = "";
  struct mg_fs *fs = u->dash->upload_fs ? u->dash->upload_fs : &mg_fs_posix;
  struct mg_field *name_field = NULL, *size_field = NULL;
  size_t i;

  if (!set->get_dir(u, dir, sizeof(dir))) return false;

  for (i = 0; set->fields[i].name != NULL; i++) {
    if (name_field == NULL && set->fields[i].type == MG_VAL_STR &&
        strcmp(set->fields[i].name, "name") == 0)
      name_field = &set->fields[i];
    if (size_field == NULL && strcmp(set->fields[i].name, "size") == 0)
      size_field = &set->fields[i];
  }
  if (name_field == NULL) return false;

  if (*set->index == -1) {  // Size query: count all files
    int count = 0;
    while (mg_fs_ls(fs, dir, fname, sizeof(fname))) count++;
    *set->index = count;
    return true;
  }

  {  // Regular read: scan to *set->index
    int target = *set->index, cur = 0;
    while (mg_fs_ls(fs, dir, fname, sizeof(fname))) {
      if (cur++ == target) {
        mg_snprintf((char *) name_field->value, name_field->value_size, "%s",
                    fname);
        if (size_field != NULL) {
          char path[512];
          size_t sz = 0;
          mg_snprintf(path, sizeof(path), "%s/%s", dir, fname);
          fs->st(path, &sz, NULL);
          if (size_field->type == MG_VAL_UINT64)
            *(uint64_t *) size_field->value = (uint64_t) sz;
          else if (size_field->type == MG_VAL_INT)
            *(int *) size_field->value = (int) sz;
        }
        return true;
      }
    }
    *set->index = -1;  // No more entries
    return true;
  }
}

static bool mg_dash_set_file_name(struct mg_field_set *set,
                                  struct mg_str name) {
  size_t i;
  for (i = 0; set->fields[i].name != NULL; i++) {
    struct mg_field *f = &set->fields[i];
    if (f->type == MG_VAL_STR && strcmp(f->name, "name") == 0) {
      mg_snprintf((char *) f->value, f->value_size, "%.*s", (int) name.len,
                  name.buf);
      return true;
    }
  }
  return false;
}

static inline void mg_log_http_req(struct mg_connection *c,
                                   struct mg_http_message *hm) {
  int len = 0;
  size_t n, spaces = 0, body_n = hm->body.len;
  struct mg_http_message tmp;
  memset(&tmp, 0, sizeof(tmp));
  len = mg_http_parse((char *) c->send.buf, c->send.len, &tmp);
  n = (len < 0 || (size_t) len > c->send.len) ? c->send.len : (size_t) len;
  while ((n + spaces) < c->send.len && spaces < c->send.len &&
         (c->send.buf[c->send.len - spaces - 1] == '\r' ||
          c->send.buf[c->send.len - spaces - 1] == '\n'))
    spaces++;
  // hm->body.len comes from Content-Length and can be larger than the bytes
  // actually buffered so far (e.g. mid-stream uploads); cap the preview to
  // what's actually present in c->recv, or we'd read past its end
  {
    char *recv_end = (char *) c->recv.buf + c->recv.len;
    if (hm->body.buf >= (char *) c->recv.buf && hm->body.buf <= recv_end) {
      size_t avail = (size_t) (recv_end - hm->body.buf);
      if (body_n > avail) body_n = avail;
    } else {
      body_n = 0;
    }
  }
  MG_DEBUG(("%lu %.*s %.*s%s%.*s %.*s: %lu %.*s -> %lu %.*s", c->id,
            hm->method.len, hm->method.buf, hm->uri.len, hm->uri.buf,
            hm->query.len > 0 ? "?" : "", hm->query.len, hm->query.buf,
            c->send.len > 15 ? 3 : 0, &c->send.buf[9], hm->body.len, body_n,
            hm->body.buf, c->send.len - n, c->send.len - n - spaces,
            c->send.buf + n));
}

static void mg_dash_ota_cb(struct mg_connection *c, const char *errmsg) {
  mg_http_reply(c, errmsg ? 500 : 200, NULL, errmsg ? errmsg : "ok\n");
  c->is_draining = 1;
}

static void mg_dash_upload_cb(struct mg_connection *c, const char *errmsg) {
  if (errmsg) {
    mg_http_reply(c, 500, NULL, "%s\n", errmsg);
  } else {
    // mg_http_start_upload() repurposes c->data for its own bookkeeping,
    // so the field set can't be cached there. Re-derive the dashboard from
    // c->fn_data instead, and notify every file-backed array: the upload
    // could belong to any of them, and re-querying get_dir() per recipient
    // is what mg_dash_send_change() does anyway (directories can be
    // user-specific)
    struct mg_dash *dash = (struct mg_dash *) c->fn_data;
    struct mg_field_set *fs;
    mg_http_reply(c, 200, NULL, "ok\n");
    for (fs = dash->sets; fs != NULL; fs = fs->next) {
      if (fs->get_dir == NULL) continue;
      *fs->index = -1;  // Signal mg_dash_send_change() to broadcast new size
      mg_dash_send_change(c->mgr, fs);
    }
  }
  c->is_draining = 1;
}

static uint64_t mg_dash_make_expiration_time(struct mg_dash *dash) {
  unsigned t = (unsigned) dash->session_auto_expiration_seconds;
  if (t == 0) t = 3600;  // Default expiration time in seconds
  return mg_millis() + t * 1000;
}

static struct mg_dash_user *mg_dash_add_user(struct mg_dash_user **users,
                                             struct mg_dash *dash,
                                             const char *name,
                                             const char *token, int level) {
  struct mg_dash_user *u = (struct mg_dash_user *) mg_calloc(1, sizeof(*u));
  if (u != NULL) {
    mg_snprintf(u->name, sizeof(u->name), "%s", name);
    if (token == NULL) {
      mg_random_str(u->token, sizeof(u->token) - 1);
    } else {
      mg_snprintf(u->token, sizeof(u->token), "%s", token);
    }
    u->level = level;
    u->expire = mg_dash_make_expiration_time(dash);
    u->dash = dash;
    u->next = *users;
    *users = u;
  }
  return u;
}

static struct mg_dash_user *mg_dash_find_user(struct mg_dash_user *users,
                                              struct mg_dash *dash,
                                              const char *name) {
  struct mg_dash_user *u;
  for (u = users; u != NULL; u = u->next) {
    if (u->dash == dash && strcmp(u->name, name) == 0) return u;
  }
  return NULL;
}

static struct mg_dash_user *mg_dash_find_token(struct mg_dash_user *users,
                                               struct mg_dash *dash,
                                               const char *token) {
  struct mg_dash_user *u;
  for (u = users; u != NULL; u = u->next) {
    if (u->dash == dash && strcmp(u->token, token) == 0) return u;
  }
  return NULL;
}

static void mg_dash_refresh_user(struct mg_dash *dash,
                                 struct mg_dash_user *user) {
  user->expire = mg_dash_make_expiration_time(dash);
}

static void mg_dash_delete_user(struct mg_mgr *mgr, struct mg_dash_user *u) {
  struct mg_connection *conn;
  for (conn = mgr->conns; conn != NULL; conn = conn->next) {
    struct mg_dash_cdata *d = (struct mg_dash_cdata *) conn->data;
    if (conn->is_websocket && d->dash == u->dash && d->u == u) {
      d->u = NULL;
      conn->is_closing = 1;
    }
  }
  LIST_DELETE(struct mg_dash_user, &s_users, u);
  mg_free(u);
}

static void mg_dash_delete_users(struct mg_mgr *mgr, struct mg_dash *dash) {
  struct mg_dash_user *u, *tmp;
  for (u = s_users; u != NULL; u = tmp) {
    tmp = u->next;
    if (u->dash == dash) mg_dash_delete_user(mgr, u);
  }
}

// Parse HTTP requests, return authenticated user or NULL
static struct mg_dash_user *mg_dash_authenticate(struct mg_connection *c,
                                                 struct mg_http_message *hm,
                                                 struct mg_dash *dash) {
  char user[100], pass[100];
  struct mg_dash_user *u, *tmp;
  struct mg_str *ah;
  int level = 0, num_users = 0;

  if (dash->authenticate == NULL) {
    mg_snprintf(s_guest.name, sizeof(s_guest.name), "%s", "guest");
    s_guest.level = 9;
    s_guest.dash = dash;
    dash->guest = &s_guest;
    return dash->guest;
  }
  mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
  ah = mg_http_get_header(hm, "Authorization");
  // MG_DEBUG(("user [%s], pass: [%s], h: %.*s", user, pass, hm->head.len,
  // hm->head.buf));

  // Remove expired users
  for (u = s_users; u != NULL; u = tmp) {
    tmp = u->next;
    if (u->expire < mg_millis()) {
      MG_DEBUG(("Deleting expired auth %s/%d %llu %u", u->name, u->level,
                u->expire, mg_millis() - u->expire));
      mg_dash_delete_user(c->mgr, u);
    }
  }

  if (pass[0] == '\0') return NULL;

  for (u = s_users; u != NULL; u = u->next) {
    if (u->dash == dash) num_users++;
  }
  if (ah == NULL) {
    u = mg_dash_find_token(s_users, dash, pass);
    if (u != NULL) {
      mg_dash_refresh_user(dash, u);
      return u;
    }
  }

  level = dash->authenticate(user, sizeof(user), pass);
  MG_DEBUG(("user %s, level: %d", user, level));
  if (level <= 0) return NULL;

  u = mg_dash_find_user(s_users, dash, user);
  if (u != NULL) {
    if (ah == NULL) mg_snprintf(u->token, sizeof(u->token), "%s", pass);
    mg_dash_refresh_user(dash, u);
    return u;
  }

  if (num_users < 10)
    return mg_dash_add_user(&s_users, dash, user, ah == NULL ? pass : NULL,
                            level);
  // MG_DEBUG(("[%s/%s] -> %s", user, pass, result ? "OK" : "FAIL"));
  return NULL;
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

static void mg_dash_handle_del(struct mg_connection *c, struct mg_dash *dash,
                               struct mg_dash_user *u, struct mg_str *parts) {
  struct mg_field_set *set = mg_dash_find_field_set(dash, parts[0]);
  if (set == NULL || set->index == NULL) {
    mg_http_reply(c, 404, MG_JSON_HEADERS, "null\n");
  } else {
    int from = 0, to = 0, count = 0;
    if (parts[1].len) mg_str_to_num(parts[1], 10, &from, sizeof(from));
    to = parts[2].len ? 0 : from;
    if (parts[2].len) mg_str_to_num(parts[2], 10, &to, sizeof(to));
    for (*set->index = from; *set->index <= to; (*set->index)++) {
      if (set->fn && set->fn(MG_DASH_DELETE, u))
        count++;
      else
        break;
    }
    if (count) {
      *set->index = -1;
      mg_dash_send_change(c->mgr, set);
      mg_http_reply(c, 200, MG_JSON_HEADERS, "%d\n", count);
    } else {
      mg_http_reply(c, 403, MG_JSON_HEADERS, "false\n");
    }
  }
}

// Handle "POST /api/get/<set>/<index>": modify one array element. Loads the
// element at <index> first - that doubles as a read-access check and as a
// pre-fill, so that JSON keys absent from the body keep their old values -
// then overlays the values from the body and asks fn to persist them
static void mg_dash_handle_mod(struct mg_connection *c, struct mg_dash *dash,
                               struct mg_dash_user *u, struct mg_str *parts,
                               struct mg_str body) {
  struct mg_field_set *set = mg_dash_find_field_set(dash, parts[0]);
  int index = 0, count = 0;
  size_t i;
  if (set == NULL || set->index == NULL || set->fn == NULL) {
    mg_http_reply(c, 404, MG_JSON_HEADERS, "null\n");
    return;
  }
  mg_str_to_num(parts[1], 10, &index, sizeof(index));
  *set->index = index;
  if (!set->fn(MG_DASH_READ, u) || *set->index != index) {
    mg_http_reply(c, 404, MG_JSON_HEADERS, "null\n");
    return;
  }
  for (i = 0; set->fields[i].name != NULL; i++) {
    if (mg_dash_parse_field(body, &set->fields[i])) count++;
  }
  if (count && set->fn(MG_DASH_WRITE, u)) {
    mg_dash_send_change(c->mgr, set);
    mg_http_reply(c, 200, MG_JSON_HEADERS, "%d\n", count);
  } else {
    mg_http_reply(c, 403, MG_JSON_HEADERS, "false\n");
  }
}

// Handle "POST /api/add/<set>": append a new array element. Parses the body
// straight into the bound fields, then asks fn to accept and persist them as
// a new element - fn returns false to reject, e.g. when a cap is reached
static void mg_dash_handle_add(struct mg_connection *c, struct mg_dash *dash,
                               struct mg_dash_user *u, struct mg_str *parts,
                               struct mg_str body) {
  struct mg_field_set *set = mg_dash_find_field_set(dash, parts[0]);
  int count = 0;
  size_t i;
  if (set == NULL || set->index == NULL || set->fn == NULL) {
    mg_http_reply(c, 404, MG_JSON_HEADERS, "null\n");
    return;
  }
  for (i = 0; set->fields[i].name != NULL; i++) {
    if (mg_dash_parse_field(body, &set->fields[i])) count++;
  }
  if (count && set->fn(MG_DASH_ADD, u)) {
    *set->index = -1;  // Signal mg_dash_send_change() to send new size
    mg_dash_send_change(c->mgr, set);
    mg_http_reply(c, 200, MG_JSON_HEADERS, "true\n");
  } else {
    mg_http_reply(c, 403, MG_JSON_HEADERS, "false\n");
  }
}

void mg_dash_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_dash *dash = (struct mg_dash *) c->fn_data;
  struct mg_dash_cdata *d = (struct mg_dash_cdata *) c->data;

  if (ev == MG_EV_OPEN) {
    d->dash = dash;
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CLOSE && c->is_listening) {
    mg_dash_delete_users(c->mgr, dash);
  } else if (ev == MG_EV_HTTP_HDRS && d->marker == 0) {
    // Received headers - check authentication and possibly start uploads/ota
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_dash_user *u = mg_dash_authenticate(c, hm, dash);
    struct mg_str parts[3];
    memset(parts, 0, sizeof(parts));

    if (mg_match(hm->uri, mg_str("/api/hi"), NULL) ||
        mg_match(hm->uri, mg_str("/api/logout"), NULL)) {
      // Do nothing, handle them MG_EV_HTTP_MSG. We bypass auth for those
    } else if (u == NULL && mg_match(hm->uri, mg_str("/api/#"), NULL)) {
      mg_http_reply(c, 403, MG_JSON_HEADERS, "Not Authorised\n");
      d->marker = CONN_HANDLED;
    } else if (mg_match(hm->uri, mg_str("/api/login"), NULL) && u != NULL) {
      mg_handle_login(c, u);
      d->marker = CONN_HANDLED;
    } else if (mg_match(hm->uri, mg_str("/api/ota"), NULL)) {
      mg_http_start_ota(c, hm, mg_dash_ota_cb);
    } else if (mg_match(hm->uri, mg_str("/fs/*/*"), parts) &&
               (mg_strcasecmp(hm->method, mg_str("POST")) == 0 ||
                mg_strcasecmp(hm->method, mg_str("PUT")) == 0)) {
      struct mg_field_set *set = mg_dash_find_field_set(dash, parts[0]);
      struct mg_str name = parts[1];
      int len =
          mg_url_decode(name.buf, name.len, (char *) name.buf, name.len + 1, 0);
      if (len > 0 && (size_t) len <= name.len) name.len = (size_t) len;
      if (set == NULL || set->get_dir == NULL) {
        mg_http_reply(c, 404, MG_JSON_HEADERS, "Not Found\n");
        d->marker = CONN_HANDLED;
      } else if (u == NULL) {
        mg_http_reply(c, 403, MG_JSON_HEADERS, "Not Authorised\n");
        d->marker = CONN_HANDLED;
      } else if (!mg_path_is_sane(name)) {
        mg_http_reply(c, 400, MG_JSON_HEADERS, "Bad file name\n");
        d->marker = CONN_HANDLED;
      } else {
        mg_dash_set_file_name(set, name);
        if (set->fn != NULL && !set->fn(MG_DASH_WRITE, u)) {
          mg_http_reply(c, 403, MG_JSON_HEADERS, "Not Authorised\n");
          d->marker = CONN_HANDLED;
        } else {
          char dir[256];
          struct mg_fs *fs = dash->upload_fs ? dash->upload_fs : &mg_fs_posix;
          if (!set->get_dir(u, dir, sizeof(dir))) {
            mg_http_reply(c, 500, MG_JSON_HEADERS, "Upload dir error\n");
            d->marker = CONN_HANDLED;
          } else {
            mg_http_start_upload(c, hm, name, mg_str(dir), fs,
                                 mg_dash_upload_cb);
          }
        }
      }
    }
    if (d->marker != '\0') mg_log_http_req(c, hm);
  } else if (ev == MG_EV_HTTP_MSG && d->marker != '\0') {
    // The response has been send in EV_HDRS path, so we're not reponding
    // anything but clearing the marker for the next request.
    d->marker = 0;
    c->is_resp = 0;
  } else if (ev == MG_EV_HTTP_MSG && d->marker == '\0') {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_dash_user *u = mg_dash_authenticate(c, hm, dash);
    struct mg_str parts[5];
    memset(parts, 0, sizeof(parts));

    if (mg_match(hm->uri, mg_str("/api/hi"), NULL)) {
      mg_http_reply(c, 200, MG_JSON_HEADERS, "hi\n");
    } else if (mg_match(hm->uri, mg_str("/api/logout"), NULL)) {
      mg_handle_logout(c);
      mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m}", MG_ESC("method"),
                   MG_ESC("logout"));
    } else if (mg_match(hm->uri, mg_str("/api/websocket"), NULL)) {
      d->u = u;
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_match(hm->uri, mg_str("/fs/*/*"), parts)) {
      struct mg_field_set *set = mg_dash_find_field_set(dash, parts[0]);
      if (set == NULL || set->get_dir == NULL) {
        mg_http_reply(c, 404, MG_JSON_HEADERS, "Not Found");
      } else if (u == NULL) {
        mg_http_reply(c, 403, MG_JSON_HEADERS, "Not Authorised\n");
      } else {
        char dir[256], path[512];
        struct mg_fs *fs = dash->upload_fs ? dash->upload_fs : &mg_fs_posix;
        struct mg_str name = parts[1];
        int len = mg_url_decode(name.buf, name.len, (char *) name.buf,
                                name.len + 1, 0);
        if (len > 0 && (size_t) len <= name.len) name.len = (size_t) len;
        if (!mg_path_is_sane(name)) {
          mg_http_reply(c, 400, MG_JSON_HEADERS, "Bad file name\n");
          return;
        }
        if (!set->get_dir(u, dir, sizeof(dir))) {
          mg_http_reply(c, 500, MG_JSON_HEADERS, "Upload dir error\n");
          return;
        }
        mg_snprintf(path, sizeof(path), "%s/%.*s", dir, name.len, name.buf);
        if (mg_strcasecmp(hm->method, mg_str("DELETE")) == 0) {
          mg_dash_set_file_name(set, name);
          if (set->fn != NULL && !set->fn(MG_DASH_DELETE, u)) {
            mg_http_reply(c, 403, MG_JSON_HEADERS, "Not Authorised\n");
          } else {
            fs->rm(path);
            *set->index = -1;  // Signal mg_dash_send_change() to send new size
            mg_dash_send_change(c->mgr, set);
            mg_http_reply(c, 200, NULL, "true");
          }
        } else {
          mg_http_serve_file(c, hm, path, NULL);
        }
      }
    } else if (mg_match(hm->uri, mg_str("/api/del/*/*/*"), parts) ||
               mg_match(hm->uri, mg_str("/api/del/*/*"), parts)) {
      mg_dash_handle_del(c, dash, u, parts);
    } else if (mg_match(hm->uri, mg_str("/api/add/*"), parts)) {
      mg_dash_handle_add(c, dash, u, parts, hm->body);
    } else if (mg_match(hm->uri, mg_str("/api/get/*/*"), parts) &&
               mg_strcasecmp(hm->method, mg_str("POST")) == 0) {
      mg_dash_handle_mod(c, dash, u, parts, hm->body);
    } else if (mg_match(hm->uri, mg_str("/api/get/*/*/*"), parts) ||
               mg_match(hm->uri, mg_str("/api/get/*/*"), parts) ||
               mg_match(hm->uri, mg_str("/api/get/*"), parts) ||
               mg_match(hm->uri, mg_str("/api/get"), NULL)) {
      mg_http_reply(c, 200, MG_JSON_HEADERS, "%M\n", mg_dash_print_endpoint,
                    dash, u, &parts[0], &parts[1], &parts[2]);
    } else if (mg_match(hm->uri, mg_str("/api/set"), NULL)) {
      int count = mg_dash_apply(c, dash, hm->body, u);
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
  }
}
