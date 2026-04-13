#include "dash.h"
#include "json.h"
#include "ws.h"

void mg_ws_broadcast(struct mg_mgr *mgr, const char *fmt, ...) {
  struct mg_connection *c;
  va_list ap;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (!c->is_websocket) continue;
    va_start(ap, fmt);
    mg_ws_vprintf(c, WEBSOCKET_OP_TEXT, fmt, &ap);
    va_end(ap);
  }
}

static size_t print_fmt(mg_pfn_t out, void *param, va_list *ap) {
  const char *fmt = va_arg(*ap, const char *);
  ap = va_arg(*ap, va_list *);
  return mg_vxprintf(out, param, fmt, ap);
}

void mg_ws_rpc_success(struct mg_connection *c, struct mg_str req,
                       const char *fmt, ...) {
  struct mg_str id = mg_json_get_tok(req, "$.id");
  va_list ap;
  va_start(ap, fmt);
  mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%.*s,%m:%M}", MG_ESC("id"), id.len,
               id.buf, MG_ESC("result"), print_fmt, fmt, &ap);
  va_end(ap);
}

void mg_ws_rpc_error(struct mg_connection *c, struct mg_str req,
                     const char *fmt, ...) {
  struct mg_str id = mg_json_get_tok(req, "$.id");
  va_list ap;
  va_start(ap, fmt);
  mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%.*s,%m:{%m:%d,%m:%m}}", MG_ESC("id"),
               id.len, id.buf, MG_ESC("error"), MG_ESC("code"), -1,
               MG_ESC("message"), print_fmt, fmt, &ap);
  va_end(ap);
}

#if 0
void mg_get_bool(const void *ctx, void *out) {
  if (ctx != NULL && out != NULL) *(bool *) out = *(bool *) ctx;
}

void mg_get_int(const void *ctx, void *out) {
  if (ctx != NULL && out != NULL) *(int *) out = *(int *) ctx;
}

void mg_get_double(const void *ctx, void *out) {
  if (ctx != NULL && out != NULL) *(double *) out = *(double *) ctx;
}

void mg_get_string(const void *ctx, void *out) {
  if (ctx != NULL && out != NULL) *(char **) out = *(char **) ctx;
}

void mg_set_bool(void *ctx, const void *val) {
  if (ctx != NULL && val != NULL) *(bool *) ctx = *(bool *) val;
}

void mg_set_int(void *ctx, const void *val) {
  if (ctx != NULL && val != NULL) *(int *) ctx = *(const int *) val;
}

void mg_set_double(void *ctx, const void *val) {
  if (ctx != NULL && val != NULL) *(double *) ctx = *(const double *) val;
}

void mg_set_string(void *ctx, const void *val) {
  if (ctx != NULL && val != NULL) *(char **) ctx = *(char **) val;
}

size_t mg_print_one_field(mg_pfn_t out, void *arg, va_list *ap) {
  const struct mg_field *f = va_arg(*ap, const struct mg_field *);
  const char *dot = strrchr(f->name, '.');
  const char *tail = dot ? dot + 1 : f->name;  // Last part of the field name
  size_t n = mg_xprintf(out, arg, "%m:", MG_ESC(tail));
  size_t saved = n;
  if (f->get && strcmp(f->type, "bool") == 0) {
    bool val = false;
    f->get(f->data, &val);
    n += mg_xprintf(out, arg, "%s", val ? "true" : "false");
  } else if (f->get && strcmp(f->type, "int") == 0) {
    int val = 0;
    f->get(f->data, &val);
    n += mg_xprintf(out, arg, "%d", val);
  } else if (f->get && strcmp(f->type, "double") == 0) {
    double val = 0.0;
    f->get(f->data, &val);
    n += mg_xprintf(out, arg, "%g", val);
  } else if (f->get && strcmp(f->type, "string") == 0) {
    const char *val = NULL;
    f->get(f->data, &val);
    n += mg_xprintf(out, arg, "%m", MG_ESC(val));
  }
  if (n == saved) n += mg_xprintf(out, arg, "null");
  return n;
}

size_t mg_print_fields(mg_pfn_t out, void *arg, va_list *ap) {
  struct mg_field *tab = va_arg(*ap, struct mg_field *);
  const char *path = va_arg(*ap, const char *);
  const char *comma = "";
  struct mg_str prev = mg_str("");
  size_t i, j, k, n = 0, path_len = strlen(path);

  n += mg_xprintf(out, arg, "{");
  for (i = 0; tab[i].name != NULL; i++) {
    struct mg_str s;
    const char *dot;
    struct mg_str prefix;
    if (strncmp(tab[i].name, path, path_len) != 0) continue;
    s = mg_str(tab[i].name + path_len);
    dot = strrchr(s.buf, '.');
    prefix = mg_str_n(s.buf, dot ? (size_t) (dot - s.buf) : 0);
    if (mg_strcmp(prev, prefix) != 0) {
      // Find common prefix between prev and prefix
      struct mg_str common = mg_str_n(prev.buf, 0);
      while (common.len < prev.len && common.len < prefix.len &&
             prev.buf[common.len] == prefix.buf[common.len]) {
        common.len++;
      }
      // Print closing braces for fields that are no longer in the prefix
      for (j = common.len; j < prev.len; j++) {
        if (prev.buf[j] == '.') n += mg_xprintf(out, arg, "}");
      }
      if (prev.len > common.len &&
          (common.len == 0 || prev.buf[common.len - 1] == '.')) {
        n += mg_xprintf(out, arg, "}");
      }
      n += mg_xprintf(out, arg, "%s", comma), comma = ",";
      // Print opening braces for new fields in the prefix
      for (j = k = common.len; j < s.len; j++) {
        if (s.buf[j] != '.') continue;
        n += mg_xprintf(out, arg, "%m:{", mg_print_esc, j - k, s.buf + k);
        k = j + 1;
      }
      prev = prefix;
    } else {
      n += mg_xprintf(out, arg, "%s", comma), comma = ",";
    }
    n += mg_xprintf(out, arg, "%M", mg_print_one_field, &tab[i]);
  }
  // Close any remaining open braces
  for (j = 0; j < prev.len; j++) {
    if (prev.buf[j] == '.') n += mg_xprintf(out, arg, "}");
  }
  if (prev.len > 0) n += mg_xprintf(out, arg, "}");
  return n + mg_xprintf(out, arg, "}");
}
#endif