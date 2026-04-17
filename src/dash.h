#pragma once

#include "ws.h"

enum mg_val_type { MG_VAL_INT, MG_VAL_BOOL, MG_VAL_DBL, MG_VAL_STR };

union mg_val {
  int i;
  bool b;
  double d;
  struct mg_str s;
};

struct mg_field {
  const char *name;
  enum mg_val_type type;
  union mg_val value;
};

typedef void (*mg_dash_get_fn)(struct mg_field *);
typedef void (*mg_dash_set_fn)(struct mg_field *);

struct mg_dash {
  struct mg_field *fields;
  mg_dash_get_fn get;
  mg_dash_set_fn set;
};

static inline struct mg_str trimq(struct mg_str s) {  // Trim double quotes
  if (s.len > 1 && s.buf[0] == '"') s.len -= 2, s.buf++;
  return s;
}

static inline void mg_dash_broadcast(struct mg_mgr *mgr, const char *fmt, ...) {
  struct mg_connection *c;
  va_list ap;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (!c->is_websocket) continue;
    va_start(ap, fmt);
    mg_ws_vprintf(c, WEBSOCKET_OP_TEXT, fmt, &ap);
    va_end(ap);
  }
}

static inline size_t mg_print_fmt(mg_pfn_t out, void *param, va_list *ap) {
  const char *fmt = va_arg(*ap, const char *);
  ap = va_arg(*ap, va_list *);
  return mg_vxprintf(out, param, fmt, ap);
}

static inline void mg_dash_success(struct mg_connection *c, struct mg_str req,
                                   const char *fmt, ...) {
  struct mg_str id = mg_json_get_tok(req, "$.id");
  va_list ap;
  va_start(ap, fmt);
  mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%.*s,%m:%M}", MG_ESC("id"), id.len,
               id.buf, MG_ESC("result"), mg_print_fmt, fmt, &ap);
  va_end(ap);
}

static inline void mg_dash_error(struct mg_connection *c, struct mg_str req,
                                 const char *fmt, ...) {
  struct mg_str id = mg_json_get_tok(req, "$.id");
  va_list ap;
  va_start(ap, fmt);
  mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%.*s,%m:{%m:%d,%m:%m}}", MG_ESC("id"),
               id.len, id.buf, MG_ESC("error"), MG_ESC("code"), -1,
               MG_ESC("message"), mg_print_fmt, fmt, &ap);
  va_end(ap);
}

static inline struct mg_field *mg_dash_find_field(struct mg_field *fields,
                                                  struct mg_str name) {
  size_t i;
  for (i = 0; fields[i].name != NULL; i++) {
    if (mg_strcmp(name, mg_str(fields[i].name)) == 0) return &fields[i];
  }
  return NULL;
}

static inline size_t mg_print_field(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_field *f = va_arg(*ap, struct mg_field *);
  size_t n = 0;
  n += mg_xprintf(fn, arg, "%m:", MG_ESC(f->name));
  if (f->type == MG_VAL_BOOL) {
    n += mg_xprintf(fn, arg, "%s", f->value.b ? "true" : "false");
  } else if (f->type == MG_VAL_INT) {
    n += mg_xprintf(fn, arg, "%d", f->value.i);
  } else if (f->type == MG_VAL_DBL) {
    n += mg_xprintf(fn, arg, "%g", f->value.d);
  } else if (f->type == MG_VAL_STR) {
    struct mg_str s = f->value.s;
    n += mg_xprintf(fn, arg, "%m", mg_print_esc, s.len, s.buf);
  } else {
    n += mg_xprintf(fn, arg, "null");
  }
  return n;
}

static inline bool mg_parse_field(struct mg_str json, struct mg_field *f) {
  bool ok = false;
  if (f->type == MG_VAL_BOOL) {
    ok = mg_json_get_bool(json, "$", &f->value.b);
  } else if (f->type == MG_VAL_INT) {
    double d;
    ok = mg_json_get_num(json, "$", &d);
    if (ok) f->value.i = (int) d;
  } else if (f->type == MG_VAL_DBL) {
    ok = mg_json_get_num(json, "$", &f->value.d);
  } else if (f->type == MG_VAL_STR) {
    f->value.s = trimq(json);
  }
  return ok;
}

static inline size_t mg_dash_print(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_str *req = va_arg(*ap, struct mg_str *);
  struct mg_field *fields = va_arg(*ap, struct mg_field *);
  mg_dash_get_fn get = va_arg(*ap, mg_dash_get_fn);
  struct mg_str name = trimq(mg_json_get_tok(*req, "$.params"));
  struct mg_field *f = mg_dash_find_field(fields, name);
  size_t i, n = 0;
  if (name.len == 0) {
    const char *comma = "";
    n += mg_xprintf(fn, arg, "{");
    for (i = 0; fields[i].name != NULL; i++) {
      f = &fields[i];
      if (get) get(f);
      n += mg_xprintf(fn, arg, "%s", comma);
      n += mg_xprintf(fn, arg, "%M", mg_print_field, f);
      comma = ",";
    }
    n += mg_xprintf(fn, arg, "}");
  } else if (f != NULL) {
    if (get) get(f);
    n += mg_xprintf(fn, arg, "{%M}", mg_print_field, f);
  } else {
    n += mg_xprintf(fn, arg, "null");
  }
  return n;
}

static inline bool mg_dash_sync(struct mg_mgr *mgr, struct mg_field *f,
                                mg_dash_get_fn get) {
  union mg_val old_value = f->value;
  bool changed = false;
  if (get) get(f);
  // Strings require special comparison
  if (f->type == MG_VAL_STR && mg_strcmp(old_value.s, f->value.s) != 0) {
    changed = true;
  } else if (memcmp(&f->value, &old_value, sizeof(old_value)) != 0) {
    changed = true;
  }
  if (changed && f != NULL) {
    mg_dash_broadcast(mgr, "{%m:%m,%m:{%M}}", MG_ESC("method"),
                      MG_ESC("change"), MG_ESC("params"), mg_print_field, f);
  }
  return changed;
}

static inline bool mg_dash_apply(struct mg_connection *c, struct mg_str json,
                                 struct mg_field *fields, mg_dash_get_fn get,
                                 mg_dash_set_fn set) {
  struct mg_str key, val;
  size_t ofs = 0;
  struct mg_field *f = NULL;
  bool changed = false;
  while ((ofs = mg_json_next(json, ofs, &key, &val)) > 0) {
    key = trimq(key);
    if ((f = mg_dash_find_field(fields, key)) != NULL) {
      struct mg_field tmp = *f;
      mg_parse_field(val, &tmp);
      if (set) set(&tmp);
      if (mg_dash_sync(c->mgr, f, get)) changed = true;
      break;
    } else {
      MG_ERROR(("UNKNOWN FIELD: [%.*s]", key.len, key.buf));
    }
  }
  return changed;
}

static inline void mg_dash_process_msg(struct mg_connection *c,
                                       struct mg_ws_message *wm,
                                       struct mg_field *fields,
                                       mg_dash_get_fn get, mg_dash_set_fn set) {
  struct mg_str req = wm->data;
  struct mg_str method = trimq(mg_json_get_tok(req, "$.method"));
  if (mg_match(method, mg_str("get"), NULL)) {
    mg_dash_success(c, req, "%M", mg_dash_print, &req, fields, get);
  } else if (mg_match(method, mg_str("set"), NULL)) {
    struct mg_str params = trimq(mg_json_get_tok(req, "$.params"));
    bool changed = mg_dash_apply(c, params, fields, get, set);
    mg_dash_success(c, req, "%s", changed ? "true" : "false");
  } else {
    mg_dash_error(c, req, "%s", "unknown method");
  }
}

static inline void mg_dash_ev_handler(struct mg_connection *c, int ev,
                                      void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/websocket"), NULL)) {
      mg_ws_upgrade(c, hm, NULL);
    } else {
      struct mg_http_serve_opts opts = {0};
      opts.fs = &mg_fs_packed;
      mg_mem_files = mg_packed_files;
      mg_http_serve_file(c, hm, "/dashboard.html", &opts);
    }
  } else if (ev == MG_EV_WS_MSG) {
    // Add this to automatically handle "get" and "set" JSON-RPC calls
    struct mg_dash *dash = (struct mg_dash *) c->fn_data;
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    if (dash == NULL || dash->fields == NULL) {
      mg_dash_error(c, wm->data, "%s", "no fields defined");
    } else {
      mg_dash_process_msg(c, wm, dash->fields, dash->get, dash->set);
    }
  }
}
