#pragma once

#include "fmt.h"
#include "json.h"
#include "log.h"
#include "printf.h"
#include "str.h"
#include "ws.h"

enum mg_val_type { MG_VAL_INT, MG_VAL_BOOL, MG_VAL_DBL, MG_VAL_STR, MG_VAL_FN };

union mg_val {
  int i;
  bool b;
  double d;
  char *s;
  void (*fn)(void);
};

struct mg_field {
  const char *name;
  enum mg_val_type type;
  void *value;
  size_t value_size;
};

struct mg_field_set {
  const char *name;
  struct mg_field *fields;
  void (*sync)(bool is_reading);
  int read_level;
  int write_level;
};

struct mg_dash {
  struct mg_field_set *sets;
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

static inline struct mg_field_set *mg_dash_find_field_set(struct mg_dash *dash,
                                                          struct mg_str name) {
  size_t i;
  for (i = 0; dash->sets != NULL && dash->sets[i].name != NULL; i++) {
    if (mg_strcmp(name, mg_str(dash->sets[i].name)) == 0) {
      return &dash->sets[i];
    }
  }
  return NULL;
}

static inline struct mg_field *mg_dash_find_field(struct mg_field *fields,
                                                  struct mg_str name) {
  size_t i;
  for (i = 0; fields != NULL && fields[i].name != NULL; i++) {
    if (mg_strcmp(name, mg_str(fields[i].name)) == 0) return &fields[i];
  }
  return NULL;
}

static inline size_t mg_print_field(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_field *f = va_arg(*ap, struct mg_field *);
  // union mg_val val = {0};
  size_t n = 0;
  // if (f->get) f->get(&val);
  n += mg_xprintf(fn, arg, "%m:", MG_ESC(f->name));
  if (f->type == MG_VAL_BOOL) {
    n += mg_xprintf(fn, arg, "%s", *(bool *) f->value ? "true" : "false");
  } else if (f->type == MG_VAL_INT) {
    n += mg_xprintf(fn, arg, "%d", *(int *) f->value);
  } else if (f->type == MG_VAL_DBL) {
    n += mg_xprintf(fn, arg, "%.2f", *(double *) f->value);
  } else if (f->type == MG_VAL_STR) {
    n += mg_xprintf(fn, arg, "%m", MG_ESC(f->value));
  } else {
    n += mg_xprintf(fn, arg, "null");
  }
  return n;
}

static inline size_t mg_print_field_set(mg_pfn_t fn, void *arg, va_list *ap) {
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

static inline size_t mg_dash_print_name(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_dash *dash = va_arg(*ap, struct mg_dash *);
  struct mg_str *name = va_arg(*ap, struct mg_str *);
  struct mg_field_set *set = mg_dash_find_field_set(dash, *name);
  size_t i, n = 0;
  if (name->len == 0) {
    n += mg_xprintf(fn, arg, "{");
    for (i = 0; dash->sets != NULL && dash->sets[i].name != NULL; i++) {
      if (dash->sets[i].sync) dash->sets[i].sync(false);
      if (i > 0) n += mg_xprintf(fn, arg, ",");
      n += mg_xprintf(fn, arg, "%m:", MG_ESC(dash->sets[i].name));
      n += mg_xprintf(fn, arg, "%M", mg_print_field_set, &dash->sets[i]);
    }
    n += mg_xprintf(fn, arg, "}");
  } else if (set != NULL) {
    if (set->sync) set->sync(false);
    n += mg_xprintf(fn, arg, "%M", mg_print_field_set, set);
  } else {
    n += mg_xprintf(fn, arg, "null");
  }
  return n;
}

static inline size_t mg_dash_print(mg_pfn_t fn, void *arg, va_list *ap) {
  struct mg_str *req = va_arg(*ap, struct mg_str *);
  struct mg_dash *dash = va_arg(*ap, struct mg_dash *);
  struct mg_str name = trimq(mg_json_get_tok(*req, "$.params"));
  return mg_xprintf(fn, arg, "%M", mg_dash_print_name, dash, &name);
}

static inline void mg_dash_send_change(struct mg_mgr *mgr, struct mg_dash *dash,
                                       const char *name) {
  struct mg_field_set *set = mg_dash_find_field_set(dash, mg_str(name));
  if (set != NULL) {
    if (set->sync) set->sync(false);
    mg_dash_broadcast(mgr, "{%m:%m,%m:{%m:%M}}", MG_ESC("method"),
                      MG_ESC("change"), MG_ESC("params"), MG_ESC(set->name),
                      mg_print_field_set, set);
  }
}

static inline int mg_dash_parse_field(struct mg_str json, struct mg_field *f) {
  char json_path[128];
  bool ok = false;
  mg_snprintf(json_path, sizeof(json_path), "$.%s", f->name);
  if (f->type == MG_VAL_BOOL) {
    ok = mg_json_get_bool(json, json_path, (bool *) f->value);
  } else if (f->type == MG_VAL_INT) {
    double d;
    ok = mg_json_get_num(json, json_path, &d);
    if (ok) *(int *) f->value = (int) d;
  } else if (f->type == MG_VAL_DBL) {
    ok = mg_json_get_num(json, json_path, (double *) f->value);
  } else if (f->type == MG_VAL_STR) {
    ok = mg_json_unescape(json, json_path, (char *) f->value, f->value_size);
  }
  return ok;
}

static inline int mg_dash_apply(struct mg_connection *c, struct mg_dash *dash,
                                struct mg_str json) {
  struct mg_str key, val;
  size_t ofs = 0;
  int count = 0;
  // struct mg_field *f = NULL;
  while ((ofs = mg_json_next(json, ofs, &key, &val)) > 0) {
    struct mg_field_set *set = mg_dash_find_field_set(dash, trimq(key));
    if (set != NULL) {
      size_t i;
      for (i = 0; set->fields[i].name != NULL; i++) {
        if (mg_dash_parse_field(val, &set->fields[i])) count++;
      }
      if (count > 0 && set->sync != NULL) {
        set->sync(true);
        mg_dash_send_change(c->mgr, dash, set->name);
      }
    } else {
      MG_ERROR(("UNKNOWN SET: [%.*s]", key.len, key.buf));
    }
  }
  return count;
}

static inline void mg_dash_process_msg(struct mg_connection *c,
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

#define MG_NO_CACHE_HEADERS "Cache-Control: no-cache\r\n"
#define MG_JSON_HEADERS "Content-Type: application/json\r\n" MG_NO_CACHE_HEADERS

static inline void mg_dash_ev_handler(struct mg_connection *c, int ev,
                                      void *ev_data) {
  struct mg_dash *dash = (struct mg_dash *) c->fn_data;
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_str parts[5];
    memset(parts, 0, sizeof(parts));
    if (mg_match(hm->uri, mg_str("/api/websocket"), NULL)) {
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_match(hm->uri, mg_str("/api/get/#"), parts) ||
               mg_match(hm->uri, mg_str("/api/get"), NULL)) {
      mg_http_reply(c, 200, MG_JSON_HEADERS, "%M\n", mg_dash_print_name, dash,
                    &parts[0]);
    } else if (mg_match(hm->uri, mg_str("/api/set"), NULL)) {
      int count = mg_dash_apply(c, dash, hm->body);
      mg_http_reply(c, 200, MG_JSON_HEADERS, "%d\n", count);
    } else {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.fs = &mg_fs_packed;
      mg_mem_files = mg_packed_files;
      mg_http_serve_file(c, hm, "/dashboard.html", &opts);
    }
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

extern void mongoose_init(void);
extern void mongoose_poll(void);
