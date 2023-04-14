#include "rpc.h"
#include "printf.h"

void mg_rpc_add(struct mg_rpc **head, struct mg_str method,
                void (*fn)(struct mg_rpc_req *), void *fn_data) {
  struct mg_rpc *rpc = (struct mg_rpc *) calloc(1, sizeof(*rpc));
  if (rpc != NULL) {
    rpc->method = mg_strdup(method), rpc->fn = fn, rpc->fn_data = fn_data;
    rpc->next = *head, *head = rpc;
  }
}

void mg_rpc_del(struct mg_rpc **head, void (*fn)(struct mg_rpc_req *)) {
  struct mg_rpc *r;
  while ((r = *head) != NULL) {
    if (r->fn == fn || fn == NULL) {
      *head = r->next;
      free((void *) r->method.ptr);
      free(r);
    } else {
      head = &(*head)->next;
    }
  }
}

static void mg_rpc_call(struct mg_rpc_req *r, struct mg_str method) {
  struct mg_rpc *h = r->head == NULL ? NULL : *r->head;
  while (h != NULL && !mg_match(method, h->method, NULL)) h = h->next;
  if (h != NULL) {
    r->rpc = h;
    h->fn(r);
  } else {
    mg_rpc_err(r, -32601, "\"%.*s not found\"", (int) method.len, method.ptr);
  }
}

void mg_rpc_process(struct mg_rpc_req *r) {
  int len, off = mg_json_get(r->frame, "$.method", &len);
  if (off > 0 && r->frame.ptr[off] == '"') {
    struct mg_str method = mg_str_n(&r->frame.ptr[off + 1], (size_t) len - 2);
    mg_rpc_call(r, method);
  } else if ((off = mg_json_get(r->frame, "$.result", &len)) > 0 ||
             (off = mg_json_get(r->frame, "$.error", &len)) > 0) {
    mg_rpc_call(r, mg_str(""));  // JSON response! call "" method handler
  } else {
    mg_rpc_err(r, -32700, "%m", mg_print_esc, (int) r->frame.len,
               r->frame.ptr);  // Invalid
  }
}

void mg_rpc_vok(struct mg_rpc_req *r, const char *fmt, va_list *ap) {
  int len, off = mg_json_get(r->frame, "$.id", &len);
  if (off > 0) {
    mg_xprintf(r->pfn, r->pfn_data, "{%m:%.*s,%m:", mg_print_esc, 0, "id", len,
               &r->frame.ptr[off], mg_print_esc, 0, "result");
    mg_vxprintf(r->pfn, r->pfn_data, fmt == NULL ? "null" : fmt, ap);
    mg_xprintf(r->pfn, r->pfn_data, "}");
  }
}

void mg_rpc_ok(struct mg_rpc_req *r, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_rpc_vok(r, fmt, &ap);
  va_end(ap);
}

void mg_rpc_verr(struct mg_rpc_req *r, int code, const char *fmt, va_list *ap) {
  int len, off = mg_json_get(r->frame, "$.id", &len);
  mg_xprintf(r->pfn, r->pfn_data, "{");
  if (off > 0) {
    mg_xprintf(r->pfn, r->pfn_data, "%m:%.*s,", mg_print_esc, 0, "id", len,
               &r->frame.ptr[off]);
  }
  mg_xprintf(r->pfn, r->pfn_data, "%m:{%m:%d,%m:", mg_print_esc, 0, "error",
             mg_print_esc, 0, "code", code, mg_print_esc, 0, "message");
  mg_vxprintf(r->pfn, r->pfn_data, fmt == NULL ? "null" : fmt, ap);
  mg_xprintf(r->pfn, r->pfn_data, "}}");
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
    if (h->method.len == 0) continue;  // Ignore response handler
    len += mg_xprintf(pfn, pfn_data, "%s%m", h == *head ? "" : ",",
                      mg_print_esc, (int) h->method.len, h->method.ptr);
  }
  return len;
}

void mg_rpc_list(struct mg_rpc_req *r) {
  mg_rpc_ok(r, "[%M]", print_methods, r->head);
}
