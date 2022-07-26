#include "rpc.h"

struct mg_rpc {
  struct mg_rpc *next;
  struct mg_str method;
  void (*fn)(struct mg_rpc_req *);
  void *fn_data;
};

void mg_rpc_add(void **head, struct mg_str method,
                void (*fn)(struct mg_rpc_req *), void *fn_data) {
  struct mg_rpc *rpc = (struct mg_rpc *) calloc(1, sizeof(*rpc));
  rpc->method = mg_strdup(method), rpc->fn = fn, rpc->fn_data = fn_data;
  rpc->next = (struct mg_rpc *) *head, *head = rpc;
}

void mg_rpc_free(void **head) {
  while (head != NULL && *head != NULL) {
    struct mg_rpc *rpc = *(struct mg_rpc **) head;
    *head = rpc->next;
    free((void *) rpc->method.ptr);
    free(rpc);
  }
}

void mg_rpc_process(void **head, struct mg_str s, mg_pfn_t pfn, void *pfnd) {
  struct mg_rpc_req req = {{s.ptr, s.len}, pfn, pfnd, NULL};
  int len, off = mg_json_get(s.ptr, (int) s.len, "$.method", &len);
  if (off > 0 && s.ptr[off] == '"') {
    struct mg_str m = mg_str_n(&s.ptr[off + 1], (size_t) len - 2);
    struct mg_rpc *h = *(struct mg_rpc **) head;
    while (h != NULL && !mg_match(m, h->method, NULL)) h = h->next;
    if (h != NULL) {
      req.fn_data = h->fn_data;
      h->fn(&req);
    } else {
      mg_rpc_err(&req, -32601, "\"%.*s not found\"", (int) m.len, m.ptr);
    }
  } else {
    mg_rpc_err(&req, -32700, "%.*Q", (int) s.len, s.ptr);
  }
}

void mg_rpc_vok(struct mg_rpc_req *r, const char *fmt, va_list *ap) {
  int len, off = mg_json_get(r->frame.ptr, (int) r->frame.len, "$.id", &len);
  if (off > 0) {
    mg_rprintf(r->pfn, r->pfn_data, "{%Q:%.*s,%Q:", "id", len,
               &r->frame.ptr[off], "result");
    mg_vrprintf(r->pfn, r->pfn_data, fmt == NULL ? "null" : fmt, ap);
    mg_rprintf(r->pfn, r->pfn_data, "}");
  }
}

void mg_rpc_ok(struct mg_rpc_req *r, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_rpc_vok(r, fmt, &ap);
  va_end(ap);
}

void mg_rpc_verr(struct mg_rpc_req *r, int code, const char *fmt, va_list *ap) {
  int len, off = mg_json_get(r->frame.ptr, (int) r->frame.len, "$.id", &len);
  mg_rprintf(r->pfn, r->pfn_data, "{");
  if (off > 0) {
    mg_rprintf(r->pfn, r->pfn_data, "%Q:%.*s,", "id", len, &r->frame.ptr[off]);
  }
  mg_rprintf(r->pfn, r->pfn_data, "%Q:{%Q:%d,%Q:", "error", "code", code,
             "message");
  mg_vrprintf(r->pfn, r->pfn_data, fmt == NULL ? "null" : fmt, ap);
  mg_rprintf(r->pfn, r->pfn_data, "}}");
}

void mg_rpc_err(struct mg_rpc_req *r, int code, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_rpc_verr(r, code, fmt, &ap);
  va_end(ap);
}

static size_t print_methods(mg_pfn_t pfn, void *pfn_data, va_list *ap) {
  struct mg_rpc *h, **head = (struct mg_rpc **) va_arg(*ap, void **);
  size_t len = 0;
  for (h = *head; h != NULL; h = h->next) {
    len += mg_rprintf(pfn, pfn_data, "%s%.*Q", h == *head ? "" : ",",
                      (int) h->method.len, h->method.ptr);
  }
  return len;
}

void mg_rpc_list(struct mg_rpc_req *r) {
  mg_rpc_ok(r, "[%M]", print_methods, r->fn_data);
}
